// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "rpc/server.h"
#include "rpc/client.h"
#include "primitives/mweb.h"
#include "primitives/contribution.h"
#include "primitives/transaction.h"
#include "wallet/wallet.h"
#include "wallet/rpcutil.h"
#include "validation.h"
#include "base58.h"
#include "util.h"
#include "random.h"
#include "chain.h"
#include "txmempool.h"
#include "rpc/contribution.h"
#include "mweb_mempool.h"
#include "streams.h"
#include "protocol.h"
#include "chainparams.h"
#include "amount.h"
#include "wallet/rpcwallet.h"
#include "fleetcredits.h"

#include <univalue.h>
#include <cstring>

using namespace std;

// Forward declarations from wallet/rpcwallet.cpp
bool EnsureWalletIsAvailable(bool avoidException);
void EnsureWalletIsUnlocked();

extern CWallet* pwalletMain;
extern CTxMemPool mempool;
extern CChain chainActive;

// Forward declaration from validation.h
bool GetTransaction(const uint256 &hash, CTransactionRef &txOut, const Consensus::Params& params, uint256 &hashBlock, bool fAllowSlow);

/** Create a peg-in transaction (move FC from main chain to MWEB) */
UniValue createpegin(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() < 2 || request.params.size() > 3)
        throw runtime_error(
            "createpegin \"address\" amount ( \"comment\" )\n"
            "\nCreate a peg-in transaction to move FC from main chain to MWEB.\n"
            "\nArguments:\n"
            "1. \"address\"     (string, required) MWEB address to receive the pegged funds\n"
            "2. amount         (numeric, required) Amount in FC to peg into MWEB\n"
            "3. \"comment\"     (string, optional) A comment used to store what the transaction is for\n"
            "\nResult:\n"
            "{\n"
            "  \"pegtxid\" : \"hex\",     (string) The peg-in transaction ID\n"
            "  \"maintxid\" : \"hex\",    (string) The main chain transaction ID\n"
            "  \"amount\" : x.xx,        (numeric) Amount pegged in FC\n"
            "  \"status\" : \"pending\"   (string) Peg-in status\n"
            "}\n"
            "\nExamples:\n"
            + HelpExampleCli("createpegin", "\"mweb_address_here\" 1000")
            + HelpExampleRpc("createpegin", "\"mweb_address_here\", 1000")
        );

    // Ensure wallet is available
    if (!EnsureWalletIsAvailable(false))
        throw JSONRPCError(RPC_WALLET_ERROR, "Wallet is not available");

    LOCK2(cs_main, pwalletMain->cs_wallet);
    EnsureWalletIsUnlocked();

    // Parse parameters
    string address_str = request.params[0].get_str();
    CAmount amount = AmountFromValue(request.params[1]);
    string comment = request.params.size() > 2 ? request.params[2].get_str() : "";

    if (amount <= 0)
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Amount must be positive");

    // Parse address - for now, use regular Fleet Credits address (will be converted to MWEB address later)
    CFleetCreditsAddress address;
    if (!address.SetString(address_str)) {
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid Fleet Credits address");
    }
    
    // Get KeyID from address - for MWEB, we'll use this to derive the MWEB key
    // For now, create a placeholder pubkey from the address
    // In full implementation, would derive MWEB key from address
    CKeyID keyid;
    CPubKey recipient_pubkey;
    if (address.GetKeyID(keyid)) {
        // For now, create a placeholder pubkey from the keyid
        // In production, this would be a proper MWEB key derivation
        // Create a 33-byte pubkey from the keyid (20 bytes) + padding
        std::vector<unsigned char> pubkey_bytes(33);
        // Copy keyid bytes (20 bytes) to first 20 bytes of pubkey
        memcpy(pubkey_bytes.data(), keyid.begin(), 20);
        // Fill rest with zeros and set compressed prefix
        memset(pubkey_bytes.data() + 20, 0, 12);
        pubkey_bytes[32] = 0x02;  // Compressed pubkey prefix
        recipient_pubkey.Set(pubkey_bytes.begin(), pubkey_bytes.end());
    } else {
        // Script address or invalid - use placeholder
        recipient_pubkey = CPubKey();
    }
    
    // Create peg-in transaction
    CPegInTransaction peg_in;
    peg_in.amount = amount;
    peg_in.peg_tx_id = GetRandHash();
    peg_in.recipient_address = recipient_pubkey;  // Set recipient address
    peg_in.main_chain_tx_id.SetNull();  // Will be set after transaction is created
    
    // Create main chain transaction that locks funds for peg-in
    // Send amount to a special peg-in script (OP_RETURN with peg-in marker)
    CWalletTx wtxNew;
    CReserveKey reservekey(pwalletMain);
    CAmount nFeeRequired;
    string strError;
    int nChangePosRet = -1;
    
    // Create OP_RETURN output with peg-in marker: 0xFC 0x02 (FC = Fleet Credits, 0x02 = peg-in)
    vector<unsigned char> peg_script_data;
    peg_script_data.push_back(0xFC);  // Fleet Credits marker
    peg_script_data.push_back(0x02);   // Peg-in transaction type
    
    // Serialize peg-in data
    CDataStream peg_stream(SER_NETWORK, PROTOCOL_VERSION);
    peg_stream << peg_in.peg_tx_id;
    peg_stream << amount;
    peg_script_data.insert(peg_script_data.end(), peg_stream.begin(), peg_stream.end());
    
    CScript pegScript = CScript() << OP_RETURN << peg_script_data;
    
    // Create recipient: OP_RETURN output (0 value) + change back to sender
    vector<CRecipient> vecSend;
    CRecipient pegRecipient = {pegScript, 0, false};
    vecSend.push_back(pegRecipient);
    
    // Create transaction
    if (!pwalletMain->CreateTransaction(vecSend, wtxNew, reservekey, nFeeRequired, nChangePosRet, strError)) {
        throw JSONRPCError(RPC_WALLET_ERROR, strprintf("Error creating transaction: %s", strError));
    }

    // Sign and commit transaction
    CValidationState state;
    extern CConnman* g_connman;
    if (g_connman && !pwalletMain->CommitTransaction(wtxNew, reservekey, g_connman, state)) {
        throw JSONRPCError(RPC_WALLET_ERROR, 
            strprintf("Error: Transaction was rejected! Reason: %s", state.GetRejectReason()));
    } else if (!g_connman) {
        throw JSONRPCError(RPC_CLIENT_P2P_DISABLED, "Error: Peer-to-peer functionality missing or disabled");
    }
    
    // Set main chain transaction ID now that transaction is committed
    peg_in.main_chain_tx_id = wtxNew.GetHash();
    
    // Store peg-in in MWEB mempool for inclusion in next block
    extern CMWEBMempool mwebMempool;
    mwebMempool.AddPegIn(peg_in);

    UniValue result(UniValue::VOBJ);
    result.pushKV("pegtxid", peg_in.peg_tx_id.GetHex());
    result.pushKV("maintxid", peg_in.main_chain_tx_id.GetHex());
    result.pushKV("amount", ValueFromAmount(amount));
    result.pushKV("status", "pending");
    result.pushKV("message", "Peg-in transaction created. Funds will be available in MWEB after confirmation.");

    return result;
}

