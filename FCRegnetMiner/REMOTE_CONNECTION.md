# Connecting to Remote Fleet Credits Node

You can connect the miner to a remote Fleet Credits node instead of a local one.

## Quick Example

```bash
# Connect to remote node at 96.126.123.195
python3 fc_miner.py --rpc-url http://96.126.123.195:22555 mine 10

# With RPC authentication (if configured)
python3 fc_miner.py --rpc-url http://96.126.123.195:22555 --rpc-user myuser --rpc-password mypass mine 10
```

## Network Ports

| Network | RPC Port | P2P Port |
|---------|----------|----------|
| **Mainnet** | 22555 | 22556 |
| **Testnet** | 44555 | 44556 |
| **Regtest** | 18332 | 18444 |

## Remote Server Setup

### 1. Configure RPC on Remote Server

On your remote server (`96.126.123.195`), edit `fleetcredits.conf`:

```conf
# Enable RPC server
server=1

# RPC port (mainnet: 22555, testnet: 44555, regtest: 18332)
rpcport=22555

# RPC authentication (recommended for remote access)
rpcuser=yourusername
rpcpassword=yourpassword

# Allow RPC from specific IPs (for security)
rpcallowip=YOUR_LOCAL_IP
# Or allow from anywhere (less secure, only for testing)
# rpcallowip=0.0.0.0/0

# RPC bind address (0.0.0.0 = listen on all interfaces)
rpcbind=0.0.0.0
```

### 2. Firewall Configuration

Make sure the RPC port is open on your remote server:

```bash
# Ubuntu/Debian (ufw)
sudo ufw allow 22555/tcp

# Or iptables
sudo iptables -A INPUT -p tcp --dport 22555 -j ACCEPT
```

### 3. Connect from Miner

```bash
# Mainnet
python3 fc_miner.py --rpc-url http://96.126.123.195:22555 --rpc-user your_username --rpc-password your_secure_password mine 10

# Testnet
python3 fc_miner.py --rpc-url http://96.126.123.195:44555 --rpc-user your_username --rpc-password your_secure_password mine 10
```

## Security Considerations

### ⚠️ Important Security Notes

1. **Never expose RPC without authentication** - Always use `rpcuser` and `rpcpassword`
2. **Use `rpcallowip`** - Restrict RPC access to specific IPs
3. **Use HTTPS/TLS** - For production, consider setting up a reverse proxy with SSL
4. **Firewall rules** - Only open RPC port to trusted IPs

### Recommended Setup

```conf
# fleetcredits.conf on remote server
server=1
rpcport=22555
rpcuser=miner_user
rpcpassword=STRONG_PASSWORD_HERE
rpcallowip=YOUR_HOME_IP/32  # Only allow your IP
rpcbind=127.0.0.1  # Only bind to localhost, use SSH tunnel instead
```

### Using SSH Tunnel (More Secure)

Instead of exposing RPC directly, use SSH tunnel:

```bash
# Create SSH tunnel (run this first)
ssh -L 22555:localhost:22555 user@96.126.123.195

# Then connect miner to localhost (tunneled)
python3 fc_miner.py --rpc-url http://127.0.0.1:22555 --rpc-user your_username --rpc-password your_secure_password mine 10
```

## Testing Connection

Test if you can connect:

```bash
# Test connection
python3 fc_miner.py --rpc-url http://96.126.123.195:22555 status

# If it works, you'll see:
# 🔗 Connected to Fleet Credits node
#    Network: main
#    Blocks: 1234
#    Difficulty: 1.23
```

## Troubleshooting

### "Connection refused"

**Problem:** RPC port not open or daemon not running.

**Solutions:**
1. Check if daemon is running on remote server
2. Verify firewall allows RPC port
3. Check `rpcbind` setting in `fleetcredits.conf`

### "RPC Error: RPC_IN_WARMUP"

**Problem:** Daemon is still starting up.

**Solution:** Wait a few seconds and try again.

### "RPC Error: RPC_WALLET_UNLOCK_NEEDED"

**Problem:** Wallet is encrypted and needs unlocking.

**Solution:** Unlock wallet on remote server:
```bash
# On remote server
fleetcredits-cli walletpassphrase "your_passphrase" 60
```

### "Connection timeout"

**Problem:** Network issue or firewall blocking.

**Solutions:**
1. Check if you can reach the server: `ping 96.126.123.195`
2. Test port: `telnet 96.126.123.195 22555`
3. Verify firewall rules

## Example: Full Remote Mining Setup

```bash
# 1. On remote server (96.126.123.195)
# Edit ~/.fleetcredits/fleetcredits.conf:
server=1
rpcport=22555
rpcuser=miner
rpcpassword=secure_password_123
rpcallowip=YOUR_IP_HERE
rpcbind=0.0.0.0

# Restart daemon
fleetcredits-cli stop
fleetcreditsd -daemon

# 2. On your local machine (miner)
cd FCRegnetMiner
python3 fc_miner.py \
  --rpc-url http://96.126.123.195:22555 \
  --rpc-user miner \
  --rpc-password secure_password_123 \
  mine 10
```

## Auto-Mining on Remote Node

```bash
# Auto-mine blocks on remote node
python3 fc_miner.py \
  --rpc-url http://96.126.123.195:22555 \
  --rpc-user miner \
  --rpc-password secure_password_123 \
  auto --interval 60
```

This will mine blocks continuously, sending rewards to the wallet on the remote server.

