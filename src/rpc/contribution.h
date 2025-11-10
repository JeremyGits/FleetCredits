// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FLEETCREDITS_RPC_CONTRIBUTION_H
#define FLEETCREDITS_RPC_CONTRIBUTION_H

#include "primitives/contribution.h"
#include "primitives/transaction.h"

// Forward declarations
class CTransaction;

/** Extract contribution transaction from a single transaction
 * Returns true if a contribution was found and extracted, false otherwise
 */
bool ExtractContributionFromTransaction(const CTransaction& tx, CContributionTransaction& contrib_tx);

#endif // FLEETCREDITS_RPC_CONTRIBUTION_H

