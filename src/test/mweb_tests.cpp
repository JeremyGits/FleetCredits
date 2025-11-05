// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "test/mweb_test.h"
#include "primitives/mweb.h"
#include "primitives/contribution.h"
#include "primitives/block.h"
#include "primitives/verification.h"
#include "random.h"
#include "hash.h"

#include <boost/test/unit_test.hpp>
#include <vector>

BOOST_FIXTURE_TEST_SUITE(mweb_tests, TestingSetup)

BOOST_AUTO_TEST_CASE(mweb_transaction_structure)
{
    // Test creating a basic MWEB transaction
    uint256 blinding_factor = GetRandHash();
    CMWEBTransaction tx = MWEBTest::CreateTestMWEBTransaction(1000 * COIN, blinding_factor);
    
    BOOST_CHECK(MWEBTest::VerifyMWEBTransactionStructure(tx));
    BOOST_CHECK(!tx.tx_hash.IsNull());
    BOOST_CHECK_EQUAL(tx.outputs.size(), 1);
    BOOST_CHECK_EQUAL(tx.kernels.size(), 1);
}

BOOST_AUTO_TEST_CASE(mweb_extension_block_structure)
{
    // Test creating an MWEB extension block
    std::vector<CMWEBTransaction> txs;
    
    // Create a few test transactions
    for (int i = 0; i < 3; i++) {
        uint256 blinding_factor = GetRandHash();
        txs.push_back(MWEBTest::CreateTestMWEBTransaction((i + 1) * 1000 * COIN, blinding_factor));
    }
    
    CMWEBExtensionBlock block = MWEBTest::CreateTestMWEBExtensionBlock(txs);
    
    BOOST_CHECK(MWEBTest::VerifyMWEBExtensionBlockStructure(block));
    BOOST_CHECK_EQUAL(block.mweb_txs.size(), 3);
    BOOST_CHECK(!block.extension_block_hash.IsNull());
}

BOOST_AUTO_TEST_CASE(mweb_peg_in_out)
{
    // Test peg-in transaction
    CPubKey recipient;
    // Create a valid compressed public key (33 bytes)
    // GetRandHash() returns a 32-byte uint256, so we need to pad it for a 33-byte pubkey
    uint256 hash = GetRandHash();
    std::vector<unsigned char> pubkey_bytes(33);
    // Copy 32 bytes from hash, then add compressed pubkey prefix
    for (size_t i = 0; i < 32; i++) {
        pubkey_bytes[i] = hash.begin()[i];
    }
    pubkey_bytes[32] = 0x02; // Set valid compressed pubkey prefix
    recipient.Set(pubkey_bytes.begin(), pubkey_bytes.end());
    
    CPegInTransaction peg_in = MWEBTest::CreateTestPegInTransaction(5000 * COIN, recipient);
    
    BOOST_CHECK_EQUAL(peg_in.amount, 5000 * COIN);
    BOOST_CHECK(!peg_in.peg_tx_id.IsNull());
    BOOST_CHECK(!peg_in.main_chain_tx_id.IsNull());
    
    // Test peg-out transaction
    CPedersenCommitment commitment = MWEB::CreateCommitment(3000 * COIN, GetRandHash());
    CPegOutTransaction peg_out = MWEBTest::CreateTestPegOutTransaction(commitment, 3000 * COIN, recipient);
    
    BOOST_CHECK_EQUAL(peg_out.amount, 3000 * COIN);
    BOOST_CHECK(!peg_out.peg_tx_id.IsNull());
    BOOST_CHECK(!peg_out.mweb_input_commitment.IsNull());
}

