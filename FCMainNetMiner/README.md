# Fleet Credits Mainnet Miner

A mining tool for Fleet Credits mainnet. This tool connects to your Fleet Credits mainnet node and mines blocks.

## ⚠️ Important Notes

**This is REAL mining on mainnet:**
- Real Scrypt Proof-of-Work
- Real difficulty (not instant like regtest)
- Real block rewards
- Real network competition

**Mining on mainnet requires:**
- Significant computational power (GPU/ASIC recommended)
- Time (blocks may take minutes/hours depending on difficulty)
- Properly configured node with RPC access

## Features

- ✅ Connect to local or remote Fleet Credits mainnet RPC
- ✅ Mine blocks on mainnet
- ✅ Show mining statistics
- ✅ Auto-mining mode (continuous block generation)
- ✅ Simple CLI interface

## Requirements

- Fleet Credits daemon running on mainnet
- RPC access enabled (default: port 22555 for mainnet)
- Python 3.6+
- RPC credentials configured (if RPC auth is enabled)

## Quick Start

### Local Node

```bash
cd FCMainNetMiner
python3 fc_miner.py --help
python3 fc_miner.py mine 1    # Mine 1 block (may take time!)
python3 fc_miner.py status     # Check status
```

### With RPC Credentials

```bash
python3 fc_miner.py \
  --rpc-url http://127.0.0.1:22555 \
  --rpc-user YOUR_USER \
  --rpc-password YOUR_PASS \
  mine 1
```

### Remote Node

```bash
python3 fc_miner.py \
  --rpc-url http://96.126.123.195:22555 \
  --rpc-user YOUR_USER \
  --rpc-password YOUR_PASS \
  mine 1
```

## Usage

### Mine Specific Number of Blocks

```bash
# Mine 1 block (may take significant time on mainnet!)
python3 fc_miner.py mine 1

# Mine 10 blocks
python3 fc_miner.py mine 10
```

### Auto-Mining (Continuous)

```bash
# Auto-mine with 10 second intervals (default - faster operation)
python3 fc_miner.py auto

# Auto-mine with custom interval (faster = lower number)
python3 fc_miner.py auto --interval 5    # Very fast (5 seconds)
python3 fc_miner.py auto --interval 10   # Fast (10 seconds, default)
python3 fc_miner.py auto --interval 30    # Moderate (30 seconds)
python3 fc_miner.py auto --interval 120   # Slower (2 minutes)

# Auto-mine up to 100 blocks
python3 fc_miner.py auto --max-blocks 100
```

### Check Status

```bash
python3 fc_miner.py status
```

### Verify Setup

```bash
# Verify RPC connection and mining configuration
python3 fc_miner.py verify
```

This will check:
- ✅ Blockchain connection
- ✅ Wallet accessibility
- ✅ Mining info availability
- ✅ Generate RPC call functionality

**Use this first if you're having issues!**

## Configuration

### RPC Settings

The miner defaults to:
- **RPC URL:** `http://127.0.0.1:22555` (mainnet default)
- **RPC User:** None (if RPC auth disabled)
- **RPC Password:** None (if RPC auth disabled)

### fleetcredits.conf Example

```ini
# Mainnet Configuration
server=1
rpcuser=username-here
rpcpassword=mypassword?!
rpcallowip=127.0.0.1
rpcport=22555

# Network
listen=1
bind=0.0.0.0
port=22556
```

## Mainnet vs Regtest

| Feature | Mainnet | Regtest |
|---------|---------|---------|
| **Difficulty** | Real (high) | Minimal (instant) |
| **Block Time** | Minutes/Hours | Seconds |
| **RPC Port** | 22555 | 18332 |
| **Network** | Production | Local testing |
| **Rewards** | Real FC | Test FC |

## Mining Performance

**Mainnet mining is REAL Scrypt mining:**
- Requires significant hash power
- Block times depend on network difficulty
- Competition with other miners
- May take hours to find a block

**For serious mainnet mining:**
- Use dedicated mining hardware (GPU/ASIC)
- Consider joining a mining pool
- Monitor difficulty and network hash rate

## Troubleshooting

### Verify Your Setup First

**Always run `verify` first if you're having issues:**

```bash
python3 fc_miner.py verify
```

This will diagnose:
- RPC connection problems
- Wallet issues
- Mining configuration problems
- RPC call format issues

### Connection Failed

```
❌ Failed to connect to Fleet Credits node
```

**Solutions:**
1. Ensure daemon is running: `fleetcredits-cli getblockchaininfo`
2. Check RPC port (22555 for mainnet)
3. Verify RPC credentials in `fleetcredits.conf`
4. Check firewall settings

### Mining Takes Too Long

This is normal on mainnet! Real mining requires:
- Computational power
- Time (difficulty-based)
- Network competition

Consider:
- Using more powerful hardware (lets hit it)
- Joining a mining pool (not until stratus though, sorry dudes)
- Using regtest for testing

## Examples

### Local Mining

```bash
# Check status first
python3 fc_miner.py status

# Mine 1 block
python3 fc_miner.py mine 1

# Auto-mine with 2 minute intervals
python3 fc_miner.py auto --interval 120
```

### Remote Mining

```bash
# Connect to remote mainnet node
python3 fc_miner.py \
  --rpc-url http://96.126.123.195:22555 \
  --rpc-user your-username \
  --rpc-password mypassword?! \
  mine 1
```
