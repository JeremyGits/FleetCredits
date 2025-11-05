# Fleet Credits Conversion Plan
## From Fleet Credits Core to Fleet Credits

This document tracks the systematic conversion of Fleet Credits Core codebase to Fleet Credits.

## Phase 1: Core Network Parameters ✅ In Progress

### Files to Modify:
- [x] `src/chainparams.cpp` - Block reward (10k FC), block time (60s), no halving
- [ ] `src/chainparams.h` - Update branding
- [ ] `src/chainparamsbase.cpp` - Port numbers (22556/22555 mainnet, 44556/44555 testnet)
- [ ] `src/consensus/params.h` - Consensus parameters

## Phase 2: Branding & Naming

### Files to Modify:
- [ ] `README.md` - Complete rebrand
- [ ] `src/clientversion.h` - Client version string
- [ ] `src/version.h` - Protocol version
- [ ] All `fleetcredits*` binary names → `fleetcredits*`
- [ ] All `FC` references → `FC`
- [ ] All `Fleet Credits` references → `Fleet Credits`

## Phase 3: Fee Structure

### Files to Modify:
- [ ] `src/policy/fees.cpp` - Zero-fee for <1000 FC transactions
- [ ] `src/policy/policy.cpp` - Fee calculation logic
- [ ] `src/validation.cpp` - Fee validation rules

## Phase 4: Contribution System (Major Addition)

### New Files to Create:
- [ ] `src/contrib/contribtx.h` - Contribution transaction structure
- [ ] `src/contrib/contribtx.cpp` - Contribution transaction logic
- [ ] `src/contrib/verification.h` - Verification system
- [ ] `src/contrib/verification.cpp` - Oracle/human validator logic
- [ ] `src/contrib/rewards.h` - Reward calculation
- [ ] `src/contrib/rewards.cpp` - Reward distribution

### Files to Modify:
- [ ] `src/primitives/transaction.h` - Add CONTRIB_TX type
- [ ] `src/validation.cpp` - Contribution validation rules
- [ ] `src/miner.cpp` - Include contribution txs in blocks

## Phase 5: MWEB Integration (Major Addition)

### New Files to Create:
- [ ] `src/mweb/mweb.h` - MWEB header
- [ ] `src/mweb/kernel.h` - Kernel structure
- [ ] `src/mweb/commitment.h` - Pedersen commitments
- [ ] `src/mweb/peg.h` - Peg-in/peg-out logic
- [ ] `src/mweb/extension_block.h` - Extension block structure

### Files to Modify:
- [ ] `src/block.h` - MWEB extension block support
- [ ] `src/validation.cpp` - MWEB validation

## Phase 6: Governance System

### New Files to Create:
- [ ] `src/governance/proposal.h` - Governance proposals
- [ ] `src/governance/voting.h` - On-chain voting
- [ ] `src/governance/execution.h` - Proposal execution

## Phase 7: Community Reserve

### Files to Modify:
- [ ] `src/policy/fees.cpp` - Reserve allocation (1% of fees)
- [ ] `src/wallet/wallet.cpp` - Reserve management

## Phase 8: Build System

### Files to Modify:
- [ ] `configure.ac` - Package name, version
- [ ] `Makefile.am` - Binary names
- [ ] `contrib/debian/*` - Debian package names
- [ ] `contrib/rpm/*` - RPM package names

## Phase 9: Documentation

### Files to Update:
- [ ] `README.md` - Complete Fleet Credits documentation
- [ ] `doc/*` - All documentation files
- [ ] `INSTALL.md` - Installation instructions

## Phase 10: Tests

### Files to Update:
- [ ] `src/test/*` - Update all test files
- [ ] `qa/rpc-tests/*` - RPC test updates

## Progress Tracking

- **Started**: 2025-10-31
- **Current Phase**: Phase 1 - Core Network Parameters
- **Status**: ✅ In Progress

## Notes

- Maintain MIT license
- Keep Scrypt PoW algorithm
- Port numbers: 22556/22555 (mainnet), 44556/44555 (testnet)
- Block time: 60 seconds
- Block reward: 10,000 FC (no halving)
- Genesis block: Fair launch, no premine