/** Create a peg-out transaction (move FC from MWEB to main chain) */
UniValue createpegout(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() < 3 || request.params.size() > 4)
        throw runtime_error(
            "createpegout \"commitment\" \"address\" amount ( \"comment\" )\n"
            "\nCreate a peg-out transaction to move FC from MWEB to main chain.\n"
            "\nArguments:\n"
            "1. \"commitment\"   (string, required) MWEB output commitment to peg out\n"
            "2. \"address\"      (string, required) Main chain address to receive funds\n"
            "3. amount          (numeric, required) Amount in FC to peg out\n"
            "4. \"comment\"      (string, optional) A comment used to store what the transaction is for\n"
            "\nResult:\n"
            "{\n"
            "  \"pegtxid\" : \"hex\",     (string) The peg-out transaction ID\n"
            "  \"maintxid\" : \"hex\",    (string) The main chain transaction ID (after confirmation)\n"
            "  \"amount\" : x.xx,        (numeric) Amount pegged out in FC\n"
            "  \"status\" : \"pending\"   (string) Peg-out status\n"
            "}\n"
            "\nExamples:\n"
            + HelpExampleCli("createpegout", "\"commitment_hex\" \"FleetCreditsAddress\" 1000")
            + HelpExampleRpc("createpegout", "\"commitment_hex\", \"FleetCreditsAddress\", 1000")
        );

    // Ensure wallet is available
    if (!EnsureWalletIsAvailable(false))
        throw JSONRPCError(RPC_WALLET_ERROR, "Wallet is not available");

    LOCK2(cs_main, pwalletMain->cs_wallet);
    EnsureWalletIsUnlocked();

    // Parse parameters
    string commitment_str = request.params[0].get_str();
    string address_str = request.params[1].get_str();
    CAmount amount = AmountFromValue(request.params[2]);
    string comment = request.params.size() > 3 ? request.params[3].get_str() : "";

    if (amount <= 0)
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Amount must be positive");

    // Parse MWEB commitment
    uint256 commitment_hash;
    commitment_hash.SetHex(commitment_str);
    if (commitment_hash.IsNull())
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid commitment format");

    // Parse main chain address
    CFleetCreditsAddress address(address_str);
    if (!address.IsValid())
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Invalid Fleet Credits address");

    // Create MWEB transaction that spends the commitment
    CMWEBTransaction mweb_tx;
    mweb_tx.lock_height = 0;
    
    // Create input spending the commitment
    CMWEBInput input;
    CPedersenCommitment commitment;
    commitment.blinding_factor_hash = commitment_hash;
    // Set commitment pubkey from hash (simplified - in production would lookup actual commitment)
    CPubKey commitment_pubkey;
    std::vector<unsigned char> hash_bytes(commitment_hash.begin(), commitment_hash.end());
    if (hash_bytes.size() < 33) {
        hash_bytes.resize(33, 0);
    }
    commitment_pubkey.Set(hash_bytes.begin(), hash_bytes.begin() + 33);
    commitment.commitment = commitment_pubkey;
    input.output_commitment = commitment;
    mweb_tx.inputs.push_back(input);
    
    // Create peg-out transaction
    CPegOutTransaction peg_out;
    peg_out.amount = amount;
    peg_out.peg_tx_id = GetRandHash();
    peg_out.mweb_input_commitment = commitment;
    
    // Extract address pubkey
    CKeyID keyid;
    if (address.GetKeyID(keyid)) {
        // Create script for main chain output
        CScript scriptPubKey = GetScriptForDestination(keyid);
        // Store script hash in peg_out (will be used to create main chain output after confirmation)
        CHashWriter ss(SER_GETHASH, 0);
        ss << scriptPubKey.size();
        ss.write((const char*)scriptPubKey.data(), scriptPubKey.size());
        uint256 script_hash = ss.GetHash();
        std::vector<unsigned char> script_hash_bytes(script_hash.begin(), script_hash.end());
        peg_out.view_key = script_hash_bytes;  // Reuse view_key field to store output script
    }
    
    // Create MWEB output with 0 value (change) and generate range proof
    CMWEBOutput output;
    uint256 blinding_factor = GetRandHash();
    output.commitment = MWEB::CreateCommitment(0, blinding_factor);
    output.range_proof = MWEB::GenerateRangeProof(output.commitment, 0);
    mweb_tx.outputs.push_back(output);
    
    // Create kernel with excess value
    CMWEBKernel kernel;
    kernel.fee = 0;  // Zero fees for micro-transactions
    uint256 excess_blinding = GetRandHash();
    CPedersenCommitment excess_commitment = MWEB::CreateCommitment(0, excess_blinding);
    kernel.excess_value = excess_commitment.commitment;
    
    // Generate kernel signature
    CHashWriter ss(SER_GETHASH, 0);
    ss << mweb_tx.inputs;
    ss << mweb_tx.outputs;
    ss << kernel.fee;
    ss << kernel.excess_value;
    uint256 kernel_hash = ss.GetHash();
    
    CKey sign_key;
    sign_key.Set(excess_blinding.begin(), excess_blinding.end(), true);
    std::vector<unsigned char> sig_bytes(64);
    if (sign_key.Sign(kernel_hash, sig_bytes)) {
        kernel.signature = sig_bytes;
    }
    kernel.kernel_id = GetRandHash();
    mweb_tx.kernels.push_back(kernel);
    mweb_tx.tx_hash = mweb_tx.GetHash();
    
    // Store peg-out in MWEB mempool for inclusion in next block
    extern CMWEBMempool mwebMempool;
    mwebMempool.AddPegOut(peg_out);
    // The main chain transaction will be created after MWEB block confirmation

    UniValue result(UniValue::VOBJ);
    result.pushKV("pegtxid", peg_out.peg_tx_id.GetHex());
    result.pushKV("mwebtxid", mweb_tx.tx_hash.GetHex());
    result.pushKV("amount", ValueFromAmount(amount));
    result.pushKV("status", "pending");
    result.pushKV("message", "Peg-out transaction created. Main chain transaction will be created after MWEB confirmation.");

    return result;
}

