// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "mwebsuitepage.h"
#include "ui_mwebsuitepage.h"

#include "guiutil.h"
#include "walletmodel.h"
#include "clientmodel.h"
#include "fleetcreditsunits.h"
#include "fleetcreditsgui.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>
#include <QIODevice>

MWEBSuitePage::MWEBSuitePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MWEBSuitePage),
    clientModel(0),
    walletModel(0)
{
    ui->setupUi(this);

    // Connect signals
    connect(ui->pegInButton, SIGNAL(clicked()), this, SLOT(onPegInClicked()));
    connect(ui->pegOutButton, SIGNAL(clicked()), this, SLOT(onPegOutClicked()));
    connect(ui->exportViewKeysButton, SIGNAL(clicked()), this, SLOT(onExportViewKeysClicked()));
}

MWEBSuitePage::~MWEBSuitePage()
{
    delete ui;
}

void MWEBSuitePage::setClientModel(ClientModel *model)
{
    this->clientModel = model;
}

void MWEBSuitePage::setWalletModel(WalletModel *model)
{
    this->walletModel = model;
    if (model)
    {
        // Set up MWEB transaction model
        // TODO: Create MWEBTransactionModel
        
        updateDisplayUnit();
    }
}

void MWEBSuitePage::onPegInClicked()
{
    if (!walletModel || !walletModel->getOptionsModel())
        return;

    QString address = ui->pegInAddress->text();
    QString amountStr = ui->pegInAmount->text();
    
    if (address.isEmpty() || amountStr.isEmpty())
    {
        QMessageBox::warning(this, tr("Peg-In"),
            tr("Please enter both address and amount."));
        return;
    }

    bool ok;
    double amount = amountStr.toDouble(&ok);
    if (!ok || amount <= 0)
    {
        QMessageBox::warning(this, tr("Peg-In"),
            tr("Please enter a valid amount."));
        return;
    }

    // Call RPC: createpegin
    // This will be implemented via RPC calls
    QMessageBox::information(this, tr("Peg-In"),
        tr("Peg-in functionality will be connected to RPC."));
}

void MWEBSuitePage::onPegOutClicked()
{
    if (!walletModel || !walletModel->getOptionsModel())
        return;

    QString commitment = ui->pegOutCommitment->text();
    QString address = ui->pegOutAddress->text();
    QString amountStr = ui->pegOutAmount->text();
    
    if (commitment.isEmpty() || address.isEmpty() || amountStr.isEmpty())
    {
        QMessageBox::warning(this, tr("Peg-Out"),
            tr("Please enter commitment, address, and amount."));
        return;
    }

    // Call RPC: createpegout
    // This will be implemented via RPC calls
    QMessageBox::information(this, tr("Peg-Out"),
        tr("Peg-out functionality will be connected to RPC."));
}

void MWEBSuitePage::onExportViewKeysClicked()
{
    // Export view keys for auditing
    QString viewKeys = ui->viewKeysDisplay->toPlainText();
    if (viewKeys.isEmpty())
    {
        QMessageBox::information(this, tr("View Keys"),
            tr("No view keys available."));
        return;
    }

    QString filename = GUIUtil::getSaveFileName(this,
        tr("Export View Keys"), QString(),
        tr("Text files (*.txt);;All files (*)"), nullptr);

    if (filename.isEmpty())
        return;

    // Save view keys to file
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, tr("Export View Keys"),
            tr("Could not save view keys to file."));
        return;
    }

    QTextStream out(&file);
    out << viewKeys;
    file.close();

    QMessageBox::information(this, tr("Export View Keys"),
        tr("View keys exported successfully."));
}

void MWEBSuitePage::refreshMWEBTransactions()
{
    // Refresh MWEB transaction list via RPC
    // Call: listmwebtxs
}

void MWEBSuitePage::updateDisplayUnit()
{
    if (walletModel && walletModel->getOptionsModel())
    {
        // Update amount display units if needed
    }
}

