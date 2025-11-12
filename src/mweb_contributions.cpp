// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/mweb.h"
#include "primitives/contribution.h"
#include "hash.h"
#include "util.h"
#include "streams.h"
#include "protocol.h"

/** Extract contribution transactions from MWEB extension block
 * MWEB extension blocks contain privacy-sensitive contributions
 * (e.g., ETHICAL_REVIEW) with hidden amounts
 */
std::vector<CContributionTransaction> ExtractContributionsFromMWEB(const CMWEBExtensionBlock& mweb_block)
{
    std::vector<CContributionTransaction> contributions;
    
    // Scan MWEB transactions for embedded contribution data
    // MWEB transactions embed contribution proof_data in their outputs
    // via the view_key field (for selective disclosure)
    for (const auto& mweb_tx : mweb_block.mweb_txs) {
        // Check each output for contribution metadata in view_key field
        for (const auto& output : mweb_tx.outputs) {
            if (output.view_key.empty()) {
                continue;  // No metadata in this output
            }
            
            // Try to deserialize contribution data from view_key
            try {
                CDataStream ss(output.view_key, SER_NETWORK, PROTOCOL_VERSION);
                
                // Read contribution type and proof hash
                uint8_t contrib_type_byte;
                uint256 proof_hash;
                
                ss >> contrib_type_byte;
                ss >> proof_hash;
                
                // Create contribution transaction
                CContributionTransaction contrib_tx;
                contrib_tx.contrib_type = static_cast<ContributionType>(contrib_type_byte);
                contrib_tx.proof_data.hash = proof_hash;
                contrib_tx.tx_id = mweb_tx.tx_hash;
                contrib_tx.requires_mweb = true;
                
                // Set timestamp from transaction if available
                // For MWEB, we use the block timestamp
                contrib_tx.timestamp = GetTime();
                
                // Try to read additional metadata if available
                if (!ss.empty()) {
                    try {
                        ss >> contrib_tx.proof_data.evidence;
                    } catch (...) {
                        // Evidence is optional
                    }
                }
                
                // Validate contribution structure
                if (contrib_tx.contrib_type >= CODE_CONTRIBUTION && 
                    contrib_tx.contrib_type <= ETHICAL_REVIEW) {
                    contributions.push_back(contrib_tx);
                }
            } catch (const std::exception& e) {
                // Invalid contribution data, skip this output
                continue;
            }
        }
    }
    
    return contributions;
}
