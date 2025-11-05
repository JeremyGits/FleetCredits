// Copyright (c) 2022 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

/**
 * Utility functions for RPC commands
 */
#ifndef FLEETCREDITS_WALLET_RPCUTIL_H
#define FLEETCREDITS_WALLET_RPCUTIL_H

#include "fs.h"
#include "util.h"

fs::path GetBackupDirFromInput(std::string strUserFilename);

#endif // FLEETCREDITS_WALLET_RPCUTIL_H
