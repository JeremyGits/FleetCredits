# Fleet Credits Blockchain Network Architecture

## How Blockchain Networks Work

### 1. **Peer-to-Peer Network (Like BitTorrent/iMesh)**

Yes, blockchain networks are **peer-to-peer** just like BitTorrent! Here's how:

- **Each node runs a daemon** (`fleetcreditsd`) that:
  - Connects to other nodes on the network
  - Maintains a copy of the blockchain
  - Validates transactions and blocks
  - Broadcasts new transactions/blocks to peers

- **No central server** - The network is decentralized:
  - Nodes discover each other through:
    - **DNS seeds** (hardcoded IP addresses)
    - **Peer exchange** (existing peers tell you about other peers)
    - **Manual connections** (`-addnode` or `-connect` flags)

- **Network isolation** via:
  - **Magic bytes** (`pchMessageStart`) - Unique 4-byte identifier
  - **Port numbers** (P2P port 22556 for mainnet)
  - **Chain parameters** (genesis block hash, consensus rules)

### 2. **Why You're Seeing Old Dogecoin Data**

The client is trying to sync Dogecoin's blockchain because:

1. **Old seed nodes** - Still pointing to Dogecoin nodes
2. **Same message start bytes** - Network thinks it's Dogecoin
3. **Same port** - Could connect to Dogecoin nodes
4. **Old checkpoints** - References to Dogecoin blocks

### 3. **Fleet Credits Network Setup**

We need to create a **completely isolated network**:

```
┌─────────────────────────────────────────────────────────┐
│            Fleet Credits Network                        │
│                                                         │
│  ┌──────────┐    ┌──────────┐    ┌──────────┐         │
│  │  Node 1  │◄──►│  Node 2  │◄──►│  Node 3  │         │
│  │(Daemon)  │    │(Daemon)  │    │(Daemon)  │         │
│  └──────────┘    └──────────┘    └──────────┘         │
│       │                │                │              │
│       └────────────────┼────────────────┘              │
│                        │                               │
│                  Blockchain Data                        │
│                  (Shared across all)                    │
└─────────────────────────────────────────────────────────┘
```

### 4. **Starting the Fleet Credits Network**

#### Option A: **Solo Development (Regtest)**
```bash
# Run in regtest mode - isolated, no network needed
./fleetcreditsd -regtest -daemon

# Generate blocks locally
./fleetcredits-cli -regtest generate 100
```

#### Option B: **Private Testnet**
```bash
# Start node 1
./fleetcreditsd -testnet -daemon -listen -bind=0.0.0.0:44556

# Start node 2, connect to node 1
./fleetcreditsd -testnet -daemon -addnode=192.168.1.100:44556

# Nodes will discover each other and form a network
```

#### Option C: **Public Mainnet (Future)**
```bash
# Once seed nodes are deployed:
./fleetcreditsd -daemon

# Nodes automatically discover via DNS seeds
```

### 5. **Network Discovery Process**

```
1. Client starts
   ↓
2. Try to connect to seed nodes (if enabled)
   ↓
3. If seeds fail, try manual connections (-addnode)
   ↓
4. Once connected to ANY peer:
   - Request peer addresses (getaddr)
   - Peers share their known peers
   - Try connecting to those peers
   ↓
5. Network grows organically
```

### 6. **Message Start Bytes (Magic Bytes)**

These are **critical** for network isolation:

```cpp
// Dogecoin uses:
pchMessageStart[0] = 0xc0;  // "Ð"
pchMessageStart[1] = 0xc0;  // "Ð"
pchMessageStart[2] = 0xc0;  // "Ð"
pchMessageStart[3] = 0xc0;  // "Ð"

// Fleet Credits MUST use DIFFERENT values:
pchMessageStart[0] = 0xFC;  // "F" for Fleet
pchMessageStart[1] = 0x43;  // "C" for Credits
pchMessageStart[2] = 0x52;  // "R" for Credits
pchMessageStart[3] = 0x45;  // "E" for Credits
```

**Why this matters:**
- Nodes reject messages without correct magic bytes
- Prevents connecting to wrong network
- Ensures network isolation

### 7. **Genesis Block**

The genesis block is the **first block** in the blockchain:

- **Unique hash** identifies Fleet Credits network
- **All nodes must agree** on genesis block
- **Different genesis = different network**

### 8. **Blockchain Data Storage**

The blockchain data is stored locally on each node:

```
~/.fleetcredits/
├── blocks/
│   ├── blk00000.dat  (Block data)
│   ├── blk00001.dat
│   └── ...
├── chainstate/       (UTXO database)
├── wallet.dat        (Your wallet)
└── fleetcredits.conf (Config file)
```

**Each node maintains its own copy** - no central database!

### 9. **Network Modes**

| Mode | Purpose | Network Isolation | Peer Discovery |
|------|---------|-------------------|----------------|
| **Mainnet** | Production | Via magic bytes + seeds | DNS seeds + P2P |
| **Testnet** | Testing | Via magic bytes + seeds | DNS seeds + P2P |
| **Regtest** | Development | Complete isolation | None (local only) |

### 10. **Setting Up Fleet Credits Network**

#### Step 1: Fix Chain Parameters
- Change message start bytes (✅ Done)
- Remove old Dogecoin seeds (✅ Done)
- Set unique genesis block (✅ Done)

#### Step 2: For Development (Regtest)
```bash
# Already isolated - just run:
./fleetcreditsd -regtest -daemon
```

#### Step 3: For Private Testnet
```bash
# Node 1 (First node):
./fleetcreditsd -testnet -daemon -listen -port=44556

# Node 2 (Connect to node 1):
./fleetcreditsd -testnet -daemon -addnode=IP_OF_NODE_1:44556

# Node 3 (Connect to either):
./fleetcreditsd -testnet -daemon -addnode=IP_OF_NODE_1:44556 -addnode=IP_OF_NODE_2:44556
```

#### Step 4: For Public Mainnet (Future)
```bash
# Deploy seed nodes first, then:
./fleetcreditsd -daemon

# Nodes will discover each other automatically
```

### 11. **Why "Connecting to Peers" Takes Time**

When you see "Connecting to peers...":

1. **No peers found** - Network is new, no seed nodes yet
2. **Firewall blocking** - Port 22556 not open
3. **Wrong network** - Trying to connect to Dogecoin nodes
4. **Regtest mode** - No network connections (by design)

**Solution for development:**
- Use `-regtest` mode (no network needed)
- Or manually connect nodes with `-addnode`
- Or start mining locally to create blocks

### 12. **Key Takeaways**

✅ **Blockchain = Peer-to-peer network** (like BitTorrent)
✅ **Each node runs a daemon** (fleetcreditsd)
✅ **No central server** - fully decentralized
✅ **Network isolation** via magic bytes + genesis block
✅ **For development** - use `-regtest` (isolated, fast)
✅ **For testing** - use `-testnet` with manual connections
✅ **For production** - deploy seed nodes first

### 13. **Next Steps**

1. ✅ Fix chain parameters (removing Dogecoin seeds, changing magic bytes)
2. ✅ Set up fresh genesis block
3. ✅ Test in regtest mode (isolated)
4. 🔄 Deploy testnet seed nodes (when ready)
5. 🔄 Deploy mainnet seed nodes (when ready)

---

**The blockchain runs on YOUR computer** (and every other node's computer). There's no "server" - it's a distributed network where everyone maintains the same ledger!

