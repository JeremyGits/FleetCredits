// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "mweb_mempool.h"

#include "primitives/mweb.h"
#include "sync.h"

CMWEBMempool mwebMempool;

bool CMWEBMempool::AddMWEBTransaction(const CMWEBTransaction& tx)
{
    LOCK(cs_mweb_mempool);
    
    // Verify transaction before adding
    if (!tx.VerifyBalance()) {
        return false;
    }
    if (!tx.VerifyRangeProofs()) {
        return false;
    }
    
    mweb_txs[tx.tx_hash] = tx;
    return true;
}

bool CMWEBMempool::RemoveMWEBTransaction(const uint256& txid)
{
    LOCK(cs_mweb_mempool);
    
    auto it = mweb_txs.find(txid);
    if (it != mweb_txs.end()) {
        mweb_txs.erase(it);
        return true;
    }
    return false;
}

std::vector<CMWEBTransaction> CMWEBMempool::GetMWEBTransactions() const
{
    LOCK(cs_mweb_mempool);
    
    std::vector<CMWEBTransaction> result;
    result.reserve(mweb_txs.size());
    for (const auto& pair : mweb_txs) {
        result.push_back(pair.second);
    }
    return result;
}

bool CMWEBMempool::AddPegIn(const CPegInTransaction& peg_in)
{
    LOCK(cs_mweb_mempool);
    
    if (peg_in.amount <= 0 || peg_in.peg_tx_id.IsNull()) {
        return false;
    }
    
    peg_ins[peg_in.peg_tx_id] = peg_in;
    return true;
}

bool CMWEBMempool::RemovePegIn(const uint256& peg_tx_id)
{
    LOCK(cs_mweb_mempool);
    
    auto it = peg_ins.find(peg_tx_id);
    if (it != peg_ins.end()) {
        peg_ins.erase(it);
        return true;
    }
    return false;
}

std::vector<CPegInTransaction> CMWEBMempool::GetPegIns() const
{
    LOCK(cs_mweb_mempool);
    
    std::vector<CPegInTransaction> result;
    result.reserve(peg_ins.size());
    for (const auto& pair : peg_ins) {
        result.push_back(pair.second);
    }
    return result;
}

bool CMWEBMempool::AddPegOut(const CPegOutTransaction& peg_out)
{
    LOCK(cs_mweb_mempool);
    
    if (peg_out.amount <= 0 || peg_out.peg_tx_id.IsNull()) {
        return false;
    }
    
    if (peg_out.mweb_input_commitment.IsNull()) {
        return false;
    }
    
    peg_outs[peg_out.peg_tx_id] = peg_out;
    return true;
}

bool CMWEBMempool::RemovePegOut(const uint256& peg_tx_id)
{
    LOCK(cs_mweb_mempool);
    
    auto it = peg_outs.find(peg_tx_id);
    if (it != peg_outs.end()) {
        peg_outs.erase(it);
        return true;
    }
    return false;
}

std::vector<CPegOutTransaction> CMWEBMempool::GetPegOuts() const
{
    LOCK(cs_mweb_mempool);
    
    std::vector<CPegOutTransaction> result;
    result.reserve(peg_outs.size());
    for (const auto& pair : peg_outs) {
        result.push_back(pair.second);
    }
    return result;
}

void CMWEBMempool::Clear()
{
    LOCK(cs_mweb_mempool);
    
    mweb_txs.clear();
    peg_ins.clear();
    peg_outs.clear();
}

size_t CMWEBMempool::Size() const
{
    LOCK(cs_mweb_mempool);
    
    return mweb_txs.size() + peg_ins.size() + peg_outs.size();
}

