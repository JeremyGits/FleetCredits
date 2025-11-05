// Copyright (c) 2015-2022 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// 
// note from the developer:
// thanks bitcoin and dogecoin, I love you still and forever, thank you Satoshi (and Shibetoshi) for everything
// what you have done for the world is amazing, you are a true legend.
// you are the reason why I am here today, I will always be grateful to you for your contributions to the world.
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/random/uniform_int.hpp>
#include <boost/random/mersenne_twister.hpp>

#include "policy/policy.h"
#include "arith_uint256.h"
#include "fleetcredits.h"
#include "txmempool.h"
#include "util.h"
#include "fleetcredits-fees.h"
#include "primitives/contribution.h"
#include "primitives/block.h"
#include "primitives/mweb.h"
#include "fleetcredits.h"
#include "primitives/verification.h"
#include "script/script.h"
#include "streams.h"

int static generateMTRandom(unsigned int s, int range)
{
    boost::mt19937 gen(s);
    boost::uniform_int<> dist(1, range);
    return dist(gen);
}

// Fleet Credits: Normally minimum difficulty blocks can only occur in between
// retarget blocks. However, once we introduce Digishield every block is
// a retarget, so we need to handle minimum difficulty on all blocks.
bool AllowDigishieldMinDifficultyForBlock(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    // check if the chain allows minimum difficulty blocks
    if (!params.fPowAllowMinDifficultyBlocks)
        return false;

    // check if the chain allows minimum difficulty blocks on recalc blocks
    if (pindexLast->nHeight < 157500)
    // if (!params.fPowAllowDigishieldMinDifficultyBlocks)
        return false;

    // Allow for a minimum block time if the elapsed time > 2*nTargetSpacing
    return (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2);
}

unsigned int CalculateFleetCreditsNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    int nHeight = pindexLast->nHeight + 1;
    const int64_t retargetTimespan = params.nPowTargetTimespan;
    const int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    int64_t nModulatedTimespan = nActualTimespan;
    int64_t nMaxTimespan;
    int64_t nMinTimespan;

    if (params.fDigishieldDifficultyCalculation) //DigiShield implementation - thanks to RealSolid & WDC for this code
    {
        // amplitude filter - thanks to daft27 for this code
        nModulatedTimespan = retargetTimespan + (nModulatedTimespan - retargetTimespan) / 8;

        nMinTimespan = retargetTimespan - (retargetTimespan / 4);
        nMaxTimespan = retargetTimespan + (retargetTimespan / 2);
    } else if (nHeight > 10000) {
        nMinTimespan = retargetTimespan / 4;
        nMaxTimespan = retargetTimespan * 4;
    } else if (nHeight > 5000) {
        nMinTimespan = retargetTimespan / 8;
        nMaxTimespan = retargetTimespan * 4;
    } else {
        nMinTimespan = retargetTimespan / 16;
        nMaxTimespan = retargetTimespan * 4;
    }

    // Limit adjustment step
    if (nModulatedTimespan < nMinTimespan)
        nModulatedTimespan = nMinTimespan;
    else if (nModulatedTimespan > nMaxTimespan)
        nModulatedTimespan = nMaxTimespan;

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    bnNew *= nModulatedTimespan;
    bnNew /= retargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

bool CheckAuxPowProofOfWork(const CBlockHeader& block, const Consensus::Params& params)
{
    /* Except for legacy blocks with full version 1, ensure that
       the chain ID is correct.  Legacy blocks are not allowed since
       the merge-mining start, which is checked in AcceptBlockHeader
       where the height is known.  */
    if (!block.IsLegacy() && params.fStrictChainId && block.GetChainId() != params.nAuxpowChainId)
        return error("%s : block does not have our chain ID"
                     " (got %d, expected %d, full nVersion %d)",
                     __func__, block.GetChainId(),
                     params.nAuxpowChainId, block.nVersion);

    /* If there is no auxpow, just check the block hash.  */
    if (!block.auxpow) {
        if (block.IsAuxpow())
            return error("%s : no auxpow on block with auxpow version",
                         __func__);

        if (!CheckProofOfWork(block.GetPoWHash(), block.nBits, params))
            return error("%s : non-AUX proof of work failed", __func__);

        return true;
    }

    /* We have auxpow.  Check it.  */

    if (!block.IsAuxpow())
        return error("%s : auxpow on block with non-auxpow version", __func__);

    if (!CheckProofOfWork(block.auxpow->getParentBlockPoWHash(), block.nBits, params))
        return error("%s : AUX proof of work failed", __func__);

    // CAuxPow::check is only available when auxpow.cpp is linked (server builds)
    // For fleetcredits-tx, we can't fully validate auxpow, so return false
    // This function should not be called in fleetcredits-tx context anyway
    // Note: We can't call block.auxpow->check() here because it's not linked
    // The real implementation is in auxpow.cpp which is only in server library
    return false; // Cannot fully validate auxpow without server library
}

