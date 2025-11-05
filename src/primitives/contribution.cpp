// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/contribution.h"

#include "hash.h"
#include "pubkey.h"
#include "uint256.h"
#include "amount.h"

#include <algorithm>

/** Calculate bonus multiplier based on bonus level and contribution type */
double GetBonusMultiplier(uint32_t bonus_level, ContributionType contrib_type) {
    double multiplier = 1.0;
    
    switch (bonus_level) {
        case BONUS_LOW:
            multiplier = 1.05;
            break;
        case BONUS_MEDIUM:
            multiplier = 1.10;
            break;
        case BONUS_HIGH:
            multiplier = 1.15;
            break;
        case BONUS_CRITICAL:
            // AI validation gets 1.25x, others get 1.20x
            multiplier = (contrib_type == AI_VALIDATION) ? 1.25 : 1.20;
            break;
        default:
            multiplier = 1.0;
    }
    
    return multiplier;
}

/** Calculate individual contributor reward
 * Returns the reward amount for a single contributor based on their contribution
 * The reward is a portion of the bonus amount above the base reward
 */
CAmount CalculateContributorReward(const CContributionTransaction& contrib, CAmount baseReward, CAmount totalBonusReward) {
    if (contrib.bonus_level == BONUS_NONE) {
        return 0;  // No reward for contributions without bonus
    }
    
    // Calculate this contributor's share of the bonus
    // For now, distribute bonus equally among all contributors
    // Future: Could weight by contribution type, bonus level, etc.
    double multiplier = GetBonusMultiplier(contrib.bonus_level, contrib.contrib_type);
    double baseMultiplier = 1.0;  // Base multiplier (no bonus)
    
    // Calculate this contributor's portion of the bonus
    // Use a simple formula: contributor gets portion based on their bonus multiplier
    // Total bonus = totalBonusReward - baseReward
    CAmount bonusAmount = totalBonusReward - baseReward;
    
    if (bonusAmount <= 0) {
        return 0;
    }
    
    // Distribute bonus proportionally based on individual multiplier
    // Weight = (multiplier - 1.0) / (sum of all (multiplier - 1.0))
    // For simplicity, each contributor gets a share based on their multiplier
    double contributorBonusShare = (multiplier - baseMultiplier);
    
    // Normalize: if highest bonus is 1.25x, share = (1.25-1.0) = 0.25
    // This gives higher bonus contributors more of the bonus pool
    CAmount contributorReward = static_cast<CAmount>(baseReward * contributorBonusShare);
    
    return contributorReward;
}

/** Get bonus level name as string */
std::string GetBonusLevelName(uint32_t bonus_level) {
    switch (bonus_level) {
        case BONUS_LOW:
            return "LOW";
        case BONUS_MEDIUM:
            return "MEDIUM";
        case BONUS_HIGH:
            return "HIGH";
        case BONUS_CRITICAL:
            return "CRITICAL";
        default:
            return "NONE";
    }
}

/** Get contribution type name as string */
std::string GetContributionTypeName(ContributionType type) {
    switch (type) {
        case CODE_CONTRIBUTION:
            return "CODE_CONTRIBUTION";
        case CHARITABLE_ACT:
            return "CHARITABLE_ACT";
        case CREATIVE_WORK:
            return "CREATIVE_WORK";
        case EDUCATIONAL_CONTENT:
            return "EDUCATIONAL_CONTENT";
        case CODE_MENTORSHIP:
            return "CODE_MENTORSHIP";
        case AI_VALIDATION:
            return "AI_VALIDATION";
        case DATA_LABELING:
            return "DATA_LABELING";
        case ETHICAL_REVIEW:
            return "ETHICAL_REVIEW";
        default:
            return "UNKNOWN";
    }
}

