// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FLEETCREDITS_MWEB_MEMPOOL_H
#define FLEETCREDITS_MWEB_MEMPOOL_H

#include "primitives/mweb.h"
#include "sync.h"
#include "uint256.h"

#include <map>
#include <set>
#include <vector>

/** MWEB Mempool
 * Stores pending MWEB transactions, peg-ins, and peg-outs
 * for inclusion in the next MWEB extension block
 */
class CMWEBMempool
{
private:
    mutable CCriticalSection cs_mweb_mempool;
    
    // Pending MWEB transactions
    std::map<uint256, CMWEBTransaction> mweb_txs;
    
    // Pending peg-in transactions
    std::map<uint256, CPegInTransaction> peg_ins;
    
    // Pending peg-out transactions
    std::map<uint256, CPegOutTransaction> peg_outs;

public:
    CMWEBMempool() {}
    
    /** Add MWEB transaction to mempool */
    bool AddMWEBTransaction(const CMWEBTransaction& tx);
    
    /** Remove MWEB transaction from mempool */
    bool RemoveMWEBTransaction(const uint256& txid);
    
    /** Get all pending MWEB transactions */
    std::vector<CMWEBTransaction> GetMWEBTransactions() const;
    
    /** Add peg-in transaction */
    bool AddPegIn(const CPegInTransaction& peg_in);
    
    /** Remove peg-in transaction */
    bool RemovePegIn(const uint256& peg_tx_id);
    
    /** Get all pending peg-ins */
    std::vector<CPegInTransaction> GetPegIns() const;
    
    /** Add peg-out transaction */
    bool AddPegOut(const CPegOutTransaction& peg_out);
    
    /** Remove peg-out transaction */
    bool RemovePegOut(const uint256& peg_tx_id);
    
    /** Get all pending peg-outs */
    std::vector<CPegOutTransaction> GetPegOuts() const;
    
    /** Clear all pending transactions (called when block is mined) */
    void Clear();
    
    /** Get total number of pending transactions */
    size_t Size() const;
};

// Global MWEB mempool instance
extern CMWEBMempool mwebMempool;

#endif // FLEETCREDITS_MWEB_MEMPOOL_H

