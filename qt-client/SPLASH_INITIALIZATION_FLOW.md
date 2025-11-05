# Fleet Credits Splash Screen - Initialization Flow

## Current Implementation Status

✅ **Splash Screen Image**: Now using `fleetcoin.png` (Fleet Credits image)
✅ **Initialization Flow**: Matches original Dogecoin/Bitcoin Core client

## Initialization Sequence During Splash Screen

The splash screen displays messages during the following initialization steps:

### 1. **Block Index Loading** (init.cpp:1492)
```cpp
uiInterface.SafeInitMessage(_("Loading block index..."));
```
- Loads blockchain index from disk
- Builds block index tree
- Checks for corrupted blocks

### 2. **Block Rewinding** (init.cpp:1545)
```cpp
uiInterface.SafeInitMessage(_("Rewinding blocks..."));
```
- If chain state mismatch detected, rewinds to last valid block
- Only happens if reindexing or chain corruption detected

### 3. **Block Verification** (init.cpp:1552)
```cpp
uiInterface.SafeInitMessage(_("Verifying blocks..."));
```
- Verifies all blocks loaded from disk
- Checks PoW, signatures, merkle roots
- Shows progress percentage (0-100%)

### 4. **Wallet Loading** (wallet.cpp:3705, init.cpp:1623)
```cpp
uiInterface.SafeInitMessage(_("Loading wallet..."));
// Called from CWallet::InitLoadWallet()
```
- Checks if wallet.dat exists
- If exists: loads wallet keys and transactions
- If not exists: creates new wallet
- Verifies wallet integrity

### 5. **Wallet Verification** (wallet.cpp:460)
```cpp
uiInterface.SafeInitMessage(_("Verifying wallet..."));
```
- Verifies wallet file integrity
- Checks encryption status
- Validates key storage

### 6. **Rescanning** (if -rescan flag used)
```cpp
ShowProgress(_("Rescanning..."), percentage);
```
- Rescans blockchain for wallet transactions
- Shows progress percentage

## Complete Initialization Flow

```
1. Splash screen appears
   ↓
2. Intro dialog (if first run) - picks data directory
   ↓
3. AppInitMain() starts in background thread
   ↓
4. "Loading block index..." (splash screen)
   ↓
5. "Rewinding blocks..." (if needed)
   ↓
6. "Verifying blocks..." with progress % (splash screen)
   ↓
7. "Loading wallet..." (splash screen)
   ↓
8. "Verifying wallet..." (splash screen)
   ↓
9. Wallet loaded, models created
   ↓
10. "Done loading" (splash screen)
   ↓
11. Splash screen closes, main window appears
```

## Code Locations

- **Splash Screen Creation**: `fleetcredits/src/qt/fleetcredits.cpp:689-694`
- **Splash Screen Class**: `fleetcredits/src/qt/splashscreen.cpp`
- **Init Messages**: `fleetcredits/src/init.cpp` (lines 1492, 1545, 1552)
- **Wallet Loading**: `fleetcredits/src/wallet/wallet.cpp` (line 3705)
- **Wallet Verification**: `fleetcredits/src/wallet/wallet.cpp` (line 460)

## Image Resource

- **Resource File**: `fleetcredits/src/qt/fleetcredits.qrc` (line 3)
- **Icon Path**: `:/icons/fleetcredits` → `res/icons/fleetcredits.png`
- **Source Image**: `fleetcredits/qt-client/resources/icons/fleetcoin.png`
- **Status**: ✅ Updated to use Fleet Credits image

## Next Steps

1. ✅ Splash screen image updated to Fleet Credits
2. ✅ Initialization flow matches original client
3. ✅ All initialization messages displayed on splash screen
4. ✅ Wallet loading happens during splash screen phase

The implementation now matches the original Dogecoin/Bitcoin Core client behavior!

