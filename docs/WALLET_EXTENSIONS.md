# Wallet Structure Extensions for Fleet Credits

## Overview

The Fleet Credits wallet (`wallet.dat`) extends the standard Bitcoin/Dogecoin wallet format to support:

1. **MWEB (Mimblewimble Extension Blocks)** data
2. **Contribution transaction** metadata
3. **Reward tracking** for contributor payouts
4. **Oracle staking** information
5. **View keys** for MWEB auditing

## Current Wallet Structure

The wallet uses Berkeley DB with key-value pairs:
- `("tx", hash)` → `CWalletTx` (serialized transaction)
- `("key", pubkey)` → `(privkey, hash)` (private keys)
- `("ckey", pubkey)` → `encrypted_privkey` (encrypted keys)
- `("name", address)` → `account_name` (address labels)
- `("pool", index)` → `CKeyPool` (key pool entries)
- `("hdchain", ...)` → `CHDChain` (HD wallet chain)

## New Wallet Record Types

### 1. MWEB View Keys
**Key Format:** `("mwebviewkey", view_key_hash)`  
**Value:** `CViewKey` (serialized, private key encrypted)

Stores view keys for decrypting MWEB transactions. View keys are encrypted when stored.

**Usage:**
- Decrypt MWEB transaction details for auditing
- Required for ETHICAL_REVIEW contributions routed through MWEB

### 2. MWEB UTXO Commitments
**Key Format:** `("mwebutxo", commitment_hash)`  
**Value:** `CMWEBUTXOEntry` (commitment, blinding factor hint, amount hint)

Tracks MWEB UTXOs owned by this wallet. The blinding factor is stored encrypted.

**Structure:**
```cpp
class CMWEBUTXOEntry {
    CPedersenCommitment commitment;    // The commitment
    uint256 blinding_factor_hash;      // Hash of blinding factor (for verification)
    CAmount amount_hint;               // Optional: cached amount (for faster queries)
    uint64_t block_height;             // Block height when created
    bool is_spent;                     // Whether this UTXO is spent
};
```

### 3. Contribution Transaction Metadata
**Key Format:** `("contrib", tx_hash)`  
**Value:** `CContributionWalletEntry` (contribution metadata)

Links standard transactions to contribution data. This allows the wallet to:
- Track which transactions contain contributions
- Display contribution details in UI
- Query reward history

**Structure:**
```cpp
class CContributionWalletEntry {
    uint256 tx_hash;                   // Transaction hash
    CContributionTransaction contrib;  // Contribution transaction data
    ContributionStatus status;         // PENDING, APPROVED, REJECTED
    int64_t confirmation_count;       // Number of confirmations
    CAmount reward_amount;             // Reward received (if any)
    uint256 reward_tx_hash;            // Transaction that paid the reward
};
```

### 4. Contributor Reward History
**Key Format:** `("reward", reward_id)`  
**Value:** `CRewardEntry` (reward payout information)

Tracks all contributor rewards received. Each reward entry represents a payout from a coinbase transaction.

**Structure:**
```cpp
class CRewardEntry {
    uint256 reward_id;                 // Unique reward ID (hash of block + contributor)
    uint256 block_hash;                // Block that paid the reward
    uint64_t block_height;             // Block height
    uint256 contrib_tx_hash;           // Original contribution transaction
    CAmount reward_amount;             // Amount received
    ContributionType contrib_type;    // Type of contribution
    uint32_t bonus_level;              // Bonus level applied
    int64_t timestamp;                 // When reward was received
};
```

### 5. Oracle Staking Information
**Key Format:** `("oracle", pubkey)`  
**Value:** `COracleStakeEntry` (oracle staking data)

Stores oracle staking information if this wallet is participating as an oracle.

**Structure:**
```cpp
class COracleStakeEntry {
    CPubKey oracle_pubkey;             // Oracle's public key
    CAmount staked_amount;             // Amount staked in FC
    uint32_t reputation_score;         // Current reputation (0-1000)
    uint64_t staking_start_height;     // Block height when staking started
    uint64_t last_vote_height;         // Last block height voted on
    std::vector<uint256> active_votes; // Active verification votes
    bool is_active;                    // Whether oracle is currently active
};
```

### 6. MWEB Transaction Cache
**Key Format:** `("mwebtx", tx_hash)`  
**Value:** `CMWEBWalletTx` (MWEB transaction with decrypted data)

Caches MWEB transactions with decrypted amounts (using view keys). This allows the wallet to:
- Display MWEB transaction history
- Calculate MWEB balances
- Show transaction details in UI

**Structure:**
```cpp
class CMWEBWalletTx {
    uint256 tx_hash;                   // MWEB transaction hash
    CMWEBTransaction mweb_tx;          // The MWEB transaction
    std::map<uint256, CAmount> input_amounts;   // Decrypted input amounts
    std::map<uint256, CAmount> output_amounts; // Decrypted output amounts
    CAmount fee;                       // Transaction fee
    int64_t timestamp;                 // Transaction timestamp
    uint64_t block_height;             // Block height (if confirmed)
    bool is_mine;                      // Whether this wallet owns any outputs
};
```

### 7. Peg Transactions
**Key Format:** `("peg", peg_tx_id)`  
**Value:** `CPegWalletEntry` (peg-in/peg-out information)

Tracks peg-in and peg-out transactions.

**Structure:**
```cpp
class CPegWalletEntry {
    uint256 peg_tx_id;                 // Unique peg transaction ID
    bool is_peg_in;                    // true = peg-in, false = peg-out
    uint256 main_chain_tx_id;          // Main chain transaction hash
    uint256 mweb_tx_id;                // MWEB transaction hash (if applicable)
    CAmount amount;                    // Amount pegged
    CPubKey address;                  // Address (main chain or MWEB)
    int64_t timestamp;                 // When peg occurred
    uint64_t block_height;            // Block height
    bool is_confirmed;                 // Whether peg is confirmed
};
```