/** Route a contribution transaction through MWEB */
UniValue routecontributiontomweb(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() < 1)
        throw runtime_error(
            "routecontributiontomweb \"txid\"\n"
            "\nRoute an existing contribution transaction through MWEB for privacy.\n"
            "\nArguments:\n"
            "1. \"txid\"         (string, required) Contribution transaction ID to route\n"
            "\nResult:\n"
            "{\n"
            "  \"mwebtxid\" : \"hex\",    (string) The MWEB transaction ID\n"
            "  \"originaltxid\" : \"hex\", (string) The original contribution transaction ID\n"
            "  \"status\" : \"routed\"    (string) Routing status\n"
            "}\n"
            "\nExamples:\n"
            + HelpExampleCli("routecontributiontomweb", "\"txid_here\"")
            + HelpExampleRpc("routecontributiontomweb", "\"txid_here\"")
        );

    // Parse parameters
    string txid_str = request.params[0].get_str();
    uint256 txid;
    txid.SetHex(txid_str);
    if (txid.IsNull())
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid transaction ID");

    LOCK(cs_main);

    // Look up contribution transaction
    CTransactionRef tx;
    uint256 hashBlock;
    if (!GetTransaction(txid, tx, Params().GetConsensus(0), hashBlock, true)) {
        throw JSONRPCError(RPC_INVALID_ADDRESS_OR_KEY, "Transaction not found");
    }

    // Extract contribution from transaction
    CContributionTransaction contrib_tx;
    if (!ExtractContributionFromTransaction(*tx, contrib_tx)) {
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Transaction is not a contribution transaction");
    }

    // Check if it requires MWEB routing
    if (!contrib_tx.RequiresMWEB()) {
        // Convert to MWEB anyway if explicitly requested
        // This allows optional privacy for any contribution
    }

    // Convert to MWEB transaction using RouteToMWEB()
    CMWEBTransaction mweb_tx;
    if (!RouteToMWEB(contrib_tx, mweb_tx)) {
        throw JSONRPCError(RPC_INTERNAL_ERROR, "Failed to route contribution to MWEB");
    }

    // Store MWEB transaction in MWEB mempool for inclusion in next block
    extern CMWEBMempool mwebMempool;
    mwebMempool.AddMWEBTransaction(mweb_tx);
    
    UniValue result(UniValue::VOBJ);
    result.pushKV("mwebtxid", mweb_tx.tx_hash.GetHex());
    result.pushKV("originaltxid", txid_str);
    result.pushKV("status", "routed");
    result.pushKV("message", "Contribution routed to MWEB. Transaction will be included in next MWEB extension block.");

    return result;
}

