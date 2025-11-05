// Copyright (c) 2025-2026 The Fleet Credits Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef FLEETCREDITS_PRIMITIVES_MWEB_H
#define FLEETCREDITS_PRIMITIVES_MWEB_H

#include "pubkey.h"
#include "serialize.h"
#include "uint256.h"

#include <vector>

// Forward declaration
class CContributionTransaction;

/** MWEB (Mimblewimble Extension Blocks) for Fleet Credits
 * 
 * MWEB provides:
 * - Privacy: Hidden transaction amounts via Pedersen commitments
 * - Scalability: Cut-through reduces blockchain bloat
 * - Confidentiality: Mandatory for ETHICAL_REVIEW contributions
 * - Optional privacy for standard transactions
 */

/** Pedersen Commitment: C = vG + rH
 * Where:
 *   v = value (amount in FC, hidden)
 *   r = blinding factor (secret)
 *   G = generator for value
 *   H = generator for blinding factor
 */
class CPedersenCommitment {
public:
    CPubKey commitment;  // 33-byte compressed public key representing the commitment
    uint256 blinding_factor_hash;  // Hash of blinding factor (for verification)

    CPedersenCommitment() {
        // Default-constructed CPubKey is already invalid
        blinding_factor_hash.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(commitment);
        READWRITE(blinding_factor_hash);
    }

    bool IsNull() const {
        return !commitment.IsValid();
    }
};

/** Range Proof
 * Proves that a Pedersen commitment represents a non-negative value
 * without revealing the actual amount
 */
class CRangeProof {
public:
    std::vector<unsigned char> proof_data;  // Bulletproofs or similar range proof
    uint32_t value_range_min;  // Minimum value (typically 0)
    uint64_t value_range_max;  // Maximum value (MAX_MONEY) - changed to uint64_t to avoid overflow

    CRangeProof()
        : value_range_min(0)
        , value_range_max(0)
    {}

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(proof_data);
        READWRITE(value_range_min);
        READWRITE(value_range_max);
    }

    bool IsValid() const {
        return !proof_data.empty() && value_range_max > value_range_min;
    }
};

/** MWEB Input
 * References a previous MWEB output via its commitment
 */
class CMWEBInput {
public:
    CPedersenCommitment output_commitment;  // Commitment being spent
    std::vector<unsigned char> witness;     // Witness data proving ownership

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(output_commitment);
        READWRITE(witness);
    }
};

/** MWEB Output
 * Creates a new MWEB UTXO with hidden amount
 */
class CMWEBOutput {
public:
    CPedersenCommitment commitment;  // Pedersen commitment to value
    CRangeProof range_proof;         // Proof that value is non-negative
    std::vector<unsigned char> view_key;  // Optional: for selective disclosure

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(commitment);
        READWRITE(range_proof);
        READWRITE(view_key);
    }
};

/** MWEB Kernel
 * Contains excess value (sum of inputs - outputs) and signature
 * Ensures transaction balances without revealing amounts
 */
class CMWEBKernel {
public:
    CPubKey excess_value;  // Public key representing excess (change)
    std::vector<unsigned char> signature;  // Signature proving excess is valid
    uint64_t fee;  // Fee in FC (may be hidden or visible)
    uint256 kernel_id;  // Unique identifier for this kernel

    CMWEBKernel()
        : fee(0)
    {
        kernel_id.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(excess_value);
        READWRITE(signature);
        READWRITE(fee);
        READWRITE(kernel_id);
    }

    bool IsValid() const {
        return excess_value.IsValid() && !signature.empty();
    }
};

/** MWEB Transaction
 * Mimblewimble-style transaction with hidden amounts
 */
class CMWEBTransaction {
public:
    std::vector<CMWEBInput> inputs;
    std::vector<CMWEBOutput> outputs;
    std::vector<CMWEBKernel> kernels;
    uint64_t lock_height;  // Minimum block height for this transaction
    uint256 tx_hash;

    CMWEBTransaction()
        : lock_height(0)
    {
        tx_hash.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(inputs);
        READWRITE(outputs);
        READWRITE(kernels);
        READWRITE(lock_height);
        READWRITE(tx_hash);
    }

