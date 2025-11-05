# Fleet Credits Windows Build and Debug Guide

## Current Status
The codebase has been fully converted to Fleet Credits naming conventions. However, we're experiencing I/O errors in WSL when trying to rebuild.

## Windows Build with MSVC/MinGW

To compile and debug on Windows with windbg:

### Option 1: MSVC Build (Recommended for windbg)
```powershell
# Install dependencies via vcpkg or manually
# Then build with MSVC solution file (if available)
# Or use CMake to generate Visual Studio solution
```

### Option 2: MinGW Build
```powershell
# Install MinGW-w64
# Then configure and build:
cd fleetcredits
./configure --prefix=/c/fleetcredits-install
make
```

### Option 3: Use Pre-built Binaries
If binaries were previously compiled, they should be in:
- `src/fleetcreditsd.exe` (or `fleetcreditsd` on Linux)
- `src/fleetcredits-cli.exe`
- `src/fleetcredits-tx.exe`
- `src/qt/fleetcredits-qt.exe`

## Running with windbg

### Debugging fleetcreditsd
```powershell
windbg fleetcreditsd.exe
# Or with command line arguments:
windbg fleetcreditsd.exe --testnet -daemon
```

### Debugging fleetcredits-qt
```powershell
windbg fleetcredits-qt.exe
```

### Common windbg Commands
```
!analyze -v              # Analyze crash dump
!heap -s                 # Show heap summary
!locks                   # Show locks
!threads                 # Show threads
g                        # Go/continue execution
bp <address>             # Set breakpoint
```

## Known Issues to Check

1. **File Renaming**: All files renamed but may need recompilation
2. **Resource Files**: QT resources need regeneration
3. **Include Paths**: Verify all includes resolve correctly
4. **Library Linking**: Check that all Fleet Credits libraries link properly

## Next Steps

1. Compile on Windows (MSVC or MinGW)
2. Run with windbg to identify runtime issues
3. Check for:
   - Missing DLLs
   - Runtime errors
   - Assertion failures
   - Memory issues
   - Network initialization problems

## WSL I/O Issues

The I/O errors in WSL are likely due to:
- Windows Defender scanning files
- Filesystem synchronization issues
- Permission problems

Solution: Build natively on Windows instead of WSL.

