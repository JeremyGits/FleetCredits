# Quick Start Guide

## Step 1: Start Fleet Credits Daemon

You need to start the Fleet Credits daemon **before** running the miner.

### Option A: Start Daemon in Foreground (Recommended for First Time)

```bash
cd /mnt/d/seriousprojects/FleetCredits/fleetcredits
./src/fleetcreditsd -regtest -mocktime=1776643200 -printtoconsole
```

**Note:** The `-mocktime=1776643200` flag is required for regtest because the Fleet Credits genesis block has a future timestamp (April 20, 2026).

Keep this terminal open. You'll see daemon output here.

### Option B: Start Daemon in Background

```bash
cd /mnt/d/seriousprojects/FleetCredits/fleetcredits
./src/fleetcreditsd -regtest -mocktime=1776643200 -daemon
```

Check if it's running:
```bash
./src/fleetcredits-cli -regtest getblockchaininfo
```

### Option C: Use Qt Client (Includes Daemon)

```bash
cd /mnt/d/seriousprojects/FleetCredits/fleetcredits
./src/qt/fleetcredits-qt -regtest -mocktime=1776643200
```

The Qt client automatically starts its own daemon. The `-mocktime=1776643200` flag is required for regtest.

## Step 2: Run the Miner

In a **new terminal**:

```bash
cd /mnt/d/seriousprojects/FleetCredits/FCRegnetMiner

# Install Python requests if needed
pip3 install requests

# Mine 10 blocks
python3 fc_miner.py mine 10

# Or auto-mine continuously
python3 fc_miner.py auto
```

## Troubleshooting

### "Connection refused" Error

**Problem:** Daemon isn't running or RPC isn't accessible.

**Solution:**
1. Make sure daemon is running with `-mocktime=1776643200` (see Step 1)
2. Check RPC port (default: 42068 for regtest)
3. If using custom RPC settings, use `--rpc-url` flag

### "RPC Error: Method not found"

**Problem:** Daemon might not be fully started yet.

**Solution:** Wait a few seconds after starting daemon, then try again.

### Finding the Daemon Binary

If `./src/fleetcreditsd` doesn't exist:

```bash
# Find the daemon
find /mnt/d/seriousprojects/FleetCredits/fleetcredits -name "fleetcreditsd" -type f

# Common locations:
# - ./src/fleetcreditsd (after compilation)
# - ./bin/fleetcreditsd (after make install)
```

## Example Workflow

```bash
# Terminal 1: Start daemon
cd /mnt/d/seriousprojects/FleetCredits/fleetcredits
./src/fleetcreditsd -regtest -mocktime=1776643200 -printtoconsole

# Terminal 2: Run miner
cd /mnt/d/seriousprojects/FleetCredits/FCRegnetMiner
python3 fc_miner.py mine 10
```

## RPC Configuration

By default, the miner connects to:
- **URL:** `http://127.0.0.1:42068` (regtest default)
- **Auth:** None (if RPC auth not configured)

If you've configured RPC authentication in `fleetcredits.conf`:

```bash
python3 fc_miner.py --rpc-user myuser --rpc-password mypass mine 10
```

## Next Steps

Once the miner is working:
- Try `python3 fc_miner.py auto` for continuous mining
- Check your balance: `python3 fc_miner.py status`
- Use `fleetcredits-cli -regtest getbalance` to verify rewards

