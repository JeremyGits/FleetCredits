// Copyright (c) 2011-2016 The Fleet Credits Core developers
// Copyright (c) 2022 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "guiutil.h"
#if defined(HAVE_CONFIG_H)
#include "config/fleetcredits-config.h"
#endif

#include <iostream>
#include <string>

#include "peerdialog.h"

#include "ui_addpeerdialog.h"
#include "ui_testpeerdialog.h"

#include "net.h"
#include "net_processing.h"
#include "netbase.h"
#include "protocol.h"
#include "chainparams.h"
#include "util.h"

#include <stdio.h>

#include <QMessageBox>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QUrl>

/** Function to manage peers */
QString PeerTools::ManagePeer(QString type, QString peer)
{
    std::string peerAddress = peer.toStdString();

    if (peerAddress.size() > 256) {
      return tr("Error: Node address is invalid");
    }

    if(!g_connman)
        return tr("Error: Peer-to-peer functionality missing or disabled");

    if (type == "onetry")
    {
        // Check for localhost/self-connection attempt
        CService selfService;
        if (Lookup(peerAddress.c_str(), selfService, 0, false)) {
            // Check if it's localhost (127.x.x.x) - most common case
            // Note: IsLocal() checks if it's a bound local address, which may not catch all cases
            if (selfService.IsIPv4() && selfService.GetByte(3) == 127) {
                return tr("Warning: Attempting to connect to localhost (127.x.x.x).\n\n") +
                       tr("For testing peer connections, you need TWO instances:\n") +
                       tr("Instance 1: -port=18444 -rpcport=18332 -datadir=./node1\n") +
                       tr("Instance 2: -port=18445 -rpcport=18333 -datadir=./node2\n\n") +
                       tr("Connection attempt made, but self-connections typically fail.");
            }
        }
        
        // Attempt immediate connection (one-shot)
        // OpenNetworkConnection will parse the address string internally
        CAddress addr(CService(), NODE_NONE);
        bool connected = g_connman->OpenNetworkConnection(addr, false, NULL, peerAddress.c_str(), true);
        
        if (connected) {
            return tr("Connection attempt initiated to: ") + peer + tr("\n\nPeer should appear in list within 1-5 seconds if connection succeeds.");
        } else {
            return tr("Could not initiate connection. Possible reasons:\n") +
                   tr("- Peer is unreachable\n") +
                   tr("- Already connected to this peer\n") +
                   tr("- Self-connection attempt (localhost)\n") +
                   tr("- Network not active\n\n") +
                   tr("Address: ") + peer;
        }
    }

    if (type == "add")
    {
        // Check for localhost/self-connection attempt
        CService selfService;
        if (Lookup(peerAddress.c_str(), selfService, 0, false)) {
            // Check if it's localhost (127.x.x.x) - most common case
            if (selfService.IsIPv4() && selfService.GetByte(3) == 127) {
                return tr("Warning: Attempting to connect to localhost (127.x.x.x).\n\n") +
                       tr("For testing peer connections, you need TWO instances:\n") +
                       tr("Instance 1: -port=18444 -rpcport=18332 -datadir=./node1\n") +
                       tr("Instance 2: -port=18445 -rpcport=18333 -datadir=./node2\n\n") +
                       tr("Node added to persistent list, but connection may fail if this is the same instance.");
            }
        }
        
        // Add to persistent list
        if(!g_connman->AddNode(peerAddress))
            return tr("Error: Unable to add node (may already be in list)");
        
        // Also attempt immediate connection
        CAddress addr(CService(), NODE_NONE);
        bool connected = g_connman->OpenNetworkConnection(addr, false, NULL, peerAddress.c_str(), false, false, true);
        
        if (connected) {
            return tr("Node added and connection initiated: ") + peer + tr("\n\nPeer should appear in list within 1-5 seconds.");
        } else {
            return tr("Node added to persistent list: ") + peer + tr("\n\nConnection attempt made. Peer will appear when connection succeeds (may take a few seconds).\n\n") +
                   tr("Note: If connecting to localhost, you need TWO instances on different ports.");
        }
    }
    else if(type == "remove")
    {
        bool disconnected = false;
        bool removedFromList = false;
        
        // First, remove from persistent list to prevent reconnection
        removedFromList = g_connman->RemoveAddedNode(peerAddress);
        
        // Disconnect by address string (FindNode is private, so we use DisconnectNode directly)
        disconnected = g_connman->DisconnectNode(peerAddress);
        
        if (removedFromList && disconnected) {
            return tr("Removed from list and disconnected: ") + peer;
        } else if (removedFromList) {
            return tr("Removed from persistent list (disconnection may take a moment): ") + peer;
        } else if (disconnected) {
            return tr("Disconnected (was not in persistent list): ") + peer;
        } else {
            return tr("Node not found in list or connected peers. It may have already disconnected: ") + peer;
        }
    }

    return tr("Returned OK.");
}

