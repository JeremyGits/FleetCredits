// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/verification.h"

#include "hash.h"
#include "pubkey.h"
#include "uint256.h"
#include "util.h"

#include <algorithm>
#include <random>

// TODO: GitHub API integration for code contribution verification
// For now, we'll use placeholder logic

/** Verify a code contribution (GitHub commit) */
bool CContributionVerifier::VerifyCodeContribution(const CCodeContributionTransaction& code_tx)
{
    // Basic validation first
    if (!ValidateCodeContribution(code_tx)) {
        return false;
    }

    // TODO: Verify GitHub commit exists and is valid
    // This would involve:
    // 1. Extract commit hash from proof_data.evidence
    // 2. Call GitHub API to verify commit exists
    // 3. Verify commit belongs to the repo_url
    // 4. Verify commit hasn't been used before (replay prevention)
    // 5. Check that lines_changed matches actual diff
    
    // Placeholder: Assume valid if basic checks pass
    // In production, this would call external API
    return true;
}

/** Verify an AI validation contribution */
bool CContributionVerifier::VerifyAIValidation(const CAIValidationTransaction& ai_tx)
{
    // Basic validation first
    if (!ValidateAIValidation(ai_tx)) {
        return false;
    }

    // Verify accuracy score threshold
    if (ai_tx.accuracy_score < 0.8f) {
        return false;
    }

    // TODO: Verify validation report hash
    // This would involve:
    // 1. Verify validation_report_hash points to valid report
    // 2. Verify human_override_hash if human_override_verified is true
    // 3. Check for replay attacks using nonce
    
    // Placeholder: Assume valid if basic checks pass
    return true;
}

/** Verify data labeling contribution */
bool CContributionVerifier::VerifyDataLabeling(const CDataLabelingTransaction& label_tx)
{
    // Basic validation first
    if (!ValidateDataLabeling(label_tx)) {
        return false;
    }

    // Verify inter-annotator agreement threshold
    if (label_tx.inter_annotator_agreement < 0.7f) {
        return false;
    }

    // Verify minimum annotator count
    if (label_tx.annotator_count < 3) {
        return false;
    }

    // TODO: Verify labels_hash points to valid IPFS data
    // TODO: Verify inter-annotator agreement calculation
    
    // Placeholder: Assume valid if basic checks pass
    return true;
}

/** Verify ethical review contribution */
bool CContributionVerifier::VerifyEthicalReview(const CEthicalReviewTransaction& review_tx)
{
    // Basic validation first
    if (!ValidateEthicalReview(review_tx)) {
        return false;
    }

    // Verify rationale length
    if (review_tx.rationale_length < 100) {
        return false;
    }

    // Ethical reviews require oracle consensus
    // This should be checked separately via ProcessOracleVotes
    
    // Placeholder: Assume valid if basic checks pass
    return true;
}

/** Verify charitable act contribution (requires oracle consensus) */
bool CContributionVerifier::VerifyCharitableAct(const CContributionTransaction& contrib_tx,
                                                const std::vector<COracleVote>& votes)
{
    // Basic validation first
    if (!ValidateContributionTransaction(contrib_tx)) {
        return false;
    }

    // Count approve votes
    uint32_t approve_count = 0;
    uint32_t total_votes = 0;
    
    for (const auto& vote : votes) {
        if (vote.vote_choice == VOTE_APPROVE) {
            approve_count++;
        }
        if (vote.vote_choice != VOTE_ABSTAIN) {
            total_votes++;
        }
    }

    // Require 3 of 5 approval (60% threshold)
    if (total_votes < 3) {
        return false;
    }
    
    float approval_rate = static_cast<float>(approve_count) / total_votes;
    return approval_rate >= 0.6f;
}

