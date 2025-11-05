# Fleet Credits Testing Guide

## Testing the Contribution System

### Available RPC Commands

1. **submitcontribution** - Submit a contribution transaction
2. **getcontributionstatus** - Check the status of a contribution
3. **listcontributions** - List all contributions (optionally filtered by address/type)
4. **routecontributiontomweb** - Route a contribution to MWEB for privacy

### Quick Test Commands

Make sure your daemon is running first:
```bash
./src/fleetcreditsd -regtest -daemon
```

#### 1. Get a test address and fund it:
```bash
# Get new address
./src/fleetcredits-cli -regtest getnewaddress "test"

# Generate blocks (replace ADDRESS with your address)
./src/fleetcredits-cli -regtest generatetoaddress 10 "YOUR_ADDRESS_HERE"
```

#### 2. Submit a Code Contribution:
```bash
./src/fleetcredits-cli -regtest submitcontribution \
  "CODE_CONTRIBUTION" \
  "abc123def456githubcommithash" \
  '{"github_commit":"abc123","lines_changed":50,"repository":"test/repo"}'
```

This will return a transaction ID (txid).

#### 3. Mine a block to confirm:
```bash
./src/fleetcredits-cli -regtest generatetoaddress 1 "YOUR_ADDRESS_HERE"
```

#### 4. Check contribution status:
```bash
./src/fleetcredits-cli -regtest getcontributionstatus "TXID_HERE"
```

#### 5. List all contributions:
```bash
# List all
./src/fleetcredits-cli -regtest listcontributions

# Filter by address
./src/fleetcredits-cli -regtest listcontributions "YOUR_ADDRESS_HERE"

# Filter by type
./src/fleetcredits-cli -regtest listcontributions "" "CODE_CONTRIBUTION"
```

### Contribution Types

- `CODE_CONTRIBUTION` - Open source code commits
- `CHARITABLE_ACT` - Verified charitable activities
- `CREATIVE_WORK` - Art, music, writing
- `EDUCATIONAL_CONTENT` - Tutorials, documentation
- `CODE_MENTORSHIP` - Code review and mentorship
- `AI_VALIDATION` - Human validation of AI outputs
- `DATA_LABELING` - Dataset annotation for AI
- `ETHICAL_REVIEW` - Ethical assessment of AI systems (requires MWEB)

---

## Testing MWEB (Mimblewimble Extension Blocks)

### Available RPC Commands

1. **createpegin** - Move FC from main chain to MWEB (peg-in)
2. **createpegout** - Move FC from MWEB back to main chain (peg-out)
3. **listmwebtxs** - List MWEB transactions
4. **routecontributiontomweb** - Route a contribution to MWEB

### Quick Test Commands

#### 1. Create a Peg-in (Main Chain → MWEB):
```bash
./src/fleetcredits-cli -regtest createpegin "YOUR_ADDRESS_HERE" 1000.0 "Test peg-in"
```

This moves 1000 FC from the main chain into MWEB for private transactions.

#### 2. Mine block to confirm:
```bash
./src/fleetcredits-cli -regtest generatetoaddress 1 "YOUR_ADDRESS_HERE"
```

#### 3. List MWEB transactions:
```bash
# List all MWEB transactions
./src/fleetcredits-cli -regtest listmwebtxs

# List for specific address
./src/fleetcredits-cli -regtest listmwebtxs "YOUR_ADDRESS_HERE" 10
```

#### 4. Create a Peg-out (MWEB → Main Chain):
```bash
# Requires a commitment from MWEB side
./src/fleetcredits-cli -regtest createpegout \
  "COMMITMENT_HEX_HERE" \
  "MAIN_CHAIN_ADDRESS" \
  500.0 \
  "Test peg-out"
```

