# Fleet Credits Core

<div align="center">

![Fleet Credits](share/pixmaps/fleetcredits256.svg)

**A decentralized cryptocurrency with MWEB privacy features and contribution-based rewards**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

</div>

---

## What is Fleet Credits?

**Fleet Credits (FC)** is an open-source cryptocurrency project built on proven blockchain technology. It is a **technical blockchain implementation** that combines:

- **Scrypt Proof-of-Work** consensus mechanism (similar to Dogecoin)
- **Mimblewimble Extension Blocks (MWEB)** for optional transaction privacy
- **Contribution-based reward system** that recognizes various types of value creation
- **Fixed block rewards** (10,000 FC per block) with 60-second block times

Fleet Credits is **not a political program, social movement, or organizational system**. It is purely a **technical cryptocurrency project** focused on blockchain innovation, privacy features, and recognizing contributions to open-source development.

### Key Features

- 🔒 **MWEB Privacy**: Optional transaction privacy using Mimblewimble Extension Blocks
- 🎯 **Contribution Rewards**: 8 types of contributions (code, education, creative work, etc.) with dynamic reward multipliers
- ⚡ **Fast Blocks**: 60-second block times for quick transaction confirmation
- 💰 **Fixed Rewards**: Predictable 10,000 FC block rewards without halving
- 🔐 **Scrypt PoW**: ASIC-resistant mining using Scrypt algorithm
- 🌐 **Network Isolation**: Unique magic bytes and genesis block for independent network

---

## Important Clarifications

### What Fleet Credits IS:
- ✅ A **technical cryptocurrency project** with blockchain innovations
- ✅ An **open-source software project** for developers and users
- ✅ A **decentralized network** running on peer-to-peer technology
- ✅ A **privacy-focused cryptocurrency** with optional MWEB features
- ✅ A **contribution recognition system** for technical contributions

### What Fleet Credits IS NOT:
- ❌ **NOT a political or ideological program**
- ❌ **NOT a social or economic system redesign**
- ❌ **NOT affiliated with any government or organization**
- ❌ **NOT a "New World Order" or conspiracy theory project**
- ❌ **NOT a replacement for traditional financial systems**
- ❌ **NOT a social program or welfare system**

Fleet Credits is a **technical software project** built by developers, for developers and cryptocurrency users. It focuses on blockchain technology, cryptography, and peer-to-peer networking—not political or social agendas.

---

## Technical Architecture

### Consensus Mechanism
- **Algorithm**: Scrypt Proof-of-Work
- **Block Time**: 60 seconds
- **Block Reward**: 10,000 FC (fixed, no halving)
- **Difficulty Adjustment**: Every block

### Network Ports

| Function | Mainnet | Testnet | Regtest |
|----------|---------|---------|---------|
| P2P      | 22556   | 44556   | 18444   |
| RPC      | 22555   | 44555   | 18332   |

### MWEB (Mimblewimble Extension Blocks)
- **Optional Privacy**: Transactions can be routed through MWEB for privacy
- **Pedersen Commitments**: Hide transaction amounts
- **Range Proofs**: Prove values are non-negative without revealing amounts
- **Cut-through**: Reduce blockchain bloat by removing intermediate transactions
- **Peg-in/Peg-out**: Move funds between main chain and MWEB

### Contribution System
Fleet Credits recognizes 8 types of contributions:
1. **CODE_CONTRIBUTION**: Open-source code contributions
2. **CHARITABLE_ACT**: Verified charitable activities
3. **CREATIVE_WORK**: Art, music, writing, etc.
4. **EDUCATIONAL_CONTENT**: Tutorials, documentation, courses
5. **CODE_MENTORSHIP**: Teaching and mentoring developers
6. **AI_VALIDATION**: Validating AI model outputs
7. **DATA_LABELING**: Contributing labeled datasets
8. **ETHICAL_REVIEW**: Security audits and ethical reviews (requires MWEB)

Each contribution type has dynamic reward multipliers (1.0x to 1.25x) based on community validation.

---

## Getting Started

### Installation

See [INSTALL.md](INSTALL.md) for detailed installation instructions.

**Quick Start:**
```bash
# Clone the repository
git clone https://github.com/JeremyGits/FleetCredits.git
cd FleetCredits

# Build dependencies (if needed)
./autogen.sh
./configure
make
```

### Running Fleet Credits

**Daemon (headless):**
```bash
./src/fleetcreditsd -daemon
```

**Qt GUI:**
```bash
./src/qt/fleetcredits-qt
```

**Command-line interface:**
```bash
./src/fleetcredits-cli getblockchaininfo
```

### Development

See [BUILD_GUIDE.md](BUILD_GUIDE.md) for build instructions and [CONTRIBUTING.md](CONTRIBUTING.md) for contribution guidelines.

---

## Documentation

- [Installation Guide](INSTALL.md)
- [Build Guide](BUILD_GUIDE.md)
- [Contribution Guide](CONTRIBUTING.md)
- [Security Policy](SECURITY.md)
- [Network Architecture](docs/NETWORK_ARCHITECTURE.md)
- [MWEB Implementation](docs/MWEB_IMPLEMENTATION.md)
- [Contribution System](docs/CONTRIBUTION_SYSTEM_STATUS.md)

### API Documentation

The JSON-RPC API is self-documenting:
```bash
fleetcredits-cli help                    # List all commands
fleetcredits-cli help <command>         # Detailed command help
```

---

## Contributing

Fleet Credits is an open-source project. We welcome contributions!

**How to contribute:**
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request

See [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines.

**Types of contributions we welcome:**
- Code improvements and bug fixes
- Documentation updates
- Testing and test coverage
- Security audits
- Feature suggestions

---

## Development Status

**Current Status**: Active Development

**Completed Features:**
- ✅ Core blockchain implementation
- ✅ MWEB Extension Blocks (Mimblewimble)
- ✅ Contribution system architecture
- ✅ Scrypt Proof-of-Work consensus
- ✅ Qt GUI client
- ✅ RPC API

**In Progress:**
- 🔄 Oracle verification system
- 🔄 Direct contributor payments
- 🔄 Enhanced MWEB cryptography

**Future Plans:**
- 📋 Full oracle integration
- 📋 Advanced privacy features
- 📋 Cross-chain compatibility

---

## License

Fleet Credits Core is released under the terms of the MIT license. See [COPYING](COPYING) for more information.

---

## Disclaimer

**Fleet Credits is experimental software**. Use at your own risk. The developers make no warranties about the security, functionality, or suitability of this software for any purpose.

**Not Financial Advice**: This software and documentation do not constitute financial advice. Cryptocurrencies are volatile and risky investments.

**No Political Affiliation**: Fleet Credits is a technical project with no political, ideological, or organizational affiliations. It is not associated with any government, corporation, or social movement.

---

## Community

- **GitHub Issues**: [Report bugs or request features](https://github.com/JeremyGits/FleetCredits/issues)
- **Discussions**: [Join the conversation](https://github.com/JeremyGits/FleetCredits/discussions)

---

## Acknowledgments

Fleet Credits Core is built on the shoulders of giants:
- **Bitcoin Core**: The original cryptocurrency implementation
- **Dogecoin Core**: Scrypt PoW and AuxPow implementation
- **Mimblewimble**: Privacy protocol inspiration
- **All Contributors**: Thank you for your contributions!

---

<div align="center">

**Fleet Credits - A Technical Blockchain Innovation**

Built with ❤️ by developers, for developers

</div>
