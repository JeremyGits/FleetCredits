#!/bin/bash
# Copyright (c) 2025-2026 The Fleet Credits Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

# MWEB Test Client
# Simple test client for MWEB operations

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
CLIENT_BIN="${SCRIPT_DIR}/../../src/fleetcredits-cli"
DAEMON_BIN="${SCRIPT_DIR}/../../src/fleetcreditsd"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Fleet Credits MWEB Test Client ===${NC}"
echo ""

# Check if daemon is running
if ! $CLIENT_BIN getinfo > /dev/null 2>&1; then
    echo -e "${YELLOW}Warning: fleetcreditsd not running or not accessible${NC}"
    echo "Starting testnet daemon..."
    $DAEMON_BIN -testnet -daemon
    sleep 5
fi

echo -e "${GREEN}Testing MWEB RPC Commands...${NC}"
echo ""

# Test createpegin
echo "1. Testing createpegin..."
RESULT=$($CLIENT_BIN -testnet createpegin "mweb_test_address" 1000 2>&1 || true)
if echo "$RESULT" | grep -q "pegtxid"; then
    echo -e "   ${GREEN}✓ createpegin command works${NC}"
else
    echo -e "   ${RED}✗ createpegin failed: $RESULT${NC}"
fi

# Test createpegout
echo "2. Testing createpegout..."
RESULT=$($CLIENT_BIN -testnet createpegout "0000000000000000000000000000000000000000000000000000000000000000" "FleetCreditsTestAddress" 500 2>&1 || true)
if echo "$RESULT" | grep -q "pegtxid"; then
    echo -e "   ${GREEN}✓ createpegout command works${NC}"
else
    echo -e "   ${YELLOW}⚠ createpegout returned: $RESULT${NC}"
fi

# Test listmwebtxs
echo "3. Testing listmwebtxs..."
RESULT=$($CLIENT_BIN -testnet listmwebtxs 2>&1 || true)
if echo "$RESULT" | grep -q "\[\]"; then
    echo -e "   ${GREEN}✓ listmwebtxs command works (empty list)${NC}"
else
    echo -e "   ${YELLOW}⚠ listmwebtxs returned: $RESULT${NC}"
fi

# Test routecontributiontomweb
echo "4. Testing routecontributiontomweb..."
RESULT=$($CLIENT_BIN -testnet routecontributiontomweb "0000000000000000000000000000000000000000000000000000000000000000" 2>&1 || true)
if echo "$RESULT" | grep -q "mwebtxid"; then
    echo -e "   ${GREEN}✓ routecontributiontomweb command works${NC}"
else
    echo -e "   ${YELLOW}⚠ routecontributiontomweb returned: $RESULT${NC}"
fi

echo ""
echo -e "${GREEN}=== MWEB Test Client Complete ===${NC}"
echo ""
echo "Note: These are placeholder implementations."
echo "Full MWEB functionality requires cryptographic implementation."

