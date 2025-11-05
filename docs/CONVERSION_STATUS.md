# Fleet Credits Conversion Status
## Starting Conversion: October 31, 2025

### Phase 1: Core Network Parameters - IN PROGRESS

**File**: `src/chainparams.cpp`
- [ ] Update copyright headers (Fleet Credits)
- [ ] Change block reward: 10,000 FC (no halving)
- [ ] Set block time: 60 seconds  
- [ ] Remove halving intervals
- [ ] Create new genesis block (fair launch)
- [ ] Update message start bytes (0xFC prefix)
- [ ] Simplify consensus (remove AuxPoW for now)
- [ ] Update base58 prefixes (FC addresses)
- [ ] Clear checkpoints (new chain)
- [ ] Update seeds (will be empty initially)

**File**: `src/chainparams.h`
- [ ] Update branding comments

**File**: `src/chainparamsbase.cpp`
- [ ] Port numbers already correct (22556/22555 mainnet, 44556/44555 testnet)

### Next Steps After Phase 1:
1. Update README.md with Fleet Credits branding
2. Update clientversion.h for binary names
3. Start fee structure changes
4. Begin contribution system implementation

### Notes:
- Keeping Scrypt PoW
- Maintaining UTXO model
- Genesis block will be fair launch (no premine)
- Need to generate new genesis block hash

