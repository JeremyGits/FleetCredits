// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Fleet Credits Core developers
// Copyright (c) 2022-2024 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// 
// note from the developer:
// thanks bitcoin and dogecoin, I love you still and forever, thank you Satoshi (and Shibetoshi) for everything
// what you have done for the world is amazing, you are a true legend.
// you are the reason why I am here today, I will always be grateful to you for your contributions to the world.
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"
#include "pow.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1386325540, nBits=0x1e0ffff0, nNonce=99943, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Nintondo";
    const CScript genesisOutputScript = CScript() << ParseHex("040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
public:
    CMainParams() {
        strNetworkID = "main";

        // Blocks 0 - 144999 are conventional difficulty calculation
        consensus.nSubsidyHalvingInterval = 100000;
        consensus.nMajorityEnforceBlockUpgrade = 1500;
        consensus.nMajorityRejectBlockOutdated = 1900;
        consensus.nMajorityWindow = 2000;
        // Fleet Credits - Fresh blockchain, no historical BIP heights
        // BIP34, BIP65, BIP66 will be enforced from block 1 onwards for new chain
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256(); // Will be set when block 1 is mined
        consensus.BIP65Height = 1;
        consensus.BIP66Height = 1;
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20;
        consensus.nPowTargetTimespan = 4 * 60 * 60; // pre-digishield: 4 hours
        consensus.nPowTargetSpacing = 60; // 1 minute
        consensus.fDigishieldDifficultyCalculation = false;
        consensus.nCoinbaseMaturity = 30;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowAllowDigishieldMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 9576; // 95% of 10,080
        consensus.nMinerConfirmationWindow = 10080; // 60 * 24 * 7 = 10,080 blocks, or one week
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        // XXX: BIP heights and hashes all need to be updated to Fleet Credits values
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1462060800; // May 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1479168000; // November 15th, 2016.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 0; // Disabled

        // Fleet Credits - Fresh blockchain, no previous work
        consensus.nMinimumChainWork = uint256(); // Genesis block only

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256(); // Genesis block only

        // AuxPoW parameters
        consensus.nAuxpowChainId = 0x0062; // 98 - Josh Wise!
        consensus.fStrictChainId = true;
        consensus.fAllowLegacyBlocks = true;
        consensus.nHeightEffective = 0;

        // Blocks 145000 - 371336 are Digishield without AuxPoW
        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 145000;
        digishieldConsensus.fSimplifiedRewards = true;
        digishieldConsensus.fDigishieldDifficultyCalculation = true;
        digishieldConsensus.nPowTargetTimespan = 60; // post-digishield: 1 minute
        digishieldConsensus.nCoinbaseMaturity = 240;

        // Blocks 371337+ are AuxPoW
        auxpowConsensus = digishieldConsensus;
        auxpowConsensus.nHeightEffective = 371337;
        auxpowConsensus.fAllowLegacyBlocks = false;

        // Assemble the binary search tree of consensus parameters
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         * 
         * Fleet Credits uses unique magic bytes to ensure network isolation from other blockchains.
         * FC = Fleet Credits (0xFC = 252, 0x43 = 'C', 0x52 = 'R', 0x45 = 'E')
         */
        pchMessageStart[0] = 0xFC;  // Fleet Credits unique identifier
        pchMessageStart[1] = 0x43;   // 'C' for Credits
        pchMessageStart[2] = 0x52;   // 'R' for Credits
        pchMessageStart[3] = 0x45;   // 'E' for Credits
        nDefaultPort = 42069;
        nPruneAfterHeight = 100000;

        // Fleet Credits Genesis Block - Fresh start with current timestamp
        // Timestamp: November 4, 2025 00:00:00 UTC (1730688000) - Current date for launch
        // This creates a NEW blockchain starting from scratch
        genesis = CreateGenesisBlock(1730688000, 0, 0x1e0ffff0, 1, 10000 * COIN);

        // Mine the genesis block to ensure it has valid PoW
        // CRITICAL: Genesis block MUST have valid PoW for blockchain integrity
        const Consensus::Params& mainConsensusParams = consensus;
        uint32_t maxNonceAttempts = 0xFFFFFFFF; // Max uint32_t
        uint32_t nonceAttempts = 0;
        
        while (!CheckProofOfWork(genesis.GetPoWHash(), genesis.nBits, mainConsensusParams)) {
            ++genesis.nNonce;
            ++nonceAttempts;
            
            // Prevent infinite loop - if we can't find a solution, try different timestamp
            if (genesis.nNonce == 0) {
                genesis.nTime++;
                genesis.nNonce = 0;
                nonceAttempts = 0;
                
                // Safety: if we've tried too many timestamps, something is wrong
                if (genesis.nTime > 1730688100) { // 100 seconds later
                    assert(false && "CRITICAL: Failed to mine mainnet genesis block with valid PoW");
                }
            }
            
            // Safety check
            if (nonceAttempts >= maxNonceAttempts) {
                assert(false && "CRITICAL: Exceeded max nonce attempts mining mainnet genesis block");
            }
        }
        
        // Verify the mined genesis block has valid PoW (double-check)
        assert(CheckProofOfWork(genesis.GetPoWHash(), genesis.nBits, mainConsensusParams) && 
               "CRITICAL: Mined mainnet genesis block failed PoW validation");

        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;

        // Fleet Credits DNS seeds - TODO: Add Fleet Credits seed nodes when mainnet launches
        // Note: For now, we have NO seed nodes - this is a fresh network starting from scratch
        // Users must manually connect nodes using -addnode or -connect flags
        // vSeeds.push_back(CDNSSeedData("fleetcredits.org", "seed.fleetcredits.org", true));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,30);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,22);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,158);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0xfa)(0xca)(0xfd).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0xfa)(0xc3)(0x98).convert_to_container<std::vector<unsigned char> >();

        // Fleet Credits: Add public server as default seed node
        // Public server: 96.126.123.195 (IPv6: ::ffff:607e:7bc3)
        vFixedSeeds.clear();
        vFixedSeeds.push_back(SeedSpec6{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x60,0x7e,0x7b,0xc3}, 42069});

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (      0, genesis.GetHash())  // Fleet Credits genesis - new blockchain starting from scratch
            // No additional checkpoints - this is a fresh blockchain
        };

        chainTxData = ChainTxData{
            // Fleet Credits - Fresh blockchain starting from genesis
            // Timestamp: November 4, 2025 00:00:00 UTC (1730688000)
            1730688000, // * UNIX timestamp of genesis block
            0,          // * total number of transactions (new blockchain)
            0.0         // * estimated number of transactions per second (will be calculated as network grows)
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
    Consensus::Params minDifficultyConsensus;
public:
    CTestNetParams() {
        strNetworkID = "test";

        // Blocks 0 - 144999 are pre-Digishield
        consensus.nHeightEffective = 0;
        consensus.nPowTargetTimespan = 4 * 60 * 60; // pre-digishield: 4 hours
        consensus.fDigishieldDifficultyCalculation = false;
        consensus.nCoinbaseMaturity = 30;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowAllowDigishieldMinDifficultyBlocks = false;
        consensus.nSubsidyHalvingInterval = 100000;
        consensus.nMajorityEnforceBlockUpgrade = 501;
        consensus.nMajorityRejectBlockOutdated = 750;
        consensus.nMajorityWindow = 1000;
        // Fleet Credits - Fresh blockchain, no historical BIP heights
        // BIP34, BIP65, BIP66 will be enforced from block 1 onwards for new chain
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256(); // Will be set when block 1 is mined
        consensus.BIP65Height = 1;
        consensus.BIP66Height = 1;
        consensus.powLimit = uint256S("0x00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 20;
        consensus.nPowTargetTimespan = 4 * 60 * 60; // pre-digishield: 4 hours
        consensus.nPowTargetSpacing = 60; // 1 minute
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 2880; // 2 days (note this is significantly lower than Fleet Credits standard)
        consensus.nMinerConfirmationWindow = 10080; // 60 * 24 * 7 = 10,080 blocks, or one week
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        // XXX: BIP heights and hashes all need to be updated to Fleet Credits values
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1456790400; // March 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1462060800; // May 1st 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 0; // Disabled

        // Fleet Credits - Fresh blockchain, no previous work
        consensus.nMinimumChainWork = uint256(); // Genesis block only

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256(); // Genesis block only

        // AuxPoW parameters
        consensus.nAuxpowChainId = 0x0062; // 98 - Josh Wise!
        consensus.fStrictChainId = false;
        consensus.nHeightEffective = 0;
        consensus.fAllowLegacyBlocks = true;

        // Blocks 145000 - 157499 are Digishield without minimum difficulty on all blocks
        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 145000;
        digishieldConsensus.nPowTargetTimespan = 60; // post-digishield: 1 minute
        digishieldConsensus.fDigishieldDifficultyCalculation = true;
        digishieldConsensus.fSimplifiedRewards = true;
        digishieldConsensus.fPowAllowMinDifficultyBlocks = false;
        digishieldConsensus.nCoinbaseMaturity = 240;

        // Blocks 157500 - 158099 are Digishield with minimum difficulty on all blocks
        minDifficultyConsensus = digishieldConsensus;
        minDifficultyConsensus.nHeightEffective = 157500;
        minDifficultyConsensus.fPowAllowDigishieldMinDifficultyBlocks = true;
        minDifficultyConsensus.fPowAllowMinDifficultyBlocks = true;

        // Enable AuxPoW at 158100
        auxpowConsensus = minDifficultyConsensus;
        auxpowConsensus.nHeightEffective = 158100;
        auxpowConsensus.fPowAllowDigishieldMinDifficultyBlocks = true;
        auxpowConsensus.fAllowLegacyBlocks = false;

        // Assemble the binary search tree of parameters
        pConsensusRoot = &digishieldConsensus;
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &minDifficultyConsensus;
        minDifficultyConsensus.pRight = &auxpowConsensus;

        pchMessageStart[0] = 0xFC;  // Fleet Credits Testnet unique identifier
        pchMessageStart[1] = 0x54;   // 'T' for Testnet
        pchMessageStart[2] = 0x45;   // 'E' for Testnet
        pchMessageStart[3] = 0x53;   // 'S' for Testnet
        nDefaultPort = 44556;
        nPruneAfterHeight = 1000;

        // Fleet Credits Testnet Genesis Block - Fresh start
        // Timestamp: November 4, 2025 00:00:00 UTC (1730688000) - Current date for launch
        genesis = CreateGenesisBlock(1730688000, 0, 0x1e0ffff0, 1, 10000 * COIN);
        
        // Mine the genesis block to ensure it has valid PoW
        // CRITICAL: Genesis block MUST have valid PoW for blockchain integrity
        const Consensus::Params& testConsensusParams = consensus;
        uint32_t maxNonceAttempts = 0xFFFFFFFF; // Max uint32_t
        uint32_t nonceAttempts = 0;
        
        while (!CheckProofOfWork(genesis.GetPoWHash(), genesis.nBits, testConsensusParams)) {
            ++genesis.nNonce;
            ++nonceAttempts;
            
            // Prevent infinite loop - if we can't find a solution, try different timestamp
            if (genesis.nNonce == 0) {
                genesis.nTime++;
                genesis.nNonce = 0;
                nonceAttempts = 0;
                
                // Safety: if we've tried too many timestamps, something is wrong
                if (genesis.nTime > 1730688100) { // 100 seconds later
                    assert(false && "CRITICAL: Failed to mine testnet genesis block with valid PoW");
                }
            }
            
            // Safety check
            if (nonceAttempts >= maxNonceAttempts) {
                assert(false && "CRITICAL: Exceeded max nonce attempts mining testnet genesis block");
            }
        }
        
        // Verify the mined genesis block has valid PoW (double-check)
        assert(CheckProofOfWork(genesis.GetPoWHash(), genesis.nBits, testConsensusParams) && 
               "CRITICAL: Mined testnet genesis block failed PoW validation");
        
        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        minDifficultyConsensus.hashGenesisBlock = consensus.hashGenesisBlock;

        vSeeds.clear();
        // Fleet Credits testnet DNS seeds - TODO: Add Fleet Credits testnet seed nodes
        // vSeeds.push_back(CDNSSeedData("testnet.fleetcredits.org", "testseed.fleetcredits.org"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,113); // 0x71
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196); // 0xc4
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,241); // 0xf1
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xcf).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        // Fleet Credits Testnet: Add public server as default seed node
        // Public server: 96.126.123.195 (IPv6: ::ffff:607e:7bc3)
        vFixedSeeds.clear();
        vFixedSeeds.push_back(SeedSpec6{{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff,0xff,0x60,0x7e,0x7b,0xc3}, 44556});

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, genesis.GetHash())  // Fleet Credits testnet genesis - new blockchain starting from scratch
            // No additional checkpoints - this is a fresh blockchain
        };

        chainTxData = ChainTxData{
            // Fleet Credits Testnet - Fresh blockchain starting from genesis
            // Timestamp: November 4, 2025 00:00:00 UTC (1730688000)
            1730688000, // * UNIX timestamp of genesis block
            0,          // * total number of transactions (new blockchain)
            0.0         // * estimated number of transactions per second (will be calculated as network grows)
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
private:
    Consensus::Params digishieldConsensus;
    Consensus::Params auxpowConsensus;
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint256(0) >> 1;
        consensus.nPowTargetTimespan = 4 * 60 * 60; // pre-digishield: 4 hours
        consensus.nPowTargetSpacing = 1; // regtest: 1 second blocks
        consensus.fDigishieldDifficultyCalculation = false;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 540; // 75% for testchains
        consensus.nMinerConfirmationWindow = 720; // Faster than normal for regtest (2,520 instead of 10,080)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        // AuxPow parameters
        consensus.nAuxpowChainId = 0x0062; // 98 - Josh Wise!
        consensus.fStrictChainId = true;
        consensus.fAllowLegacyBlocks = true;

        // Fleet Credits parameters
        consensus.fSimplifiedRewards = true;
        consensus.nCoinbaseMaturity = 60; // For easier testability in RPC tests

        digishieldConsensus = consensus;
        digishieldConsensus.nHeightEffective = 10;
        digishieldConsensus.nPowTargetTimespan = 1; // regtest: also retarget every second in digishield mode, for conformity
        digishieldConsensus.fDigishieldDifficultyCalculation = true;

        auxpowConsensus = digishieldConsensus;
        auxpowConsensus.fAllowLegacyBlocks = false;
        auxpowConsensus.nHeightEffective = 20;

        // Assemble the binary search tree of parameters
        digishieldConsensus.pLeft = &consensus;
        digishieldConsensus.pRight = &auxpowConsensus;
        pConsensusRoot = &digishieldConsensus;

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        // Fleet Credits Regtest Genesis Block - Fresh start
        // Timestamp: November 4, 2025 00:00:00 UTC (1730688000) - Current date for launch
        // Mine genesis block with valid PoW - CRITICAL: Must have valid PoW for blockchain integrity
        genesis = CreateGenesisBlock(1730688000, 0, 0x207fffff, 1, 10000 * COIN);
        
        // Mine the genesis block to ensure it has valid PoW
        // For regtest with difficulty 0x207fffff, this should be very fast
        // This is ESSENTIAL - genesis block MUST have valid PoW, no exceptions
        const Consensus::Params& consensusParams = consensus;
        uint32_t maxNonceAttempts = 0xFFFFFFFF; // Max uint32_t
        uint32_t nonceAttempts = 0;
        
        while (!CheckProofOfWork(genesis.GetPoWHash(), genesis.nBits, consensusParams)) {
            ++genesis.nNonce;
            ++nonceAttempts;
            
            // Prevent infinite loop - if we can't find a solution with this difficulty, something is wrong
            if (genesis.nNonce == 0) {
                // Wrapped around - this shouldn't happen with such low difficulty (0x207fffff)
                // But let's be safe and use a different timestamp
                genesis.nTime++;
                genesis.nNonce = 0;
                nonceAttempts = 0;
                
                // Extra safety: if we've tried multiple timestamps, something is seriously wrong
                if (genesis.nTime > 1704067300) { // 100 seconds later
                    assert(false && "CRITICAL: Failed to mine genesis block with valid PoW - difficulty too high or bug in CheckProofOfWork");
                }
            }
            
            // Safety check - should never hit this with regtest difficulty
            if (nonceAttempts >= maxNonceAttempts) {
                assert(false && "CRITICAL: Exceeded max nonce attempts mining genesis block");
            }
        }
        
        // Verify the mined genesis block has valid PoW (double-check)
        assert(CheckProofOfWork(genesis.GetPoWHash(), genesis.nBits, consensusParams) && 
               "CRITICAL: Mined genesis block failed PoW validation - blockchain integrity compromised");
        
        consensus.hashGenesisBlock = genesis.GetHash();
        digishieldConsensus.hashGenesisBlock = consensus.hashGenesisBlock;
        auxpowConsensus.hashGenesisBlock = consensus.hashGenesisBlock;

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, consensus.hashGenesisBlock)  // Use actual Fleet Credits regtest genesis hash
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);  // 0x6f
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);  // 0xc4
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);  // 0xef
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

const Consensus::Params *Consensus::Params::GetConsensus(uint32_t nTargetHeight) const {
    if (nTargetHeight < this -> nHeightEffective && this -> pLeft != NULL) {
        return this -> pLeft -> GetConsensus(nTargetHeight);
    } else if (nTargetHeight > this -> nHeightEffective && this -> pRight != NULL) {
        const Consensus::Params *pCandidate = this -> pRight -> GetConsensus(nTargetHeight);
        if (pCandidate->nHeightEffective <= nTargetHeight) {
            return pCandidate;
        }
    }

    // No better match below the target height
    return this;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
