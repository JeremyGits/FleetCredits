# Running Fleet Credits Client with windbg

## Current Status
- ✅ Codebase fully converted to Fleet Credits naming
- ✅ All source files renamed
- ✅ All Makefiles updated
- ⚠️ WSL filesystem I/O errors preventing rebuild
- ⚠️ Binaries need to be compiled/recompiled

## To Run with windbg

### Option 1: Use Previously Compiled Binaries
If binaries exist from before the file renaming:
```powershell
cd D:\seriousprojects\FleetCredits\fleetcredits\src
windbg .\fleetcreditsd.exe --testnet
```

### Option 2: Compile Fresh on Windows
1. **Install MinGW-w64 or MSVC**
2. **Configure and Build:**
   ```powershell
   cd D:\seriousprojects\FleetCredits\fleetcredits
   # If using MinGW:
   ./configure --prefix=/c/fleetcredits-install
   make
   
   # Binaries will be in: src\fleetcreditsd.exe, src\fleetcredits-cli.exe, etc.
   ```

3. **Run with windbg:**
   ```powershell
   windbg src\fleetcreditsd.exe --testnet -daemon
   ```

## Expected Issues to Check

### 1. Missing Resource Files
- QT resources may not be regenerated
- Check: `src\qt\qrc_fleetcredits.cpp` exists
- Fix: Run `make` to regenerate resources

### 2. Include Path Issues
- Verify all `#include "fleetcredits*.h"` resolve
- Check: Compilation errors about missing headers

### 3. Library Linking
- Verify `LIBFLEETCREDITS_*` libraries link correctly
- Check: Linker errors

### 4. Runtime Issues
- Missing DLLs (QT5, OpenSSL, etc.)
- Configuration file issues
- Network initialization problems

## windbg Commands for Debugging

```windbg
# Start debugging
windbg fleetcreditsd.exe --testnet

# Useful commands:
!analyze -v              # Analyze any crashes
!heap -s                 # Show heap summary
!locks                   # Check for deadlocks
!threads                 # List all threads
g                        # Continue execution
bp <function>            # Set breakpoint at function
x fleetcredits!*         # List Fleet Credits symbols
```

## Quick Test Commands

```powershell
# Test daemon
.\fleetcreditsd.exe --testnet --version

# Test CLI
.\fleetcredits-cli.exe --testnet --version

# Test with windbg
windbg .\fleetcreditsd.exe --testnet
```

## Next Steps

1. Locate or compile Windows binaries
2. Run with windbg to capture initial errors
3. Fix any runtime issues discovered
4. Test full functionality