## Database Schema Summary

```
wallet.dat (Berkeley DB)
├── Standard Records (existing)
│   ├── ("tx", hash) → CWalletTx
│   ├── ("key", pubkey) → (privkey, hash)
│   ├── ("ckey", pubkey) → encrypted_privkey
│   ├── ("name", address) → account_name
│   └── ...
│
└── Fleet Credits Extensions (new)
    ├── ("mwebviewkey", view_key_hash) → CViewKey
    ├── ("mwebutxo", commitment_hash) → CMWEBUTXOEntry
    ├── ("contrib", tx_hash) → CContributionWalletEntry
    ├── ("reward", reward_id) → CRewardEntry
    ├── ("oracle", pubkey) → COracleStakeEntry
    ├── ("mwebtx", tx_hash) → CMWEBWalletTx
    └── ("peg", peg_tx_id) → CPegWalletEntry
```

## Migration Path

### Version 1.0 (Current)
- Standard Bitcoin/Dogecoin wallet format
- No MWEB support
- No contribution tracking

### Version 2.0 (Fleet Credits v1.0)
- Adds contribution metadata (`"contrib"` records)
- Adds reward tracking (`"reward"` records)
- Wallet version incremented to `20000`

### Version 3.0 (Fleet Credits v2.0 with MWEB)
- Adds MWEB support (`"mwebviewkey"`, `"mwebutxo"`, `"mwebtx"` records)
- Adds peg transactions (`"peg"` records)
- Wallet version incremented to `30000`

### Version 4.0 (Fleet Credits v3.0 with Oracles)
- Adds oracle staking (`"oracle"` records)
- Wallet version incremented to `40000`

## Implementation Notes

1. **Backward Compatibility**: Old wallets without Fleet Credits extensions will continue to work. New records are only added when features are used.

2. **Encryption**: 
   - MWEB view keys are encrypted using the wallet's encryption key
   - Blinding factors are never stored in plaintext (only hashes)

3. **Performance**:
   - Contribution metadata is indexed by transaction hash for fast lookups
   - Reward entries are indexed by block height for efficient queries
   - MWEB UTXOs use commitment hash as key for O(1) lookups

4. **Migration**:
   - When loading an old wallet, check `CLIENT_VERSION` in wallet.dat
   - If version < 20000, wallet is pre-Fleet Credits (no migration needed)
   - If version >= 20000, read new record types as encountered

5. **Privacy**:
   - MWEB data is only stored locally
   - View keys are encrypted
   - Contribution metadata only stores public information (transaction hash, status)

## Example Usage

### Storing a Contribution Transaction
```cpp
CWalletDB walletdb(strWalletFile);
CContributionWalletEntry contribEntry;
contribEntry.tx_hash = tx.GetHash();
contribEntry.contrib = contribution;
contribEntry.status = CONTRIB_PENDING;
walletdb.WriteContribution(contribEntry.tx_hash, contribEntry);
```

### Storing MWEB View Key
```cpp
CViewKey viewKey = CViewKey::GenerateFromSeed(seed);
walletdb.WriteMWEBViewKey(viewKey.view_key_hash, viewKey);
```

### Querying Reward History
```cpp
std::vector<CRewardEntry> rewards;
walletdb.ListRewards(rewards);  // Get all rewards
// Filter by date, amount, contribution type, etc.
```

## File: `fleetcredits/src/wallet/walletdb.h`

New methods to add:
```cpp
// MWEB
bool WriteMWEBViewKey(const uint256& view_key_hash, const CViewKey& viewKey);
bool ReadMWEBViewKey(const uint256& view_key_hash, CViewKey& viewKey);
bool WriteMWEBUTXO(const uint256& commitment_hash, const CMWEBUTXOEntry& entry);
bool ReadMWEBUTXO(const uint256& commitment_hash, CMWEBUTXOEntry& entry);
bool WriteMWEBTx(const uint256& tx_hash, const CMWEBWalletTx& mweb_tx);
bool ReadMWEBTx(const uint256& tx_hash, CMWEBWalletTx& mweb_tx);

// Contributions
bool WriteContribution(const uint256& tx_hash, const CContributionWalletEntry& entry);
bool ReadContribution(const uint256& tx_hash, CContributionWalletEntry& entry);
bool EraseContribution(const uint256& tx_hash);

// Rewards
bool WriteReward(const uint256& reward_id, const CRewardEntry& reward);
bool ReadReward(const uint256& reward_id, CRewardEntry& reward);
bool ListRewards(std::vector<CRewardEntry>& rewards);

// Oracles
bool WriteOracleStake(const CPubKey& pubkey, const COracleStakeEntry& entry);
bool ReadOracleStake(const CPubKey& pubkey, COracleStakeEntry& entry);
bool EraseOracleStake(const CPubKey& pubkey);

// Pegs
bool WritePeg(const uint256& peg_tx_id, const CPegWalletEntry& entry);
bool ReadPeg(const uint256& peg_tx_id, CPegWalletEntry& entry);
```

## Summary

The wallet structure extensions provide:
- ✅ MWEB transaction tracking and decryption
- ✅ Contribution transaction metadata
- ✅ Reward history and payout tracking
- ✅ Oracle staking information
- ✅ Peg-in/peg-out transaction tracking
- ✅ Backward compatibility with existing wallets
- ✅ Encrypted storage for sensitive data

These extensions enable the Fleet Credits wallet to fully support all project features while maintaining compatibility with the existing wallet format.

