// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "governancepage.h"

#include "guiutil.h"
#include "walletmodel.h"
#include "optionsmodel.h"
#include "clientmodel.h"
#include "fleetcreditsunits.h"
#include "rpc/server.h"
#include "rpc/protocol.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QDateTime>
#include <QTimer>
#include <QAbstractItemView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QTabWidget>
#include <QTableView>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QInputDialog>

GovernancePage::GovernancePage(QWidget *parent) :
    QWidget(parent),
    clientModel(0),
    walletModel(0),
    proposalsModel(0)
{
    setupUI();
}

GovernancePage::~GovernancePage()
{
    delete proposalsModel;
}

void GovernancePage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);
    
    // Create tab widget
    tabWidget = new QTabWidget(this);
    
    // Proposals Tab
    proposalsTab = new QWidget();
    QVBoxLayout *proposalsLayout = new QVBoxLayout(proposalsTab);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    refreshButton = new QPushButton(tr("Refresh"), this);
    voteButton = new QPushButton(tr("Vote"), this);
    voteButton->setEnabled(false);
    selectedProposalLabel = new QLabel(tr("No proposal selected"), this);
    buttonLayout->addWidget(selectedProposalLabel);
    buttonLayout->addStretch();
    buttonLayout->addWidget(refreshButton);
    buttonLayout->addWidget(voteButton);
    
    proposalsTable = new QTableView(this);
    setupProposalsTable();
    
    proposalsLayout->addLayout(buttonLayout);
    proposalsLayout->addWidget(proposalsTable);
    
    // Submit Tab
    submitTab = new QWidget();
    QVBoxLayout *submitLayout = new QVBoxLayout(submitTab);
    
    QFormLayout *formLayout = new QFormLayout();
    
    proposalTypeCombo = new QComboBox(this);
    proposalTypeCombo->addItem(tr("Parameter Change"), "PARAMETER_CHANGE");
    proposalTypeCombo->addItem(tr("Reserve Spending"), "RESERVE_SPENDING");
    proposalTypeCombo->addItem(tr("Oracle Election"), "ORACLE_ELECTION");
    proposalTypeCombo->addItem(tr("Feature Add"), "FEATURE_ADD");
    proposalTypeCombo->addItem(tr("Feature Remove"), "FEATURE_REMOVE");
    proposalTypeCombo->addItem(tr("Emergency"), "EMERGENCY");
    formLayout->addRow(tr("Proposal Type:"), proposalTypeCombo);
    
    titleInput = new QLineEdit(this);
    titleInput->setPlaceholderText(tr("Enter proposal title"));
    formLayout->addRow(tr("Title:"), titleInput);
    
    descriptionInput = new QTextEdit(this);
    descriptionInput->setPlaceholderText(tr("Enter full proposal description"));
    descriptionInput->setMinimumHeight(150);
    formLayout->addRow(tr("Description:"), descriptionInput);
    
    proposalDataInput = new QTextEdit(this);
    proposalDataInput->setPlaceholderText(tr("Enter proposal data as JSON (e.g., {\"amount\": 1000, \"address\": \"...\"})"));
    proposalDataInput->setMinimumHeight(100);
    formLayout->addRow(tr("Proposal Data (JSON):"), proposalDataInput);
    
    submitButton = new QPushButton(tr("Submit Proposal"), this);
    
    submitLayout->addLayout(formLayout);
    submitLayout->addStretch();
    submitLayout->addWidget(submitButton);
    
    // Add tabs
    tabWidget->addTab(proposalsTab, tr("Proposals"));
    tabWidget->addTab(submitTab, tr("Submit Proposal"));
    
    mainLayout->addWidget(tabWidget);
    
    // Connect signals
    connect(refreshButton, SIGNAL(clicked()), this, SLOT(onRefreshClicked()));
    connect(voteButton, SIGNAL(clicked()), this, SLOT(onVoteClicked()));
    connect(proposalsTable, SIGNAL(clicked(QModelIndex)), this, SLOT(onProposalClicked(QModelIndex)));
    connect(submitButton, SIGNAL(clicked()), this, SLOT(onSubmitProposalClicked()));
    connect(proposalTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onProposalTypeChanged()));
}

