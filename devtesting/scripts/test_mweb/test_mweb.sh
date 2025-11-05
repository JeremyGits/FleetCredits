#!/bin/bash
# Test script for Fleet Credits MWEB (Mimblewimble Extension Blocks)
# Run with: ./test_mweb.sh

set -e

CLI="./src/fleetcredits-cli -regtest"
REGTEST_NET="regtest"

echo "========================================="
echo "Fleet Credits MWEB Test"
echo "========================================="
echo ""

# Step 1: Get addresses
echo "Step 1: Getting addresses..."
MAIN_ADDRESS=$($CLI getnewaddress "main")
echo "  Main chain address: $MAIN_ADDRESS"
echo ""

# Step 2: Generate blocks to fund
echo "Step 2: Generating blocks..."
$CLI generatetoaddress 10 "$MAIN_ADDRESS" > /dev/null
BALANCE=$($CLI getbalance)
echo "  Balance: $BALANCE FC"
echo ""

# Step 3: Test MWEB Peg-in (move FC from main chain to MWEB)
echo "Step 3: Testing MWEB Peg-in..."
echo "  Creating peg-in transaction (main chain -> MWEB)..."
PEGIN_AMOUNT="1000.0"
PEGIN_RESULT=$($CLI createpegin "$MAIN_ADDRESS" "$PEGIN_AMOUNT" "Test peg-in" 2>&1)
echo "  Peg-in result:"
echo "$PEGIN_RESULT"
echo ""

# Step 4: Mine block to confirm peg-in
echo "Step 4: Mining block to confirm peg-in..."
$CLI generatetoaddress 1 "$MAIN_ADDRESS" > /dev/null
echo "  Block mined"
echo ""

# Step 5: Test MWEB Peg-out (move FC from MWEB back to main chain)
echo "Step 5: Testing MWEB Peg-out..."
echo "  Note: This requires a commitment from MWEB side"
echo "  (MWEB implementation needed for full test)"
echo ""

# Step 6: List MWEB transactions
echo "Step 6: Listing MWEB transactions..."
MWEB_TXS=$($CLI listmwebtxs 2>&1)
echo "  MWEB transactions:"
echo "$MWEB_TXS"
echo ""

echo "========================================="
echo "MWEB Test Complete!"
echo "========================================="
echo ""
echo "Note: Full MWEB functionality requires:"
echo "  - MWEB extension blocks implementation"
echo "  - Pedersen commitments"
echo "  - Range proofs"
echo "  - View keys for auditing"

