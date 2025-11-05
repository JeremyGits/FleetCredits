// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FLEETCREDITS_WALLET_WALLETEXTENSIONS_H
#define FLEETCREDITS_WALLET_WALLETEXTENSIONS_H

#include "primitives/contribution.h"
#include "primitives/mweb.h"
#include "amount.h"
#include "serialize.h"
#include "pubkey.h"
#include "uint256.h"

#include <vector>
#include <map>

/** Contribution status for wallet tracking */
enum ContributionStatus {
    CONTRIB_PENDING = 0,      // Awaiting oracle verification
    CONTRIB_APPROVED = 1,      // Approved by oracles
    CONTRIB_REJECTED = 2,      // Rejected by oracles
    CONTRIB_REWARDED = 3       // Reward has been paid
};

/** MWEB UTXO entry stored in wallet */
class CMWEBUTXOEntry {
public:
    CPedersenCommitment commitment;    // The commitment
    uint256 blinding_factor_hash;       // Hash of blinding factor (for verification)
    CAmount amount_hint;                // Optional: cached amount (for faster queries)
    uint64_t block_height;             // Block height when created
    bool is_spent;                     // Whether this UTXO is spent

    CMWEBUTXOEntry()
        : amount_hint(0)
        , block_height(0)
        , is_spent(false)
    {
        blinding_factor_hash.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(commitment);
        READWRITE(blinding_factor_hash);
        READWRITE(amount_hint);
        READWRITE(block_height);
        READWRITE(is_spent);
    }
};

/** Contribution wallet entry linking transactions to contributions */
class CContributionWalletEntry {
public:
    uint256 tx_hash;                   // Transaction hash
    CContributionTransaction contrib;  // Contribution transaction data
    ContributionStatus status;         // PENDING, APPROVED, REJECTED, REWARDED
    int64_t confirmation_count;        // Number of confirmations
    CAmount reward_amount;             // Reward received (if any)
    uint256 reward_tx_hash;            // Transaction that paid the reward
    uint64_t approval_block_height;    // Block height when approved (if applicable)

    CContributionWalletEntry()
        : status(CONTRIB_PENDING)
        , confirmation_count(0)
        , reward_amount(0)
        , approval_block_height(0)
    {
        tx_hash.SetNull();
        reward_tx_hash.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(tx_hash);
        READWRITE(contrib);
        // Serialize ContributionStatus as uint8_t
        uint8_t status_byte = static_cast<uint8_t>(status);
        READWRITE(status_byte);
        if (ser_action.ForRead()) {
            status = static_cast<ContributionStatus>(status_byte);
        }
        READWRITE(confirmation_count);
        READWRITE(reward_amount);
        READWRITE(reward_tx_hash);
        READWRITE(approval_block_height);
    }
};

/** Reward entry tracking contributor payouts */
class CRewardEntry {
public:
    uint256 reward_id;                 // Unique reward ID (hash of block + contributor)
    uint256 block_hash;                // Block that paid the reward
    uint64_t block_height;             // Block height
    uint256 contrib_tx_hash;           // Original contribution transaction
    CAmount reward_amount;             // Amount received
    ContributionType contrib_type;      // Type of contribution
    uint32_t bonus_level;              // Bonus level applied
    int64_t timestamp;                 // When reward was received

    CRewardEntry()
        : block_height(0)
        , reward_amount(0)
        , contrib_type(CODE_CONTRIBUTION)
        , bonus_level(BONUS_NONE)
        , timestamp(0)
    {
        reward_id.SetNull();
        block_hash.SetNull();
        contrib_tx_hash.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(reward_id);
        READWRITE(block_hash);
        READWRITE(block_height);
        READWRITE(contrib_tx_hash);
        READWRITE(reward_amount);
        // Serialize ContributionType as uint8_t
        uint8_t contrib_type_byte = static_cast<uint8_t>(contrib_type);
        READWRITE(contrib_type_byte);
        if (ser_action.ForRead()) {
            contrib_type = static_cast<ContributionType>(contrib_type_byte);
        }
        READWRITE(bonus_level);
        READWRITE(timestamp);
    }
};

/** Oracle staking entry */
class COracleStakeEntry {
public:
    CPubKey oracle_pubkey;             // Oracle's public key
    CAmount staked_amount;             // Amount staked in FC
    uint32_t reputation_score;         // Current reputation (0-1000)
    uint64_t staking_start_height;     // Block height when staking started
    uint64_t last_vote_height;         // Last block height voted on
    std::vector<uint256> active_votes; // Active verification votes
    bool is_active;                    // Whether oracle is currently active

    COracleStakeEntry()
        : staked_amount(0)
        , reputation_score(0)
        , staking_start_height(0)
        , last_vote_height(0)
        , is_active(false)
    {}

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(oracle_pubkey);
        READWRITE(staked_amount);
        READWRITE(reputation_score);
        READWRITE(staking_start_height);
        READWRITE(last_vote_height);
        READWRITE(active_votes);
        READWRITE(is_active);
    }
};

/** MWEB wallet transaction with decrypted data */
class CMWEBWalletTx {
public:
    uint256 tx_hash;                   // MWEB transaction hash
    CMWEBTransaction mweb_tx;           // The MWEB transaction
    std::map<uint256, CAmount> input_amounts;    // Decrypted input amounts
    std::map<uint256, CAmount> output_amounts;   // Decrypted output amounts
    CAmount fee;                       // Transaction fee
    int64_t timestamp;                 // Transaction timestamp
    uint64_t block_height;             // Block height (if confirmed)
    bool is_mine;                      // Whether this wallet owns any outputs

    CMWEBWalletTx()
        : fee(0)
        , timestamp(0)
        , block_height(0)
        , is_mine(false)
    {
        tx_hash.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(tx_hash);
        READWRITE(mweb_tx);
        READWRITE(input_amounts);
        READWRITE(output_amounts);
        READWRITE(fee);
        READWRITE(timestamp);
        READWRITE(block_height);
        READWRITE(is_mine);
    }
};

/** Peg transaction entry */
class CPegWalletEntry {
public:
    uint256 peg_tx_id;                 // Unique peg transaction ID
    bool is_peg_in;                    // true = peg-in, false = peg-out
    uint256 main_chain_tx_id;          // Main chain transaction hash
    uint256 mweb_tx_id;                // MWEB transaction hash (if applicable)
    CAmount amount;                    // Amount pegged
    CPubKey address;                  // Address (main chain or MWEB)
    int64_t timestamp;                 // When peg occurred
    uint64_t block_height;            // Block height
    bool is_confirmed;                 // Whether peg is confirmed

    CPegWalletEntry()
        : is_peg_in(true)
        , amount(0)
        , timestamp(0)
        , block_height(0)
        , is_confirmed(false)
    {
        peg_tx_id.SetNull();
        main_chain_tx_id.SetNull();
        mweb_tx_id.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(peg_tx_id);
        READWRITE(is_peg_in);
        READWRITE(main_chain_tx_id);
        READWRITE(mweb_tx_id);
        READWRITE(amount);
        READWRITE(address);
        READWRITE(timestamp);
        READWRITE(block_height);
        READWRITE(is_confirmed);
    }
};

#endif // FLEETCREDITS_WALLET_WALLETEXTENSIONS_H