void GovernancePage::setupProposalsTable()
{
    proposalsModel = new QStandardItemModel(0, 7, this);
    proposalsModel->setHeaderData(0, Qt::Horizontal, tr("Proposal ID"), Qt::DisplayRole);
    proposalsModel->setHeaderData(1, Qt::Horizontal, tr("Title"), Qt::DisplayRole);
    proposalsModel->setHeaderData(2, Qt::Horizontal, tr("Type"), Qt::DisplayRole);
    proposalsModel->setHeaderData(3, Qt::Horizontal, tr("Status"), Qt::DisplayRole);
    proposalsModel->setHeaderData(4, Qt::Horizontal, tr("Yes Votes"), Qt::DisplayRole);
    proposalsModel->setHeaderData(5, Qt::Horizontal, tr("No Votes"), Qt::DisplayRole);
    proposalsModel->setHeaderData(6, Qt::Horizontal, tr("Voting Power"), Qt::DisplayRole);
    
    proposalsTable->setModel(proposalsModel);
    proposalsTable->setAlternatingRowColors(true);
    proposalsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    proposalsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    proposalsTable->setSortingEnabled(true);
    proposalsTable->verticalHeader()->hide();
    proposalsTable->horizontalHeader()->setStretchLastSection(true);
    
    proposalsTable->setColumnWidth(0, 200);
    proposalsTable->setColumnWidth(1, 250);
    proposalsTable->setColumnWidth(2, 150);
    proposalsTable->setColumnWidth(3, 100);
    proposalsTable->setColumnWidth(4, 80);
    proposalsTable->setColumnWidth(5, 80);
    proposalsTable->setColumnWidth(6, 120);
}

void GovernancePage::setClientModel(ClientModel *model)
{
    this->clientModel = model;
}

void GovernancePage::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
    if (model)
    {
        refreshProposals();
    }
}

void GovernancePage::refreshProposals()
{
    refreshProposalsList();
}

void GovernancePage::refreshProposalsList()
{
    if (!clientModel || !walletModel) {
        return;
    }
    
    // Clear existing rows
    proposalsModel->removeRows(0, proposalsModel->rowCount());
    
    // Call listproposals RPC
    JSONRPCRequest req;
    req.strMethod = "listproposals";
    req.params = UniValue(UniValue::VARR);
    
    try {
        UniValue result = tableRPC.execute(req);
        
        if (result.isArray()) {
            for (size_t i = 0; i < result.size(); i++) {
                const UniValue& entry = result[i];
                
                QString proposalId = QString::fromStdString(entry["proposal_id"].get_str());
                QString title = QString::fromStdString(entry["title"].get_str());
                QString type = QString::fromStdString(entry["type"].get_str());
                QString status = QString::fromStdString(entry["status"].get_str());
                int yesVotes = entry["yes_votes"].get_int();
                int noVotes = entry["no_votes"].get_int();
                qint64 votingPower = entry["total_voting_power"].get_int64();
                
                QList<QStandardItem*> row;
                row << new QStandardItem(proposalId.left(16) + "...");
                row << new QStandardItem(title);
                row << new QStandardItem(type);
                row << new QStandardItem(status);
                row << new QStandardItem(QString::number(yesVotes));
                row << new QStandardItem(QString::number(noVotes));
                row << new QStandardItem(FleetCreditsUnits::formatWithUnit(walletModel->getOptionsModel()->getDisplayUnit(), votingPower * COIN, false, FleetCreditsUnits::separatorAlways));
                
                // Store full proposal ID in UserRole
                row[0]->setData(proposalId, Qt::UserRole);
                
                proposalsModel->appendRow(row);
            }
        }
    } catch (const std::exception& e) {
        QMessageBox::warning(this, tr("Error"), tr("Failed to load proposals: %1").arg(QString::fromStdString(e.what())));
    }
}

void GovernancePage::onProposalTypeChanged()
{
    // Update placeholder text or validation based on proposal type
}

