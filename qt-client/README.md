# Fleet Credits Qt Client - Standalone Build

## ✅ Complete Setup

This is a **standalone Qt client** that links to Fleet Credits core libraries. No autotools complexity!

## Quick Start

```bash
# 1. Build core libraries (first time only)
cd /mnt/d/seriousprojects/FleetCredits/fleetcredits
./configure --without-gui
make -j4

# 2. Build Qt client
cd qt-client
./BUILD_SCRIPT.sh
```

Or manually:
```bash
cd qt-client
qmake FleetCredits.pro
make -j4
```

## What's Included

✅ **All Qt source files** (70+ files)  
✅ **All UI forms** (23 .ui files)  
✅ **MWEB Suite page**  
✅ **Contributions page**  
✅ **Modern UI components**  
✅ **Wallet functionality**  
✅ **RPC console**  
✅ **All resources** (icons, styles, translations)  

## Project File

`FleetCredits.pro` is the complete qmake project file that:
- Lists all Qt source files
- Lists all UI forms
- Links to all core libraries
- Sets up proper include paths
- Configures resources

## Entry Point

The main entry point is `../src/qt/fleetcredits.cpp` - the existing, working entry point that:
- Initializes Fleet Credits Core
- Creates the GUI
- Handles wallet loading
- Sets up all connections

**We're reusing all your existing code!**

## Building in Qt Creator

1. Open Qt Creator
2. File → Open File or Project
3. Select `qt-client/FleetCredits.pro`
4. Configure build paths
5. Click Build (or press Ctrl+B)

## Output

The executable will be at: `../bin/fleetcredits-qt`

## Troubleshooting

### Qt Not Found
```bash
sudo apt-get install qt5-default qtbase5-dev qt5-qmake qttools5-dev
```

### Core Libraries Missing
```bash
cd ../src && make
```

### Config Header Missing
```bash
cd ../src && ./configure --without-gui
```

## Why This Approach

- ✅ **No autotools** - Just Qt's standard build system
- ✅ **Faster** - Only rebuilds GUI when GUI changes
- ✅ **Easier** - Standard Qt development workflow
- ✅ **IDE-friendly** - Works perfectly with Qt Creator
- ✅ **Separate concerns** - GUI build separate from core build

The core libraries are still built with autotools (because they need to be), but the GUI is built with qmake (because it's easier and designed for Qt).

## Next Steps

1. Build it: `./BUILD_SCRIPT.sh`
2. Run it: `../bin/fleetcredits-qt`
3. Enjoy: No more autotools frustration! 🎉
