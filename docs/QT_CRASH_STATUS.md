# QT Client Crash - Current Status

## Problem
QT client crashes with `boost::signals2::no_slots_error` during initialization.

## What We've Tried
1. ✅ Safe wrapper functions (`SafeThreadSafeMessageBox`, etc.) - catches exception but still crashes
2. ✅ Dummy slot connections at global scope - doesn't prevent exception
3. ✅ Dummy slot connections in `FleetCreditsApplication` constructor - doesn't prevent exception  
4. ✅ Dummy slot connections in `FleetCreditsCore::initialize()` with `std::call_once` - doesn't prevent exception
5. ✅ Changed signal combiner to `last_value<bool, false>` - syntax may not be valid
6. ✅ Direct call to `SafeThreadSafeMessageBox` in `LockDataDirectory` - still crashes

## Root Cause
The exception is thrown DURING the signal emission, before any try-catch can handle it. The `boost::signals2::last_value<bool>` combiner throws `no_slots_error` when no slots are connected, and this happens synchronously during the signal call.

## Next Steps
1. Create a custom combiner that returns `false` instead of throwing
2. Or ensure slots are ALWAYS connected before ANY code runs (maybe in a static initializer that runs before `main()`)
3. Or disable checkpoint validation entirely if that's what's triggering the error

## Workaround
For now, use `fleetcredits-cli` for testing. The QT client needs this fix before it can be used.

