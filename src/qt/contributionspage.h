// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FLEETCREDITS_QT_CONTRIBUTIONSPAGE_H
#define FLEETCREDITS_QT_CONTRIBUTIONSPAGE_H

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
QT_END_NAMESPACE

namespace Ui {
    class ContributionsPage;
}

/**
 * Contributions Page - Submit and track contributions
 * 
 * Features:
 * - Submit contributions (all 8 types)
 * - View contribution status
 * - Track rewards
 * - Filter and search contributions
 */
class ContributionsPage : public QWidget
{
    Q_OBJECT

public:
    explicit ContributionsPage(QWidget *parent = 0);
    ~ContributionsPage();

    void setClientModel(ClientModel *clientModel);
    void setWalletModel(WalletModel *walletModel);

public Q_SLOTS:
    void refreshContributions();
    void updateRewards();

private Q_SLOTS:
    void onContribTypeChanged();
    void onSubmitClicked();
    void onContributionClicked(const QModelIndex &index);

private:
    Ui::ContributionsPage *ui;
    ClientModel *clientModel;
    WalletModel *walletModel;
};

#endif // FLEETCREDITS_QT_CONTRIBUTIONSPAGE_H

