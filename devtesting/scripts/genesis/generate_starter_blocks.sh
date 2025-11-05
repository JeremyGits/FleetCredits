#!/bin/bash
# Generate starter blocks for Fleet Credits testnet
# Run this after the daemon starts to create initial blockchain state

set -e

RPC_USER="${RPC_USER:-test}"
RPC_PASS="${RPC_PASS:-test}"
NUM_BLOCKS="${NUM_BLOCKS:-100}"

echo "=== Generating $NUM_BLOCKS starter blocks for Fleet Credits testnet ==="

# Get a new address
ADDR=$(./fleetcredits-cli -testnet -rpcuser="$RPC_USER" -rpcpassword="$RPC_PASS" getnewaddress 2>&1 | tail -1)
echo "Mining to address: $ADDR"

# Generate blocks
echo "Generating blocks (this may take a moment)..."
BLOCKHASHES=$(./fleetcredits-cli -testnet -rpcuser="$RPC_USER" -rpcpassword="$RPC_PASS" generatetoaddress "$NUM_BLOCKS" "$ADDR" 2>&1)

if [ $? -eq 0 ]; then
    BLOCK_COUNT=$(./fleetcredits-cli -testnet -rpcuser="$RPC_USER" -rpcpassword="$RPC_PASS" getblockcount 2>&1)
    echo "✅ Successfully generated blocks!"
    echo "Current block height: $BLOCK_COUNT"
    echo "First block hash: $(echo "$BLOCKHASHES" | head -1)"
else
    echo "❌ Failed to generate blocks"
    exit 1
fi

