// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FLEETCREDITS_PRIMITIVES_GOVERNANCE_H
#define FLEETCREDITS_PRIMITIVES_GOVERNANCE_H

#include "primitives/transaction.h"
#include "pubkey.h"
#include "serialize.h"
#include "uint256.h"

#include <string>
#include <vector>

/** Governance proposal types */
enum ProposalType {
    PROPOSAL_PARAMETER_CHANGE = 0x01,  // Change protocol parameters
    PROPOSAL_RESERVE_SPENDING = 0x02,  // Spend from Community Reserve
    PROPOSAL_ORACLE_ELECTION = 0x03,    // Elect/remove oracles
    PROPOSAL_FEATURE_ADD = 0x04,        // Add new feature
    PROPOSAL_FEATURE_REMOVE = 0x05,    // Remove feature
    PROPOSAL_EMERGENCY = 0x06           // Emergency proposal (requires higher threshold)
};

/** Proposal status */
enum ProposalStatus {
    PROPOSAL_PENDING = 0x01,      // Submitted, waiting for discussion period
    PROPOSAL_ACTIVE = 0x02,        // Active voting period
    PROPOSAL_APPROVED = 0x03,      // Approved by voters
    PROPOSAL_REJECTED = 0x04,      // Rejected by voters
    PROPOSAL_EXECUTED = 0x05,      // Executed (for reserve spending, etc.)
    PROPOSAL_EXPIRED = 0x06        // Expired without reaching quorum
};

/** Vote choice */
enum VoteChoice {
    VOTE_YES = 0x01,
    VOTE_NO = 0x02,
    VOTE_ABSTAIN = 0x03
};

/** Governance proposal transaction */
class CGovernanceProposal {
public:
    uint256 proposal_id;           // Hash of proposal data
    CPubKey proposer;              // Public key of proposer
    ProposalType proposal_type;    // Type of proposal
    std::string title;             // Proposal title
    std::string description;      // Full proposal description
    std::vector<unsigned char> proposal_data;  // Type-specific data (JSON or binary)
    uint64_t submission_time;      // Unix timestamp
    uint64_t discussion_end_time;  // End of discussion period
    uint64_t voting_start_time;    // Start of voting period
    uint64_t voting_end_time;      // End of voting period
    uint64_t execution_time;       // When proposal should be executed (if approved)
    ProposalStatus status;          // Current status
    CAmount required_stake;        // Minimum stake required to submit (anti-spam)
    uint32_t quorum_percentage;    // Required participation percentage (e.g., 10%)
    uint32_t approval_percentage;  // Required approval percentage (e.g., 51%)
    std::vector<unsigned char> signature;  // Proposer's signature
    
    CGovernanceProposal()
        : proposal_type(PROPOSAL_PARAMETER_CHANGE)
        , submission_time(0)
        , discussion_end_time(0)
        , voting_start_time(0)
        , voting_end_time(0)
        , execution_time(0)
        , status(PROPOSAL_PENDING)
        , required_stake(0)
        , quorum_percentage(10)
        , approval_percentage(51)
    {
        proposal_id.SetNull();
    }
    
    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(proposal_id);
        READWRITE(proposer);
        uint8_t proposal_type_byte = static_cast<uint8_t>(proposal_type);
        READWRITE(proposal_type_byte);
        if (ser_action.ForRead()) {
            proposal_type = static_cast<ProposalType>(proposal_type_byte);
        }
        READWRITE(title);
        READWRITE(description);
        READWRITE(proposal_data);
        READWRITE(submission_time);
        READWRITE(discussion_end_time);
        READWRITE(voting_start_time);
        READWRITE(voting_end_time);
        READWRITE(execution_time);
        uint8_t status_byte = static_cast<uint8_t>(status);
        READWRITE(status_byte);
        if (ser_action.ForRead()) {
            status = static_cast<ProposalStatus>(status_byte);
        }
        READWRITE(required_stake);
        READWRITE(quorum_percentage);
        READWRITE(approval_percentage);
        READWRITE(signature);
    }
    
    /** Calculate proposal ID from proposal data */
    uint256 CalculateProposalId() const;
};

/** Governance vote transaction */
class CGovernanceVote {
public:
    uint256 vote_id;               // Hash of vote data
    uint256 proposal_id;           // Proposal being voted on
    CPubKey voter;                 // Public key of voter
    VoteChoice choice;             // YES, NO, or ABSTAIN
    CAmount voting_power;          // Voter's stake/balance at time of vote
    uint64_t vote_time;            // Unix timestamp
    std::vector<unsigned char> signature;  // Voter's signature
    
    CGovernanceVote()
        : choice(VOTE_ABSTAIN)
        , voting_power(0)
        , vote_time(0)
    {
        vote_id.SetNull();
        proposal_id.SetNull();
    }
    
    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(vote_id);
        READWRITE(proposal_id);
        READWRITE(voter);
        uint8_t choice_byte = static_cast<uint8_t>(choice);
        READWRITE(choice_byte);
        if (ser_action.ForRead()) {
            choice = static_cast<VoteChoice>(choice_byte);
        }
        READWRITE(voting_power);
        READWRITE(vote_time);
        READWRITE(signature);
    }
    
    /** Calculate vote ID from vote data */
    uint256 CalculateVoteId() const;
};

/** Proposal execution result */
class CProposalExecution {
public:
    uint256 proposal_id;
    uint256 execution_tx_id;        // Transaction that executed the proposal
    uint64_t execution_time;
    bool success;                  // Whether execution succeeded
    std::string error_message;     // Error if execution failed
    
    CProposalExecution()
        : execution_time(0)
        , success(false)
    {
        proposal_id.SetNull();
        execution_tx_id.SetNull();
    }
    
    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(proposal_id);
        READWRITE(execution_tx_id);
        READWRITE(execution_time);
        READWRITE(success);
        READWRITE(error_message);
    }
};

// Function declarations
bool ValidateGovernanceProposal(const CGovernanceProposal& proposal);
bool ValidateGovernanceVote(const CGovernanceVote& vote);
uint256 CalculateProposalId(const CGovernanceProposal& proposal);
uint256 CalculateVoteId(const CGovernanceVote& vote);

#endif // FLEETCREDITS_PRIMITIVES_GOVERNANCE_H

