# Build Commands for Fleet Credits Qt Client

## Complete Build Process

```bash
# Navigate to qt-client directory
cd /mnt/d/seriousprojects/FleetCredits/fleetcredits/qt-client

# Clean previous build (optional but recommended)
make clean

# Generate Makefile from .pro file
qmake FleetCredits.pro

# Build with parallel jobs (adjust -j4 to your CPU cores)
make -j4

# Or build without parallel jobs (slower but easier to see errors)
make
```

## Check if Build Succeeded

```bash
# Check if binary exists
ls -lh ../bin/fleetcredits-qt

# If it exists, you can run it
../bin/fleetcredits-qt
```

## If Build Fails

Check the last few lines of output for errors. Common issues:
- Missing libraries → Install with `apt-get install`
- Linking errors → Check library paths in `.pro` file
- Compilation errors → Check source code fixes

## Quick One-Liner

```bash
cd /mnt/d/seriousprojects/FleetCredits/fleetcredits/qt-client && make clean && qmake FleetCredits.pro && make -j4
```
