# Fleet Credits Naming Conventions

## Overview
This document defines the naming conventions for Fleet Credits source code, files, and identifiers.

## Binary Names
- **Main daemon**: `fleetcreditsd`
- **CLI interface**: `fleetcredits-cli`
- **Transaction utility**: `fleetcredits-tx`
- **QT GUI client**: `fleetcredits-qt`

## Source File Naming Convention

### Core Files
- `fleetcredits.cpp` / `fleetcredits.h` - Core Fleet Credits logic (block subsidy, PoW calculations)
- `fleetcredits-fees.cpp` / `fleetcredits-fees.h` - Fee calculation system
- `fleetcreditsconsensus.cpp` / `fleetcreditsconsensus.h` - Consensus library API
- `fleetcreditsd.cpp` - Main daemon entry point
- `fleetcredits-cli.cpp` - CLI interface entry point
- `fleetcredits-tx.cpp` - Transaction utility entry point

### Configuration Files
- `config/fleetcredits-config.h` - Build configuration header
- `share/fleetcredits.conf` - Example configuration file

### QT Files
- `qt/fleetcredits.cpp` - Main QT application entry point
- `qt/fleetcreditsgui.h` / `qt/fleetcreditsgui.cpp` - Main GUI window
- `qt/fleetcreditsunits.h` / `qt/fleetcreditsunits.cpp` - Currency unit handling
- `qt/fleetcreditsaddressvalidator.h` / `qt/fleetcreditsaddressvalidator.cpp` - Address validation
- `qt/fleetcreditsamountfield.h` / `qt/fleetcreditsamountfield.cpp` - Amount input field

### Resource Files
- `qt/fleetcredits.qrc` - QT resource file
- `qt/fleetcredits_locale.qrc` - QT locale resource file
- `qt/qrc_fleetcredits.cpp` - Generated QT resource C++ file
- `qt/qrc_fleetcredits_locale.cpp` - Generated QT locale resource C++ file
- `fleetcreditsd-res.rc` - Windows resource file for daemon
- `fleetcredits-cli-res.rc` - Windows resource file for CLI
- `fleetcredits-tx-res.rc` - Windows resource file for tx utility

### Library Names
- `libfleetcreditsconsensus` - Consensus library
- `libfleetcreditsqt.a` - QT library

### Package Names
- `libfleetcreditsconsensus.pc` - Pkg-config file for consensus library

## Code Identifiers

### Namespaces
- Use `FleetCredits` namespace for Fleet Credits-specific code

### Class Names
- `CFleetCreditsAddress` - Address class
- `CFleetCreditsSecret` - Secret/private key class
- `FleetCreditsGUI` - Main GUI class
- `FleetCreditsCore` - Core application class
- `FleetCreditsApplication` - QT application class
- `FleetCreditsUnits` - Currency units class

### Function Names
- `GetFleetCreditsBlockSubsidy()` - Block subsidy calculation
- `CalculateFleetCreditsNextWorkRequired()` - Difficulty calculation
- `GetFleetCreditsFeeRate()` - Fee rate calculation
- `GetFleetCreditsMinRelayFee()` - Minimum relay fee

### Constants
- `CLIENT_VERSION_*` - Version macros (unchanged)
- `FC` - Currency code/symbol
- `MICRO_TX_THRESHOLD` - Threshold for zero-fee transactions

### Network Parameters
- Ports: Mainnet P2P `22556`, RPC `22555`; Testnet P2P `44556`, RPC `44555`
- Message start bytes: `0xfc, 0x3c, 0x8e, 0xdc` (to be finalized)

## Legacy Compatibility
During the transition period, legacy aliases are maintained:
- `dogecoin-qt` → `fleetcredits-qt`
- `dogecoind` → `fleetcreditsd`
- `dogecoin-cli` → `fleetcredits-cli`
- `dogecoin-tx` → `fleetcredits-tx`

These aliases will be removed in a future version.

## File Extensions
- `.cpp` - C++ source files
- `.h` - Header files
- `.qrc` - QT resource files
- `.ui` - QT UI designer files
- `.ts` - QT translation source files
- `.qm` - QT translation compiled files
- `.rc` - Windows resource files

## Directory Structure
- `src/` - Source code
- `src/qt/` - QT GUI source code
- `src/config/` - Configuration headers
- `src/script/` - Script-related code
- `src/wallet/` - Wallet code
- `src/rpc/` - RPC server code
- `docs/` - Documentation
- `devtesting/scripts/` - Development and testing scripts

## Variable Naming
- Use `camelCase` for variables and functions
- Use `PascalCase` for classes and types
- Use `UPPER_CASE` for constants and macros
- Use `snake_case` for file names

## Git/Repository
- Repository name: `fleetcredits`
- Package name: `fleetcredits-core` or `fleetcredits`

## Conversion Priority
1. Core source files (dogecoin.cpp → fleetcredits.cpp)
2. Fee system files (dogecoin-fees.* → fleetcredits-fees.*)
3. Entry point files (dogecoind.cpp → fleetcreditsd.cpp)
4. QT files (qt/dogecoin.* → qt/fleetcredits.*)
5. Resource files (.qrc, .rc)
6. Update all Makefile references
7. Update all includes throughout codebase
