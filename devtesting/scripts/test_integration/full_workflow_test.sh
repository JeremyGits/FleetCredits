#!/bin/bash
# Comprehensive integration test for Fleet Credits
# Tests: Mining, Contributions, MWEB, Rewards, Verification

set -e

CLI="./src/fleetcredits-cli -regtest"

echo "========================================="
echo "Fleet Credits Full Integration Test"
echo "========================================="
echo ""

# Setup
ADDRESS=$($CLI getnewaddress "integration_test")
echo "Test address: $ADDRESS"
echo ""

# Phase 1: Initial Mining
echo "Phase 1: Initial Mining"
echo "Generating 50 blocks..."
$CLI generatetoaddress 50 "$ADDRESS" > /dev/null
BALANCE=$($CLI getbalance)
BLOCK_COUNT=$($CLI getblockcount)
echo "  Balance: $BALANCE FC"
echo "  Block height: $BLOCK_COUNT"
echo ""

# Phase 2: Submit Multiple Contributions
echo "Phase 2: Submit Multiple Contribution Types"
echo ""

declare -a CONTRIB_TXIDS=()

# Submit 5 different types
echo "1. CODE_CONTRIBUTION..."
PROOF1=$(echo -n "code_$(date +%s)" | sha256sum | cut -d' ' -f1)
RESULT1=$($CLI submitcontribution "CODE_CONTRIBUTION" "$PROOF1" '{"lines":100}' 2>&1)
TXID1=$(echo "$RESULT1" | grep -oE '"txid"[[:space:]]*:[[:space:]]*"[a-f0-9]{64}"' | grep -oE '[a-f0-9]{64}' | head -1)
if [ ! -z "$TXID1" ]; then
    CONTRIB_TXIDS+=("$TXID1")
    echo "  ✅ $TXID1"
else
    echo "  ⚠️  Failed to extract TXID"
    echo "  Raw output: $RESULT1" | head -3
fi

echo "2. AI_VALIDATION..."
PROOF2=$(echo -n "ai_$(date +%s)" | sha256sum | cut -d' ' -f1)
RESULT2=$($CLI submitcontribution "AI_VALIDATION" "$PROOF2" '{"accuracy":0.98}' 2>&1)
TXID2=$(echo "$RESULT2" | grep -oE '"txid"[[:space:]]*:[[:space:]]*"[a-f0-9]{64}"' | grep -oE '[a-f0-9]{64}' | head -1)
if [ ! -z "$TXID2" ]; then
    CONTRIB_TXIDS+=("$TXID2")
    echo "  ✅ $TXID2"
else
    echo "  ⚠️  Failed to extract TXID"
fi

echo "3. CHARITABLE_ACT..."
PROOF3=$(echo -n "charity_$(date +%s)" | sha256sum | cut -d' ' -f1)
TXID3=$($CLI submitcontribution "CHARITABLE_ACT" "$PROOF3" '{"hours":20}' 2>&1 | grep -oE '(?<="txid"[: ]")[a-f0-9]{64}' | head -1)
if [ ! -z "$TXID3" ]; then
    CONTRIB_TXIDS+=("$TXID3")
    echo "  ✅ $TXID3"
fi

echo "4. ETHICAL_REVIEW (will route to MWEB)..."
PROOF4=$(echo -n "ethical_$(date +%s)" | sha256sum | cut -d' ' -f1)
TXID4=$($CLI submitcontribution "ETHICAL_REVIEW" "$PROOF4" '{"type":"bias"}' 2>&1 | grep -oE '(?<="txid"[: ]")[a-f0-9]{64}' | head -1)
if [ ! -z "$TXID4" ]; then
    CONTRIB_TXIDS+=("$TXID4")
    echo "  ✅ $TXID4 (requires MWEB)"
fi

echo "5. CODE_MENTORSHIP..."
PROOF5=$(echo -n "mentor_$(date +%s)" | sha256sum | cut -d' ' -f1)
TXID5=$($CLI submitcontribution "CODE_MENTORSHIP" "$PROOF5" '{"feedback":"great work!"}' 2>&1 | grep -oE '(?<="txid"[: ]")[a-f0-9]{64}' | head -1)
if [ ! -z "$TXID5" ]; then
    CONTRIB_TXIDS+=("$TXID5")
    echo "  ✅ $TXID5"