CAmount GetFleetCreditsBlockSubsidy(int nHeight, const Consensus::Params& consensusParams, uint256 prevHash)
{
    int halvings = nHeight / consensusParams.nSubsidyHalvingInterval;

    if (!consensusParams.fSimplifiedRewards)
    {
        // Old-style rewards derived from the previous block hash
        const std::string cseed_str = prevHash.ToString().substr(7, 7);
        const char* cseed = cseed_str.c_str();
        char* endp = NULL;
        long seed = strtol(cseed, &endp, 16);
        CAmount maxReward = (1000000 >> halvings) - 1;
        int rand = generateMTRandom(seed, maxReward);

        return (1 + rand) * COIN;
    } else if (nHeight < (6 * consensusParams.nSubsidyHalvingInterval)) {
        // New-style constant rewards for each halving interval
        return (500000 * COIN) >> halvings;
    } else {
        // Constant inflation - Fleet Credits: Fixed 10,000 FC per block
        return 10000 * COIN;
    }
}

/** Get block subsidy with contribution bonuses applied */
CAmount GetFleetCreditsBlockSubsidyWithContributions(int nHeight, const Consensus::Params& consensusParams, uint256 prevHash, const std::vector<CContributionTransaction>& contrib_txs)
{
    // Get base reward
    CAmount baseReward = GetFleetCreditsBlockSubsidy(nHeight, consensusParams, prevHash);
    
    // If no contributions, return base reward
    if (contrib_txs.empty()) {
        return baseReward;
    }
    
    // Find highest bonus level from verified contributions
    uint32_t maxBonusLevel = BONUS_NONE;
    ContributionType maxContribType = CODE_CONTRIBUTION;
    
    for (const auto& contrib : contrib_txs) {
        // Note: In full implementation, we would verify contributions here
        // For now, we assume they are pre-verified
        if (contrib.bonus_level > maxBonusLevel) {
            maxBonusLevel = contrib.bonus_level;
            maxContribType = contrib.contrib_type;
        }
    }
    
    // Apply bonus multiplier
    if (maxBonusLevel > BONUS_NONE) {
        double multiplier = GetBonusMultiplier(maxBonusLevel, maxContribType);
        return static_cast<CAmount>(baseReward * multiplier);
    }
    
    return baseReward;
}

/** Extract contribution transactions from a block
 * Contribution transactions are embedded in OP_RETURN outputs with format:
 * OP_RETURN <0xFC> <0x01> <serialized_contribution_data>
 * Where 0xFC 0x01 is the contribution marker (FC = Fleet Credits, 0x01 = contribution tx type)
 */
std::vector<CContributionTransaction> ExtractContributionsFromBlock(const CBlock& block)
{
    std::vector<CContributionTransaction> contributions;
    
    // Contribution marker: 0xFC 0x01 (Fleet Credits Contribution)
    const unsigned char CONTRIB_MARKER[] = {0xFC, 0x01};
    const size_t MARKER_SIZE = 2;
    
    // Scan all transactions in the block (skip coinbase)
    for (size_t i = 1; i < block.vtx.size(); i++) {
        const CTransaction& tx = *block.vtx[i];
        
        // Check each output for OP_RETURN with contribution marker
        for (const auto& txout : tx.vout) {
            const CScript& script = txout.scriptPubKey;
            
            // Check if this is an OP_RETURN output
            if (script.size() > 0 && script[0] == OP_RETURN) {
                // OP_RETURN outputs have format: OP_RETURN <data>
                // Check if it starts with our contribution marker
                if (script.size() >= MARKER_SIZE + 1) {
                    // Skip OP_RETURN byte (index 0), check marker
                    bool matches_marker = true;
                    for (size_t j = 0; j < MARKER_SIZE; j++) {
                        if (script[j + 1] != CONTRIB_MARKER[j]) {
                            matches_marker = false;
                            break;
                        }
                    }
                    
                    if (matches_marker) {
                        // Extract contribution data (everything after marker)
                        std::vector<unsigned char> contrib_data(
                            script.begin() + 1 + MARKER_SIZE,
                            script.end()
                        );
                        
                        // Try to deserialize contribution transaction
                        try {
                            CContributionTransaction contrib_tx;
                            CDataStream ss(contrib_data, SER_NETWORK, PROTOCOL_VERSION);
                            ss >> contrib_tx;
                            
                            // Validate basic structure
                            if (ValidateContributionTransaction(contrib_tx)) {
                                // Set transaction ID from the actual transaction
                                contrib_tx.tx_id = tx.GetHash();
                                
                                // Check if this contribution requires MWEB
                                // If so, it should be in the MWEB extension block, not main chain
                                if (contrib_tx.RequiresMWEB()) {
                                    // MWEB-required contributions are extracted from MWEB extension blocks
                                    // via ExtractContributionsFromMWEB() during block validation.
                                    // Skip extraction from main chain to avoid duplicates.
                                    continue;
                                }
                                
                                contributions.push_back(contrib_tx);
                            }
                        } catch (const std::exception& e) {
                            // Invalid contribution data, skip
                            // Log error in production: LogPrintf("Failed to parse contribution: %s\n", e.what());
                        }
                    }
                }
            }
        }
    }
    
    return contributions;
}


