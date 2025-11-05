#!/bin/bash
# Test script for Fleet Credits Reward Calculation
# Tests different bonus levels and reward multipliers

set -e

CLI="./src/fleetcredits-cli -regtest"

echo "========================================="
echo "Testing Contribution Reward Calculation"
echo "========================================="
echo ""

ADDRESS=$($CLI getnewaddress "reward_test")
$CLI generatetoaddress 10 "$ADDRESS" > /dev/null
BALANCE=$($CLI getbalance)
echo "Initial balance: $BALANCE FC"
echo ""

# Test different contribution types with different bonus levels
echo "Testing reward calculation for different contribution types..."
echo ""

# Get block info before contributions
BLOCK_BEFORE=$($CLI getblockcount)
echo "Block height before: $BLOCK_BEFORE"
echo ""

# Submit contributions that should have different bonus levels
# BONUS_NONE = 0 (1.0x)
# BONUS_LOW = 1 (1.05x)
# BONUS_MEDIUM = 2 (1.10x)
# BONUS_HIGH = 3 (1.15x)
# BONUS_CRITICAL = 4 (1.20x or 1.25x for AI_VALIDATION)

echo "1. Submitting CODE_CONTRIBUTION (typically BONUS_LOW or BONUS_MEDIUM)..."
PROOF1=$(echo -n "code_contrib_$(date +%s)" | sha256sum | cut -d' ' -f1)
TXID1=$($CLI submitcontribution "CODE_CONTRIBUTION" "$PROOF1" '{"lines_changed":100,"repository":"major/repo"}' 2>&1 | grep -oE '(?<="txid"[: ]")[a-f0-9]{64}' | head -1)
echo "  TXID: $TXID1"
echo ""

echo "2. Submitting AI_VALIDATION (highest potential bonus - 1.25x)..."
PROOF2=$(echo -n "ai_validation_$(date +%s)" | sha256sum | cut -d' ' -f1)
TXID2=$($CLI submitcontribution "AI_VALIDATION" "$PROOF2" '{"accuracy":0.99,"model":"GPT-4","critical":true}' 2>&1 | grep -oE '(?<="txid"[: ]")[a-f0-9]{64}' | head -1)
echo "  TXID: $TXID2"
echo ""

echo "3. Submitting CHARITABLE_ACT (typically BONUS_MEDIUM)..."
PROOF3=$(echo -n "charity_$(date +%s)" | sha256sum | cut -d' ' -f1)
TXID3=$($CLI submitcontribution "CHARITABLE_ACT" "$PROOF3" '{"hours":50,"verified":true}' 2>&1 | grep -oE '(?<="txid"[: ]")[a-f0-9]{64}' | head -1)
echo "  TXID: $TXID3"
echo ""

# Mine blocks to confirm
echo "Mining blocks to confirm contributions..."
$CLI generatetoaddress 3 "$ADDRESS" > /dev/null
echo "Blocks mined"
echo ""

# Check reward for each
echo "Checking rewards for each contribution..."
echo ""

for i in 1 2 3; do
    eval TXID=\$TXID$i
    if [ ! -z "$TXID" ]; then
        STATUS=$($CLI getcontributionstatus "$TXID" 2>&1)
        CONTRIB_TYPE=$(echo "$STATUS" | grep -oE '(?<="contrib_type"[: ]")[A-Z_]+' | head -1)
        BONUS_LEVEL=$(echo "$STATUS" | grep -oE '(?<="bonus_level"[: ])[0-9]+' | head -1)
        REWARD=$(echo "$STATUS" | grep -oE '(?<="reward"[: ])[0-9.]+' | head -1)
        
        echo "Contribution $i ($CONTRIB_TYPE):"
        echo "  TXID: $TXID"
        echo "  Bonus Level: $BONUS_LEVEL"
        echo "  Reward: $REWARD FC"
        
        # Calculate expected multiplier
        case $BONUS_LEVEL in
            0) MULTIPLIER="1.00x (BONUS_NONE)" ;;
            1) MULTIPLIER="1.05x (BONUS_LOW)" ;;
            2) MULTIPLIER="1.10x (BONUS_MEDIUM)" ;;
            3) MULTIPLIER="1.15x (BONUS_HIGH)" ;;
            4) 
                if [ "$CONTRIB_TYPE" = "AI_VALIDATION" ]; then
                    MULTIPLIER="1.25x (BONUS_CRITICAL - AI_VALIDATION)"
                else
                    MULTIPLIER="1.20x (BONUS_CRITICAL)"
                fi
                ;;
            *) MULTIPLIER="Unknown" ;;
        esac
        echo "  Multiplier: $MULTIPLIER"
        echo ""
    fi
done

# Check block rewards
echo "Checking block reward with contributions..."
BLOCK_AFTER=$($CLI getblockcount)
echo "Block height after: $BLOCK_AFTER"
echo ""

# Get latest block info
LATEST_BLOCK=$($CLI getblockcount)
BLOCK_HASH=$($CLI getblockhash $LATEST_BLOCK)
BLOCK_INFO=$($CLI getblock "$BLOCK_HASH" 2>&1)
echo "Latest block info:"
echo "  Height: $LATEST_BLOCK"
echo "  Hash: $BLOCK_HASH"
echo ""

echo "========================================="
echo "Reward Calculation Test Complete!"
echo "========================================="
echo ""
echo "Note: Actual reward calculation happens during:"
echo "  - Block validation (when contribution is included)"
echo "  - Block reward distribution (based on bonus multipliers)"
echo ""