BOOST_AUTO_TEST_CASE(mweb_contribution_routing)
{
    // Test that ETHICAL_REVIEW contributions route to MWEB
    CContributionTransaction ethical_review;
    ethical_review.contrib_type = ETHICAL_REVIEW;
    ethical_review.requires_mweb = true;
    
    BOOST_CHECK(ethical_review.RequiresMWEB());
    BOOST_CHECK(MWEBTest::TestContributionRouting(ethical_review));
    
    // Test that regular contributions don't route
    CContributionTransaction code_contrib;
    code_contrib.contrib_type = CODE_CONTRIBUTION;
    code_contrib.requires_mweb = false;
    
    BOOST_CHECK(!code_contrib.RequiresMWEB());
    BOOST_CHECK(MWEBTest::TestContributionRouting(code_contrib));
}

BOOST_AUTO_TEST_CASE(mweb_block_integration)
{
    // Test MWEB extension block integration with main chain block
    CBlock main_block;
    main_block.SetNull();
    
    // Create MWEB extension block
    std::vector<CMWEBTransaction> mweb_txs;
    uint256 blinding_factor = GetRandHash();
    mweb_txs.push_back(MWEBTest::CreateTestMWEBTransaction(10000 * COIN, blinding_factor));
    
    CMWEBExtensionBlock mweb_block = MWEBTest::CreateTestMWEBExtensionBlock(mweb_txs);
    main_block.mweb_extension.reset(new CMWEBExtensionBlock(mweb_block));
    
    BOOST_CHECK(main_block.mweb_extension != nullptr);
    BOOST_CHECK(MWEBTest::VerifyMWEBExtensionBlockStructure(*main_block.mweb_extension));
    BOOST_CHECK_EQUAL(main_block.mweb_extension->mweb_txs.size(), 1);
}

BOOST_AUTO_TEST_CASE(mweb_commitment_verification)
{
    // Test Pedersen commitment creation and verification
    uint64_t amount = 5000 * COIN;
    uint256 blinding_factor = GetRandHash();
    
    CPedersenCommitment commitment = MWEB::CreateCommitment(amount, blinding_factor);
    
    BOOST_CHECK(!commitment.IsNull());
    BOOST_CHECK(MWEB::VerifyCommitment(commitment));
    
    // Test range proof generation
    CRangeProof proof = MWEB::GenerateRangeProof(commitment, amount);
    
    BOOST_CHECK(proof.IsValid());
    BOOST_CHECK_EQUAL(proof.value_range_min, 0);
    BOOST_CHECK(proof.value_range_max <= MAX_MONEY);
    
    // Test range proof verification
    BOOST_CHECK(MWEB::VerifyRangeProof(proof, commitment));
}

BOOST_AUTO_TEST_CASE(mweb_cut_through)
{
    // Test cut-through optimization
    CMWEBExtensionBlock block;
    
    // Create transactions with outputs that are spent as inputs in same block
    // (simplified test - in real implementation, cut-through removes these)
    
    std::vector<CMWEBTransaction> txs;
    for (int i = 0; i < 2; i++) {
        uint256 blinding_factor = GetRandHash();
        txs.push_back(MWEBTest::CreateTestMWEBTransaction((i + 1) * 1000 * COIN, blinding_factor));
    }
    
    block.mweb_txs = txs;
    block.prev_mweb_hash = GetRandHash();
    block.mweb_root = GetRandHash();
    
    // Test cut-through function
    CMWEBExtensionBlock cut_block = MWEB::CutThrough(block);
    
    BOOST_CHECK(MWEBTest::VerifyMWEBExtensionBlockStructure(cut_block));
}

BOOST_AUTO_TEST_CASE(mweb_view_key)
{
    // Test view key generation
    uint256 seed = GetRandHash();
    CViewKey view_key = CViewKey::GenerateFromSeed(seed);
    
    BOOST_CHECK(view_key.view_public_key.IsValid());
    BOOST_CHECK(!view_key.view_key_hash.IsNull());
    
    // Test transaction decryption (placeholder - will be fully implemented later)
    CMWEBTransaction tx = MWEBTest::CreateTestMWEBTransaction(1000 * COIN, GetRandHash());
    BOOST_CHECK(view_key.DecryptTransaction(tx));
}

BOOST_AUTO_TEST_SUITE_END()

