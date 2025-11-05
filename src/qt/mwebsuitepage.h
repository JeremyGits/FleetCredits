// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FLEETCREDITS_QT_MWEBSUITEPAGE_H
#define FLEETCREDITS_QT_MWEBSUITEPAGE_H

#include <QWidget>
#include <QModelIndex>

class WalletModel;
class ClientModel;

QT_BEGIN_NAMESPACE
class QTableView;
class QLineEdit;
class QPushButton;
class QTextEdit;
class QLabel;
class QComboBox;
QT_END_NAMESPACE

namespace Ui {
    class MWEBSuitePage;
}

/**
 * MWEB Suite Page - Complete Mimblewimble Extension Blocks interface
 * 
 * Features:
 * - Peg-in (Main Chain → MWEB)
 * - Peg-out (MWEB → Main Chain)
 * - MWEB transaction history
 * - View keys management
 * - Contribution routing to MWEB
 */
class MWEBSuitePage : public QWidget
{
    Q_OBJECT

public:
    explicit MWEBSuitePage(QWidget *parent = 0);
    ~MWEBSuitePage();

    void setClientModel(ClientModel *clientModel);
    void setWalletModel(WalletModel *walletModel);

public Q_SLOTS:
    void refreshMWEBTransactions();

private Q_SLOTS:
    void onPegInClicked();
    void onPegOutClicked();
    void onExportViewKeysClicked();
    void updateDisplayUnit();

private:
    Ui::MWEBSuitePage *ui;
    ClientModel *clientModel;
    WalletModel *walletModel;
};

#endif // FLEETCREDITS_QT_MWEBSUITEPAGE_H

