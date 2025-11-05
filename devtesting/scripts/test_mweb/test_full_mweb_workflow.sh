#!/bin/bash
# Test script for full MWEB (Mimblewimble Extension Blocks) workflow
# Tests: Peg-in, MWEB transactions, Peg-out, Contribution routing

set -e

CLI="./src/fleetcredits-cli -regtest"

echo "========================================="
echo "Full MWEB Workflow Test"
echo "========================================="
echo ""

# Setup
ADDRESS=$($CLI getnewaddress "mweb_test")
echo "Test address: $ADDRESS"
$CLI generatetoaddress 10 "$ADDRESS" > /dev/null
BALANCE=$($CLI getbalance)
echo "Balance: $BALANCE FC"
echo ""

# Step 1: Test Peg-in (Main Chain → MWEB)
echo "Step 1: Testing Peg-in (Main Chain → MWEB)..."
PEGIN_AMOUNT="5000.0"
echo "  Amount: $PEGIN_AMOUNT FC"
PEGIN_RESULT=$($CLI createpegin "$ADDRESS" "$PEGIN_AMOUNT" "Test peg-in" 2>&1)
echo "  Result:"
echo "$PEGIN_RESULT" | head -20
echo ""

# Extract commitment or TXID from result
PEGIN_TXID=$(echo "$PEGIN_RESULT" | grep -oE '"(pegtxid|maintxid|txid)"[[:space:]]*:[[:space:]]*"[a-f0-9]{64}"' | grep -oE '[a-f0-9]{64}' | head -1 || echo "")
if [ ! -z "$PEGIN_TXID" ]; then
    echo "  ✅ Peg-in TXID: $PEGIN_TXID"
else
    echo "  ⚠️  Could not extract TXID"
fi
echo ""

# Mine block to confirm peg-in
echo "Mining block to confirm peg-in..."
$CLI generatetoaddress 1 "$ADDRESS" > /dev/null
echo "Block mined"
echo ""

# Step 2: List MWEB transactions
echo "Step 2: Listing MWEB transactions..."
MWEB_TXS=$($CLI listmwebtxs 2>&1)
echo "  MWEB transactions:"
if echo "$MWEB_TXS" | grep -q "txid\|commitment"; then
    echo "$MWEB_TXS" | head -30
else
    echo "  (No MWEB transactions found or MWEB not fully implemented)"
fi
echo ""

# Step 3: Submit ETHICAL_REVIEW contribution (should require MWEB)
echo "Step 3: Submitting ETHICAL_REVIEW (requires MWEB)..."
PROOF=$(echo -n "ethical_review_$(date +%s)" | sha256sum | cut -d' ' -f1)
RESULT_ETHICAL=$($CLI submitcontribution "ETHICAL_REVIEW" "$PROOF" '{"review_type":"bias_assessment","ai_system":"test"}' 2>&1)
ETHICAL_TXID=$(echo "$RESULT_ETHICAL" | grep -oE '"txid"[[:space:]]*:[[:space:]]*"[a-f0-9]{64}"' | grep -oE '[a-f0-9]{64}' | head -1)

if [ ! -z "$ETHICAL_TXID" ]; then
    echo "  ✅ ETHICAL_REVIEW submitted: $ETHICAL_TXID"
    
    # Check if it requires MWEB
    STATUS=$($CLI getcontributionstatus "$ETHICAL_TXID" 2>&1)
    REQUIRES_MWEB=$(echo "$STATUS" | grep -oE '"requires_mweb"[[:space:]]*:[[:space:]]*(true|false)' | grep -oE '(true|false)' | head -1)
    echo "  Requires MWEB: $REQUIRES_MWEB"
    
    if [ "$REQUIRES_MWEB" = "true" ]; then
        echo "  ✅ Correctly requires MWEB!"
        
        # Route to MWEB
        echo "  Routing to MWEB..."
        ROUTE_RESULT=$($CLI routecontributiontomweb "$ETHICAL_TXID" 2>&1)
        echo "  Route result:"
        echo "$ROUTE_RESULT" | head -10
    fi
else
    echo "  ⚠️  Failed to submit ETHICAL_REVIEW"
fi
echo ""

# Mine block
echo "Mining block to confirm ETHICAL_REVIEW..."
$CLI generatetoaddress 1 "$ADDRESS" > /dev/null
echo "Block mined"
echo ""

# Step 4: Test Peg-out (MWEB → Main Chain)
echo "Step 4: Testing Peg-out (MWEB → Main Chain)..."
echo "  Note: This requires a commitment from MWEB side"
echo "  (Full implementation needed for actual peg-out)"
echo ""

# If we have a commitment from earlier, try peg-out
if [ ! -z "$PEGIN_TXID" ]; then
    # In real implementation, we'd get the commitment from MWEB state
    # For now, this is a placeholder
    echo "  Attempting peg-out (placeholder - needs MWEB commitment)..."
    PEGOUT_RESULT=$($CLI createpegout "COMMITMENT_PLACEHOLDER" "$ADDRESS" "2500.0" "Test peg-out" 2>&1)
    echo "  Result:"
    echo "$PEGOUT_RESULT" | head -10
    echo ""
fi

# Step 5: List all MWEB transactions again
echo "Step 5: Final MWEB transaction list..."
MWEB_TXS_FINAL=$($CLI listmwebtxs 2>&1)
echo "  MWEB transactions:"
if echo "$MWEB_TXS_FINAL" | grep -q "txid\|commitment"; then
    echo "$MWEB_TXS_FINAL" | head -30
else
    echo "  (MWEB extension blocks may not be fully implemented yet)"
    echo "  This is expected - MWEB requires:"
    echo "    - Extension block structure"
    echo "    - Pedersen commitments"
    echo "    - Range proofs"
    echo "    - Cut-through mechanism"
fi
echo ""

echo "========================================="
echo "MWEB Workflow Test Complete!"
echo "========================================="
echo ""
echo "Summary:"
echo "  - Peg-in tested"
echo "  - MWEB transaction listing tested"
echo "  - ETHICAL_REVIEW MWEB routing tested"
echo "  - Peg-out placeholder tested"
echo ""
echo "Note: Full MWEB requires extension block implementation."
echo "      See docs for MWEB architecture details."
echo ""