/** Calculate proof data hash */
uint256 CalculateProofDataHash(const CProofData& proof_data) {
    CHashWriter ss(SER_GETHASH, 0);
    uint8_t contrib_type_byte = static_cast<uint8_t>(proof_data.contrib_type);
    ss << contrib_type_byte;
    ss << proof_data.evidence;
    ss << proof_data.nonce;
    ss << proof_data.timestamp;
    ss << proof_data.metadata;
    return ss.GetHash();
}

/** Validate basic contribution transaction requirements */
bool ValidateContributionTransaction(const CContributionTransaction& contrib_tx) {
    // Check timestamp is reasonable (not too far in future/past)
    // This would be checked against current time in actual validation
    
    // Check signature is not empty
    if (contrib_tx.signature.empty()) {
        return false;
    }
    
    // Check contributor public key is valid
    if (!contrib_tx.contributor.IsValid()) {
        return false;
    }
    
    // Check proof data hash matches
    uint256 calculated_hash = CalculateProofDataHash(contrib_tx.proof_data);
    if (calculated_hash != contrib_tx.proof_data.hash) {
        return false;
    }
    
    // Check MWEB requirement for sensitive types
    if (contrib_tx.RequiresMWEB() && !contrib_tx.requires_mweb) {
        return false;
    }
    
    return true;
}

/** Validate code contribution specific requirements */
bool ValidateCodeContribution(const CCodeContributionTransaction& code_tx) {
    // Must have >10 lines changed
    if (code_tx.lines_changed <= 10) {
        return false;
    }
    
    // Must have account age >30 days
    if (code_tx.account_age_days <= 30) {
        return false;
    }
    
    // Must pass linting
    if (!code_tx.passes_lint) {
        return false;
    }
    
    // Must have valid commit hash
    if (code_tx.commit_hash.IsNull()) {
        return false;
    }
    
    return ValidateContributionTransaction(code_tx);
}

/** Validate AI validation specific requirements */
bool ValidateAIValidation(const CAIValidationTransaction& ai_tx) {
    // Accuracy score must be >= 0.8
    if (ai_tx.accuracy_score < 0.8f) {
        return false;
    }
    
    // Must have validation report hash
    if (ai_tx.validation_report_hash.IsNull()) {
        return false;
    }
    
    return ValidateContributionTransaction(ai_tx);
}

/** Validate data labeling specific requirements */
bool ValidateDataLabeling(const CDataLabelingTransaction& label_tx) {
    // Must have >= 3 annotators
    if (label_tx.annotator_count < 3) {
        return false;
    }
    
    // Inter-annotator agreement must be >= 0.7
    if (label_tx.inter_annotator_agreement < 0.7f) {
        return false;
    }
    
    // Must have valid labels hash
    if (label_tx.labels_hash.IsNull()) {
        return false;
    }
    
    return ValidateContributionTransaction(label_tx);
}

/** Validate ethical review specific requirements */
bool ValidateEthicalReview(const CEthicalReviewTransaction& review_tx) {
    // Rationale must be >= 100 words
    if (review_tx.rationale_length < 100) {
        return false;
    }
    
    // Must have rationale hash
    if (review_tx.rationale_hash.IsNull()) {
        return false;
    }
    
    // MWEB is mandatory for ethical reviews
    if (!review_tx.requires_mweb) {
        return false;
    }
    
    return ValidateContributionTransaction(review_tx);
}

/** Validate mentorship specific requirements */
bool ValidateMentorship(const CMentorshipTransaction& mentor_tx) {
    // Must have valid mentor and mentee
    if (!mentor_tx.mentor.IsValid() || !mentor_tx.mentee.IsValid()) {
        return false;
    }
    
    // Must have mentor signature
    if (mentor_tx.mentor_signature.empty()) {
        return false;
    }
    
    // Must have valid approval status
    if (mentor_tx.approval_status != APPROVED && 
        mentor_tx.approval_status != REJECTED && 
        mentor_tx.approval_status != NEEDS_WORK) {
        return false;
    }
    
    return ValidateContributionTransaction(mentor_tx);
}

