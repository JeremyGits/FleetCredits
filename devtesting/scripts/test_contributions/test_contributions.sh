#!/bin/bash
# Test script for Fleet Credits Contribution System
# Run with: ./test_contributions.sh

set -e

CLI="./src/fleetcredits-cli -regtest"
REGTEST_NET="regtest"

echo "========================================="
echo "Fleet Credits Contribution System Test"
echo "========================================="
echo ""

# Step 1: Get a new address for testing
echo "Step 1: Getting test address..."
ADDRESS=$($CLI getnewaddress "test_contrib")
echo "  Test address: $ADDRESS"
echo ""

# Step 2: Generate some blocks to fund the address
echo "Step 2: Generating blocks to fund address..."
BLOCKS=$($CLI generatetoaddress 10 "$ADDRESS")
echo "  Generated blocks: $BLOCKS"
echo ""

# Step 3: Check balance
echo "Step 3: Checking balance..."
BALANCE=$($CLI getbalance)
echo "  Balance: $BALANCE FC"
echo ""

# Step 4: Test 1 - Submit a CODE_CONTRIBUTION
echo "Step 4: Testing CODE_CONTRIBUTION..."
echo "  Submitting contribution with proof hash..."
PROOF_HASH=$(echo -n "test_commit_$(date +%s)" | sha256sum | cut -d' ' -f1)
METADATA='{"github_commit":"abc123def456","lines_changed":50,"repository":"test/repo"}'

TXID=$($CLI submitcontribution "CODE_CONTRIBUTION" "$PROOF_HASH" "$METADATA" 2>&1)
echo "  Transaction ID: $TXID"
echo ""

# Step 5: Wait for confirmation
echo "Step 5: Mining a block to confirm transaction..."
$CLI generatetoaddress 1 "$ADDRESS" > /dev/null
echo "  Block mined"
echo ""

# Step 6: Check contribution status
echo "Step 6: Checking contribution status..."
if [[ $TXID =~ ^[a-f0-9]{64}$ ]]; then
    STATUS=$($CLI getcontributionstatus "$TXID" 2>&1)
    echo "  Status: $STATUS"
else
    echo "  Warning: Invalid TXID format, skipping status check"
    echo "  Raw output: $TXID"
fi
echo ""

# Step 7: List all contributions
echo "Step 7: Listing all contributions..."
CONTRIBS=$($CLI listcontributions 2>&1)
echo "  Contributions found:"
echo "$CONTRIBS" | head -20
echo ""

echo "========================================="
echo "Test Complete!"
echo "========================================="

