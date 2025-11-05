# MWEB Testing Guide
# Copyright (c) 2025-2026 The Fleet Credits Core developers

## MWEB Testing Infrastructure

MWEB functionality is **fully integrated** into the core Fleet Credits client. Testing can be done through:

1. **Unit Tests** (integrated into test suite)
2. **RPC Test Clients** (Python/Bash scripts)
3. **Manual RPC Testing** (via fleetcredits-cli)

## Quick Start

### 1. Unit Tests

```bash
cd fleetcredits/src
make test/test_fleetcredits
./test/test_fleetcredits --run_test=mweb_tests
```

### 2. RPC Tests

```bash
# Start daemon
fleetcreditsd -testnet -daemon

# Run Python test client
python3 devtesting/scripts/mweb/mweb_test_client.py

# Or run bash script
./devtesting/scripts/mweb/mweb_test_client.sh
```

### 3. Manual Testing

```bash
# Test peg-in
fleetcredits-cli -testnet createpegin "mweb_address" 1000

# Test peg-out
fleetcredits-cli -testnet createpegout "commitment_hex" "address" 500

# List MWEB transactions
fleetcredits-cli -testnet listmwebtxs

# Route contribution
fleetcredits-cli -testnet routecontributiontomweb "txid"
```

## Test Files

- `src/test/mweb_test.h` - Test utilities header
- `src/test/mweb_test.cpp` - Test utilities implementation
- `src/test/mweb_tests.cpp` - Unit test suite
- `devtesting/scripts/mweb/mweb_test_client.py` - Python RPC test client
- `devtesting/scripts/mweb/mweb_test_client.sh` - Bash RPC test client

## Note

Current implementations use placeholder cryptography. Full MWEB functionality requires cryptographic primitives (Pedersen commitments, Bulletproofs) to be implemented.

