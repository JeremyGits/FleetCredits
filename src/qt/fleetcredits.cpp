// Copyright (c) 2011-2016 The Fleet Credits Core developers
// Copyright (c) 2021-2023 The Fleet Credits Core developers
// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#if defined(HAVE_CONFIG_H)
#include "config/fleetcredits-config.h"
#endif

#include "fleetcreditsgui.h"

#include "chainparams.h"
#include "clientmodel.h"
#include "compat.h"
#include "fs.h"
#include "guiconstants.h"
#include "guiutil.h"
#include "init.h"
#include "intro.h"
#include "networkstyle.h"
#include "optionsmodel.h"
#include "platformstyle.h"
#include "rpc/server.h"
#include "scheduler.h"
#include "splashscreen.h"
#include "ui_interface.h"
#include "utilitydialog.h"
#include "util.h"
#include "warnings.h"
#include "winshutdownmonitor.h"

#ifdef ENABLE_WALLET
#include "paymentserver.h"
#include "walletmodel.h"
#include "wallet/wallet.h"
#include "validationinterface.h"
#include "fs.h"
#endif

#include <stdint.h>

#include <boost/thread.hpp>
#include <mutex>

#include <QApplication>
#include <QDebug>
#include <QLibraryInfo>
#include <QLocale>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>
#include <QTranslator>

#if defined(QT_STATICPLUGIN)
#include <QtPlugin>
#if QT_VERSION < 0x050400
Q_IMPORT_PLUGIN(AccessibleFactory)
#endif
#if defined(QT_QPA_PLATFORM_XCB)
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin);
#elif defined(QT_QPA_PLATFORM_WINDOWS)
Q_IMPORT_PLUGIN(QMinimalIntegrationPlugin);
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
Q_IMPORT_PLUGIN(QWindowsPrinterSupportPlugin);
#elif defined(QT_QPA_PLATFORM_COCOA)
Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin);
#endif
#endif

// Declare meta types used for QMetaObject::invokeMethod
Q_DECLARE_METATYPE(bool*)
Q_DECLARE_METATYPE(CAmount)

static void InitMessage(const std::string &message)
{
    LogPrintf("init message: %s\n", message);
}

/*
   Translate string to current locale using Qt.
 */
static std::string Translate(const char* psz)
{
    return QCoreApplication::translate("fleetcredits-core", psz).toStdString();
}

static QString GetLangTerritory()
{
    QSettings settings;
    // Get desired locale (e.g. "de_DE")
    // 1) System default language
    QString lang_territory = QLocale::system().name();
    // 2) Language from QSettings
    QString lang_territory_qsettings = settings.value("language", "").toString();
    if(!lang_territory_qsettings.isEmpty())
        lang_territory = lang_territory_qsettings;
    // 3) -lang command line argument
    lang_territory = QString::fromStdString(GetArg("-lang", lang_territory.toStdString()));
    return lang_territory;
}

