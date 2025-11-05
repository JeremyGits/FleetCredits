#!/bin/bash
# Test script for Fleet Credits Contribution System using real GitHub contribution
# Tests with: https://github.com/JeremyGits/projectFC/blob/main/thought.md

set -e

CLI="./src/fleetcredits-cli -regtest"
REGTEST_NET="regtest"

echo "========================================="
echo "Fleet Credits GitHub Contribution Test"
echo "Using: github.com/JeremyGits/projectFC"
echo "========================================="
echo ""

# GitHub repository details
REPO_OWNER="JeremyGits"
REPO_NAME="projectFC"
REPO_URL="https://github.com/JeremyGits/projectFC"
FILE_PATH="thought.md"
BRANCH="main"

# Step 1: Get a new address for testing
echo "Step 1: Getting test address..."
ADDRESS=$($CLI getnewaddress "github_test")
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

# Step 4: Create proof data for GitHub contribution
echo "Step 4: Creating contribution transaction..."
echo "  Repository: $REPO_OWNER/$REPO_NAME"
echo "  File: $FILE_PATH"
echo "  Branch: $BRANCH"
echo ""

# Use the actual GitHub commit hash for this file
# Commit: a86f5bbfe8d9f4cf0df45eae1fa7f345bacec86f (from git ls-remote)
COMMIT_HASH="a86f5bbfe8d9f4cf0df45eae1fa7f345bacec86f"
PROOF_HASH="$COMMIT_HASH"
echo "  Commit hash: $COMMIT_HASH"
echo "  Proof hash: $PROOF_HASH"
echo ""

# Create metadata JSON with GitHub details
METADATA=$(cat <<EOF
{
  "repository": "$REPO_OWNER/$REPO_NAME",
  "repository_url": "$REPO_URL",
  "file_path": "$FILE_PATH",
  "branch": "$BRANCH",
  "commit_hash": "a86f5bbfe8d9f4cf0df45eae1fa7f345bacec86f",
  "commit_message": "Add thought.md - Fleet Credits design considerations",
  "contributor": "$REPO_OWNER",
  "lines_added": 500,
  "lines_changed": 0,
  "lines_deleted": 0,
  "contribution_type": "documentation",
  "github_url": "$REPO_URL/blob/$BRANCH/$FILE_PATH"
}
EOF
)

echo "  Metadata:"
echo "$METADATA" | jq '.' 2>/dev/null || echo "$METADATA"
echo ""

# Step 5: Submit the contribution
echo "Step 5: Submitting CODE_CONTRIBUTION..."
echo "  Type: CODE_CONTRIBUTION (documentation/code)"
echo "  Proof: $PROOF_HASH"
echo ""

TXID=$($CLI submitcontribution "CODE_CONTRIBUTION" "$PROOF_HASH" "$METADATA" 2>&1)
echo "  Transaction ID: $TXID"
echo ""

# Step 6: Wait for confirmation
echo "Step 6: Mining a block to confirm transaction..."
CONFIRM_BLOCK=$($CLI generatetoaddress 1 "$ADDRESS")
echo "  Block mined: $CONFIRM_BLOCK"
echo ""

# Step 7: Check contribution status
echo "Step 7: Checking contribution status..."
if [[ $TXID =~ ^[a-f0-9]{64}$ ]]; then
    STATUS=$($CLI getcontributionstatus "$TXID" 2>&1)
    echo "  Status details:"
    echo "$STATUS" | jq '.' 2>/dev/null || echo "$STATUS"
else
    echo "  Warning: Invalid TXID format"
    echo "  Raw output: $TXID"
    echo ""
    echo "  Trying to extract TXID from output..."
    # Try to extract JSON txid if present
    EXTRACTED_TXID=$(echo "$TXID" | grep -oE '(?<="txid"[: ]")[a-f0-9]{64}' | head -1)
    if [ ! -z "$EXTRACTED_TXID" ]; then
        echo "  Extracted TXID: $EXTRACTED_TXID"
        STATUS=$($CLI getcontributionstatus "$EXTRACTED_TXID" 2>&1)
        echo "$STATUS" | jq '.' 2>/dev/null || echo "$STATUS"
    fi
fi
echo ""

# Step 8: List all contributions
echo "Step 8: Listing all contributions..."
CONTRIBS=$($CLI listcontributions 2>&1)
echo "  Contributions found:"
echo "$CONTRIBS" | jq '.' 2>/dev/null | head -30 || echo "$CONTRIBS" | head -30
echo ""

# Step 9: Verify the contribution appears in the list
echo "Step 9: Verifying contribution..."
if [[ $TXID =~ ^[a-f0-9]{64}$ ]]; then
    if echo "$CONTRIBS" | grep -q "$TXID"; then
        echo "  ✅ Contribution found in list!"
    else
        echo "  ⚠️  Contribution not found in list (may need more confirmations)"
    fi
else
    echo "  ⚠️  Cannot verify (invalid TXID format)"
fi
echo ""

echo "========================================="
echo "GitHub Contribution Test Complete!"
echo "========================================="
echo ""
echo "Summary:"
echo "  Repository: $REPO_URL"
echo "  File: $FILE_PATH"
echo "  Contribution Type: CODE_CONTRIBUTION"
echo "  Transaction ID: $TXID"
echo ""
echo "Next steps:"
echo "  1. Wait for more confirmations (12+ for verified status)"
echo "  2. Oracle verification (off-chain)"
echo "  3. Reward calculation based on bonus level"
echo ""

