# Fleet Credits Daemon Debugging Guide

## Problem: No Output When Running Daemon

If you run `./src/fleetcreditsd -regtest -printtoconsole` and see no output, here's how to diagnose and fix it.

## Common Issues

### 1. Wrong Binary Path

**Problem:** The daemon binary might be in `./bin/` not `./src/`

**Solution:**
```bash
# Check where the binary actually is:
find . -name "fleetcreditsd" -type f

# Usually it's in:
./bin/fleetcreditsd

# So run:
./bin/fleetcreditsd -regtest -printtoconsole
```

### 2. Daemon Exiting Immediately

**Problem:** The daemon crashes or exits before logging starts

**Solution - Check Exit Code:**
```bash
# Run and check exit code
./bin/fleetcreditsd -regtest -printtoconsole
echo "Exit code: $?"

# If exit code is non-zero, daemon failed
```

### 3. Check Debug Log

**Problem:** Output might be going to debug.log instead of console

**Solution:**
```bash
# Check if debug.log exists and has content
tail -f ~/.fleetcredits/regtest/debug.log

# Or with custom datadir:
tail -f ./test-data/regtest/debug.log
```

### 4. Binary Not Executable

**Problem:** Binary might not have execute permissions

**Solution:**
```bash
chmod +x ./bin/fleetcreditsd
./bin/fleetcreditsd -regtest -printtoconsole
```

## Proper Launch Commands

**Note:** Binary location depends on your build. After compilation, binaries are typically in `./src/`. After `make install`, they're in `./bin/`. Use `find . -name "fleetcreditsd"` to locate yours.

### Regtest Mode (Recommended for Testing)

```bash
# Start daemon in foreground with console output
# (Use ./src/fleetcreditsd if compiled but not installed, ./bin/fleetcreditsd if installed)
./src/fleetcreditsd -regtest -printtoconsole

# Or start in background and tail log
./src/fleetcreditsd -regtest -daemon
tail -f ~/.fleetcredits/regtest/debug.log
```

### Testnet Mode

```bash
# Start daemon with console output
./src/fleetcreditsd -testnet -printtoconsole

# Or background + log
./src/fleetcreditsd -testnet -daemon
tail -f ~/.fleetcredits/testnet3/debug.log
```

### Mainnet Mode

```bash
# Start daemon with console output
./src/fleetcreditsd -printtoconsole

# Or background + log
./src/fleetcreditsd -daemon
tail -f ~/.fleetcredits/debug.log
```

## Debugging Steps

### Step 1: Find Binary Location
```bash
# Find where the binary actually is
find . -name "fleetcreditsd" -type f -executable

# Usually it's in one of these locations:
# - ./src/fleetcreditsd (after compilation, before make install)
# - ./bin/fleetcreditsd (after make install)
# - ./windows-build/fleetcreditsd (Windows cross-compiled, only if actively building)
```

### Step 2: Verify Binary Exists
```bash
# If in src/ (most common after compilation):
ls -la ./src/fleetcreditsd

# Or if installed to bin/:
ls -la ./bin/fleetcreditsd
```

### Step 3: Test Version
```bash
# Use the path from Step 1
./src/fleetcreditsd -version
# Or: ./bin/fleetcreditsd -version
```

### Step 4: Run with Verbose Logging
```bash
# Use the correct path
./src/fleetcreditsd -regtest -printtoconsole -debug=all
# Should show lots of output
```

### Step 5: Check for Errors
```bash
# Run and capture stderr
./src/fleetcreditsd -regtest -printtoconsole 2>&1 | tee daemon-output.log

# Check what was printed
cat daemon-output.log
```

### Step 5: Check Data Directory
```bash
# Verify data directory exists
ls -la ~/.fleetcredits/regtest/

# Or check custom datadir
ls -la ./test-data/regtest/
```

## Expected Output

When daemon starts successfully, you should see:

```
Fleet Credits version [version number]
Initializing...
Loading blockchain...
Loading wallet...
Done loading
```

## Common Error Messages

### "Error: Specified data directory does not exist"
**Fix:** Create the directory or use `-datadir=` flag
```bash
mkdir -p ~/.fleetcredits/regtest
./bin/fleetcreditsd -regtest -printtoconsole
```

### "Error: couldn't connect to server"
**Fix:** Daemon isn't running. Start it first:
```bash
./bin/fleetcreditsd -regtest -printtoconsole &
./bin/fleetcredits-cli -regtest getblockchaininfo
```

### "Bind address already in use"
**Fix:** Another daemon is already running
```bash
# Find and kill existing daemon
pkill -f fleetcreditsd
# Wait a moment, then restart
./bin/fleetcreditsd -regtest -printtoconsole
```

## Quick Debug Script

Save this as `debug-daemon.sh`:

```bash
#!/bin/bash

echo "=== Fleet Credits Daemon Debug ==="
echo ""

echo "1. Checking binary location..."
if [ -f "./src/fleetcreditsd" ]; then
    echo "✓ Found: ./src/fleetcreditsd (compiled, not installed)"
    DAEMON="./src/fleetcreditsd"
elif [ -f "./bin/fleetcreditsd" ]; then
    echo "✓ Found: ./bin/fleetcreditsd (installed)"
    DAEMON="./bin/fleetcreditsd"
else
    echo "✗ Binary not found in src/ or bin/!"
    echo "Searching for binary..."
    DAEMON=$(find . -name "fleetcreditsd" -type f -executable 2>/dev/null | head -1)
    if [ -n "$DAEMON" ]; then
        echo "✓ Found: $DAEMON"
    else
        echo "✗ Binary not found anywhere!"
        exit 1
    fi
fi

echo ""
echo "2. Checking permissions..."
ls -la $DAEMON

echo ""
echo "3. Testing version..."
$DAEMON -version

echo ""
echo "4. Starting daemon with verbose output..."
$DAEMON -regtest -printtoconsole -debug=all 2>&1 | head -50
```

Run it:
```bash
chmod +x debug-daemon.sh
./debug-daemon.sh
```

## Still Not Working?

If you still see no output:

1. **Check if process is running:**
   ```bash
   ps aux | grep fleetcreditsd
   ```

2. **Check system logs:**
   ```bash
   dmesg | tail -20
   journalctl -xe | tail -20
   ```

3. **Try running from different directory:**
   ```bash
   cd /path/to/fleetcredits
   ./bin/fleetcreditsd -regtest -printtoconsole
   ```

4. **Check for missing dependencies:**
   ```bash
   ldd ./bin/fleetcreditsd
   ```