void GovernancePage::onSubmitProposalClicked()
{
    if (!walletModel) {
        QMessageBox::warning(this, tr("Error"), tr("Wallet not available"));
        return;
    }
    
    QString proposalType = proposalTypeCombo->currentData().toString();
    QString title = titleInput->text().trimmed();
    QString description = descriptionInput->toPlainText().trimmed();
    QString proposalData = proposalDataInput->toPlainText().trimmed();
    
    if (title.isEmpty() || description.isEmpty() || proposalData.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Please fill in all fields"));
        return;
    }
    
    // Validate JSON
    QJsonParseError error;
    QJsonDocument::fromJson(proposalData.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError) {
        QMessageBox::warning(this, tr("Error"), tr("Invalid JSON in proposal data: %1").arg(error.errorString()));
        return;
    }
    
    // Call submitproposal RPC
    JSONRPCRequest req;
    req.strMethod = "submitproposal";
    req.params = UniValue(UniValue::VARR);
    req.params.push_back(proposalType.toStdString());
    req.params.push_back(title.toStdString());
    req.params.push_back(description.toStdString());
    req.params.push_back(proposalData.toStdString());
    
    try {
        UniValue result = tableRPC.execute(req);
        
        QString proposalId = QString::fromStdString(result["proposal_id"].get_str());
        QString txid = QString::fromStdString(result["txid"].get_str());
        
        QMessageBox::information(this, tr("Success"), 
            tr("Proposal submitted successfully!\n\nProposal ID: %1\nTransaction ID: %2")
            .arg(proposalId).arg(txid));
        
        // Clear form
        titleInput->clear();
        descriptionInput->clear();
        proposalDataInput->clear();
        
        // Refresh proposals list
        QTimer::singleShot(2000, this, SLOT(refreshProposals()));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, tr("Error"), 
            tr("Failed to submit proposal: %1").arg(QString::fromStdString(e.what())));
    }
}

void GovernancePage::onProposalClicked(const QModelIndex &index)
{
    if (!index.isValid()) {
        voteButton->setEnabled(false);
        selectedProposalLabel->setText(tr("No proposal selected"));
        return;
    }
    
    QString proposalId = index.sibling(index.row(), 0).data(Qt::UserRole).toString();
    QString title = index.sibling(index.row(), 1).data(Qt::DisplayRole).toString();
    
    selectedProposalLabel->setText(tr("Selected: %1").arg(title));
    voteButton->setEnabled(true);
    
    // Store selected proposal ID in vote button
    voteButton->setProperty("proposalId", proposalId);
}

void GovernancePage::onVoteClicked()
{
    QString proposalId = voteButton->property("proposalId").toString();
    if (proposalId.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No proposal selected"));
        return;
    }
    
    // Show vote dialog
    bool ok;
    QString choice = QInputDialog::getItem(this, tr("Vote on Proposal"), 
        tr("Choose your vote:"), 
        QStringList() << tr("YES") << tr("NO") << tr("ABSTAIN"), 
        0, false, &ok);
    
    if (!ok || choice.isEmpty()) {
        return;
    }
    
    // Call vote RPC
    JSONRPCRequest req;
    req.strMethod = "vote";
    req.params = UniValue(UniValue::VARR);
    req.params.push_back(proposalId.toStdString());
    req.params.push_back(choice.toStdString());
    
    try {
        UniValue result = tableRPC.execute(req);
        
        QString voteId = QString::fromStdString(result["vote_id"].get_str());
        QString txid = QString::fromStdString(result["txid"].get_str());
        
        QMessageBox::information(this, tr("Success"), 
            tr("Vote submitted successfully!\n\nVote ID: %1\nTransaction ID: %2")
            .arg(voteId).arg(txid));
        
        // Refresh proposals list
        QTimer::singleShot(2000, this, SLOT(refreshProposals()));
    } catch (const std::exception& e) {
        QMessageBox::critical(this, tr("Error"), 
            tr("Failed to submit vote: %1").arg(QString::fromStdString(e.what())));
    }
}

void GovernancePage::onRefreshClicked()
{
    refreshProposals();
}

