// Copyright (c) 2015-2022 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "amount.h"
#include "chain.h"
#include "chainparams.h"
#include "primitives/contribution.h"
#include "primitives/block.h"

// Forward declarations
class CMWEBExtensionBlock;

/** Extract contributions from MWEB extension block */
std::vector<CContributionTransaction> ExtractContributionsFromMWEB(const CMWEBExtensionBlock& mweb_block);

// Forward declaration
class CMWEBTransaction;
/** Route contribution transaction through MWEB if required */
bool RouteToMWEB(const CContributionTransaction& contrib_tx, CMWEBTransaction& mweb_tx);

bool AllowDigishieldMinDifficultyForBlock(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params);
CAmount GetFleetCreditsBlockSubsidy(int nHeight, const Consensus::Params& consensusParams, uint256 prevHash);
CAmount GetFleetCreditsBlockSubsidyWithContributions(int nHeight, const Consensus::Params& consensusParams, uint256 prevHash, const std::vector<CContributionTransaction>& contrib_txs);
std::vector<CContributionTransaction> ExtractContributionsFromBlock(const CBlock& block);
unsigned int CalculateFleetCreditsNextWorkRequired(const CBlockIndex* pindexLast, int64_t nLastRetargetTime, const Consensus::Params& params);

/**
 * Check proof-of-work of a block header, taking auxpow into account.
 * @param block The block header.
 * @param params Consensus parameters.
 * @return True iff the PoW is correct.
 */
bool CheckAuxPowProofOfWork(const CBlockHeader& block, const Consensus::Params& params);


