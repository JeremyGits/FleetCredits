#!/bin/bash
# Test script for all Fleet Credits Contribution Types
# Tests all 8 contribution types to verify they work correctly

set -e

CLI="./src/fleetcredits-cli -regtest"

echo "========================================="
echo "Testing All Contribution Types"
echo "========================================="
echo ""

# Get address and fund it
ADDRESS=$($CLI getnewaddress "all_types_test")
echo "Test address: $ADDRESS"
$CLI generatetoaddress 10 "$ADDRESS" > /dev/null
echo "Funded with 10 blocks"
echo ""

declare -a CONTRIB_TYPES=(
    "CODE_CONTRIBUTION"
    "CHARITABLE_ACT"
    "CREATIVE_WORK"
    "EDUCATIONAL_CONTENT"
    "CODE_MENTORSHIP"
    "AI_VALIDATION"
    "DATA_LABELING"
    "ETHICAL_REVIEW"
)

declare -a TXIDS=()

# Submit each contribution type
for CONTRIB_TYPE in "${CONTRIB_TYPES[@]}"; do
    echo "Testing: $CONTRIB_TYPE"
    
    # Generate unique proof hash for each type
    PROOF_HASH=$(echo -n "${CONTRIB_TYPE}_$(date +%s%N)" | sha256sum | cut -d' ' -f1)
    
    # Create type-specific metadata
    case $CONTRIB_TYPE in
        CODE_CONTRIBUTION)
            METADATA='{"github_commit":"abc123","lines_changed":50,"repository":"test/repo"}'
            ;;
        CHARITABLE_ACT)
            METADATA='{"organization":"Test Charity","hours_volunteered":10,"verified":true}'
            ;;
        CREATIVE_WORK)
            METADATA='{"medium":"digital_art","title":"Test Artwork","license":"CC0"}'
            ;;
        EDUCATIONAL_CONTENT)
            METADATA='{"type":"tutorial","title":"How to Use Fleet Credits","views":100}'
            ;;
        CODE_MENTORSHIP)
            METADATA='{"mentee":"student123","feedback_length":200,"approval_status":"APPROVED"}'
            ;;
        AI_VALIDATION)
            METADATA='{"ai_model":"GPT-4","validation_type":"code_review","accuracy":0.95}'
            ;;
        DATA_LABELING)
            METADATA='{"dataset":"image_classification","labels_created":1000,"quality_score":0.98}'
            ;;
        ETHICAL_REVIEW)
            METADATA='{"review_type":"bias_assessment","ai_system":"recommendation_engine","severity":"low"}'
            ;;
    esac
    
    RESULT=$($CLI submitcontribution "$CONTRIB_TYPE" "$PROOF_HASH" "$METADATA" 2>&1)
    
    # Extract TXID from JSON response (compatible with basic grep)
    TXID=$(echo "$RESULT" | grep -oE '"txid"[[:space:]]*:[[:space:]]*"[a-f0-9]{64}"' | grep -oE '[a-f0-9]{64}' | head -1)
    
    # Fallback: try jq if available
    if [ -z "$TXID" ] && command -v jq >/dev/null 2>&1; then
        TXID=$(echo "$RESULT" | jq -r '.txid' 2>/dev/null | grep -oE '[a-f0-9]{64}' | head -1)
    fi
    
    if [ ! -z "$TXID" ]; then
        echo "  ✅ Submitted: $TXID"
        TXIDS+=("$TXID")
    else
        echo "  ⚠️  Failed to extract TXID"
        echo "  Raw: $RESULT"
    fi
    echo ""
done

# Mine blocks to confirm all
echo "Mining blocks to confirm all contributions..."
$CLI generatetoaddress ${#TXIDS[@]} "$ADDRESS" > /dev/null
echo "Mined ${#TXIDS[@]} blocks"
echo ""

# Check status of each
echo "Checking status of all contributions..."
for TXID in "${TXIDS[@]}"; do
    if [ ! -z "$TXID" ]; then
        STATUS=$($CLI getcontributionstatus "$TXID" 2>&1)
        CONTRIB_TYPE=$(echo "$STATUS" | grep -oE '"contrib_type"[[:space:]]*:[[:space:]]*"[A-Z_]+"' | grep -oE '[A-Z_]+' | head -1)
        STATE=$(echo "$STATUS" | grep -oE '"status"[[:space:]]*:[[:space:]]*"[a-z_]+"' | grep -oE '[a-z_]+' | tail -1)
        echo "  $CONTRIB_TYPE: $STATE ($TXID)"
    fi
done
echo ""

# List all contributions
echo "Listing all contributions..."
CONTRIBS=$($CLI listcontributions 2>&1)
COUNT=$(echo "$CONTRIBS" | grep -c '"txid"' || echo "0")
echo "  Found $COUNT total contributions"
echo ""

# Test filtering by type
echo "Testing filters..."
for CONTRIB_TYPE in "${CONTRIB_TYPES[@]}"; do
    FILTERED=$($CLI listcontributions "" "$CONTRIB_TYPE" 2>&1)
    FILTERED_COUNT=$(echo "$FILTERED" | grep -c '"txid"' || echo "0")
    echo "  $CONTRIB_TYPE: $FILTERED_COUNT"
done
echo ""

# Check if ETHICAL_REVIEW routes to MWEB
echo "Checking MWEB routing for ETHICAL_REVIEW..."
ETHICAL_TXID=""
for i in "${!CONTRIB_TYPES[@]}"; do
    if [ "${CONTRIB_TYPES[$i]}" = "ETHICAL_REVIEW" ]; then
        ETHICAL_TXID="${TXIDS[$i]}"
        break
    fi
done

if [ ! -z "$ETHICAL_TXID" ]; then
    STATUS=$($CLI getcontributionstatus "$ETHICAL_TXID" 2>&1)
    REQUIRES_MWEB=$(echo "$STATUS" | grep -oE '"requires_mweb"[[:space:]]*:[[:space:]]*(true|false)' | grep -oE '(true|false)' | head -1)
    echo "  ETHICAL_REVIEW requires_mweb: $REQUIRES_MWEB"
    if [ "$REQUIRES_MWEB" = "true" ]; then
        echo "  ✅ ETHICAL_REVIEW correctly requires MWEB!"
        echo "  Attempting to route to MWEB..."
        ROUTE_RESULT=$($CLI routecontributiontomweb "$ETHICAL_TXID" 2>&1)
        echo "  Route result: $ROUTE_RESULT"
    fi
fi
echo ""

echo "========================================="
echo "All Contribution Types Test Complete!"
echo "========================================="
echo ""
echo "Summary:"
echo "  Total types tested: ${#CONTRIB_TYPES[@]}"
echo "  Successful submissions: ${#TXIDS[@]}"
echo ""

