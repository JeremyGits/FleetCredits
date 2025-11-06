// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/governance.h"

#include "hash.h"
#include "pubkey.h"
#include "uint256.h"

#include <algorithm>

/** Calculate proposal ID from proposal data */
uint256 CGovernanceProposal::CalculateProposalId() const {
    CHashWriter ss(SER_GETHASH, 0);
    ss << proposer;
    ss << static_cast<uint8_t>(proposal_type);
    ss << title;
    ss << description;
    ss << proposal_data;
    ss << submission_time;
    return ss.GetHash();
}

/** Calculate vote ID from vote data */
uint256 CGovernanceVote::CalculateVoteId() const {
    CHashWriter ss(SER_GETHASH, 0);
    ss << proposal_id;
    ss << voter;
    ss << static_cast<uint8_t>(choice);
    ss << vote_time;
    return ss.GetHash();
}

/** Validate governance proposal */
bool ValidateGovernanceProposal(const CGovernanceProposal& proposal) {
    // Check proposer public key is valid
    if (!proposal.proposer.IsValid()) {
        return false;
    }
    
    // Check proposal ID matches calculated ID
    if (proposal.proposal_id != proposal.CalculateProposalId()) {
        return false;
    }
    
    // Check title is not empty
    if (proposal.title.empty()) {
        return false;
    }
    
    // Check description is not empty
    if (proposal.description.empty()) {
        return false;
    }
    
    // Check time ordering
    if (proposal.discussion_end_time <= proposal.submission_time) {
        return false;
    }
    if (proposal.voting_start_time <= proposal.discussion_end_time) {
        return false;
    }
    if (proposal.voting_end_time <= proposal.voting_start_time) {
        return false;
    }
    
    // Check quorum and approval percentages are reasonable
    if (proposal.quorum_percentage == 0 || proposal.quorum_percentage > 100) {
        return false;
    }
    if (proposal.approval_percentage == 0 || proposal.approval_percentage > 100) {
        return false;
    }
    
    // Check signature is not empty
    if (proposal.signature.empty()) {
        return false;
    }
    
    return true;
}

/** Validate governance vote */
bool ValidateGovernanceVote(const CGovernanceVote& vote) {
    // Check voter public key is valid
    if (!vote.voter.IsValid()) {
        return false;
    }
    
    // Check proposal ID is not null
    if (vote.proposal_id.IsNull()) {
        return false;
    }
    
    // Check vote ID matches calculated ID
    if (vote.vote_id != vote.CalculateVoteId()) {
        return false;
    }
    
    // Check voting power is positive
    if (vote.voting_power <= 0) {
        return false;
    }
    
    // Check signature is not empty
    if (vote.signature.empty()) {
        return false;
    }
    
    return true;
}

/** Calculate proposal ID (standalone function) */
uint256 CalculateProposalId(const CGovernanceProposal& proposal) {
    return proposal.CalculateProposalId();
}

/** Calculate vote ID (standalone function) */
uint256 CalculateVoteId(const CGovernanceVote& vote) {
    return vote.CalculateVoteId();
}

