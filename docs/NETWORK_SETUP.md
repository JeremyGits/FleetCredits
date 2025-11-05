# Fleet Credits Network Setup Guide

## Overview

Fleet Credits is a **fresh blockchain network** starting from scratch. Unlike forks that inherit old chain data, Fleet Credits has:
- **Unique magic bytes** (`0xFC 0x43 0x52 0x45` for Mainnet) to ensure network isolation
- **No seed nodes** initially - nodes must be manually connected
- **New genesis block** with current timestamp (November 4, 2025)
- **Properly mined genesis blocks** with valid Proof-of-Work

## Network Modes

### Mainnet (Production Network)
- **P2P Port**: 22556
- **RPC Port**: 22555
- **Magic Bytes**: `0xFC 0x43 0x52 0x45` (FCRE)
- **Genesis Block**: Properly mined with valid PoW
- **Status**: Ready for launch

### Testnet (Testing Network)
- **P2P Port**: 44556
- **RPC Port**: 44555
- **Magic Bytes**: `0xFC 0x54 0x45 0x53` (FTES)
- **Genesis Block**: Properly mined with valid PoW
- **Status**: Ready for testing

### Regtest (Local Development)
- **P2P Port**: 18444
- **RPC Port**: 18332
- **Magic Bytes**: `0xfa 0xbf 0xb5 0xda`
- **Genesis Block**: Properly mined with valid PoW
- **Status**: Ready for local testing

## Starting a Fresh Network

### Step 1: Clear Old Blockchain Data

If you have old blockchain data from Dogecoin or previous Fleet Credits runs, **you must clear it**:

```bash
# Stop the Fleet Credits client first
# Then remove old chain data:

# For Mainnet/Testnet:
rm -rf ~/.fleetcredits/blocks/*
rm -rf ~/.fleetcredits/chainstate/*
# OR if using custom datadir:
rm -rf ./test-data/blocks/*
rm -rf ./test-data/chainstate/*

# Keep wallet.dat if you want to keep your wallet
# (or backup it first: cp wallet.dat wallet.dat.backup)
```

### Step 2: Start the Client

**For Mainnet:**
```bash
# Binary location: ./src/fleetcreditsd (after compilation) or ./bin/fleetcreditsd (after make install)
./src/fleetcreditsd -printtoconsole
# OR with custom datadir:
./src/fleetcreditsd -datadir=./test-data -printtoconsole
```

**For Testnet:**
```bash
./src/fleetcreditsd -testnet -printtoconsole
```

**For Regtest (Local Testing):**
```bash
./src/fleetcreditsd -regtest -printtoconsole
```

**For Qt GUI:**
```bash
./src/fleetcredits-qt -printtoconsole
# OR:
./src/fleetcredits-qt -testnet -printtoconsole
```

**Note:** If you ran `make install`, binaries will be in `./bin/` instead of `./src/`. Use `find . -name "fleetcreditsd"` to locate your binary.

### Step 3: Verify Network Isolation

Check that you're on the Fleet Credits network:

```bash
# Using RPC:
./bin/fleetcredits-cli getblockchaininfo | grep chain

# Should show:
# "chain": "main" (or "test" for testnet, "regtest" for regtest)

# Check magic bytes match:
# Mainnet: 0xFC 0x43 0x52 0x45
# Testnet: 0xFC 0x54 0x45 0x53
```

### Step 4: Connect Nodes Manually

Since there are no seed nodes initially, nodes must be connected manually:

**Option 1: Using `-addnode` flag:**
```bash
./bin/fleetcreditsd -addnode=IP_ADDRESS:22556
```

**Option 2: Using `-connect` flag (connect only to specified nodes):**
```bash
./bin/fleetcreditsd -connect=IP_ADDRESS:22556
```

**Option 3: Using RPC after startup:**
```bash
./bin/fleetcredits-cli addnode "IP_ADDRESS:22556" "add"
```

**Option 4: In Qt GUI:**
- Go to Settings → Network
- Add node manually: `IP_ADDRESS:22556`

## Network Discovery

### Current Status
- **No DNS seeds** - Fleet Credits starts with no seed nodes
- **No fixed seeds** - Network discovery disabled by default
- **Manual connections required** - Nodes must be manually connected

### Future Plans
When the network grows, we can add:
- DNS seed nodes (e.g., `seed.fleetcredits.org`)
- Fixed seed nodes (hardcoded IP addresses)
- Automatic peer discovery

## Troubleshooting

### "Network activity disabled"
This means the client is in regtest mode or connections are disabled. Check:
- Are you using `-regtest` flag? (Regtest disables network)
- Check `fleetcredits.conf` for `connect=0` or `noconnect=1`

### "Connecting to peers..." but no connections
- **Expected behavior** - No seed nodes exist yet
- Manually connect to other nodes using `-addnode` or `addnode` RPC

### "1 year and 44 weeks behind"
This indicates old blockchain data is being loaded. **Solution:**
1. Stop the client
2. Clear `blocks/` and `chainstate/` directories
3. Restart the client

### Wrong genesis block hash
Make sure you're using the latest Fleet Credits code with properly mined genesis blocks. The genesis block should validate with `CheckProofOfWork`.

## Network Architecture

Fleet Credits uses a **peer-to-peer network** where:
- Nodes connect directly to each other (no central server)
- Each node maintains its own copy of the blockchain
- Transactions and blocks are broadcast to all connected peers
- Consensus is reached through Proof-of-Work mining

## Next Steps

1. **For Development**: Use `-regtest` mode for local testing
2. **For Testing**: Use `-testnet` mode with other testers
3. **For Production**: Use Mainnet once network is established

## Security Notes

- **Never share your wallet.dat file** - it contains private keys
- **Verify you're on the correct network** - check magic bytes and genesis hash
- **Use testnet for testing** - don't use real funds on testnet
- **Firewall rules**: Ensure ports 22556 (Mainnet) or 44556 (Testnet) are open if you want incoming connections

