// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "contributionspage.h"
#include "ui_contributionspage.h"

#include "guiutil.h"
#include "walletmodel.h"
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

ContributionsPage::ContributionsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContributionsPage),
    clientModel(0),
    walletModel(0),
    contributionsModel(0)
{
    ui->setupUi(this);

    // Set up contributions table
    setupContributionsTable();

    // Connect signals
    connect(ui->contribTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onContribTypeChanged()));
    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(onSubmitClicked()));
    connect(ui->contributionsTable, SIGNAL(clicked(QModelIndex)), this, SLOT(onContributionClicked(QModelIndex)));
}

ContributionsPage::~ContributionsPage()
{
    delete contributionsModel;
    delete ui;
}

void ContributionsPage::setupContributionsTable()
{
    // Create model for contributions table
    contributionsModel = new QStandardItemModel(0, 6, this);
    contributionsModel->setHeaderData(0, Qt::Horizontal, tr("Transaction ID"), Qt::DisplayRole);
    contributionsModel->setHeaderData(1, Qt::Horizontal, tr("Type"), Qt::DisplayRole);
    contributionsModel->setHeaderData(2, Qt::Horizontal, tr("Status"), Qt::DisplayRole);
    contributionsModel->setHeaderData(3, Qt::Horizontal, tr("Confirmations"), Qt::DisplayRole);
    contributionsModel->setHeaderData(4, Qt::Horizontal, tr("Reward"), Qt::DisplayRole);
    contributionsModel->setHeaderData(5, Qt::Horizontal, tr("Date"), Qt::DisplayRole);
    
    // Set model on table
    ui->contributionsTable->setModel(contributionsModel);
    
    // Configure table appearance
    ui->contributionsTable->setAlternatingRowColors(true);
    ui->contributionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->contributionsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->contributionsTable->setSortingEnabled(true);
    ui->contributionsTable->verticalHeader()->hide();
    ui->contributionsTable->horizontalHeader()->setStretchLastSection(true);
    
    // Resize columns
    ui->contributionsTable->setColumnWidth(0, 200);  // TXID
    ui->contributionsTable->setColumnWidth(1, 150);  // Type
    ui->contributionsTable->setColumnWidth(2, 100);  // Status
    ui->contributionsTable->setColumnWidth(3, 100);  // Confirmations
    ui->contributionsTable->setColumnWidth(4, 100);  // Reward
    ui->contributionsTable->setColumnWidth(5, 150);  // Date
}

void ContributionsPage::setClientModel(ClientModel *model)
{
    this->clientModel = model;
}

void ContributionsPage::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
    if (model)
    {
        refreshContributions();
        updateRewards();
    }
}

void ContributionsPage::onContribTypeChanged()
{
    QString contribType = ui->contribTypeCombo->currentText();
    
    // Update placeholder text based on contribution type
    if (contribType == "CODE_CONTRIBUTION")
    {
        ui->proofDataInput->setPlaceholderText("GitHub commit hash (e.g., abc123def456...)");
    }
    else if (contribType == "ETHICAL_REVIEW")
    {
        ui->proofDataInput->setPlaceholderText("Ethical review proof (will route to MWEB)");
    }
    else
    {
        ui->proofDataInput->setPlaceholderText("Proof Data (IPFS hash, verification link, etc.)");
    }
}

