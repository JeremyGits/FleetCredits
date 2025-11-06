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
        // Parse address:port properly
        CService addr;
        if (!Lookup(peerAddress.c_str(), addr, 0, false) || !addr.IsValid()) {
            return tr("Error: Invalid peer address format. Use IP:PORT (e.g., 96.126.123.195:22556)");
        }
        
        // Check for localhost/self-connection attempt
        if (addr.IsIPv4() && addr.GetByte(3) == 127) {
            return tr("Warning: Attempting to connect to localhost (127.x.x.x).\n\n") +
                   tr("For testing peer connections, you need TWO instances:\n") +
                   tr("Instance 1: -port=18444 -rpcport=18332 -datadir=./node1\n") +
                   tr("Instance 2: -port=18445 -rpcport=18333 -datadir=./node2\n\n") +
                   tr("Connection attempt made, but self-connections typically fail.");
        }
        
        // Attempt immediate connection (one-shot)
        // OpenNetworkConnection is async, so it may return false even if connection is initiated
        CAddress addrObj(addr, NODE_NONE);
        g_connman->OpenNetworkConnection(addrObj, false, NULL, NULL, true);
        
        return tr("Connection attempt initiated to: ") + peer + 
               tr("\n\nPeer should appear in list within 1-5 seconds if connection succeeds.\n\n") +
               tr("Note: Connection is asynchronous. If peer doesn't appear, it may be unreachable or the network may still be starting.");
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
        
        // Parse address:port properly
        CService addr;
        if (!Lookup(peerAddress.c_str(), addr, 0, false) || !addr.IsValid()) {
            return tr("Error: Invalid peer address format. Use IP:PORT (e.g., 96.126.123.195:22556)");
        }
        
        // Add to persistent list (this also triggers connection attempts)
        // AddNode now handles same IP with different ports (updates the port automatically)
        if(!g_connman->AddNode(peerAddress)) {
            // Check if it's already in the list with same IP (AddNode should have updated it, but check anyway)
            std::vector<AddedNodeInfo> addedNodes = g_connman->GetAddedNodeInfo();
            bool found = false;
            std::string existingFormat;
            
            // Extract IP from peerAddress
            size_t lastColon = peerAddress.find_last_of(':');
            std::string peerIP = (lastColon != std::string::npos) ? peerAddress.substr(0, lastColon) : peerAddress;
            
            for (const AddedNodeInfo& info : addedNodes) {
                std::string storedAddr = info.strAddedNode;
                size_t storedColon = storedAddr.find_last_of(':');
                std::string storedIP = (storedColon != std::string::npos) ? storedAddr.substr(0, storedColon) : storedAddr;
                if (storedIP == peerIP && !peerIP.empty()) {
                    found = true;
                    existingFormat = storedAddr;
                    break;
                }
            }
            
            if (found && existingFormat != peerAddress) {
                // Same IP exists with different port - update it
                g_connman->RemoveAddedNode(existingFormat);
                if (g_connman->AddNode(peerAddress)) {
                    // Successfully updated
                    CAddress addrObj(addr, NODE_NONE);
                    g_connman->OpenNetworkConnection(addrObj, false, NULL, NULL, false, false, true);
                    return tr("Updated existing node port: ") + QString::fromStdString(existingFormat) + 
                           tr(" -> ") + peer + tr("\n\nConnection attempt initiated.");
                }
            }
            
            // Try to trigger immediate connection anyway (peer may already be in list with exact same format)
            CAddress addrObj(addr, NODE_NONE);
            g_connman->OpenNetworkConnection(addrObj, false, NULL, NULL, false, false, true);
            
            return tr("Node already in list (exact match), but connection attempt made: ") + peer + 
                   tr("\n\nIf peer is already connected, it will appear in the list. Otherwise, check back in a few seconds.");
        }
        
        // Also attempt immediate connection (OpenNetworkConnection is async, so it may return false even if initiated)
        CAddress addrObj(addr, NODE_NONE);
        g_connman->OpenNetworkConnection(addrObj, false, NULL, NULL, false, false, true);
        
        return tr("Node added and connection initiated: ") + peer + 
               tr("\n\nPeer should appear in list within 1-5 seconds if connection succeeds.\n\n") +
               tr("Note: Connection is asynchronous. If peer doesn't appear, it may be unreachable or the network may still be starting.");
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
