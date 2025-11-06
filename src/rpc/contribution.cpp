// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "rpc/server.h"
#include "rpc/client.h"
#include "base58.h"
#include "consensus/validation.h"
#include "primitives/contribution.h"
#include "primitives/transaction.h"
#include "primitives/mweb.h"
#include "mweb_mempool.h"
#include "validation.h"
#include "net.h"
#include "net_processing.h"
#include "txmempool.h"
#include "util.h"
#include "utilstrencodings.h"
#include "wallet/wallet.h"
#include "wallet/rpcutil.h"
#include "script/script.h"
#include "streams.h"
#include "protocol.h"
#include "chain.h"
#include "chainparams.h"
#include "fleetcredits.h"

#include <univalue.h>

using namespace std;

extern CWallet* pwalletMain;

// Forward declarations from wallet/rpcwallet.cpp
bool EnsureWalletIsAvailable(bool avoidException);
void EnsureWalletIsUnlocked();

// Forward declaration from validation.cpp
bool GetTransaction(const uint256 &hash, CTransactionRef &txOut, const Consensus::Params& consensusParams, uint256 &hashBlock, bool fAllowSlow);

/** Extract contribution transaction from a single transaction
 * Returns true if a contribution was found and extracted, false otherwise
 */
bool ExtractContributionFromTransaction(const CTransaction& tx, CContributionTransaction& contrib_tx)
{
    // Contribution marker: 0xFC 0x01 (Fleet Credits Contribution)
    const unsigned char CONTRIB_MARKER[] = {0xFC, 0x01};
    const size_t MARKER_SIZE = 2;
    
    // Check each output for OP_RETURN with contribution marker
    for (const auto& txout : tx.vout) {
        const CScript& script = txout.scriptPubKey;
        
        // Check if this is an OP_RETURN output
        if (script.size() > 0 && script[0] == OP_RETURN) {
            // CScript adds a length prefix when pushing a vector
            // Format: OP_RETURN <length_byte> <0xFC> <0x01> <data...>
            // Or if length >= 76: OP_RETURN OP_PUSHDATA1 <length_byte> <0xFC> <0x01> <data...>
            
            size_t dataStart = 1; // Skip OP_RETURN
            size_t lengthBytes = 0;
            
            // Check for length prefix
            if (script.size() > dataStart) {
                unsigned char lengthByte = script[dataStart];
                
                // If length < 76, it's a single byte length prefix
                if (lengthByte < OP_PUSHDATA1) {
                    lengthBytes = 1;
                    dataStart += 1;
                }
                // If length == OP_PUSHDATA1 (0x4c), next byte is the length
                else if (lengthByte == OP_PUSHDATA1 && script.size() > dataStart + 1) {
                    lengthBytes = 2; // OP_PUSHDATA1 + length byte
                    dataStart += 2;
                }
                // If length == OP_PUSHDATA2 (0x4d), next 2 bytes are the length
                else if (lengthByte == OP_PUSHDATA2 && script.size() > dataStart + 2) {
                    lengthBytes = 3; // OP_PUSHDATA2 + 2 length bytes
                    dataStart += 3;
                }
                // If length == OP_PUSHDATA4 (0x4e), next 4 bytes are the length
                else if (lengthByte == OP_PUSHDATA4 && script.size() > dataStart + 4) {
                    lengthBytes = 5; // OP_PUSHDATA4 + 4 length bytes
                    dataStart += 5;
                }
            }
            
            // Check if we have enough bytes for the marker
            if (script.size() >= dataStart + MARKER_SIZE) {
                // Check marker (after length prefix)
                bool matches_marker = true;
                for (size_t j = 0; j < MARKER_SIZE; j++) {
                    if (script[dataStart + j] != CONTRIB_MARKER[j]) {
                        matches_marker = false;
                        break;
                    }
                }
                
                if (matches_marker) {
                    // Extract contribution data (everything after marker)
                    std::vector<unsigned char> contrib_data(
                        script.begin() + dataStart + MARKER_SIZE,
                        script.end()
                    );
                    
                    // Try to deserialize contribution transaction
                    try {
                        CDataStream ss(contrib_data, SER_NETWORK, PROTOCOL_VERSION);
                        uint8_t contrib_type_byte;
                        ss >> contrib_type_byte;
                        contrib_tx.contrib_type = static_cast<ContributionType>(contrib_type_byte);
                        ss >> contrib_tx.proof_data.hash;
                        ss >> contrib_tx.proof_data.evidence;
                        ss >> contrib_tx.timestamp;
                        ss >> contrib_tx.bonus_level;
                        uint8_t requires_mweb_byte;
                        ss >> requires_mweb_byte;
                        contrib_tx.requires_mweb = (requires_mweb_byte != 0);
                        
                        // Set transaction ID from the actual transaction
                        contrib_tx.tx_id = tx.GetHash();
                        
                        // Extract contributor from first input (if available)
                        if (!tx.vin.empty()) {
                            // Try to extract pubkey from scriptSig or witness
                            // For now, we'll set a placeholder - contributor address would need to be extracted differently
                            // The contributor pubkey should be embedded in the contribution data or we derive it from inputs
                            // For simplicity, we'll leave it as is for now
                        }
                        
                        return true;
                    } catch (const std::exception& e) {
                        // Invalid contribution data
                        return false;
                    }
                }
            }
        }
    }
    
    return false;
}