void ContributionsPage::onSubmitClicked()
{
    if (!walletModel || !walletModel->getOptionsModel())
        return;

    QString contribType = ui->contribTypeCombo->currentText();
    QString proofData = ui->proofDataInput->text();
    QString metadata = ui->metadataInput->toPlainText();

    if (proofData.isEmpty())
    {
        QMessageBox::warning(this, tr("Submit Contribution"),
            tr("Please enter proof data."));
        return;
    }

    // Validate metadata JSON if provided
    if (!metadata.isEmpty())
    {
        QJsonParseError error;
        QJsonDocument::fromJson(metadata.toUtf8(), &error);
        if (error.error != QJsonParseError::NoError)
        {
            QMessageBox::warning(this, tr("Submit Contribution"),
                tr("Invalid JSON in metadata field: %1").arg(error.errorString()));
            return;
        }
    }

    // Call RPC: submitcontribution
    try {
        JSONRPCRequest req;
        req.strMethod = "submitcontribution";
        req.params = UniValue(UniValue::VARR);
        req.params.push_back(contribType.toStdString());
        req.params.push_back(proofData.toStdString());
        if (!metadata.isEmpty()) {
            req.params.push_back(metadata.toStdString());
        }
        req.fHelp = false;
        
        // Execute RPC call
        UniValue result = tableRPC.execute(req);
        
        // Parse result
        if (result.isObject()) {
            std::string txid = "";
            std::string status = "";
            
            if (result.exists("txid")) {
                txid = result["txid"].get_str();
            }
            if (result.exists("status")) {
                status = result["status"].get_str();
            }
            
            QString message = tr("Contribution submitted successfully!\n\n");
            message += tr("Type: %1\n").arg(contribType);
            message += tr("Proof: %1\n").arg(proofData);
            if (!txid.empty()) {
                message += tr("\nTransaction ID: %1").arg(QString::fromStdString(txid));
            }
            if (!status.empty()) {
                message += tr("\nStatus: %1").arg(QString::fromStdString(status));
            }
            
            // Add note about confirmation
            message += tr("\n\nNote: In regtest mode, you may need to mine a block (generate 1) for the contribution to appear in the list.");
            
            QMessageBox::information(this, tr("Submit Contribution"), message);
            
            // Clear form
            ui->proofDataInput->clear();
            ui->metadataInput->clear();
            
            // Refresh contributions list with delays (transaction may need time to propagate/confirm)
            // Immediate refresh (in case it's already confirmed)
            refreshContributions();
            
            // Refresh again after 2 seconds (in case it just got confirmed)
            QTimer::singleShot(2000, this, SLOT(refreshContributions()));
            
            // Refresh again after 5 seconds (in case block was just mined)
            QTimer::singleShot(5000, this, SLOT(refreshContributions()));
        } else {
            QMessageBox::information(this, tr("Submit Contribution"),
                tr("Contribution submitted. Result: %1").arg(QString::fromStdString(result.write())));
        }
    } catch (const std::exception& e) {
        QString errorMsg = QString::fromStdString(e.what());
        QMessageBox::critical(this, tr("Submit Contribution Error"),
            tr("Failed to submit contribution:\n\n%1").arg(errorMsg));
    }
}

void ContributionsPage::onContributionClicked(const QModelIndex &index)
{
    if (!index.isValid() || !contributionsModel) {
        return;
    }
    
    // Get TXID from user role (stored in first column)
    QModelIndex txidIndex = contributionsModel->index(index.row(), 0);
    QString txid = contributionsModel->data(txidIndex, Qt::UserRole).toString();
    
    if (txid.isEmpty()) {
        return;
    }
    
    // Call getcontributionstatus RPC
    try {
        JSONRPCRequest req;
        req.strMethod = "getcontributionstatus";
        req.params = UniValue(UniValue::VARR);
        req.params.push_back(txid.toStdString());
        req.fHelp = false;
        
        UniValue result = tableRPC.execute(req);
        
        if (result.isObject()) {
            QString details = tr("Contribution Details\n\n");
            details += tr("Transaction ID: %1\n").arg(txid);
            
            if (result.exists("type")) {
                details += tr("Type: %1\n").arg(QString::fromStdString(result["type"].get_str()));
            }
            if (result.exists("status")) {
                details += tr("Status: %1\n").arg(QString::fromStdString(result["status"].get_str()));
            }
            if (result.exists("confirmations")) {
                details += tr("Confirmations: %1\n").arg(result["confirmations"].get_int());
            }
            if (result.exists("reward")) {
                QString rewardStr = FleetCreditsUnits::formatWithUnit(
                    walletModel->getOptionsModel()->getDisplayUnit(),
                    result["reward"].get_real() * COIN);
                details += tr("Reward: %1\n").arg(rewardStr);
            }
            if (result.exists("bonus_level")) {
                details += tr("Bonus Level: %1\n").arg(QString::fromStdString(result["bonus_level"].get_str()));
            }
            
            QMessageBox::information(this, tr("Contribution Details"), details);
        } else {
            QMessageBox::information(this, tr("Contribution Details"),
                tr("Transaction ID: %1\n\nStatus: %2").arg(txid, QString::fromStdString(result.write())));
        }
    } catch (const std::exception& e) {
        QMessageBox::warning(this, tr("Contribution Details"),
            tr("Failed to get contribution status:\n\n%1").arg(QString::fromStdString(e.what())));
    }
}

