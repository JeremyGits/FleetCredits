# QT Client Crash Fix - boost::signals2::no_slots_error

## Problem
The QT client crashes on startup with `boost::signals2::no_slots_error` because `InitError()` is called from a background thread (`FleetCreditsCore::initialize()`) before any slots are connected to the `ThreadSafeMessageBox` signal.

## Root Cause
- `AppInitSanityChecks()` → `LockDataDirectory()` → `InitError()` is called from a background thread
- The background thread starts immediately when `requestInitialize()` is called
- Dummy slots are connected later in `main()`, but the exception is thrown before that

## Solution Applied
1. **Safe Wrappers**: Added `SafeThreadSafeMessageBox`, `SafeInitMessage`, `SafeLoadWallet` methods that catch `no_slots_error`
2. **Early Connections**: Connected dummy slots at global scope in `ui_interface.cpp` before `main()` runs
3. **InitError Protection**: Added connection check in `InitError()` itself as a last resort
4. **Non-Throwing Combiner**: Changed `last_value<bool>` to `last_value<bool, false>` to prevent exceptions (note: this syntax may not be valid in all boost versions)

## Status
**CRITICAL: Still crashing.** The dummy slot connections at global scope are not preventing the exception. The signal is being emitted from a background thread before the global initializers run.

## Next Steps
1. Connect dummy slots directly in `CClientUIInterface` constructor
2. Or ensure slots are connected in `FleetCreditsApplication` constructor before `requestInitialize()` is called
3. Or wrap ALL signal emissions in try-catch blocks (already done for some)

