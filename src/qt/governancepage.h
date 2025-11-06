// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FLEETCREDITS_QT_GOVERNANCEPAGE_H
#define FLEETCREDITS_QT_GOVERNANCEPAGE_H

#include <QWidget>
#include <QModelIndex>

class WalletModel;
class ClientModel;

QT_BEGIN_NAMESPACE
class QComboBox;
class QLineEdit;
class QTextEdit;
class QTableView;
class QPushButton;
class QLabel;
class QStandardItemModel;
class QTabWidget;
QT_END_NAMESPACE

/**
 * Governance Page - Submit proposals and vote
 * 
 * Features:
 * - Submit governance proposals
 * - View and vote on proposals
 * - Track proposal status and voting results
 */
class GovernancePage : public QWidget
{
    Q_OBJECT

public:
    explicit GovernancePage(QWidget *parent = 0);
    ~GovernancePage();

    void setClientModel(ClientModel *clientModel);
    void setWalletModel(WalletModel *walletModel);

public Q_SLOTS:
    void refreshProposals();

private Q_SLOTS:
    void onProposalTypeChanged();
    void onSubmitProposalClicked();
    void onProposalClicked(const QModelIndex &index);
    void onVoteClicked();
    void onRefreshClicked();

private:
    ClientModel *clientModel;
    WalletModel *walletModel;
    QStandardItemModel *proposalsModel;
    
    // UI elements (created programmatically)
    QTabWidget *tabWidget;
    QWidget *proposalsTab;
    QWidget *submitTab;
    
    // Proposals tab
    QTableView *proposalsTable;
    QPushButton *refreshButton;
    QPushButton *voteButton;
    QLabel *selectedProposalLabel;
    
    // Submit tab
    QComboBox *proposalTypeCombo;
    QLineEdit *titleInput;
    QTextEdit *descriptionInput;
    QTextEdit *proposalDataInput;
    QPushButton *submitButton;
    
    void setupUI();
    void setupProposalsTable();
    void refreshProposalsList();
};

#endif // FLEETCREDITS_QT_GOVERNANCEPAGE_H