/** Submit a contribution transaction to the network */
UniValue submitcontribution(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() < 2) {
        throw runtime_error(
            "submitcontribution \"contrib_type\" \"proof_data\" [\"metadata\"]\n"
            "\nSubmit a contribution transaction to the Fleet Credits network.\n"
            "\nArguments:\n"
            "1. contrib_type    (string, required) Contribution type: CODE_CONTRIBUTION, CHARITABLE_ACT, CREATIVE_WORK, EDUCATIONAL_CONTENT, CODE_MENTORSHIP, AI_VALIDATION, DATA_LABELING, ETHICAL_REVIEW\n"
            "2. proof_data      (string, required) Proof data (GitHub commit hash, IPFS hash, etc.)\n"
            "3. metadata        (string, optional) Additional metadata JSON string\n"
            "\nResult:\n"
            "{\n"
            "  \"txid\" : \"hash\",     (string) The transaction ID\n"
            "  \"status\" : \"pending\", (string) Transaction status\n"
            "}\n"
            "\nExamples:\n"
            + HelpExampleCli("submitcontribution", "\"CODE_CONTRIBUTION\" \"abc123def456...\"")
            + HelpExampleRpc("submitcontribution", "\"CODE_CONTRIBUTION\", \"abc123def456...\"")
        );
    }

    // Parse contribution type
    string contrib_type_str = request.params[0].get_str();
    ContributionType contrib_type;
    
    if (contrib_type_str == "CODE_CONTRIBUTION") {
        contrib_type = CODE_CONTRIBUTION;
    } else if (contrib_type_str == "CHARITABLE_ACT") {
        contrib_type = CHARITABLE_ACT;
    } else if (contrib_type_str == "CREATIVE_WORK") {
        contrib_type = CREATIVE_WORK;
    } else if (contrib_type_str == "EDUCATIONAL_CONTENT") {
        contrib_type = EDUCATIONAL_CONTENT;
    } else if (contrib_type_str == "CODE_MENTORSHIP") {
        contrib_type = CODE_MENTORSHIP;
    } else if (contrib_type_str == "AI_VALIDATION") {
        contrib_type = AI_VALIDATION;
    } else if (contrib_type_str == "DATA_LABELING") {
        contrib_type = DATA_LABELING;
    } else if (contrib_type_str == "ETHICAL_REVIEW") {
        contrib_type = ETHICAL_REVIEW;
    } else {
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid contribution type");
    }

    // Check if wallet is available (required for signing)
    if (!EnsureWalletIsAvailable(false)) {
        throw JSONRPCError(RPC_METHOD_NOT_FOUND, "Wallet functionality is required for contribution transactions");
    }

    LOCK2(cs_main, pwalletMain->cs_wallet);
    EnsureWalletIsUnlocked();

    // Parse proof data
    string proof_data_str = request.params[1].get_str();
    vector<unsigned char> proof_data(proof_data_str.begin(), proof_data_str.end());

    // Parse optional metadata
    vector<unsigned char> metadata;
    if (request.params.size() > 2 && !request.params[2].isNull()) {
        string metadata_str = request.params[2].get_str();
        metadata.assign(metadata_str.begin(), metadata_str.end());
    }

    // Create contribution transaction object
    CContributionTransaction contrib_tx;
    contrib_tx.contrib_type = contrib_type;
    contrib_tx.proof_data.evidence = proof_data;
    contrib_tx.proof_data.metadata = metadata;
    contrib_tx.proof_data.timestamp = GetTime();
    contrib_tx.proof_data.contrib_type = contrib_type;
    
    // Calculate proof data hash
    contrib_tx.proof_data.hash = CalculateProofDataHash(contrib_tx.proof_data);
    
    // Set MWEB requirement for sensitive types
    if (contrib_type == ETHICAL_REVIEW) {
        contrib_tx.requires_mweb = true;
    }

    // Get contributor public key from wallet
    CPubKey pubKey;
    if (!pwalletMain->GetKeyFromPool(pubKey)) {
        throw JSONRPCError(RPC_WALLET_KEYPOOL_RAN_OUT, "Error: Keypool ran out, please call keypoolrefill first");
    }
    contrib_tx.contributor = pubKey;

    // Serialize contribution data for OP_RETURN
    CDataStream contribStream(SER_NETWORK, PROTOCOL_VERSION);
    uint8_t contrib_type_byte = static_cast<uint8_t>(contrib_tx.contrib_type);
    contribStream << contrib_type_byte;
    contribStream << contrib_tx.proof_data.hash;
    contribStream << contrib_tx.proof_data.evidence;
    contribStream << contrib_tx.timestamp;
    contribStream << contrib_tx.bonus_level;
    uint8_t requires_mweb_byte = contrib_tx.requires_mweb ? 1 : 0;
    contribStream << requires_mweb_byte;
    
    vector<unsigned char> contrib_data(contribStream.begin(), contribStream.end());
    
    // Limit OP_RETURN data size (MAX_SCRIPT_ELEMENT_SIZE is 520 bytes)
    if (contrib_data.size() > MAX_SCRIPT_ELEMENT_SIZE) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, 
            strprintf("Contribution data too large (%d bytes, max %d bytes)", 
                     contrib_data.size(), MAX_SCRIPT_ELEMENT_SIZE));
    }

    // Check if MWEB routing is required
    if (contrib_tx.requires_mweb) {
        // Route to MWEB - create MWEB transaction
        CMWEBTransaction mweb_tx;
        if (RouteToMWEB(contrib_tx, mweb_tx)) {
            // Submit MWEB transaction to MWEB mempool
            extern CMWEBMempool mwebMempool;
            mwebMempool.AddMWEBTransaction(mweb_tx);
            // For now, return placeholder indicating MWEB routing
            UniValue result(UniValue::VOBJ);
            result.pushKV("txid", mweb_tx.tx_hash.ToString());
            result.pushKV("mwebtxid", mweb_tx.tx_hash.ToString());
            result.pushKV("status", "pending_mweb");
            result.pushKV("message", "Contribution routed to MWEB (MWEB submission pending full implementation)");
            return result;
        }
    }

    // Create standard transaction with OP_RETURN output
    CWalletTx wtxNew;
    CReserveKey reservekey(pwalletMain);
    CAmount nFeeRequired;
    string strError;
    int nChangePosRet = -1;
    
    // Create recipients vector with OP_RETURN output
    vector<CRecipient> vecSend;
    
    // Create OP_RETURN script with contribution marker and data
    // Format: OP_RETURN <0xFC> <0x01> <serialized_contribution_data>
    // Where 0xFC 0x01 is the contribution marker (FC = Fleet Credits, 0x01 = contribution tx type)
    vector<unsigned char> contrib_script_data;
    contrib_script_data.push_back(0xFC);  // Fleet Credits marker
    contrib_script_data.push_back(0x01);   // Contribution transaction type
    contrib_script_data.insert(contrib_script_data.end(), contrib_data.begin(), contrib_data.end());
    
    CScript contribScript = CScript() << OP_RETURN << contrib_script_data;
    
    // Add OP_RETURN output (0 value)
    CRecipient contribRecipient = {contribScript, 0, false};
    vecSend.push_back(contribRecipient);
    
    // Create transaction
    if (!pwalletMain->CreateTransaction(vecSend, wtxNew, reservekey, nFeeRequired, nChangePosRet, strError)) {
        throw JSONRPCError(RPC_WALLET_ERROR, strprintf("Error creating transaction: %s", strError));
    }

    // Sign transaction
    CValidationState state;
    if (g_connman && !pwalletMain->CommitTransaction(wtxNew, reservekey, g_connman.get(), state)) {
        throw JSONRPCError(RPC_WALLET_ERROR, 
            strprintf("Error: Transaction was rejected! Reason: %s", state.GetRejectReason()));
    } else if (!g_connman) {
        throw JSONRPCError(RPC_CLIENT_P2P_DISABLED, "Error: Peer-to-peer functionality missing or disabled");
    }

    // Update contribution transaction ID with actual transaction hash
    contrib_tx.tx_id = wtxNew.GetHash();

    UniValue result(UniValue::VOBJ);
    result.pushKV("txid", contrib_tx.tx_id.ToString());
    result.pushKV("status", "pending");
    result.pushKV("contrib_type", contrib_type_str);
    result.pushKV("requires_mweb", contrib_tx.requires_mweb);
    result.pushKV("message", "Contribution transaction submitted successfully");

    return result;
}