void ContributionsPage::refreshContributions()
{
    if (!clientModel || !walletModel || !contributionsModel) {
        return;
    }
    
    // Clear existing rows
    contributionsModel->removeRows(0, contributionsModel->rowCount());
    
    // Call RPC: listcontributions
    try {
        JSONRPCRequest req;
        req.strMethod = "listcontributions";
        req.params = UniValue(UniValue::VARR);
        req.fHelp = false;
        
        // Execute RPC call
        UniValue result = tableRPC.execute(req);
        
        // Parse result - should be an array of contribution objects
        if (result.isArray()) {
            for (size_t i = 0; i < result.size(); i++) {
                const UniValue& entry = result[i];
                if (!entry.isObject()) continue;
                
                // Extract fields
                std::string txid = "";
                std::string type = "";
                std::string status = "";
                int confirmations = 0;
                double reward = 0.0;
                int64_t timestamp = 0;
                std::string bonus_level = "";
                
                if (entry.exists("txid")) txid = entry["txid"].get_str();
                if (entry.exists("type")) type = entry["type"].get_str();
                if (entry.exists("status")) status = entry["status"].get_str();
                if (entry.exists("confirmations")) confirmations = entry["confirmations"].get_int();
                if (entry.exists("reward")) reward = entry["reward"].get_real();
                if (entry.exists("timestamp")) timestamp = entry["timestamp"].get_int64();
                if (entry.exists("bonus_level")) bonus_level = entry["bonus_level"].get_str();
                
                // Create row
                int row = contributionsModel->rowCount();
                contributionsModel->insertRow(row);
                
                // Format TXID (show first 8 and last 8 chars)
                QString txidStr = QString::fromStdString(txid);
                QString shortTxid = txidStr.length() > 16 
                    ? txidStr.left(8) + "..." + txidStr.right(8)
                    : txidStr;
                
                // Format reward
                QString rewardStr = FleetCreditsUnits::formatWithUnit(walletModel->getOptionsModel()->getDisplayUnit(), 
                                                                      reward * COIN);
                
                // Format date
                QString dateStr = "";
                if (timestamp > 0) {
                    QDateTime dateTime = QDateTime::fromTime_t(timestamp);
                    dateStr = dateTime.toString("yyyy-MM-dd hh:mm:ss");
                }
                
                // Set data
                contributionsModel->setData(contributionsModel->index(row, 0), shortTxid, Qt::DisplayRole);
                contributionsModel->setData(contributionsModel->index(row, 0), txidStr, Qt::ToolTipRole); // Full TXID in tooltip
                contributionsModel->setData(contributionsModel->index(row, 1), QString::fromStdString(type), Qt::DisplayRole);
                contributionsModel->setData(contributionsModel->index(row, 2), QString::fromStdString(status), Qt::DisplayRole);
                contributionsModel->setData(contributionsModel->index(row, 3), confirmations, Qt::DisplayRole);
                contributionsModel->setData(contributionsModel->index(row, 4), rewardStr, Qt::DisplayRole);
                contributionsModel->setData(contributionsModel->index(row, 5), dateStr, Qt::DisplayRole);
                
                // Store full TXID in user role for clicking
                contributionsModel->setData(contributionsModel->index(row, 0), txidStr, Qt::UserRole);
            }
            
            // Sort by date (newest first) if we have data
            if (contributionsModel->rowCount() > 0) {
                ui->contributionsTable->sortByColumn(5, Qt::DescendingOrder);
            }
        }
    } catch (const std::exception& e) {
        // Silently fail - contributions might not be available yet
        // This is normal if no contributions have been confirmed yet
        // or if the transaction is still in mempool
    }
}

void ContributionsPage::updateRewards()
{
    // Update rewards display
    // Calculate total rewards from contributions
    // TODO: Implement reward calculation
}

