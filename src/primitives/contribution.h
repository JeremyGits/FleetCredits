// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FLEETCREDITS_PRIMITIVES_CONTRIBUTION_H
#define FLEETCREDITS_PRIMITIVES_CONTRIBUTION_H

#include "primitives/transaction.h"
#include "pubkey.h"
#include "serialize.h"
#include "uint256.h"

#include <string>
#include <vector>

/** Contribution transaction types as defined in ERD */
enum ContributionType {
    CODE_CONTRIBUTION = 0x01,
    CHARITABLE_ACT = 0x02,
    CREATIVE_WORK = 0x03,
    EDUCATIONAL_CONTENT = 0x04,
    CODE_MENTORSHIP = 0x05,
    AI_VALIDATION = 0x06,
    DATA_LABELING = 0x07,
    ETHICAL_REVIEW = 0x08
};

/** Bonus levels for contribution rewards */
enum BonusLevel {
    BONUS_NONE = 0,
    BONUS_LOW = 1,      // 1.05x multiplier
    BONUS_MEDIUM = 2,   // 1.10x multiplier
    BONUS_HIGH = 3,     // 1.15x multiplier
    BONUS_CRITICAL = 4  // 1.20x multiplier (1.25x for AI_VALIDATION)
};

/** Approval status for mentorship transactions */
enum ApprovalStatus {
    APPROVED = 0x01,
    REJECTED = 0x02,
    NEEDS_WORK = 0x03
};

/** Proof data structure for contribution transactions */
class CProofData {
public:
    uint256 hash;
    ContributionType contrib_type;
    std::vector<unsigned char> evidence;  // IPFS hash, GitHub commit, etc.
    std::vector<unsigned char> nonce;      // Unique per AI validation
    uint64_t timestamp;
    std::vector<unsigned char> metadata;

    CProofData() : contrib_type(CODE_CONTRIBUTION), timestamp(0) {
        hash.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(hash);
        uint8_t contrib_type_byte = static_cast<uint8_t>(contrib_type);
        READWRITE(contrib_type_byte);
        if (ser_action.ForRead()) {
            contrib_type = static_cast<ContributionType>(contrib_type_byte);
        }
        READWRITE(evidence);
        READWRITE(nonce);
        READWRITE(timestamp);
        READWRITE(metadata);
    }
};

/** Base contribution transaction structure */
class CContributionTransaction {
public:
    uint256 tx_id;
    CPubKey contributor;
    ContributionType contrib_type;
    CProofData proof_data;
    uint64_t timestamp;
    uint32_t bonus_level;  // BONUS_LOW, BONUS_MEDIUM, BONUS_HIGH, BONUS_CRITICAL
    bool requires_mweb;    // Mandatory for ETHICAL_REVIEW
    std::vector<unsigned char> signature;

    CContributionTransaction() 
        : contrib_type(CODE_CONTRIBUTION)
        , timestamp(0)
        , bonus_level(BONUS_NONE)
        , requires_mweb(false)
    {
        tx_id.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(tx_id);
        READWRITE(contributor);
        uint8_t contrib_type_byte = static_cast<uint8_t>(contrib_type);
        READWRITE(contrib_type_byte);
        if (ser_action.ForRead()) {
            contrib_type = static_cast<ContributionType>(contrib_type_byte);
        }
        READWRITE(proof_data);
        READWRITE(timestamp);
        READWRITE(bonus_level);
        READWRITE(requires_mweb);
        READWRITE(signature);
    }

    /** Check if this contribution type requires MWEB */
    bool RequiresMWEB() const {
        return requires_mweb || contrib_type == ETHICAL_REVIEW;
    }
};

/** Mentorship transaction extension */
class CMentorshipTransaction : public CContributionTransaction {
public:
    CPubKey mentor;
    CPubKey mentee;
    std::vector<unsigned char> mentor_signature;
    std::vector<unsigned char> mentee_signature;
    ApprovalStatus approval_status;
    uint256 feedback_hash;
    uint256 improvement_commit_hash;

    CMentorshipTransaction() 
        : approval_status(NEEDS_WORK)
    {
        contrib_type = CODE_MENTORSHIP;
        feedback_hash.SetNull();
        improvement_commit_hash.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        // Serialize base class
        CContributionTransaction::SerializationOp(s, ser_action);
        
        // Serialize mentorship-specific fields
        READWRITE(mentor);
        READWRITE(mentee);
        READWRITE(mentor_signature);
        READWRITE(mentee_signature);
        uint8_t approval_status_byte = static_cast<uint8_t>(approval_status);
        READWRITE(approval_status_byte);
        if (ser_action.ForRead()) {
            approval_status = static_cast<ApprovalStatus>(approval_status_byte);
        }
        READWRITE(feedback_hash);
        READWRITE(improvement_commit_hash);
    }
};