/** Get status of a contribution transaction */
UniValue getcontributionstatus(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 1) {
        throw runtime_error(
            "getcontributionstatus \"txid\"\n"
            "\nGet the status of a contribution transaction.\n"
            "\nArguments:\n"
            "1. txid           (string, required) The contribution transaction ID\n"
            "\nResult:\n"
            "{\n"
            "  \"txid\" : \"hash\",         (string) The transaction ID\n"
            "  \"status\" : \"verified\",    (string) Verification status\n"
            "  \"bonus_level\" : \"HIGH\",   (string) Bonus level applied\n"
            "  \"reward\" : 11500,          (numeric) Reward amount in FC\n"
            "}\n"
            "\nExamples:\n"
            + HelpExampleCli("getcontributionstatus", "\"abc123...\"")
            + HelpExampleRpc("getcontributionstatus", "\"abc123...\"")
        );
    }

    LOCK(cs_main);

    uint256 txid = uint256S(request.params[0].get_str());

    // Look up transaction in blockchain/mempool
    CTransactionRef tx;
    uint256 hashBlock;
    if (!GetTransaction(txid, tx, Params().GetConsensus(0), hashBlock, true)) {
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Transaction not found in mempool or blockchain");
    }

    // Extract contribution data from transaction
    CContributionTransaction contrib_tx;
    if (!ExtractContributionFromTransaction(*tx, contrib_tx)) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Transaction is not a contribution transaction");
    }

    // Determine verification status
    string status = "pending";
    int confirmations = 0;
    
    if (!hashBlock.IsNull()) {
        BlockMap::iterator mi = mapBlockIndex.find(hashBlock);
        if (mi != mapBlockIndex.end() && (*mi).second) {
            CBlockIndex* pindex = (*mi).second;
            if (chainActive.Contains(pindex)) {
                confirmations = 1 + chainActive.Height() - pindex->nHeight;
                // Consider verified after 12 confirmations (as per whitepaper)
                if (confirmations >= 12) {
                    status = "verified";
                } else {
                    status = "confirmed";
                }
            } else {
                status = "orphaned";
            }
        }
    } else {
        // Transaction is in mempool
        status = "pending";
        confirmations = 0;
    }

    // Calculate reward based on bonus level
    CAmount baseReward = 10000 * COIN;  // Base block reward
    double multiplier = GetBonusMultiplier(contrib_tx.bonus_level, contrib_tx.contrib_type);
    CAmount reward = static_cast<CAmount>(baseReward * multiplier);

    UniValue result(UniValue::VOBJ);
    result.pushKV("txid", txid.ToString());
    result.pushKV("status", status);
    result.pushKV("confirmations", confirmations);
    result.pushKV("contrib_type", GetContributionTypeName(contrib_tx.contrib_type));
    result.pushKV("bonus_level", GetBonusLevelName(contrib_tx.bonus_level));
    result.pushKV("reward", reward / COIN);  // Return in FC units
    result.pushKV("requires_mweb", contrib_tx.requires_mweb);
    if (!hashBlock.IsNull()) {
        result.pushKV("blockhash", hashBlock.ToString());
    }

    return result;
}