    /** Calculate transaction hash */
    uint256 GetHash() const;

    /** Verify transaction balance (sum of inputs = sum of outputs + fees) */
    bool VerifyBalance() const;

    /** Verify all range proofs */
    bool VerifyRangeProofs() const;
};

/** Peg-in Transaction
 * Moves FC from main chain to MWEB
 */
class CPegInTransaction {
public:
    uint256 main_chain_tx_id;  // Transaction ID on main chain
    CPubKey recipient_address;  // MWEB address receiving the funds
    uint64_t amount;  // Amount in FC (visible for peg-in)
    uint256 peg_tx_id;  // Unique identifier for this peg

    CPegInTransaction()
        : amount(0)
    {
        peg_tx_id.SetNull();
        main_chain_tx_id.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(main_chain_tx_id);
        READWRITE(recipient_address);
        READWRITE(amount);
        READWRITE(peg_tx_id);
    }
};

/** Peg-out Transaction
 * Moves FC from MWEB back to main chain
 */
class CPegOutTransaction {
public:
    CPedersenCommitment mweb_input_commitment;  // MWEB output being spent
    CPubKey main_chain_address;  // Address receiving funds on main chain
    uint64_t amount;  // Amount in FC (visible for peg-out)
    std::vector<unsigned char> view_key;  // For proving the amount
    uint256 peg_tx_id;  // Unique identifier for this peg

    CPegOutTransaction()
        : amount(0)
    {
        peg_tx_id.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(mweb_input_commitment);
        READWRITE(main_chain_address);
        READWRITE(amount);
        READWRITE(view_key);
        READWRITE(peg_tx_id);
    }
};

/** MWEB Extension Block
 * Attaches to main chain block, contains MWEB transactions
 */
class CMWEBExtensionBlock {
public:
    uint256 prev_mweb_hash;  // Hash of previous MWEB extension block
    std::vector<CMWEBTransaction> mweb_txs;
    std::vector<CPegInTransaction> peg_ins;
    std::vector<CPegOutTransaction> peg_outs;
    uint256 mweb_root;  // Merkle root of MWEB UTXO set
    uint256 extension_block_hash;

    CMWEBExtensionBlock() {
        prev_mweb_hash.SetNull();
        mweb_root.SetNull();
        extension_block_hash.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(prev_mweb_hash);
        READWRITE(mweb_txs);
        READWRITE(peg_ins);
        READWRITE(peg_outs);
        READWRITE(mweb_root);
        READWRITE(extension_block_hash);
    }

    /** Calculate extension block hash */
    uint256 GetHash() const;

    /** Verify all MWEB transactions in this block */
    bool VerifyAll() const;
};

/** View Key
 * Allows selective disclosure of MWEB transaction details for auditing
 */
class CViewKey {
public:
    CPubKey view_public_key;  // Public key for viewing
    std::vector<unsigned char> view_private_key;  // Private key (stored securely, encrypted)
    uint256 view_key_hash;    // Hash of view key pair

    CViewKey() {
        view_key_hash.SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        // Only serialize public key and hash, never private key
        READWRITE(view_public_key);
        READWRITE(view_key_hash);
    }

    /** Generate view key from seed */
    static CViewKey GenerateFromSeed(const uint256& seed);

    /** Decrypt MWEB transaction details using view key */
    bool DecryptTransaction(const CMWEBTransaction& tx) const;
};

/** MWEB Helper Functions */
namespace MWEB {
    /** Create a Pedersen commitment for an amount */
    CPedersenCommitment CreateCommitment(uint64_t amount, const uint256& blinding_factor);

    /** Verify a Pedersen commitment */
    bool VerifyCommitment(const CPedersenCommitment& commitment);

    /** Generate range proof for a commitment */
    CRangeProof GenerateRangeProof(const CPedersenCommitment& commitment, uint64_t amount);

    /** Verify a range proof */
    bool VerifyRangeProof(const CRangeProof& proof, const CPedersenCommitment& commitment);

    /** Calculate cut-through (remove intermediate outputs spent in same block) */
    CMWEBExtensionBlock CutThrough(const CMWEBExtensionBlock& block);

}

#endif // FLEETCREDITS_PRIMITIVES_MWEB_H