/** Set up translations */
static void initTranslations(QTranslator &qtTranslatorBase, QTranslator &qtTranslator, QTranslator &translatorBase, QTranslator &translator)
{
    // Remove old translators
    QApplication::removeTranslator(&qtTranslatorBase);
    QApplication::removeTranslator(&qtTranslator);
    QApplication::removeTranslator(&translatorBase);
    QApplication::removeTranslator(&translator);

    // Get desired locale (e.g. "de_DE")
    // 1) System default language
    QString lang_territory = GetLangTerritory();

    // Convert to "de" only by truncating "_DE"
    QString lang = lang_territory;
    lang.truncate(lang_territory.lastIndexOf('_'));

    // Load language files for configured locale:
    // - First load the translator for the base language, without territory
    // - Then load the more specific locale translator

    // Load e.g. qt_de.qm
    if (qtTranslatorBase.load("qt_" + lang, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        QApplication::installTranslator(&qtTranslatorBase);

    // Load e.g. qt_de_DE.qm
    if (qtTranslator.load("qt_" + lang_territory, QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
        QApplication::installTranslator(&qtTranslator);

    // Load e.g. fleetcredits_de.qm (shortcut "de" needs to be defined in fleetcredits.qrc)
    if (translatorBase.load(lang, ":/translations/"))
        QApplication::installTranslator(&translatorBase);

    // Load e.g. fleetcredits_de_DE.qm (shortcut "de_DE" needs to be defined in fleetcredits.qrc)
    if (translator.load(lang_territory, ":/translations/"))
        QApplication::installTranslator(&translator);
}

/* qDebug() message handler --> debug.log */
void DebugMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString &msg)
{
    Q_UNUSED(context);
    const char *category = (type == QtDebugMsg) ? "qt" : NULL;
    LogPrint(category, "GUI: %s\n", msg.toStdString());
}

/** Class encapsulating Fleet Credits Core startup and shutdown.
 * Allows running startup and shutdown in a different thread from the UI thread.
 */
class FleetCreditsCore: public QObject
{
    Q_OBJECT
public:
    explicit FleetCreditsCore();

public Q_SLOTS:
    void initialize();
    void shutdown();

Q_SIGNALS:
    void initializeResult(int retval);
    void shutdownResult(int retval);
    void runawayException(const QString &message);

private:
    boost::thread_group threadGroup;
    CScheduler scheduler;

    /// Pass fatal exception message to UI thread
    void handleRunawayException(const std::exception *e);
};

/** Main Fleet Credits application object */
class FleetCreditsApplication: public QApplication
{
    Q_OBJECT
public:
    explicit FleetCreditsApplication();
    ~FleetCreditsApplication();

#ifdef ENABLE_WALLET
    /// Create payment server
    void createPaymentServer();
#endif
    /// parameter interaction/setup based on rules
    void parameterSetup();
    /// Create options model
    void createOptionsModel(bool resetSettings);
    /// Create main window
    void createWindow(const NetworkStyle *networkStyle);
    /// Create splash screen
    void createSplashScreen(const NetworkStyle *networkStyle);

    /// Request core initialization
    void requestInitialize();
    /// Request core shutdown
    void requestShutdown();

    /// Get process return value
    int getReturnValue() { return returnValue; }

    /// Get window identifier of QMainWindow (FleetCreditsGUI)
    WId getMainWinId() const;

public Q_SLOTS:
    void initializeResult(int retval);
    void shutdownResult(int retval);
    /// Handle runaway exceptions. Shows a message box with the problem and quits the program.
    void handleRunawayException(const QString &message);

Q_SIGNALS:
    void requestedInitialize();
    void requestedShutdown();
    void stopThread();
    void splashFinished(QWidget *window);

private:
    QThread *coreThread;
    OptionsModel *optionsModel;
    ClientModel *clientModel;
    FleetCreditsGUI *window;
    QTimer *pollShutdownTimer;
#ifdef ENABLE_WALLET
    PaymentServer* paymentServer;
    WalletModel *walletModel;
#endif
    int returnValue;
    const PlatformStyle *platformStyle;
    std::unique_ptr<QWidget> shutdownWindow;

    void startThread();
};

#include "fleetcredits.moc"

FleetCreditsCore::FleetCreditsCore():
    QObject()
{
}

void FleetCreditsCore::handleRunawayException(const std::exception *e)
{
    PrintExceptionContinue(e, "Runaway exception");
    Q_EMIT runawayException(QString::fromStdString(GetWarnings("gui")));
}

void FleetCreditsCore::initialize()
{
    try
    {
        qDebug() << __func__ << ": Running AppInit2 in thread";
        
        // CRITICAL: Ensure dummy slots are connected before any signals are emitted
        // This is thread-safe - boost::signals2 signals are thread-safe
        // NOTE: We use empty lambdas here to prevent boost::signals2::no_slots_error
        // The splash screen ALSO subscribes to these signals, so both handlers will be called
        // This is fine - boost::signals2 calls ALL handlers, so splash screen will receive messages
        static std::once_flag slots_connected;
        std::call_once(slots_connected, []() {
            uiInterface.ThreadSafeMessageBox.connect([](const std::string&, const std::string&, unsigned int) { return false; });
            uiInterface.ThreadSafeQuestion.connect([](const std::string&, const std::string&, const std::string&, unsigned int) { return false; });
            // Empty handlers for InitMessage and ShowProgress - splash screen ALSO subscribes
            // Both will be called, so splash screen will show messages
            uiInterface.InitMessage.connect([](const std::string&){});
            uiInterface.ShowProgress.connect([](const std::string&, int){});
#ifdef ENABLE_WALLET
            uiInterface.LoadWallet.connect([](CWallet*){});
#endif
        });
        
        if (!AppInitBasicSetup())
        {
            Q_EMIT initializeResult(false);
            return;
        }
        if (!AppInitParameterInteraction())
        {
            Q_EMIT initializeResult(false);
            return;
        }
        if (!AppInitSanityChecks())
        {
            Q_EMIT initializeResult(false);
            return;
        }
        bool success = AppInitMain(threadGroup, scheduler);
        // Convert bool to int: true (success) = 0, false (failure) = 1
        // This matches the expected format: 0 = success, non-zero = failure
        int rv = success ? 0 : 1;
        Q_EMIT initializeResult(rv);
    } catch (const std::exception& e) {
        handleRunawayException(&e);
    } catch (...) {
        handleRunawayException(NULL);
    }
}

void FleetCreditsCore::shutdown()
{
    try
    {
        qDebug() << __func__ << ": Running Shutdown in thread";
        Interrupt(threadGroup);
        threadGroup.join_all();
        Shutdown();
        qDebug() << __func__ << ": Shutdown finished";
        Q_EMIT shutdownResult(1);
    } catch (const std::exception& e) {
        handleRunawayException(&e);
    } catch (...) {
        handleRunawayException(NULL);
    }
}

static int qt_argc = 1;
static const char* qt_argv = "fleetcredits-qt";

FleetCreditsApplication::FleetCreditsApplication():
    QApplication(qt_argc, const_cast<char **>(&qt_argv)),
    coreThread(0),
    optionsModel(0),
    clientModel(0),
    window(0),
    pollShutdownTimer(0),
#ifdef ENABLE_WALLET
    paymentServer(0),
    walletModel(0),
#endif
    returnValue(0)
{
    // CRITICAL: Connect dummy slots FIRST before ANY other initialization
    // This prevents boost::signals2::no_slots_error during background thread startup
    uiInterface.ThreadSafeMessageBox.connect([](const std::string&, const std::string&, unsigned int) { return false; });
    uiInterface.ThreadSafeQuestion.connect([](const std::string&, const std::string&, const std::string&, unsigned int) { return false; });
    uiInterface.InitMessage.connect([](const std::string&){});
    uiInterface.ShowProgress.connect([](const std::string&, int){});
#ifdef ENABLE_WALLET
    uiInterface.LoadWallet.connect([](CWallet*){});
#endif
    
    setQuitOnLastWindowClosed(false);

    // UI per-platform customization
    // This must be done inside the FleetCreditsApplication constructor, or after it, because
    // PlatformStyle::instantiate requires a QApplication
    std::string platformName;
    platformName = GetArg("-uiplatform", FleetCreditsGUI::DEFAULT_UIPLATFORM);
    platformStyle = PlatformStyle::instantiate(QString::fromStdString(platformName));
    if (!platformStyle) // Fall back to "other" if specified name not found
        platformStyle = PlatformStyle::instantiate("other");
    assert(platformStyle);
}

FleetCreditsApplication::~FleetCreditsApplication()
{
    if(coreThread)
    {
        qDebug() << __func__ << ": Stopping thread";
        Q_EMIT stopThread();
        coreThread->wait();
        qDebug() << __func__ << ": Stopped thread";
    }

    delete window;
    window = 0;
#ifdef ENABLE_WALLET
    delete paymentServer;
    paymentServer = 0;
#endif
    delete optionsModel;
    optionsModel = 0;
    delete platformStyle;
    platformStyle = 0;
}

#ifdef ENABLE_WALLET
void FleetCreditsApplication::createPaymentServer()
{
    paymentServer = new PaymentServer(this, true);
}
#endif

void FleetCreditsApplication::createOptionsModel(bool resetSettings)
{
    optionsModel = new OptionsModel(NULL, resetSettings);
}

void FleetCreditsApplication::createWindow(const NetworkStyle *networkStyle)
{
    window = new FleetCreditsGUI(platformStyle, networkStyle, 0);

    pollShutdownTimer = new QTimer(window);
    connect(pollShutdownTimer, SIGNAL(timeout()), window, SLOT(detectShutdown()));
    pollShutdownTimer->start(200);
}

void FleetCreditsApplication::createSplashScreen(const NetworkStyle *networkStyle)
{
    qDebug() << "DEBUG: FleetCreditsApplication::createSplashScreen called";
    SplashScreen *splash = new SplashScreen(0, networkStyle);
    qDebug() << "DEBUG: SplashScreen object created";
    // We don't hold a direct pointer to the splash screen after creation, but the splash
    // screen will take care of deleting itself when slotFinish happens.
    splash->show();
    qDebug() << "DEBUG: Splash screen shown, processing events...";
    QApplication::processEvents(); // Ensure splash screen is painted before continuing
    qDebug() << "DEBUG: Events processed";
    connect(this, SIGNAL(splashFinished(QWidget*)), splash, SLOT(slotFinish(QWidget*)));
    connect(this, SIGNAL(requestedShutdown()), splash, SLOT(close()));
    qDebug() << "DEBUG: Splash screen connections made";
}

void FleetCreditsApplication::startThread()
{
    if(coreThread)
        return;
    coreThread = new QThread(this);
    FleetCreditsCore *executor = new FleetCreditsCore();
    executor->moveToThread(coreThread);

    /*  communication to and from thread */
    connect(executor, SIGNAL(initializeResult(int)), this, SLOT(initializeResult(int)));
    connect(executor, SIGNAL(shutdownResult(int)), this, SLOT(shutdownResult(int)));
    connect(executor, SIGNAL(runawayException(QString)), this, SLOT(handleRunawayException(QString)));
    connect(this, SIGNAL(requestedInitialize()), executor, SLOT(initialize()));
    connect(this, SIGNAL(requestedShutdown()), executor, SLOT(shutdown()));
    /*  make sure executor object is deleted in its own thread */
    connect(this, SIGNAL(stopThread()), executor, SLOT(deleteLater()));
    connect(this, SIGNAL(stopThread()), coreThread, SLOT(quit()));

    coreThread->start();
}

void FleetCreditsApplication::parameterSetup()
{
    InitLogging();
    InitParameterInteraction();
}

void FleetCreditsApplication::requestInitialize()
{
    qDebug() << __func__ << ": Requesting initialize";
    startThread();
    Q_EMIT requestedInitialize();
}

void FleetCreditsApplication::requestShutdown()
{
    // Show a simple window indicating shutdown status
    // Do this first as some of the steps may take some time below,
    // for example the RPC console may still be executing a command.
    shutdownWindow.reset(ShutdownWindow::showShutdownWindow(window));

    qDebug() << __func__ << ": Requesting shutdown";
    startThread();
    window->hide();
    window->setClientModel(0);
    pollShutdownTimer->stop();

#ifdef ENABLE_WALLET
    window->removeAllWallets();
    delete walletModel;
    walletModel = 0;
#endif
    delete clientModel;
    clientModel = 0;

    StartShutdown();

    // Request shutdown from core thread
    Q_EMIT requestedShutdown();
}

void FleetCreditsApplication::initializeResult(int retval)
{
    qDebug() << __func__ << ": Initialization result: " << retval;
    // Set exit result: 0 if successful, 1 if failure
    // retval: 0 = success, 1 = failure (from AppInitMain bool conversion)
    returnValue = retval ? 1 : 0;
    if(retval == 0)  // Success
    {
        // Log this only after AppInitMain finishes, as then logging setup is guaranteed complete
        qWarning() << "Platform customization:" << platformStyle->getName();
        
        // CRITICAL: Wait a moment to ensure all initialization messages have been processed
        // This ensures splash screen shows "Done loading" before closing
        QApplication::processEvents();
        QThread::msleep(100); // Small delay to ensure final messages are displayed

        clientModel = new ClientModel(optionsModel);
        window->setClientModel(clientModel);

#ifdef ENABLE_WALLET
        if(pwalletMain)
        {
            walletModel = new WalletModel(platformStyle, pwalletMain, optionsModel);

            window->addWallet(FleetCreditsGUI::DEFAULT_WALLET, walletModel);
            window->setCurrentWallet(FleetCreditsGUI::DEFAULT_WALLET);
        } else {
            // No wallet loaded - check if wallet file exists and try to load it
            std::string walletFile = GetArg("-wallet", DEFAULT_WALLET_DAT);
            fs::path walletPath = GetDataDir() / walletFile;
            
            if (fs::exists(walletPath)) {
                // Wallet file exists but wasn't loaded - try to load it manually
                try {
                    uiInterface.SafeInitMessage(_("Loading existing wallet..."));
                    CWallet *walletInstance = CWallet::CreateWalletFromFile(walletFile);
                    
                    if (walletInstance) {
                        // Successfully loaded wallet
                        pwalletMain = walletInstance;
                        RegisterValidationInterface(walletInstance);
                        
                        walletModel = new WalletModel(platformStyle, walletInstance, optionsModel);
                        window->addWallet(FleetCreditsGUI::DEFAULT_WALLET, walletModel);
                        window->setCurrentWallet(FleetCreditsGUI::DEFAULT_WALLET);
                        
                        // Inform user wallet was loaded
                        QMessageBox::information(window,
                            FleetCreditsGUI::tr("Wallet Loaded"),
                            FleetCreditsGUI::tr("Your existing wallet has been loaded successfully."));
                    } else {
                        // Wallet file exists but couldn't be loaded - prompt to create new
                        QMessageBox::StandardButton reply = QMessageBox::question(window, 
                            FleetCreditsGUI::tr("Wallet Load Error"),
                            FleetCreditsGUI::tr("A wallet file exists but could not be loaded.\n\n"
                                               "This may be due to:\n"
                                               "- Corrupted wallet file\n"
                                               "- Wrong wallet format\n"
                                               "- Missing dependencies\n\n"
                                               "Would you like to create a new wallet?\n\n"
                                               "Warning: Creating a new wallet will not use the existing wallet file."),
                            QMessageBox::Yes | QMessageBox::No,
                            QMessageBox::No);
                        
                        if (reply == QMessageBox::Yes) {
                            // User wants to create new wallet - remove old one first
                            QMessageBox::StandardButton confirmDelete = QMessageBox::question(window,
                                FleetCreditsGUI::tr("Replace Wallet File"),
                                FleetCreditsGUI::tr("This will remove the existing wallet file and create a new one.\n\n"
                                                   "Are you sure you want to continue?"),
                                QMessageBox::Yes | QMessageBox::No,
                                QMessageBox::No);
                            
                            if (confirmDelete == QMessageBox::Yes) {
                                // Backup old wallet first
                                fs::path backupPath = walletPath;
                                backupPath.replace_extension(".dat.backup");
                                try {
                                    fs::copy_file(walletPath, backupPath);
                                } catch (...) {
                                    // Ignore backup errors
                                }
                                
                                // Remove old wallet
                                fs::remove(walletPath);
                                
                                // Create new wallet
                                uiInterface.SafeInitMessage(_("Creating wallet..."));
                                CWallet *walletInstance = CWallet::CreateWalletFromFile(walletFile);
                                
                                if (walletInstance) {
                                    pwalletMain = walletInstance;
                                    RegisterValidationInterface(walletInstance);
                                    walletModel = new WalletModel(platformStyle, walletInstance, optionsModel);
                                    window->addWallet(FleetCreditsGUI::DEFAULT_WALLET, walletModel);
                                    window->setCurrentWallet(FleetCreditsGUI::DEFAULT_WALLET);
                                    
                                    QMessageBox::information(window,
                                        FleetCreditsGUI::tr("Wallet Created"),
                                        FleetCreditsGUI::tr("A new wallet has been created successfully.\n\n"
                                                           "The old wallet file has been backed up to:\n%1")
                                            .arg(QString::fromStdString(backupPath.string())));
                                } else {
                                    QMessageBox::critical(window,
                                        FleetCreditsGUI::tr("Wallet Creation Failed"),
                                        FleetCreditsGUI::tr("Failed to create wallet. Please check the debug window for details."));
                                }
                            }
                        }
                    }
                } catch (const std::exception& e) {
                    // Error loading wallet - show prompt
                    QMessageBox::StandardButton reply = QMessageBox::question(window, 
                        FleetCreditsGUI::tr("No Wallet Loaded"),
                        FleetCreditsGUI::tr("Could not load existing wallet:\n%1\n\n"
                                           "Would you like to create a new wallet?")
                            .arg(QString::fromStdString(e.what())),
                        QMessageBox::Yes | QMessageBox::No,
                        QMessageBox::Yes);
                    
                    if (reply == QMessageBox::Yes) {
                        // Same logic as the "Yes" branch below
                        try {
                            std::string walletFile = GetArg("-wallet", DEFAULT_WALLET_DAT);
                            fs::path walletPath = GetDataDir() / walletFile;
                            if (fs::exists(walletPath)) {
                                QMessageBox::warning(window,
                                    FleetCreditsGUI::tr("Wallet File Exists"),
                                    FleetCreditsGUI::tr("A wallet file already exists at:\n%1\n\nPlease remove it first if you want to create a new wallet.")
                                        .arg(QString::fromStdString(walletPath.string())));
                                return;
                            }

                            uiInterface.SafeInitMessage(_("Creating wallet..."));
                            CWallet *walletInstance = CWallet::CreateWalletFromFile(walletFile);
                            
                            if (!walletInstance) {
                                QMessageBox::critical(window,
                                    FleetCreditsGUI::tr("Wallet Creation Failed"),
                                    FleetCreditsGUI::tr("Failed to create wallet. Please check the debug window for details."));
                                return;
                            }

                            pwalletMain = walletInstance;
                            RegisterValidationInterface(walletInstance);

                            if (walletModel) {
                                delete walletModel;
                            }
                            walletModel = new WalletModel(platformStyle, walletInstance, optionsModel);

                            window->addWallet(FleetCreditsGUI::DEFAULT_WALLET, walletModel);
                            window->setCurrentWallet(FleetCreditsGUI::DEFAULT_WALLET);
                            
                            QMessageBox::information(window,
                                FleetCreditsGUI::tr("Wallet Created"),
                                FleetCreditsGUI::tr("Your new wallet has been created successfully!\n\n"
                                                   "Please make sure to backup your wallet file."));
                        } catch (const std::exception& e2) {
                            QMessageBox::critical(window,
                                FleetCreditsGUI::tr("Wallet Creation Error"),
                                FleetCreditsGUI::tr("Error creating wallet: %1").arg(QString::fromStdString(e2.what())));
                        }
                    }
                }
            } else {
                // No wallet file exists - prompt user to create one
                QMessageBox::StandardButton reply = QMessageBox::question(window, 
                    FleetCreditsGUI::tr("No Wallet Found"),
                    FleetCreditsGUI::tr("No wallet has been loaded.\n\n"
                                       "Would you like to create a new wallet?\n\n"
                                       "Note: You can also create a wallet from the File menu."),
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::Yes);
                
                if (reply == QMessageBox::Yes) {
                    // Create wallet immediately without restart
                    try {
                        // Get wallet file path
                        std::string walletFile = GetArg("-wallet", DEFAULT_WALLET_DAT);
                        
                        // Check if wallet file already exists
                        fs::path walletPath = GetDataDir() / walletFile;
                        if (fs::exists(walletPath)) {
                            QMessageBox::warning(window,
                                FleetCreditsGUI::tr("Wallet File Exists"),
                                FleetCreditsGUI::tr("A wallet file already exists at:\n%1\n\nPlease remove it first if you want to create a new wallet.")
                                    .arg(QString::fromStdString(walletPath.string())));
                            return;
                        }

                        // Create wallet using CreateWalletFromFile
                        uiInterface.SafeInitMessage(_("Creating wallet..."));
                        CWallet *walletInstance = CWallet::CreateWalletFromFile(walletFile);
                        
                        if (!walletInstance) {
                            QMessageBox::critical(window,
                                FleetCreditsGUI::tr("Wallet Creation Failed"),
                                FleetCreditsGUI::tr("Failed to create wallet. Please check the debug window for details."));
                            return;
                        }

                        // Set pwalletMain globally
                        pwalletMain = walletInstance;

                        // Register wallet with validation interface
                        RegisterValidationInterface(walletInstance);

                        // Create wallet model and add to GUI
                        if (walletModel) {
                            delete walletModel; // Clean up old model if exists
                        }
                        walletModel = new WalletModel(platformStyle, walletInstance, optionsModel);

                        window->addWallet(FleetCreditsGUI::DEFAULT_WALLET, walletModel);
                        window->setCurrentWallet(FleetCreditsGUI::DEFAULT_WALLET);
                        
                        QMessageBox::information(window,
                            FleetCreditsGUI::tr("Wallet Created"),
                            FleetCreditsGUI::tr("Your new wallet has been created successfully!\n\n"
                                               "Please make sure to backup your wallet file."));
                    } catch (const std::exception& e) {
                        QMessageBox::critical(window,
                            FleetCreditsGUI::tr("Wallet Creation Error"),
                            FleetCreditsGUI::tr("Error creating wallet: %1").arg(QString::fromStdString(e.what())));
                    }
                }
            }
        }
#endif

        // If -min option passed, start window minimized.
        if(GetBoolArg("-min", false))
        {
            window->showMinimized();
        }
        else
        {
            window->show();
        }
        Q_EMIT splashFinished(window);

#ifdef ENABLE_WALLET
        paymentServer->setOptionsModel(optionsModel);

        // Now that initialization/startup is done, process any command-line
        // payment requests:
        connect(paymentServer, SIGNAL(receivedPaymentRequest(SendCoinsRecipient)),
                          window, SLOT(handlePaymentRequest(SendCoinsRecipient)));
        // Now that initialization/startup is done, process any command-line fleetcredits: URIs
        connect(window, SIGNAL(receivedURI(QString)),
                         paymentServer, SLOT(handleURIOrFile(QString)));
        connect(paymentServer, SIGNAL(message(QString,QString,unsigned int)),
                         window, SLOT(message(QString,QString,unsigned int)));
        QTimer::singleShot(100, paymentServer, SLOT(uiReady()));
#endif
    } else {
        quit(); // Exit main loop on failure
    }
}

void FleetCreditsApplication::shutdownResult(int retval)
{
    qDebug() << __func__ << ": Shutdown result: " << retval;
    quit(); // Exit main loop after shutdown finished
}

void FleetCreditsApplication::handleRunawayException(const QString &message)
{
    QMessageBox::critical(0, FleetCreditsGUI::tr("Runaway exception"), FleetCreditsGUI::tr("A fatal error occurred. Fleet Credits can no longer continue safely and will quit.") + QString("\n\n") + message);
    ::exit(EXIT_FAILURE);
}

WId FleetCreditsApplication::getMainWinId() const
{
    if (!window)
        return 0;

    return window->winId();
}

#ifndef FLEETCREDITS_QT_TEST
MAIN_FUNCTION
{
    SetupEnvironment();

    /// 1. Parse command-line options. These take precedence over anything else.
    // Command-line options take precedence:
    ParseParameters(argc, argv);

    // Do not refer to data directory yet, this can be overridden by Intro::pickDataDirectory

    /// 2. Basic Qt initialization (not dependent on parameters or configuration)
    Q_INIT_RESOURCE(fleetcredits);
    Q_INIT_RESOURCE(fleetcredits_locale);

#if QT_VERSION > 0x050100
    // Generate high-dpi pixmaps
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
#if QT_VERSION >= 0x050600
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#ifdef Q_OS_MAC
    QApplication::setAttribute(Qt::AA_DontShowIconsInMenus);
#endif

    FleetCreditsApplication app;

    // Register meta types used for QMetaObject::invokeMethod
    qRegisterMetaType< bool* >();
    //   Need to pass name here as CAmount is a typedef (see http://qt-project.org/doc/qt-5/qmetatype.html#qRegisterMetaType)
    //   IMPORTANT if it is no longer a typedef use the normal variant above
    qRegisterMetaType< CAmount >("CAmount");

    /// 3. Application identification
    // must be set before OptionsModel is initialized or translations are loaded,
    // as it is used to locate QSettings
    QApplication::setOrganizationName(QAPP_ORG_NAME);
    QApplication::setOrganizationDomain(QAPP_ORG_DOMAIN);
    QApplication::setApplicationName(QAPP_APP_NAME_DEFAULT);
    GUIUtil::SubstituteFonts(GetLangTerritory());

    /// 4. Initialization of translations, so that intro dialog is in user's language
    // Now that QSettings are accessible, initialize translations
    QTranslator qtTranslatorBase, qtTranslator, translatorBase, translator;
    initTranslations(qtTranslatorBase, qtTranslator, translatorBase, translator);
    translationInterface.Translate.connect(Translate);
    
    // CRITICAL: Connect dummy slots to boost signals IMMEDIATELY to prevent no_slots_error
    // This must happen before any initialization threads start emitting signals
    uiInterface.InitMessage.connect([](const std::string&){});
    uiInterface.ShowProgress.connect([](const std::string&, int){});
    uiInterface.ThreadSafeMessageBox.connect([](const std::string&, const std::string&, unsigned int) { return false; });
    uiInterface.ThreadSafeQuestion.connect([](const std::string&, const std::string&, const std::string&, unsigned int) { return false; });
#ifdef ENABLE_WALLET
    uiInterface.LoadWallet.connect([](CWallet*){});
#endif

    // Show help message immediately after parsing command-line options (for "-lang") and setting locale,
    // but before showing splash screen.
    if (IsArgSet("-?") || IsArgSet("-h") || IsArgSet("-help") || IsArgSet("-version"))
    {
        HelpMessageDialog help(NULL, IsArgSet("-version"));
        help.showOrPrint();
        return EXIT_SUCCESS;
    }

    /// 5. Now that settings and translations are available, ask user for data directory
    // User language is set up: pick a data directory
    if (!Intro::pickDataDirectory())
        return EXIT_SUCCESS;

    /// 6. Determine availability of data directory and parse fleetcredits.conf
    /// - Do not call GetDataDir(true) before this step finishes
    fs::path dataDir = GetDataDir(false);
    if (!fs::is_directory(dataDir))
    {
        // Try to create the data directory if it doesn't exist
        try {
            fs::create_directories(dataDir);
            LogPrintf("Created data directory: %s\n", dataDir.string());
        } catch (const fs::filesystem_error& e) {
            QMessageBox::critical(0, QObject::tr(PACKAGE_NAME),
                                  QObject::tr("Error: Specified data directory \"%1\" does not exist and could not be created: %2")
                                      .arg(QString::fromStdString(GetArg("-datadir", "")))
                                      .arg(QString::fromStdString(e.what())));
            return EXIT_FAILURE;
        }
    }
    try {
        ReadConfigFile(GetArg("-conf", FLEETCREDITS_CONF_FILENAME));
    } catch (const std::exception& e) {
        QMessageBox::critical(0, QObject::tr(PACKAGE_NAME),
                              QObject::tr("Error: Cannot parse configuration file: %1. Only use key=value syntax.").arg(e.what()));
        return EXIT_FAILURE;
    }

    /// 7. Determine network (and switch to network specific options)
    // - Do not call Params() before this step
    // - Do this after parsing the configuration file, as the network can be switched there
    // - QSettings() will use the new application name after this, resulting in network-specific settings
    // - Needs to be done before createOptionsModel

    // Check for -testnet or -regtest parameter (Params() calls are only valid after this clause)
    try {
        SelectParams(ChainNameFromCommandLine());
    } catch(std::exception &e) {
        QMessageBox::critical(0, QObject::tr(PACKAGE_NAME), QObject::tr("Error: %1").arg(e.what()));
        return EXIT_FAILURE;
    }
#ifdef ENABLE_WALLET
    // Parse URIs on command line -- this can affect Params()
    PaymentServer::ipcParseCommandLine(argc, argv);
#endif

    QScopedPointer<const NetworkStyle> networkStyle(NetworkStyle::instantiate(QString::fromStdString(Params().NetworkIDString())));
    assert(!networkStyle.isNull());
    // Allow for separate UI settings for testnets
    QApplication::setApplicationName(networkStyle->getAppName());
    // Re-initialize translations after changing application name (language in network-specific settings can be different)
    initTranslations(qtTranslatorBase, qtTranslator, translatorBase, translator);

#ifdef ENABLE_WALLET
    /// 8. URI IPC sending
    // - Do this early as we don't want to bother initializing if we are just calling IPC
    // - Do this *after* setting up the data directory, as the data directory hash is used in the name
    // of the server.
    // - Do this after creating app and setting up translations, so errors are
    // translated properly.
    if (PaymentServer::ipcSendCommandLine())
        exit(EXIT_SUCCESS);

    // Start up the payment server early, too, so impatient users that click on
    // fleetcredits: links repeatedly have their payment requests routed to this process:
    app.createPaymentServer();
#endif

    /// 9. Main GUI initialization
    // Install global event filter that makes sure that long tooltips can be word-wrapped
    app.installEventFilter(new GUIUtil::ToolTipToRichTextFilter(TOOLTIP_WRAP_THRESHOLD, &app));
#if defined(Q_OS_WIN)
    // Install global event filter for processing Windows session related Windows messages (WM_QUERYENDSESSION and WM_ENDSESSION)
    qApp->installNativeEventFilter(new WinShutdownMonitor());
#endif
    // Install qDebug() message handler to route to debug.log
    qInstallMessageHandler(DebugMessageHandler);
    // Allow parameter interaction before we create the options model
    app.parameterSetup();
    // Load GUI settings from QSettings
    app.createOptionsModel(IsArgSet("-resetguisettings"));

    // CRITICAL: Create splash screen FIRST before connecting InitMessage handler
    // This ensures splash screen receives all initialization messages
    if (GetBoolArg("-splash", DEFAULT_SPLASHSCREEN) && !GetBoolArg("-min", false)) {
        qDebug() << "DEBUG: Creating splash screen...";
        app.createSplashScreen(networkStyle.data());
        qDebug() << "DEBUG: Splash screen created";
    } else {
        qDebug() << "DEBUG: Splash screen disabled or minimized mode";
    }

    // Subscribe to global signals from core - real connections
    // Note: Splash screen already connected its handlers, so this will also log to console
    // Dummy slots are already connected in FleetCreditsApplication constructor
    uiInterface.InitMessage.connect(InitMessage);

    try
    {
        app.createWindow(networkStyle.data());
        app.requestInitialize();
#if defined(Q_OS_WIN)
        WinShutdownMonitor::registerShutdownBlockReason(QObject::tr("%1 didn't yet exit safely...").arg(QObject::tr(PACKAGE_NAME)), (HWND)app.getMainWinId());
#endif
        app.exec();
        app.requestShutdown();
        app.exec();
    } catch (const std::exception& e) {
        PrintExceptionContinue(&e, "Runaway exception");
        app.handleRunawayException(QString::fromStdString(GetWarnings("gui")));
    } catch (...) {
        PrintExceptionContinue(NULL, "Runaway exception");
        app.handleRunawayException(QString::fromStdString(GetWarnings("gui")));
    }
    return app.getReturnValue();
}
#endif // FLEETCREDITS_QT_TEST
