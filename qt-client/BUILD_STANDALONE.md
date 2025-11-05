# Building Standalone Qt Client

## Quick Start

### 1. Build Core Libraries First

```bash
cd /mnt/d/seriousprojects/FleetCredits/fleetcredits

# Build core WITHOUT Qt (faster, simpler)
./configure --without-gui
make -j4
```

This builds:
- `fleetcreditsd`
- `fleetcredits-cli`
- `fleetcredits-tx`
- All core libraries (server, wallet, common, etc.)

### 2. Build Qt Library (Optional)

If you want to reuse existing Qt code:
```bash
cd src/qt
qmake && make  # Build libfleetcreditsqt.a separately
```

### 3. Build Standalone Qt Client

```bash
cd qt-client
qmake FleetCredits.pro
make
```

Or use Qt Creator:
- File → Open → `qt-client/FleetCredits.pro`
- Configure build paths
- Click Build

## What This Gives You

- ✅ **No autotools hell** - Just Qt build system
- ✅ **Faster builds** - Only rebuild GUI when GUI changes
- ✅ **Easier debugging** - Qt Creator integration
- ✅ **Modern workflow** - Standard Qt development

## Project Structure

```
fleetcredits/
├── src/                    # Core C++ (autotools)
│   ├── fleetcreditsd      # Daemon
│   ├── fleetcredits-cli   # CLI
│   └── [libraries built here]
│
└── qt-client/              # NEW: Standalone Qt project
    ├── FleetCredits.pro   # Qt project file
    ├── src/               # Qt client code
    ├── forms/             # UI files
    └── resources/         # Styles, icons
```

**The GUI is separate, but uses the SAME core libraries!**

