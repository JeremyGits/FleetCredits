# Fleet Credits Qt Client - Complete qmake Project
# Standalone Qt client linking to Fleet Credits core libraries
# This includes ALL functionality from Bitcoin/Dogecoin Qt clients

QT += core gui widgets network dbus printsupport

CONFIG += c++11
CONFIG += warn_on

TARGET = fleetcredits-qt
TEMPLATE = app

# Output directories
DESTDIR = ../bin
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui

# Defines
DEFINES += HAVE_CONFIG_H
# Note: ENABLE_WALLET and ENABLE_ZMQ are defined in config/fleetcredits-config.h
# Only define them here if they're not already in the config header
# QT_STATICPLUGIN is not needed for dynamic Qt builds

# Include paths
INCLUDEPATH += ../src
INCLUDEPATH += ../src/qt
INCLUDEPATH += ../src/qt/forms
INCLUDEPATH += ../src/qt/themes
INCLUDEPATH += ../src/leveldb/include
INCLUDEPATH += ../src/leveldb/helpers
INCLUDEPATH += ../src/secp256k1/include
INCLUDEPATH += ../src/univalue/include
INCLUDEPATH += ../build/src
INCLUDEPATH += ../build/src/config

# Main entry point - use existing fleetcredits.cpp (has ALL original functionality)
SOURCES += ../src/qt/fleetcredits.cpp

# Core Qt Application Components
SOURCES += ../src/qt/fleetcreditsgui.cpp \
    ../src/qt/clientmodel.cpp \
    ../src/qt/networkstyle.cpp \
    ../src/qt/notificator.cpp \
    ../src/qt/platformstyle.cpp \
    ../src/qt/splashscreen.cpp \
    ../src/qt/utilitydialog.cpp \
    ../src/qt/rpcconsole.cpp \
    ../src/qt/intro.cpp \
    ../src/qt/guiutil.cpp \
    ../src/qt/fleetcreditsunits.cpp \
    ../src/qt/fleetcreditsaddressvalidator.cpp \
    ../src/qt/fleetcreditsamountfield.cpp \
    ../src/qt/optionsmodel.cpp \
    ../src/qt/optionsdialog.cpp \
    ../src/qt/qvalidatedlineedit.cpp \
    ../src/qt/qvaluecombobox.cpp \
    ../src/qt/trafficgraphwidget.cpp \
    ../src/qt/modaloverlay.cpp \
    ../src/qt/peertablemodel.cpp \
    ../src/qt/bantablemodel.cpp \
    ../src/qt/csvmodelwriter.cpp \
    ../src/qt/peerdialog.cpp \
    ../src/qt/verticallabel.cpp \
    ../src/qt/openuridialog.cpp \
    ../src/qt/signverifymessagedialog.cpp \
    ../src/qt/fleetcreditsstrings.cpp

# Modern UI Components
SOURCES += ../src/qt/modernmainwindow.cpp \
    ../src/qt/modernoverviewpage.cpp \
    ../src/qt/modernnavigation.cpp \
    ../src/qt/moderncard.cpp \
    ../src/qt/thememanager.cpp \
    ../src/qt/themeswitcher.cpp \
    ../src/qt/themes/theme_manager.cpp \
    ../src/qt/customthemes.cpp

# Wallet Components (ALL original functionality)
SOURCES += ../src/qt/walletframe.cpp \
    ../src/qt/walletview.cpp \
    ../src/qt/walletmodel.cpp \
    ../src/qt/walletmodeltransaction.cpp \
    ../src/qt/overviewpage.cpp \
    ../src/qt/sendcoinsdialog.cpp \
    ../src/qt/sendcoinsentry.cpp \
    ../src/qt/receivecoinsdialog.cpp \
    ../src/qt/receiverequestdialog.cpp \
    ../src/qt/addressbookpage.cpp \
    ../src/qt/addresstablemodel.cpp \
    ../src/qt/editaddressdialog.cpp \
    ../src/qt/transactionview.cpp \
    ../src/qt/transactiontablemodel.cpp \
    ../src/qt/transactionfilterproxy.cpp \
    ../src/qt/transactionrecord.cpp \
    ../src/qt/transactiondesc.cpp \
    ../src/qt/transactiondescdialog.cpp \
    ../src/qt/recentrequeststablemodel.cpp \
    ../src/qt/coincontroldialog.cpp \
    ../src/qt/coincontroltreewidget.cpp \
    ../src/qt/askpassphrasedialog.cpp \
    ../src/qt/importkeysdialog.cpp \
    ../src/qt/paymentserver.cpp

# Fleet Credits Custom Pages
SOURCES += ../src/qt/mwebsuitepage.cpp \
    ../src/qt/contributionspage.cpp

# Platform-specific components
linux-g++ {
    SOURCES += ../src/qt/winshutdownmonitor.cpp
}

macx {
    SOURCES += ../src/qt/macdockiconhandler.mm
    SOURCES += ../src/qt/macnotificationhandler.mm
}

win32 {
    SOURCES += ../src/qt/winshutdownmonitor.cpp
    RC_FILE = ../src/qt/res/fleetcredits-qt-res.rc
}

