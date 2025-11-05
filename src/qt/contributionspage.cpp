// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "contributionspage.h"
#include "ui_contributionspage.h"

#include "guiutil.h"
#include "walletmodel.h"
#include "clientmodel.h"
#include "fleetcreditsunits.h"

#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

ContributionsPage::ContributionsPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContributionsPage),
    clientModel(0),
    walletModel(0)
{
    ui->setupUi(this);

    // Connect signals
    connect(ui->contribTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onContribTypeChanged()));
    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(onSubmitClicked()));
    connect(ui->contributionsTable, SIGNAL(clicked(QModelIndex)), this, SLOT(onContributionClicked(QModelIndex)));

    // Set up contributions table model
    // TODO: Create ContributionTableModel
}

ContributionsPage::~ContributionsPage()
{
    delete ui;
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
    // This will be implemented via RPC calls
    QMessageBox::information(this, tr("Submit Contribution"),
        tr("Contribution submission will be connected to RPC.\n\nType: %1\nProof: %2")
        .arg(contribType).arg(proofData));
}

void ContributionsPage::onContributionClicked(const QModelIndex &index)
{
    // Show contribution details dialog
    // TODO: Implement contribution details view
}

void ContributionsPage::refreshContributions()
{
    // Refresh contributions list via RPC
    // Call: listcontributions
    // TODO: Update contributionsTable model
}

void ContributionsPage::updateRewards()
{
    // Update rewards display
    // Calculate total rewards from contributions
    // TODO: Implement reward calculation
}

