# Why a Separate Qt Client Directory?

## The Problem

The autotools/automake system is **broken** for Qt:
- Qt variables get commented out even when `--with-gui=yes`
- Hundreds of variables need manual uncommenting
- Makefile regeneration doesn't work properly
- Extremely frustrating to debug

## The Solution

A **standalone Qt project** that:

1. **Uses Qt's build system** (qmake or CMake)
   - Designed FOR Qt, not adapted
   - No autotools complexity

2. **Links to existing C++ core**
   - Same libraries (`libfleetcredits_server.a`, etc.)
   - Same RPC interface
   - Same blockchain logic
   - **Just a different build system**

3. **Much simpler development**
   - Open in Qt Creator
   - Click "Build"
   - Done!

4. **Easier to customize**
   - Add new UI files easily
   - No Makefile.am editing
   - Modern Qt features

## Build Flow

```
Core C++ Libraries (autotools)
        ↓
    [built via make]
        ↓
Qt Client (qmake/CMake)
        ↓
    [links to core libs]
        ↓
  fleetcredits-qt.exe
```

## You Still Get:

- ✅ Same blockchain core
- ✅ Same consensus rules
- ✅ Same RPC commands
- ✅ All your new UI (MWEB, Contributions)
- ✅ Modern, beautiful interface

**Just a different way to build the GUI!**

The core stays the same - we're just building the Qt client separately.