# Headers (for MOC - ALL original headers)
HEADERS += ../src/qt/fleetcreditsgui.h \
    ../src/qt/clientmodel.h \
    ../src/qt/networkstyle.h \
    ../src/qt/notificator.h \
    ../src/qt/platformstyle.h \
    ../src/qt/splashscreen.h \
    ../src/qt/utilitydialog.h \
    ../src/qt/rpcconsole.h \
    ../src/qt/intro.h \
    ../src/qt/guiutil.h \
    ../src/qt/guiconstants.h \
    ../src/qt/fleetcreditsunits.h \
    ../src/qt/fleetcreditsaddressvalidator.h \
    ../src/qt/fleetcreditsamountfield.h \
    ../src/qt/optionsmodel.h \
    ../src/qt/optionsdialog.h \
    ../src/qt/qvalidatedlineedit.h \
    ../src/qt/qvaluecombobox.h \
    ../src/qt/trafficgraphwidget.h \
    ../src/qt/modaloverlay.h \
    ../src/qt/peertablemodel.h \
    ../src/qt/bantablemodel.h \
    ../src/qt/csvmodelwriter.h \
    ../src/qt/peerdialog.h \
    ../src/qt/verticallabel.h \
    ../src/qt/openuridialog.h \
    ../src/qt/signverifymessagedialog.h \
    ../src/qt/modernmainwindow.h \
    ../src/qt/modernoverviewpage.h \
    ../src/qt/modernnavigation.h \
    ../src/qt/moderncard.h \
    ../src/qt/thememanager.h \
    ../src/qt/themeswitcher.h \
    ../src/qt/themes/theme_manager.h \
    ../src/qt/customthemes.h \
    ../src/qt/walletframe.h \
    ../src/qt/walletview.h \
    ../src/qt/walletmodel.h \
    ../src/qt/walletmodeltransaction.h \
    ../src/qt/overviewpage.h \
    ../src/qt/sendcoinsdialog.h \
    ../src/qt/sendcoinsentry.h \
    ../src/qt/receivecoinsdialog.h \
    ../src/qt/receiverequestdialog.h \
    ../src/qt/addressbookpage.h \
    ../src/qt/addresstablemodel.h \
    ../src/qt/editaddressdialog.h \
    ../src/qt/transactionview.h \
    ../src/qt/transactiontablemodel.h \
    ../src/qt/transactionfilterproxy.h \
    ../src/qt/transactionrecord.h \
    ../src/qt/transactiondesc.h \
    ../src/qt/transactiondescdialog.h \
    ../src/qt/recentrequeststablemodel.h \
    ../src/qt/coincontroldialog.h \
    ../src/qt/coincontroltreewidget.h \
    ../src/qt/askpassphrasedialog.h \
    ../src/qt/importkeysdialog.h \
    ../src/qt/paymentserver.h \
    ../src/qt/mwebsuitepage.h \
    ../src/qt/contributionspage.h

# UI Forms (ALL original forms from Bitcoin/Dogecoin)
FORMS += ../src/qt/forms/intro.ui \
    ../src/qt/forms/overviewpage.ui \
    ../src/qt/forms/sendcoinsdialog.ui \
    ../src/qt/forms/sendcoinsentry.ui \
    ../src/qt/forms/receivecoinsdialog.ui \
    ../src/qt/forms/addressbookpage.ui \
    ../src/qt/forms/askpassphrasedialog.ui \
    ../src/qt/forms/coincontroldialog.ui \
    ../src/qt/forms/editaddressdialog.ui \
    ../src/qt/forms/optionsdialog.ui \
    ../src/qt/forms/debugwindow.ui \
    ../src/qt/forms/receiverequestdialog.ui \
    ../src/qt/forms/signverifymessagedialog.ui \
    ../src/qt/forms/transactiondescdialog.ui \
    ../src/qt/forms/openuridialog.ui \
    ../src/qt/forms/importkeysdialog.ui \
    ../src/qt/forms/modaloverlay.ui \
    ../src/qt/forms/helpmessagedialog.ui \
    ../src/qt/forms/addpeerdialog.ui \
    ../src/qt/forms/testpeerdialog.ui \
    ../src/qt/forms/paperwalletdialog.ui \
    ../src/qt/forms/mwebsuitepage.ui \
    ../src/qt/forms/contributionspage.ui

# Resources (icons, translations, styles)
RESOURCES += ../src/qt/fleetcredits.qrc \
    ../src/qt/fleetcredits_locale.qrc

# Link against Fleet Credits core libraries (built via autotools)
LIBS += -L../src -lfleetcredits_server
LIBS += -L../src -lfleetcredits_wallet
LIBS += -L../src -lfleetcredits_common
LIBS += -L../src -lfleetcredits_util
LIBS += -L../src -lfleetcredits_consensus
LIBS += -L../src -lfleetcredits_cli
LIBS += -L../src/crypto -lfleetcredits_crypto
LIBS += -L../src/secp256k1/.libs -lsecp256k1
LIBS += -L../src/univalue/.libs -lunivalue
LIBS += -L../src/leveldb -lleveldb -lmemenv

# System libraries
LIBS += -lboost_system \
    -lboost_filesystem \
    -lboost_thread \
    -lboost_chrono \
    -lboost_program_options
# Note: boost_signals2 is header-only, no library needed

LIBS += -lssl -lcrypto
LIBS += -ldb_cxx
LIBS += -levent -levent_pthreads
LIBS += -lminiupnpc
LIBS += -lqrencode
LIBS += -lz
LIBS += -lpthread
LIBS += -ldl

# Static Qt plugins (if needed)
linux-g++ {
    DEFINES += QT_QPA_PLATFORM_XCB
}

win32 {
    DEFINES += QT_QPA_PLATFORM_WINDOWS
}

macx {
    DEFINES += QT_QPA_PLATFORM_COCOA
}