/** List contributions by address or type */
UniValue listcontributions(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() > 2) {
        throw runtime_error(
            "listcontributions [\"address\"] [\"contrib_type\"]\n"
            "\nList contribution transactions.\n"
            "\nArguments:\n"
            "1. address        (string, optional) Filter by contributor address\n"
            "2. contrib_type   (string, optional) Filter by contribution type\n"
            "\nResult:\n"
            "[\n"
            "  {\n"
            "    \"txid\" : \"hash\",\n"
            "    \"type\" : \"CODE_CONTRIBUTION\",\n"
            "    \"status\" : \"verified\",\n"
            "    \"reward\" : 1000\n"
            "  },\n"
            "  ...\n"
            "]\n"
            "\nExamples:\n"
            + HelpExampleCli("listcontributions", "")
            + HelpExampleCli("listcontributions", "\"address\"")
            + HelpExampleRpc("listcontributions", "")
        );
    }

    LOCK(cs_main);

    // Parse optional filters
    ContributionType filter_type = CODE_CONTRIBUTION;  // Default, will be ignored if not provided
    bool filter_by_type = false;
    bool filter_by_address = false;
    CFleetCreditsAddress filter_address;
    
    if (request.params.size() > 0 && !request.params[0].isNull()) {
        string address_str = request.params[0].get_str();
        filter_address = CFleetCreditsAddress(address_str);
        if (filter_address.IsValid()) {
            filter_by_address = true;
        } else {
            // Maybe it's a contribution type instead?
            string type_str = address_str;
            if (type_str == "CODE_CONTRIBUTION") {
                filter_type = CODE_CONTRIBUTION;
                filter_by_type = true;
            } else if (type_str == "CHARITABLE_ACT") {
                filter_type = CHARITABLE_ACT;
                filter_by_type = true;
            } else if (type_str == "CREATIVE_WORK") {
                filter_type = CREATIVE_WORK;
                filter_by_type = true;
            } else if (type_str == "EDUCATIONAL_CONTENT") {
                filter_type = EDUCATIONAL_CONTENT;
                filter_by_type = true;
            } else if (type_str == "CODE_MENTORSHIP") {
                filter_type = CODE_MENTORSHIP;
                filter_by_type = true;
            } else if (type_str == "AI_VALIDATION") {
                filter_type = AI_VALIDATION;
                filter_by_type = true;
            } else if (type_str == "DATA_LABELING") {
                filter_type = DATA_LABELING;
                filter_by_type = true;
            } else if (type_str == "ETHICAL_REVIEW") {
                filter_type = ETHICAL_REVIEW;
                filter_by_type = true;
            }
        }
    }
    
    if (request.params.size() > 1 && !request.params[1].isNull()) {
        string type_str = request.params[1].get_str();
        if (type_str == "CODE_CONTRIBUTION") {
            filter_type = CODE_CONTRIBUTION;
            filter_by_type = true;
        } else if (type_str == "CHARITABLE_ACT") {
            filter_type = CHARITABLE_ACT;
            filter_by_type = true;
        } else if (type_str == "CREATIVE_WORK") {
            filter_type = CREATIVE_WORK;
            filter_by_type = true;
        } else if (type_str == "EDUCATIONAL_CONTENT") {
            filter_type = EDUCATIONAL_CONTENT;
            filter_by_type = true;
        } else if (type_str == "CODE_MENTORSHIP") {
            filter_type = CODE_MENTORSHIP;
            filter_by_type = true;
        } else if (type_str == "AI_VALIDATION") {
            filter_type = AI_VALIDATION;
            filter_by_type = true;
        } else if (type_str == "DATA_LABELING") {
            filter_type = DATA_LABELING;
            filter_by_type = true;
        } else if (type_str == "ETHICAL_REVIEW") {
            filter_type = ETHICAL_REVIEW;
            filter_by_type = true;
        }
    }

    UniValue result(UniValue::VARR);
    
    // Scan recent blocks for contribution transactions
    // Start from current tip and go back a reasonable number of blocks (e.g., 1000)
    int maxBlocks = 1000;
    int startHeight = chainActive.Height();
    int endHeight = std::max(0, startHeight - maxBlocks);
    
    for (int height = startHeight; height >= endHeight; height--) {
        CBlockIndex* pindex = chainActive[height];
        if (!pindex) continue;
        
        CBlock block;
        if (!ReadBlockFromDisk(block, pindex, Params().GetConsensus(0))) {
            continue;
        }
        
        // Scan transactions in block
        for (size_t i = 1; i < block.vtx.size(); i++) {
            const CTransaction& tx = *block.vtx[i];
            
            CContributionTransaction contrib_tx;
            if (ExtractContributionFromTransaction(tx, contrib_tx)) {
                // Apply filters
                if (filter_by_type && contrib_tx.contrib_type != filter_type) {
                    continue;
                }
                
                // Note: Address filtering would require extracting address from transaction inputs
                // For now, we'll skip address filtering or implement a simplified version
                if (filter_by_address) {
                    // TODO: Extract contributor address from transaction inputs
                    // For now, skip address filtering
                }
                
                // Determine status
                string status = "verified";
                int confirmations = startHeight - height + 1;
                if (confirmations < 12) {
                    status = confirmations > 0 ? "confirmed" : "pending";
                }
                
                // Calculate reward
                CAmount baseReward = 10000 * COIN;
                double multiplier = GetBonusMultiplier(contrib_tx.bonus_level, contrib_tx.contrib_type);
                CAmount reward = static_cast<CAmount>(baseReward * multiplier);
                
                UniValue entry(UniValue::VOBJ);
                entry.pushKV("txid", contrib_tx.tx_id.ToString());
                entry.pushKV("type", GetContributionTypeName(contrib_tx.contrib_type));
                entry.pushKV("status", status);
                entry.pushKV("confirmations", confirmations);
                entry.pushKV("reward", reward / COIN);
                entry.pushKV("bonus_level", GetBonusLevelName(contrib_tx.bonus_level));
                entry.pushKV("blockhash", pindex->GetBlockHash().ToString());
                entry.pushKV("blockheight", height);
                entry.pushKV("timestamp", contrib_tx.timestamp);
                entry.pushKV("requires_mweb", contrib_tx.requires_mweb);
                
                result.push_back(entry);
            }
        }
        
        // Also extract contributions from MWEB extension block if present
        if (block.mweb_extension) {
            std::vector<CContributionTransaction> mweb_contribs = ExtractContributionsFromMWEB(*block.mweb_extension);
            for (const auto& contrib_tx : mweb_contribs) {
                // Apply filters
                if (filter_by_type && contrib_tx.contrib_type != filter_type) {
                    continue;
                }
                
                // MWEB contributions are privacy-sensitive, address filtering may be limited
                if (filter_by_address) {
                    // TODO: Extract contributor address from MWEB transaction (requires view key)
                    // For now, skip address filtering for MWEB contributions
                }
                
                // Determine status
                string status = "verified";
                int confirmations = startHeight - height + 1;
                if (confirmations < 12) {
                    status = confirmations > 0 ? "confirmed" : "pending";
                }
                
                // Calculate reward
                CAmount baseReward = 10000 * COIN;
                double multiplier = GetBonusMultiplier(contrib_tx.bonus_level, contrib_tx.contrib_type);
                CAmount reward = static_cast<CAmount>(baseReward * multiplier);
                
                UniValue entry(UniValue::VOBJ);
                entry.pushKV("txid", contrib_tx.tx_id.ToString());
                entry.pushKV("type", GetContributionTypeName(contrib_tx.contrib_type));
                entry.pushKV("status", status);
                entry.pushKV("confirmations", confirmations);
                entry.pushKV("reward", reward / COIN);
                entry.pushKV("bonus_level", GetBonusLevelName(contrib_tx.bonus_level));
                entry.pushKV("blockhash", pindex->GetBlockHash().ToString());
                entry.pushKV("blockheight", height);
                entry.pushKV("timestamp", contrib_tx.timestamp);
                entry.pushKV("requires_mweb", true);
                entry.pushKV("mweb", true);  // Indicate this is from MWEB extension block
                
                result.push_back(entry);
            }
        }
    }
    
    // Also check mempool for pending contributions
    {
        LOCK(mempool.cs);
        std::vector<uint256> vtxid;
        mempool.queryHashes(vtxid);
        
        for (const uint256& txid : vtxid) {
            CTransactionRef tx = mempool.get(txid);
            if (!tx) continue;
            
            CContributionTransaction contrib_tx;
            if (ExtractContributionFromTransaction(*tx, contrib_tx)) {
                // Apply filters
                if (filter_by_type && contrib_tx.contrib_type != filter_type) {
                    continue;
                }
                
                UniValue entry(UniValue::VOBJ);
                entry.pushKV("txid", contrib_tx.tx_id.ToString());
                entry.pushKV("type", GetContributionTypeName(contrib_tx.contrib_type));
                entry.pushKV("status", "pending");
                entry.pushKV("confirmations", 0);
                entry.pushKV("reward", 0);  // No reward until confirmed
                entry.pushKV("bonus_level", GetBonusLevelName(contrib_tx.bonus_level));
                entry.pushKV("timestamp", contrib_tx.timestamp);
                entry.pushKV("requires_mweb", contrib_tx.requires_mweb);
                
                result.push_back(entry);
            }
        }
    }
    
    // Also check MWEB mempool for pending MWEB contributions
    {
        extern CMWEBMempool mwebMempool;
        std::vector<CMWEBTransaction> mweb_txs = mwebMempool.GetMWEBTransactions();
        
        for (const auto& mweb_tx : mweb_txs) {
            // Extract contribution data from MWEB transaction outputs
            // MWEB contributions embed data in view_key field
            for (const auto& output : mweb_tx.outputs) {
                if (!output.view_key.empty()) {
                    try {
                        CDataStream ss(output.view_key, SER_NETWORK, PROTOCOL_VERSION);
                        CContributionTransaction contrib_tx;
                        uint8_t contrib_type_byte;
                        ss >> contrib_type_byte;
                        contrib_tx.contrib_type = static_cast<ContributionType>(contrib_type_byte);
                        ss >> contrib_tx.proof_data.hash;
                        
                        // Apply filters
                        if (filter_by_type && contrib_tx.contrib_type != filter_type) {
                            continue;
                        }
                        
                        contrib_tx.tx_id = mweb_tx.tx_hash;
                        contrib_tx.requires_mweb = true;
                        contrib_tx.timestamp = GetTime();  // Use current time as placeholder
                        
                        UniValue entry(UniValue::VOBJ);
                        entry.pushKV("txid", contrib_tx.tx_id.ToString());
                        entry.pushKV("type", GetContributionTypeName(contrib_tx.contrib_type));
                        entry.pushKV("status", "pending_mweb");
                        entry.pushKV("confirmations", 0);
                        entry.pushKV("reward", 0);  // No reward until confirmed
                        entry.pushKV("bonus_level", GetBonusLevelName(contrib_tx.bonus_level));
                        entry.pushKV("timestamp", contrib_tx.timestamp);
                        entry.pushKV("requires_mweb", true);
                        entry.pushKV("mweb", true);
                        
                        result.push_back(entry);
                    } catch (const std::exception& e) {
                        // Invalid MWEB contribution data, skip
                        continue;
                    }
                }
            }
        }
    }

    return result;
}

static const CRPCCommand commands[] =
{ //  category              name                      actor (function)         safeMode
  //  --------------------- ------------------------  -----------------------  ----------
    { "contribution",       "submitcontribution",     &submitcontribution,     false,  {"contrib_type", "proof_data", "metadata"} },
    { "contribution",       "getcontributionstatus",  &getcontributionstatus, false,  {"txid"} },
    { "contribution",       "listcontributions",      &listcontributions,      false,  {"address", "contrib_type"} },
};

void RegisterContributionRPCCommands(CRPCTable &t)
{
    for (unsigned int vcidx = 0; vcidx < ARRAYLEN(commands); vcidx++)
        t.appendCommand(commands[vcidx].name, &commands[vcidx]);
}

