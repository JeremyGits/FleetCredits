// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FLEETCREDITS_TEST_MWEB_TEST_H
#define FLEETCREDITS_TEST_MWEB_TEST_H

#include "primitives/mweb.h"
#include "primitives/contribution.h"
#include "primitives/block.h"
#include "test/test_fleetcredits.h"
#include "random.h"
#include "hash.h"

#include <boost/test/unit_test.hpp>

/** MWEB Test Utilities */
namespace MWEBTest {

/** Create a test MWEB transaction with placeholder commitments */
CMWEBTransaction CreateTestMWEBTransaction(uint64_t amount, const uint256& blinding_factor);

/** Create a test MWEB extension block */
CMWEBExtensionBlock CreateTestMWEBExtensionBlock(const std::vector<CMWEBTransaction>& txs);

/** Create a test peg-in transaction */
CPegInTransaction CreateTestPegInTransaction(uint64_t amount, const CPubKey& recipient);

/** Create a test peg-out transaction */
CPegOutTransaction CreateTestPegOutTransaction(const CPedersenCommitment& commitment, uint64_t amount, const CPubKey& recipient);

/** Verify MWEB transaction structure (basic validation) */
bool VerifyMWEBTransactionStructure(const CMWEBTransaction& tx);

/** Verify MWEB extension block structure */
bool VerifyMWEBExtensionBlockStructure(const CMWEBExtensionBlock& block);

/** Test MWEB contribution routing */
bool TestContributionRouting(const CContributionTransaction& contrib_tx);

} // namespace MWEBTest

#endif // FLEETCREDITS_TEST_MWEB_TEST_H