/** AI Validation transaction extension */
class CAIValidationTransaction : public CContributionTransaction {
public:
    std::string ai_model_id;
    float accuracy_score;  // >= 0.8 required
    uint256 validation_report_hash;
    uint256 human_override_hash;
    bool human_override_verified;

    CAIValidationTransaction()
        : accuracy_score(0.0)
        , human_override_verified(false)
    {
        contrib_type = AI_VALIDATION;
        bonus_level = BONUS_CRITICAL;  // 1.25x multiplier for AI validation
        validation_report_hash.SetNull();
        human_override_hash.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        CContributionTransaction::SerializationOp(s, ser_action);
        READWRITE(ai_model_id);
        READWRITE(accuracy_score);
        READWRITE(validation_report_hash);
        READWRITE(human_override_hash);
        READWRITE(human_override_verified);
    }
};

/** Data Labeling transaction extension */
class CDataLabelingTransaction : public CContributionTransaction {
public:
    std::string dataset_id;
    uint32_t labels_count;
    uint256 labels_hash;  // IPFS hash of labeled data
    float inter_annotator_agreement;  // >= 0.7 required
    uint32_t annotator_count;  // >= 3 required

    CDataLabelingTransaction()
        : labels_count(0)
        , inter_annotator_agreement(0.0)
        , annotator_count(0)
    {
        contrib_type = DATA_LABELING;
        labels_hash.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        CContributionTransaction::SerializationOp(s, ser_action);
        READWRITE(dataset_id);
        READWRITE(labels_count);
        READWRITE(labels_hash);
        READWRITE(inter_annotator_agreement);
        READWRITE(annotator_count);
    }
};

/** Ethical Review transaction extension */
class CEthicalReviewTransaction : public CContributionTransaction {
public:
    uint256 rationale_hash;  // >= 100 words required
    uint64_t rationale_length;
    bool oracle_consensus_achieved;

    CEthicalReviewTransaction()
        : rationale_length(0)
        , oracle_consensus_achieved(false)
    {
        contrib_type = ETHICAL_REVIEW;
        requires_mweb = true;  // Mandatory MWEB for privacy
        rationale_hash.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        CContributionTransaction::SerializationOp(s, ser_action);
        READWRITE(rationale_hash);
        READWRITE(rationale_length);
        READWRITE(oracle_consensus_achieved);
    }
};

/** Code Contribution transaction extension */
class CCodeContributionTransaction : public CContributionTransaction {
public:
    std::string repo_url;
    uint256 commit_hash;
    uint32_t lines_changed;  // >10 required
    std::string language;
    bool passes_lint;
    uint32_t account_age_days;  // >30 required

    CCodeContributionTransaction()
        : lines_changed(0)
        , passes_lint(false)
        , account_age_days(0)
    {
        contrib_type = CODE_CONTRIBUTION;
        commit_hash.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        CContributionTransaction::SerializationOp(s, ser_action);
        READWRITE(repo_url);
        READWRITE(commit_hash);
        READWRITE(lines_changed);
        READWRITE(language);
        READWRITE(passes_lint);
        READWRITE(account_age_days);
    }
};

// Function declarations (defined after all classes)
double GetBonusMultiplier(uint32_t bonus_level, ContributionType contrib_type);
CAmount CalculateContributorReward(const CContributionTransaction& contrib, CAmount baseReward, CAmount totalBonusReward);
std::string GetBonusLevelName(uint32_t bonus_level);
std::string GetContributionTypeName(ContributionType type);
uint256 CalculateProofDataHash(const CProofData& proof_data);
bool ValidateContributionTransaction(const CContributionTransaction& contrib_tx);
bool ValidateCodeContribution(const CCodeContributionTransaction& code_tx);
bool ValidateAIValidation(const CAIValidationTransaction& ai_tx);
bool ValidateDataLabeling(const CDataLabelingTransaction& label_tx);
bool ValidateEthicalReview(const CEthicalReviewTransaction& review_tx);
bool ValidateMentorship(const CMentorshipTransaction& mentor_tx);

#endif // FLEETCREDITS_PRIMITIVES_CONTRIBUTION_H