/** Check if Peer is valid */
bool PeerTools::CheckPeerAddress(QString address)
{
    CNetAddr addr;
    return LookupHost(address.toStdString().c_str(), addr, true);
}

/** Get port based on current chain */
QString PeerTools::GetPort()
{
    return QString::number(Params().GetDefaultPort());
}

/** Add Peer Dialog */
AddPeerDialog::AddPeerDialog(QWidget *parent) : 
    QWidget(parent),
    ui(new Ui::AddPeerDialog)
{
    ui->setupUi(this);

    ui->peerPort->setValidator( new QIntValidator(1, 65535, this) );

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_addPeerClicked()));
}

AddPeerDialog::~AddPeerDialog()
{
    delete ui;
}

void AddPeerDialog::on_addPeerClicked()
{
    QString address = ui->peerAddress->text();
    QString port = ui->peerPort->text();
    QString data = "";

    if(address.isEmpty()) 
    {
        QMessageBox::critical(this, tr("Add Peer"), tr("Please enter an address."), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if(port.isEmpty()) 
    {
        port = PeerTools::GetPort();
        ui->peerPort->setText(port);
    }

    if(!PeerTools::CheckPeerAddress(address))
    {
        QMessageBox::critical(this, tr("Add Peer"), tr("Please enter a valid peer address."), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    data = address + ":" + port;

    QString result = PeerTools::ManagePeer("add", data);
    if(QMessageBox::Ok == QMessageBox::information(this, tr("Add Peer"), result, QMessageBox::Ok, QMessageBox::Ok))
    {
        // Emit signal that peer was added
        Q_EMIT peerAdded();
        this->close();
    }
}

/** Add Test Peer Dialog */
TestPeerDialog::TestPeerDialog(QWidget *parent) : 
    QWidget(parent),
    ui(new Ui::TestPeerDialog)
{
    ui->setupUi(this);

    ui->peerPort->setValidator( new QIntValidator(1, 65535, this) );

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_testPeerClicked()));
}

TestPeerDialog::~TestPeerDialog()
{
    delete ui;
}

void TestPeerDialog::on_testPeerClicked()
{
    QString address = ui->peerAddress->text();
    QString port = ui->peerPort->text();
    QString data = "";

    if(address.isEmpty()) 
    {
        QMessageBox::critical(this, tr("Test Peer"), tr("Please enter an address."), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if(port.isEmpty()) 
    {
        port = PeerTools::GetPort();
        ui->peerPort->setText(port);
    }

    if(!PeerTools::CheckPeerAddress(address))
    {
        QMessageBox::critical(this, tr("Test Peer"), tr("Please enter a valid peer address."), QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    data = address + ":" + port;

    QString result = PeerTools::ManagePeer("onetry", data);
    if(QMessageBox::Ok == QMessageBox::information(this, tr("Try Peer"), result, QMessageBox::Ok, QMessageBox::Ok))
    {
        this->close();
        // Note: Peer list will refresh automatically via timer, but caller can force refresh if needed
    }
}