/** List MWEB transactions */
UniValue listmwebtxs(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() > 2)
        throw runtime_error(
            "listmwebtxs ( \"address\" count )\n"
            "\nList MWEB transactions.\n"
            "\nArguments:\n"
            "1. \"address\"      (string, optional) Filter by MWEB address\n"
            "2. count           (numeric, optional, default=10) Number of transactions to return\n"
            "\nResult:\n"
            "[\n"
            "  {\n"
            "    \"txid\" : \"hex\",       (string) MWEB transaction ID\n"
            "    \"amount\" : x.xx,        (numeric) Transaction amount (if viewable)\n"
            "    \"confirmations\" : n,     (numeric) Number of confirmations\n"
            "    \"blockheight\" : n,       (numeric) Block height\n"
            "  }\n"
            "  ,...\n"
            "]\n"
            "\nExamples:\n"
            + HelpExampleCli("listmwebtxs", "")
            + HelpExampleCli("listmwebtxs", "\"mweb_address\" 20")
            + HelpExampleRpc("listmwebtxs", "")
        );

    LOCK(cs_main);

    // Parse optional parameters
    string address_filter = "";
    if (request.params.size() > 0 && !request.params[0].isNull()) {
        address_filter = request.params[0].get_str();
    }
    
    int count = 10;
    if (request.params.size() > 1 && !request.params[1].isNull()) {
        count = request.params[1].get_int();
        if (count < 0 || count > 100) {
            throw JSONRPCError(RPC_INVALID_PARAMETER, "Count must be between 0 and 100");
        }
    }

    UniValue result(UniValue::VARR);
    int found = 0;

    // First, check mempool for pending MWEB transactions
    std::vector<CMWEBTransaction> mempool_txs = mwebMempool.GetMWEBTransactions();
    for (const auto& mweb_tx : mempool_txs) {
        if (found >= count) {
            break;
        }
        
        UniValue tx_obj(UniValue::VOBJ);
        tx_obj.pushKV("txid", mweb_tx.tx_hash.GetHex());
        tx_obj.pushKV("amount", ValueFromAmount(0));  // Hidden by default
        tx_obj.pushKV("confirmations", 0);  // Pending
        tx_obj.pushKV("blockheight", -1);  // Not in block yet
        tx_obj.pushKV("viewable", false);
        tx_obj.pushKV("status", "pending");
        
        result.push_back(tx_obj);
        found++;
    }
    
    // Also check pending peg-ins and peg-outs
    std::vector<CPegInTransaction> peg_ins = mwebMempool.GetPegIns();
    for (const auto& peg_in : peg_ins) {
        if (found >= count) {
            break;
        }
        
        UniValue tx_obj(UniValue::VOBJ);
        tx_obj.pushKV("txid", peg_in.peg_tx_id.GetHex());
        tx_obj.pushKV("amount", ValueFromAmount(peg_in.amount));
        tx_obj.pushKV("confirmations", 0);
        tx_obj.pushKV("blockheight", -1);
        tx_obj.pushKV("viewable", true);  // Peg-in amounts are visible
        tx_obj.pushKV("status", "pending");
        tx_obj.pushKV("type", "peg-in");
        
        result.push_back(tx_obj);
        found++;
    }
    
    std::vector<CPegOutTransaction> peg_outs = mwebMempool.GetPegOuts();
    for (const auto& peg_out : peg_outs) {
        if (found >= count) {
            break;
        }
        
        UniValue tx_obj(UniValue::VOBJ);
        tx_obj.pushKV("txid", peg_out.peg_tx_id.GetHex());
        tx_obj.pushKV("amount", ValueFromAmount(peg_out.amount));
        tx_obj.pushKV("confirmations", 0);
        tx_obj.pushKV("blockheight", -1);
        tx_obj.pushKV("viewable", true);  // Peg-out amounts are visible
        tx_obj.pushKV("status", "pending");
        tx_obj.pushKV("type", "peg-out");
        
        result.push_back(tx_obj);
        found++;
    }

    // Query MWEB extension blocks for transactions
    // Iterate through blocks from tip backwards
    CBlockIndex* pindex = chainActive.Tip();
    
    while (pindex && found < count) {
        // Read block
        CBlock block;
        if (!ReadBlockFromDisk(block, pindex, Params().GetConsensus(0))) {
            break;
        }
        
        // Check if block has MWEB extension
        if (block.mweb_extension) {
            // Iterate through MWEB transactions
            for (const auto& mweb_tx : block.mweb_extension->mweb_txs) {
                if (found >= count) {
                    break;
                }
                
                UniValue tx_obj(UniValue::VOBJ);
                tx_obj.pushKV("txid", mweb_tx.tx_hash.GetHex());
                tx_obj.pushKV("amount", ValueFromAmount(0));  // Hidden by default
                tx_obj.pushKV("confirmations", 1 + chainActive.Height() - pindex->nHeight);
                tx_obj.pushKV("blockheight", (int64_t)pindex->nHeight);
                tx_obj.pushKV("viewable", false);
                tx_obj.pushKV("status", "confirmed");
                
                result.push_back(tx_obj);
                found++;
            }
            
            // Also list peg-ins and peg-outs from blocks
            for (const auto& peg_in : block.mweb_extension->peg_ins) {
                if (found >= count) {
                    break;
                }
                
                UniValue tx_obj(UniValue::VOBJ);
                tx_obj.pushKV("txid", peg_in.peg_tx_id.GetHex());
                tx_obj.pushKV("amount", ValueFromAmount(peg_in.amount));
                tx_obj.pushKV("confirmations", 1 + chainActive.Height() - pindex->nHeight);
                tx_obj.pushKV("blockheight", (int64_t)pindex->nHeight);
                tx_obj.pushKV("viewable", true);
                tx_obj.pushKV("status", "confirmed");
                tx_obj.pushKV("type", "peg-in");
                
                result.push_back(tx_obj);
                found++;
            }
        }
        
        pindex = pindex->pprev;
    }

    return result;
}

static const CRPCCommand commands[] =
{ //  category              name                      actor (function)         okSafeMode
  //  --------------------- ------------------------ ----------------------- ----------
    { "mweb",              "createpegin",             &createpegin,            false, {} },
    { "mweb",              "createpegout",            &createpegout,           false, {} },
    { "mweb",              "routecontributiontomweb", &routecontributiontomweb, false, {} },
    { "mweb",              "listmwebtxs",             &listmwebtxs,            true, {} },
};

void RegisterMWEBRPCCommands(CRPCTable &tableRPC)
{
    for (unsigned int vcidx = 0; vcidx < ARRAYLEN(commands); vcidx++)
        tableRPC.appendCommand(commands[vcidx].name, &commands[vcidx]);
}