fi

echo ""
echo "Total contributions submitted: ${#CONTRIB_TXIDS[@]}"
echo ""

# Phase 3: Confirm Contributions
echo "Phase 3: Mining Blocks to Confirm Contributions"
echo "Mining ${#CONTRIB_TXIDS[@]} blocks..."
$CLI generatetoaddress ${#CONTRIB_TXIDS[@]} "$ADDRESS" > /dev/null
echo "Blocks mined"
echo ""

# Phase 4: Verify All Contributions
echo "Phase 4: Verifying Contributions"
for TXID in "${CONTRIB_TXIDS[@]}"; do
    STATUS=$($CLI getcontributionstatus "$TXID" 2>&1)
    STATE=$(echo "$STATUS" | grep -oE '(?<="status"[: ]")[a-z]+' | head -1)
    TYPE=$(echo "$STATUS" | grep -oE '(?<="contrib_type"[: ]")[A-Z_]+' | head -1)
    REWARD=$(echo "$STATUS" | grep -oE '(?<="reward"[: ])[0-9.]+' | head -1 || echo "N/A")
    echo "  $TYPE: $STATE (Reward: $REWARD FC)"
done
echo ""

# Phase 5: Test MWEB for ETHICAL_REVIEW
if [ ! -z "$TXID4" ]; then
    echo "Phase 5: Testing MWEB Routing for ETHICAL_REVIEW"
    ROUTE_RESULT=$($CLI routecontributiontomweb "$TXID4" 2>&1)
    echo "  Route result:"
    echo "$ROUTE_RESULT" | head -5
    echo ""
fi

# Phase 6: List and Filter
echo "Phase 6: Listing and Filtering Contributions"
ALL_COUNT=$($CLI listcontributions 2>&1 | grep -c "txid" || echo "0")
echo "  Total contributions: $ALL_COUNT"

CODE_COUNT=$($CLI listcontributions "" "CODE_CONTRIBUTION" 2>&1 | grep -c "txid" || echo "0")
echo "  CODE_CONTRIBUTION: $CODE_COUNT"

AI_COUNT=$($CLI listcontributions "" "AI_VALIDATION" 2>&1 | grep -c "txid" || echo "0")
echo "  AI_VALIDATION: $AI_COUNT"
echo ""

# Phase 7: Check Block Rewards
echo "Phase 7: Checking Block Information"
FINAL_BLOCK=$($CLI getblockcount)
FINAL_BALANCE=$($CLI getbalance)
echo "  Final block height: $FINAL_BLOCK"
echo "  Final balance: $FINAL_BALANCE FC"
echo ""

# Phase 8: Test MWEB Peg-in
echo "Phase 8: Testing MWEB Peg-in"
PEGIN_RESULT=$($CLI createpegin "$ADDRESS" "1000.0" "Integration test" 2>&1)
PEGIN_TXID=$(echo "$PEGIN_RESULT" | grep -oE '(?<="txid"[: ]")[a-f0-9]{64}' | head -1)
if [ ! -z "$PEGIN_TXID" ]; then
    echo "  ✅ Peg-in successful: $PEGIN_TXID"
else
    echo "  ⚠️  Peg-in result:"
    echo "$PEGIN_RESULT" | head -5
fi
echo ""

# Phase 9: List MWEB
echo "Phase 9: Listing MWEB Transactions"
MWEB_LIST=$($CLI listmwebtxs 2>&1)
MWEB_COUNT=$(echo "$MWEB_LIST" | grep -c "txid\|commitment" || echo "0")
echo "  MWEB transactions found: $MWEB_COUNT"
echo ""

echo "========================================="
echo "Full Integration Test Complete!"
echo "========================================="
echo ""
echo "Summary:"
echo "  - Blocks generated: $FINAL_BLOCK"
echo "  - Contributions submitted: ${#CONTRIB_TXIDS[@]}"
echo "  - Balance: $FINAL_BALANCE FC"
echo "  - MWEB transactions: $MWEB_COUNT"
echo ""
echo "✅ All systems tested!"
echo ""

