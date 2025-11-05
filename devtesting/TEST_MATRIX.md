# Fleet Credits Test Matrix

## Quick Reference: What Can We Test?

### ✅ Core Blockchain Functions
- [x] Block generation and mining
- [x] Transaction creation and signing
- [x] Balance checking
- [x] Address generation
- [x] Block confirmation

### ✅ Contribution System (8 Types)
- [x] CODE_CONTRIBUTION - Code commits, PRs
- [x] CHARITABLE_ACT - Verified charity work
- [x] CREATIVE_WORK - Art, music, writing
- [x] EDUCATIONAL_CONTENT - Tutorials, docs
- [x] CODE_MENTORSHIP - Code review and guidance
- [x] AI_VALIDATION - Human validation of AI outputs
- [x] DATA_LABELING - Dataset annotation
- [x] ETHICAL_REVIEW - Ethical AI assessment (requires MWEB)

### ✅ Contribution Features
- [x] Submit contribution (`submitcontribution`)
- [x] Check status (`getcontributionstatus`)
- [x] List contributions (`listcontributions`)
- [x] Filter by address
- [x] Filter by type
- [x] Reward calculation
- [x] Bonus level determination
- [x] GitHub integration (real commits)

### ✅ MWEB (Mimblewimble Extension Blocks)
- [x] Peg-in (Main Chain → MWEB)
- [x] Peg-out (MWEB → Main Chain)
- [x] List MWEB transactions (`listmwebtxs`)
- [x] Route contribution to MWEB (`routecontributiontomweb`)
- [x] Automatic routing for ETHICAL_REVIEW
- [ ] Full extension block structure (in development)
- [ ] Pedersen commitments (in development)
- [ ] Range proofs (in development)

### ✅ Reward System
- [x] Base block reward (10,000 FC)
- [x] Contribution bonus multipliers:
  - BONUS_NONE (1.00x)
  - BONUS_LOW (1.05x)
  - BONUS_MEDIUM (1.10x)
  - BONUS_HIGH (1.15x)
  - BONUS_CRITICAL (1.20x or 1.25x for AI_VALIDATION)
- [x] Reward calculation during block validation
- [ ] Oracle verification (off-chain, requires oracle nodes)

### ✅ Fee System
- [x] Zero fees for micro-transactions (<1,000 FC)
- [x] Dynamic fees for larger transactions
- [x] Fee cap (0.2 FC/kB)
- [ ] Community Reserve allocation (1% of fees)

### ⏳ Pending/In Development
- [ ] Oracle verification system (off-chain)
- [ ] Full MWEB extension block implementation
- [ ] Governance voting system
- [ ] DEX integration
- [ ] Payment gateway APIs
- [ ] Mobile mining support

## Test Scenarios Covered

### Scenario 1: Developer Submits Code Contribution
1. Developer commits code to GitHub
2. Submits contribution with commit hash
3. Oracle verifies (off-chain)
4. Block includes contribution
5. Reward calculated with bonus
6. **Test**: `test_github_contribution.sh`

### Scenario 2: AI Validator Reviews Model Output
1. AI generates code/content
2. Human validator reviews and validates
3. Submits AI_VALIDATION contribution
4. Highest bonus multiplier applied (1.25x)
5. Reward distributed
6. **Test**: `test_all_contribution_types.sh`

### Scenario 3: Ethical Review with MWEB Privacy
1. Ethical review submitted (ETHICAL_REVIEW)
2. Automatically requires MWEB
3. Routed to MWEB for privacy
4. Pedersen commitments hide amounts
5. View keys enable selective disclosure
6. **Test**: `test_full_mweb_workflow.sh`

### Scenario 4: Full Workflow Integration
1. Mine blocks
2. Submit multiple contribution types
3. Route sensitive ones to MWEB
4. Verify rewards
5. Check MWEB transactions
6. Test peg-in/peg-out
7. **Test**: `full_workflow_test.sh`

## Running All Tests

```bash
# Make sure daemon is running
./src/fleetcreditsd -regtest -daemon

# Run all tests sequentially
cd devtesting/scripts

./test_contributions/test_contributions.sh
./test_contributions/test_github_contribution.sh
./test_contributions/test_all_contribution_types.sh
./test_contributions/test_reward_calculation.sh
./test_mweb/test_mweb.sh
./test_mweb/test_full_mweb_workflow.sh
./test_integration/full_workflow_test.sh

# Or run just the comprehensive integration test
./test_integration/full_workflow_test.sh
```

## Expected Results

All tests should:
- ✅ Successfully submit contributions
- ✅ Generate transaction IDs
- ✅ Confirm transactions in blocks
- ✅ Show correct status (pending → confirmed)
- ✅ Display reward information
- ✅ Handle MWEB routing correctly
- ✅ Filter contributions properly

## Troubleshooting

If tests fail:
1. Check daemon is running: `./src/fleetcredits-cli -regtest getinfo`
2. Ensure wallet is unlocked
3. Check you have sufficient balance (generate more blocks)
4. Verify RPC commands are available: `./src/fleetcredits-cli -regtest help`

