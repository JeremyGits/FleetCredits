// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FLEETCREDITS_PRIMITIVES_VERIFICATION_H
#define FLEETCREDITS_PRIMITIVES_VERIFICATION_H

#include "primitives/contribution.h"
#include "primitives/transaction.h"
#include "pubkey.h"
#include "serialize.h"
#include "uint256.h"
#include "amount.h"

#include <string>
#include <vector>

/** Oracle node for contribution verification */
class COracleNode {
public:
    CPubKey pubkey;              // Oracle's public key
    std::string node_address;    // Network address
    int64_t stake_fc;            // Staked FC (>= 500k required)
    int32_t reputation_score;    // Reputation score
    uint32_t total_votes_cast;
    uint32_t correct_votes;
    uint32_t incorrect_votes;
    bool is_active;
    uint64_t last_rotation_date;

    COracleNode() 
        : stake_fc(0)
        , reputation_score(0)
        , total_votes_cast(0)
        , correct_votes(0)
        , incorrect_votes(0)
        , is_active(false)
        , last_rotation_date(0)
    {}

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(pubkey);
        READWRITE(node_address);
        READWRITE(stake_fc);
        READWRITE(reputation_score);
        READWRITE(total_votes_cast);
        READWRITE(correct_votes);
        READWRITE(incorrect_votes);
        READWRITE(is_active);
        READWRITE(last_rotation_date);
    }

    /** Check if oracle meets minimum stake requirement */
    bool MeetsStakeRequirement() const {
        return stake_fc >= 500000 * COIN;
    }

    /** Calculate oracle accuracy rate */
    double GetAccuracyRate() const {
        if (total_votes_cast == 0) return 0.0;
        return static_cast<double>(correct_votes) / total_votes_cast;
    }
};

/** Oracle vote on a contribution */
enum OracleVoteChoice {
    VOTE_APPROVE = 0x01,
    VOTE_REJECT = 0x02,
    VOTE_ABSTAIN = 0x03
};

class COracleVote {
public:
    uint256 vote_id;
    CPubKey oracle_pubkey;
    uint256 verification_record_id;
    OracleVoteChoice vote_choice;
    uint64_t vote_timestamp;
    std::vector<unsigned char> vote_signature;
    std::string vote_reason;

    COracleVote()
        : vote_choice(VOTE_ABSTAIN)
        , vote_timestamp(0)
    {
        vote_id.SetNull();
        verification_record_id.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(vote_id);
        READWRITE(oracle_pubkey);
        READWRITE(verification_record_id);
        READWRITE(vote_choice);
        READWRITE(vote_timestamp);
        READWRITE(vote_signature);
        READWRITE(vote_reason);
    }
};

/** Verification status for contributions */
enum VerificationStatus {
    VERIFICATION_PENDING = 0x01,
    VERIFICATION_APPROVED = 0x02,
    VERIFICATION_REJECTED = 0x03,
    VERIFICATION_CHALLENGED = 0x04
};

/** Verification record for a contribution transaction */
class CVerificationRecord {
public:
    uint256 record_id;
    uint256 contrib_tx_id;
    uint32_t oracle_vote_count;
    uint32_t required_votes;     // Typically 3 of 5 (0.6 threshold)
    float consensus_score;
    bool approved;
    VerificationStatus verification_status;
    uint64_t verification_timestamp;
    std::vector<unsigned char> view_key;  // For MWEB audits

    CVerificationRecord()
        : oracle_vote_count(0)
        , required_votes(3)
        , consensus_score(0.0)
        , approved(false)
        , verification_status(VERIFICATION_PENDING)
        , verification_timestamp(0)
    {
        record_id.SetNull();
        contrib_tx_id.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(record_id);
        READWRITE(contrib_tx_id);
        READWRITE(oracle_vote_count);
        READWRITE(required_votes);
        READWRITE(consensus_score);
        READWRITE(approved);
        READWRITE(verification_status);
        READWRITE(verification_timestamp);
        READWRITE(view_key);
    }

    /** Check if consensus threshold is met (3 of 5 = 0.6) */
    bool ConsensusThresholdMet() const {
        if (oracle_vote_count < required_votes) return false;
        return consensus_score >= 0.6;
    }
};

/** Human validator pool */
class CValidatorPool {
public:
    uint256 pool_id;
    uint32_t min_reputation_threshold;  // >50 required
    uint32_t member_count;
    int64_t total_stake;
    float pool_consensus_score;
    bool is_active;

    CValidatorPool()
        : min_reputation_threshold(50)
        , member_count(0)
        , total_stake(0)
        , pool_consensus_score(0.0)
        , is_active(false)
    {
        pool_id.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(pool_id);
        READWRITE(min_reputation_threshold);
        READWRITE(member_count);
        READWRITE(total_stake);
        READWRITE(pool_consensus_score);
        READWRITE(is_active);
    }
};

/** Human validator */
class CHumanValidator {
public:
    uint256 validator_id;
    CPubKey address;
    int32_t reputation_score;  // >50 for pool entry
    uint64_t verified_contributions_count;
    int64_t stake_amount;
    bool is_pool_member;
    uint64_t joined_pool_date;

    CHumanValidator()
        : reputation_score(0)
        , verified_contributions_count(0)
        , stake_amount(0)
        , is_pool_member(false)
        , joined_pool_date(0)
    {
        validator_id.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(validator_id);
        READWRITE(address);
        READWRITE(reputation_score);
        READWRITE(verified_contributions_count);
        READWRITE(stake_amount);
        READWRITE(is_pool_member);
        READWRITE(joined_pool_date);
    }

    /** Check if validator can join a pool */
    bool CanJoinPool(uint32_t min_reputation) const {
        return reputation_score > static_cast<int32_t>(min_reputation);
    }
};

/** Verification functions */
class CContributionVerifier {
public:
    /** Verify a code contribution (GitHub commit) */
    static bool VerifyCodeContribution(const CCodeContributionTransaction& code_tx);
    
    /** Verify an AI validation contribution */
    static bool VerifyAIValidation(const CAIValidationTransaction& ai_tx);
    
    /** Verify data labeling contribution */
    static bool VerifyDataLabeling(const CDataLabelingTransaction& label_tx);
    
    /** Verify ethical review contribution */
    static bool VerifyEthicalReview(const CEthicalReviewTransaction& review_tx);
    
    /** Verify charitable act contribution (requires oracle consensus) */
    static bool VerifyCharitableAct(const CContributionTransaction& contrib_tx, 
                                    const std::vector<COracleVote>& votes);
    
    /** Create verification record for a contribution */
    static CVerificationRecord CreateVerificationRecord(const CContributionTransaction& contrib_tx);
    
    /** Process oracle votes and update verification record */
    static bool ProcessOracleVotes(CVerificationRecord& record, 
                                   const std::vector<COracleVote>& votes,
                                   const std::vector<COracleNode>& oracles);
    
    /** Select random oracles for verification (3 of 5) */
    static std::vector<COracleNode> SelectRandomOracles(const std::vector<COracleNode>& all_oracles, 
                                                         uint32_t count = 5);
};

#endif // FLEETCREDITS_PRIMITIVES_VERIFICATION_H

