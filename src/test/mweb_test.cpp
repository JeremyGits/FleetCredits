// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "test/mweb_test.h"
#include "primitives/mweb.h"
#include "primitives/contribution.h"
#include "primitives/block.h"
#include "util.h"
#include "fleetcredits.h"

namespace MWEBTest {

CMWEBTransaction CreateTestMWEBTransaction(uint64_t amount, const uint256& blinding_factor)
{
    CMWEBTransaction tx;
    tx.lock_height = 0;
    
    // Create a test output
    CMWEBOutput output;
    output.commitment = MWEB::CreateCommitment(amount, blinding_factor);
    output.range_proof = MWEB::GenerateRangeProof(output.commitment, amount);
    tx.outputs.push_back(output);
    
    // Create a test kernel
    CMWEBKernel kernel;
    kernel.fee = 0;  // Zero fees for micro-transactions
    kernel.kernel_id = GetRandHash();
    tx.kernels.push_back(kernel);
    
    tx.tx_hash = tx.GetHash();
    
    return tx;
}

CMWEBExtensionBlock CreateTestMWEBExtensionBlock(const std::vector<CMWEBTransaction>& txs)
{
    CMWEBExtensionBlock block;
    block.mweb_txs = txs;
    block.prev_mweb_hash = GetRandHash();
    block.mweb_root = GetRandHash();
    block.extension_block_hash = block.GetHash();
    
    return block;
}

CPegInTransaction CreateTestPegInTransaction(uint64_t amount, const CPubKey& recipient)
{
    CPegInTransaction peg_in;
    peg_in.amount = amount;
    peg_in.recipient_address = recipient;
    peg_in.main_chain_tx_id = GetRandHash();
    peg_in.peg_tx_id = GetRandHash();
    
    return peg_in;
}

CPegOutTransaction CreateTestPegOutTransaction(const CPedersenCommitment& commitment, uint64_t amount, const CPubKey& recipient)
{
    CPegOutTransaction peg_out;
    peg_out.mweb_input_commitment = commitment;
    peg_out.amount = amount;
    peg_out.main_chain_address = recipient;
    peg_out.peg_tx_id = GetRandHash();
    
    return peg_out;
}

bool VerifyMWEBTransactionStructure(const CMWEBTransaction& tx)
{
    // Basic structure validation
    if (tx.outputs.empty() && tx.inputs.empty()) {
        return false;  // Empty transaction
    }
    
    if (tx.kernels.empty()) {
        return false;  // Must have at least one kernel
    }
    
    // Verify all outputs have commitments and range proofs
    for (const auto& output : tx.outputs) {
        if (output.commitment.IsNull()) {
            return false;
        }
        if (!output.range_proof.IsValid()) {
            return false;
        }
    }
    
    // Verify all kernels are valid
    for (const auto& kernel : tx.kernels) {
        if (!kernel.IsValid()) {
            return false;
        }
    }
    
    return true;
}

bool VerifyMWEBExtensionBlockStructure(const CMWEBExtensionBlock& block)
{
    // Verify all transactions in block
    for (const auto& tx : block.mweb_txs) {
        if (!VerifyMWEBTransactionStructure(tx)) {
            return false;
        }
    }
    
    // Verify block hash
    if (block.extension_block_hash != block.GetHash()) {
        return false;
    }
    
    return true;
}

bool TestContributionRouting(const CContributionTransaction& contrib_tx)
{
    // Test that ETHICAL_REVIEW contributions require MWEB
    if (contrib_tx.contrib_type == ETHICAL_REVIEW) {
        if (!contrib_tx.RequiresMWEB()) {
            return false;
        }
    }
    
    // Test routing function
    CMWEBTransaction mweb_tx;
    if (RouteToMWEB(contrib_tx, mweb_tx)) {
        // Verify MWEB transaction structure
        return VerifyMWEBTransactionStructure(mweb_tx);
    }
    
    // If routing returned false, check if it should have routed
    if (contrib_tx.RequiresMWEB()) {
        return false;  // Should have routed but didn't
    }
    
    return true;  // Correctly didn't route
}

} // namespace MWEBTest

