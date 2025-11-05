# Fleet Credits Splash Screen Debugging Guide

## Issues Found Through Code Analysis

### 1. **Signal Connection Order Issue** ⚠️
**Problem:** Multiple connections to `uiInterface.InitMessage`:
- Splash screen connects in constructor (splashscreen.cpp:219)
- Main function connects at line 689 of fleetcredits.cpp
- Dummy slots connect in FleetCreditsCore::initialize() (line 264)

**Impact:** Messages might be sent before splash screen subscribes, or multiple handlers compete.

**Fix Needed:** Ensure splash screen connects BEFORE any messages are emitted.

### 2. **Thread Safety Issue** ⚠️
**Problem:** Splash screen subscribes in main thread, but initialization happens in separate thread (FleetCreditsCore runs in QThread).

**Impact:** Signals emitted from init thread might not reach splash screen properly.

**Current Status:** boost::signals2 is thread-safe, but Qt's QueuedConnection ensures proper thread crossing.

### 3. **Message Display Issue** ⚠️
**Problem:** `InitMessage` uses `Qt::QueuedConnection` which is correct, but `update()` might not be called frequently enough.

**Impact:** Messages might not appear immediately on splash screen.

### 4. **Icon Loading Issue** ⚠️
**Problem:** Icon might be null if NetworkStyle doesn't load properly.

**Current Status:** Fallback to direct resource load exists (line 82), but might fail silently.

## Recommended Fixes

### Fix 1: Ensure Splash Screen Connects First
Move splash screen creation BEFORE connecting other InitMessage handlers.

### Fix 2: Add Explicit Event Processing
Ensure `QApplication::processEvents()` is called after splash screen shows to paint messages.

### Fix 3: Add Message Queue Debugging
Add logging to see if messages are actually being received by splash screen.

### Fix 4: Check Icon Resource
Verify `:/icons/fleetcredits` resource is properly included in .qrc file.

## Testing Steps

1. Run client and check debug.log for "DEBUG:" messages
2. Check if splash screen appears
3. Check if initialization messages appear on splash screen
4. Verify icon loads correctly
5. Check if main window appears after initialization

## Known Working Parts

✅ Splash screen creation code is correct
✅ Signal subscription mechanism is correct  
✅ Message display mechanism is correct
✅ Initialization logic fix (retval == 0) is correct

## Potential Remaining Issues

- Display/X11 configuration in WSL
- Icon resource not found
- Message timing (messages sent before splash subscribes)
- Event loop not processing queued messages

