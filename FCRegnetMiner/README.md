# Fleet Credits Regtest Miner

A simple mining tool for Fleet Credits regtest networks. This tool connects to your local Fleet Credits node and mines blocks on demand.

## Features

- ✅ Connect to local Fleet Credits RPC
- ✅ Mine blocks on demand
- ✅ Show mining statistics
- ✅ Auto-mining mode (continuous block generation)
- ✅ Simple CLI interface

## Why This Exists

While you can use `fleetcredits-cli -regtest generate 1` directly, this tool provides:
- Better UX for mining operations
- Mining statistics and monitoring
- Auto-mining capabilities
- Future: GUI interface for mining

## Understanding Regtest Mining

**New to regtest mining?** See [`REGTEST_MINING_EXPLAINED.md`](REGTEST_MINING_EXPLAINED.md) for answers to:
- Why validation "stops" when you stop mining
- Whether this is real Scrypt mining
- How regtest differs from mainnet
- Why your current setup is perfect for testing

## Requirements

- Fleet Credits daemon running in regtest mode
- RPC access enabled (default: port 18332 for regtest)
- Python 3.6+ (for Python version) OR C++ compiler (for C++ version)

## Quick Start

### Python Version (Recommended for Quick Testing)

```bash
cd FCRegnetMiner
python3 fc_miner.py --help
python3 fc_miner.py mine 10  # Mine 10 blocks
python3 fc_miner.py auto      # Auto-mine blocks continuously
```

### C++ Version (For Integration)

```bash
cd FCRegnetMiner
make
./fc_miner --help
./fc_miner mine 10
./fc_miner auto
```

## Usage

### Mine Specific Number of Blocks

```bash
# Python
python3 fc_miner.py mine 5

# C++
./fc_miner mine 5
```

### Auto-Mining (Continuous)

```bash
# Python
python3 fc_miner.py auto

# C++
./fc_miner auto
```

### Connect to Custom RPC (Local or Remote)

```bash
# Local node
python3 fc_miner.py --rpc-url http://127.0.0.1:18332 --rpc-user user --rpc-password pass mine 10

# Remote node (mainnet)
python3 fc_miner.py --rpc-url http://96.126.123.195:22555 --rpc-user user --rpc-password pass mine 10

# Remote node (testnet)
python3 fc_miner.py --rpc-url http://96.126.123.195:44555 --rpc-user user --rpc-password pass mine 10
```

See [REMOTE_CONNECTION.md](REMOTE_CONNECTION.md) for detailed remote connection setup.

## Configuration

The miner reads RPC credentials from:
1. Command line arguments (recommended)
2. Default: `http://127.0.0.1:18332` (regtest default)

### Remote Connection

You can connect to remote Fleet Credits nodes! See [REMOTE_CONNECTION.md](REMOTE_CONNECTION.md) for setup instructions.

**Quick remote example:**
```bash
python3 fc_miner.py --rpc-url http://96.126.123.195:22555 --rpc-user user --rpc-password pass mine 10
```

## Future Enhancements

- [ ] GUI interface (Qt)
- [ ] Mining pool support
- [ ] Statistics dashboard
- [ ] Multi-node mining
- [ ] Contribution-aware mining (prioritize blocks with contributions)

## Note

This is specifically for **regtest** networks where:
- Blocks are instant (minimal difficulty)
- No real PoW competition
- Perfect for development/testing

For mainnet/testnet, use standard Scrypt miners (cgminer, cpuminer, etc.).

