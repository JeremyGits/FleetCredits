# MWEB Client Integration
# Copyright (c) 2025-2026 The Fleet Credits Core developers

## MWEB is Integrated into Core Client

**MWEB functionality is built directly into the Fleet Credits core client.** There is no separate MWEB client - all MWEB operations are available through the standard binaries.

## Available Binaries

### fleetcreditsd (Daemon)
- Validates MWEB extension blocks
- Processes MWEB transactions
- Handles peg-in/peg-out operations
- Integrated MWEB support

### fleetcredits-cli (CLI)
- MWEB RPC commands:
  - `createpegin` - Move FC to MWEB
  - `createpegout` - Move FC from MWEB
  - `routecontributiontomweb` - Route contributions
  - `listmwebtxs` - List MWEB transactions

### fleetcredits-qt (GUI - Future)
- MWEB transaction creation UI
- View key management
- Privacy toggle for transactions

## Why Integrated?

1. **Unified Interface** - Single client for all operations
2. **Simplified Testing** - One test suite covers everything
3. **Better UX** - Users don't need separate clients
4. **Easier Maintenance** - Single codebase

## Testing

See `MWEB_TESTING.md` for comprehensive testing guide.