/** Create verification record for a contribution */
CVerificationRecord CContributionVerifier::CreateVerificationRecord(const CContributionTransaction& contrib_tx)
{
    CVerificationRecord record;
    
    // Generate record ID from contribution tx ID
    record.record_id = contrib_tx.tx_id;
    record.contrib_tx_id = contrib_tx.tx_id;
    record.verification_timestamp = GetTime();
    record.verification_status = VERIFICATION_PENDING;
    record.required_votes = 3;  // 3 of 5 consensus
    
    // Determine if automated verification is possible
    bool can_auto_verify = false;
    
    switch (contrib_tx.contrib_type) {
        case CODE_CONTRIBUTION:
            // Can verify via GitHub API
            can_auto_verify = true;
            break;
        case AI_VALIDATION:
        case DATA_LABELING:
            // Can verify via automated checks
            can_auto_verify = true;
            break;
        case CHARITABLE_ACT:
        case ETHICAL_REVIEW:
            // Requires oracle consensus
            can_auto_verify = false;
            break;
        default:
            // Other types may need oracle verification
            can_auto_verify = false;
            break;
    }
    
    // If auto-verification is possible, mark as approved pending verification
    if (can_auto_verify) {
        record.approved = true;
        record.consensus_score = 1.0;
        record.verification_status = VERIFICATION_APPROVED;
    }
    
    return record;
}

/** Process oracle votes and update verification record */
bool CContributionVerifier::ProcessOracleVotes(CVerificationRecord& record,
                                               const std::vector<COracleVote>& votes,
                                               const std::vector<COracleNode>& oracles)
{
    // Count votes
    uint32_t approve_count = 0;
    uint32_t reject_count = 0;
    uint32_t total_votes = 0;
    
    for (const auto& vote : votes) {
        // Verify oracle is valid and active
        bool oracle_valid = false;
        for (const auto& oracle : oracles) {
            if (oracle.pubkey == vote.oracle_pubkey && oracle.is_active) {
                oracle_valid = true;
                break;
            }
        }
        
        if (!oracle_valid) {
            continue;  // Skip invalid oracle votes
        }
        
        if (vote.vote_choice == VOTE_APPROVE) {
            approve_count++;
            total_votes++;
        } else if (vote.vote_choice == VOTE_REJECT) {
            reject_count++;
            total_votes++;
        }
        // VOTE_ABSTAIN doesn't count toward total
    }
    
    // Update record
    record.oracle_vote_count = total_votes;
    
    // Calculate consensus score
    if (total_votes > 0) {
        record.consensus_score = static_cast<float>(approve_count) / total_votes;
    } else {
        record.consensus_score = 0.0;
    }
    
    // Check if consensus threshold is met (3 of 5 = 0.6)
    if (total_votes >= record.required_votes) {
        record.approved = (record.consensus_score >= 0.6);
        
        if (record.approved) {
            record.verification_status = VERIFICATION_APPROVED;
        } else {
            record.verification_status = VERIFICATION_REJECTED;
        }
        
        record.verification_timestamp = GetTime();
        return true;
    }
    
    // Not enough votes yet
    record.verification_status = VERIFICATION_PENDING;
    return false;
}

/** Select random oracles for verification (3 of 5) */
std::vector<COracleNode> CContributionVerifier::SelectRandomOracles(const std::vector<COracleNode>& all_oracles,
                                                                     uint32_t count)
{
    std::vector<COracleNode> selected;
    
    // Filter to active oracles with sufficient stake
    std::vector<COracleNode> eligible;
    for (const auto& oracle : all_oracles) {
        if (oracle.is_active && oracle.MeetsStakeRequirement()) {
            eligible.push_back(oracle);
        }
    }
    
    // Need at least 'count' eligible oracles
    if (eligible.size() < count) {
        return selected;  // Not enough oracles
    }
    
    // Select random subset
    // Use deterministic randomness based on block hash or contribution tx ID
    // For now, use simple shuffle
    std::vector<COracleNode> shuffled = eligible;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(shuffled.begin(), shuffled.end(), gen);
    
    // Take first 'count' oracles
    for (size_t i = 0; i < count && i < shuffled.size(); i++) {
        selected.push_back(shuffled[i]);
    }
    
    return selected;
}