#### 5. Route a contribution to MWEB:
```bash
# First submit a contribution
TXID=$(./src/fleetcredits-cli -regtest submitcontribution "ETHICAL_REVIEW" "proof_hash" '{}' | jq -r '.txid')

# Then route it to MWEB
./src/fleetcredits-cli -regtest routecontributiontomweb "$TXID"
```

---

## Automated Test Scripts

### Complete Test Suite:

```bash
# === Contribution System Tests ===

# Basic contribution test
./devtesting/scripts/test_contributions/test_contributions.sh

# Real GitHub contribution (uses actual repo: JeremyGits/projectFC)
./devtesting/scripts/test_contributions/test_github_contribution.sh

# Test all 8 contribution types
./devtesting/scripts/test_contributions/test_all_contribution_types.sh

# Test reward calculation and bonus multipliers
./devtesting/scripts/test_contributions/test_reward_calculation.sh

# === MWEB Tests ===

# Basic MWEB test (peg-in, peg-out)
./devtesting/scripts/test_mweb/test_mweb.sh

# Full MWEB workflow (peg-in, routing, peg-out, contributions)
./devtesting/scripts/test_mweb/test_full_mweb_workflow.sh

# === Integration Tests ===

# Complete end-to-end workflow (mining, contributions, MWEB, rewards)
./devtesting/scripts/test_integration/full_workflow_test.sh
```

### Test Script Details:

1. **test_contributions.sh** - Basic contribution submission and verification
2. **test_github_contribution.sh** - Real GitHub repo contribution test
3. **test_all_contribution_types.sh** - Tests all 8 contribution types:
   - CODE_CONTRIBUTION
   - CHARITABLE_ACT
   - CREATIVE_WORK
   - EDUCATIONAL_CONTENT
   - CODE_MENTORSHIP
   - AI_VALIDATION
   - DATA_LABELING
   - ETHICAL_REVIEW (tests MWEB routing)
4. **test_reward_calculation.sh** - Tests bonus levels and reward multipliers
5. **test_mweb.sh** - Basic MWEB peg-in/peg-out
6. **test_full_mweb_workflow.sh** - Complete MWEB workflow with contributions
7. **full_workflow_test.sh** - Comprehensive integration test

### Real GitHub Contribution Test

The `test_github_contribution.sh` script tests with an actual GitHub repository:
- **Repository**: [JeremyGits/projectFC](https://github.com/JeremyGits/projectFC)
- **File**: `thought.md`
- **Commit**: `a86f5bbfe8d9f4cf0df45eae1fa7f345bacec86f`

This demonstrates how real-world GitHub contributions can be submitted to Fleet Credits.

---

## Expected Behavior

### Contribution System:
- Contributions are embedded in transactions using `OP_RETURN` outputs
- Each contribution has a unique marker (`0xFC 0x01`)
- Contributions can be verified by oracles (off-chain)
- Rewards are calculated based on bonus levels (1.05x to 1.25x multiplier)
- `ETHICAL_REVIEW` contributions automatically route to MWEB

### MWEB:
- Peg-in transactions create commitments on the main chain
- MWEB transactions use Pedersen commitments (amounts are hidden)
- Range proofs ensure no negative amounts
- View keys allow selective disclosure for auditing
- Cut-through reduces blockchain bloat

---

## Troubleshooting

1. **"Wallet not available"** - Make sure wallet is unlocked:
   ```bash
   ./src/fleetcredits-cli -regtest walletpassphrase "your_password" 3600
   ```

2. **"Insufficient funds"** - Generate more blocks:
   ```bash
   ./src/fleetcredits-cli -regtest generatetoaddress 10 "YOUR_ADDRESS"
   ```

3. **"Invalid contribution type"** - Check spelling (must be uppercase with underscores)

4. **MWEB not working** - MWEB requires full implementation of extension blocks, which may still be in development.

---

## Next Steps

1. Test different contribution types
2. Verify reward calculations
3. Test oracle verification (requires oracle implementation)
4. Test MWEB privacy features
5. Test contribution routing to MWEB

