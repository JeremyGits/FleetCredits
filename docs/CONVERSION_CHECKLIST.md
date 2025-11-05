# Fleet Credits Comprehensive Conversion Checklist
## Systematic File Renaming & Branding Conversion

**Status**: In Progress  
**Last Updated**: 2025-10-31

---

## ✅ Completed Conversions

### Config & Template Files
- [x] `src/config/fleetcredits-config.h.in` → Fleet Credits macros
- [x] `libfleetcreditsconsensus.pc.in` → Fleet Credits library name
- [x] `share/fleetcredits.conf` → fleetcredits.conf references

### Consensus Library
- [x] `src/script/fleetcreditsconsensus.h` → Fleet Credits types/functions + legacy aliases
- [x] `src/script/fleetcreditsconsensus.cpp` → Fleet Credits implementations + legacy wrappers

### Core Source Files  
- [x] `src/fleetcreditsd.cpp` → Config header, usage strings
- [x] `src/fleetcredits-cli.cpp` → Config header
- [x] `src/fleetcredits-tx.cpp` → Config header
- [x] `src/util.cpp` → Config/PID filenames, module name
- [x] `src/init.cpp` → URLs, config header
- [x] `src/clientversion.cpp` → Client name
- [x] `src/validation.cpp` → Fee function
- [x] `src/rpc/server.cpp` → Server messages, help text
- [x] `src/rpc/misc.cpp` → Help text, address descriptions

### Build System
- [x] `configure.ac` → Package/binary names
- [x] `Makefile.am` (root) → Package names
- [x] `src/Makefile.am` → All libraries/binaries

### Base58 Address System
- [x] `src/base58.h` → Comments updated, typedef alias added

---

## ⏳ Pending File Renames

### Critical Source Files (Need Renaming)
- [ ] `src/fleetcreditsd.cpp` → `fleetcreditsd.cpp`
- [ ] `src/fleetcredits-cli.cpp` → `fleetcredits-cli.cpp`
- [ ] `src/fleetcredits-tx.cpp` → `fleetcredits-tx.cpp`
- [ ] `src/fleetcredits-fees.cpp` → `fleetcredits-fees.cpp`
- [ ] `src/fleetcredits-fees.h` → `fleetcredits-fees.h`
- [ ] `src/fleetcredits.cpp` → `fleetcredits.cpp`
- [ ] `src/fleetcredits.h` → `fleetcredits.h`

### Consensus Library Files
- [ ] `src/script/fleetcreditsconsensus.cpp` → `fleetcreditsconsensus.cpp`
- [ ] `src/script/fleetcreditsconsensus.h` → `fleetcreditsconsensus.h`

### Config Files
- [ ] `src/config/fleetcredits-config.h.in` → `fleetcredits-config.h.in`
- [ ] `libfleetcreditsconsensus.pc.in` → `libfleetcreditsconsensus.pc.in`
- [ ] `share/fleetcredits.conf` → `share/fleetcredits.conf`

### Test Files
- [ ] `src/test/dogecoin_tests.cpp` → `fleetcredits_tests.cpp`
- [ ] `src/test/test_dogecoin.cpp` → `test_fleetcredits.cpp`
- [ ] `src/test/test_dogecoin.h` → `test_fleetcredits.h`
- [ ] `src/test/test_dogecoin_fuzzy.cpp` → `test_fleetcredits_fuzzy.cpp`

### Resource Files
- [ ] `src/fleetcreditsd-res.rc` → `fleetcreditsd-res.rc`
- [ ] `src/fleetcredits-cli-res.rc` → `fleetcredits-cli-res.rc`
- [ ] `src/fleetcredits-tx-res.rc` → `fleetcredits-tx-res.rc`
- [ ] `src/qt/fleetcredits-qt-res.rc` → `fleetcredits-qt-res.rc`

### QT Files (Many)
- [ ] All `src/qt/fleetcredits*.cpp/h` → `fleetcredits*.cpp/h`
- [ ] All `src/qt/qrc_dogecoin*.cpp` → `qrc_fleetcredits*.cpp`
- [ ] All `src/qt/locale/dogecoin_*.ts/qm` → `fleetcredits_*.ts/qm`
- [ ] All `src/qt/moc_dogecoin*.cpp` → `moc_fleetcredits*.cpp`
- [ ] `src/qt/fleetcredits.moc` → `fleetcredits.moc`

### Icon/Image Files
- [ ] `src/qt/res/icons/fleetcredits.png` → `fleetcredits.png`
- [ ] `src/qt/res/icons/fleetcredits.ico` → `fleetcredits.ico`
- [ ] `src/qt/res/icons/fleetcredits.icns` → `fleetcredits.icns`
- [ ] `src/qt/res/icons/dogecoin_testnet.ico` → `fleetcredits_testnet.ico`
- [ ] `src/qt/res/src/fleetcredits.svg` → `fleetcredits.svg`

---

## ⏳ Pending Content Conversions

### Fleet Credits References to Update
- [ ] All `fleetcredits` → `fleetcredits` (lowercase)
- [ ] All `Fleet Credits` → `Fleet Credits`
- [ ] All `FC` → `FC` (currency code)
- [ ] All `fleetcreditsd` → `fleetcreditsd`
- [ ] All `fleetcredits-cli` → `fleetcredits-cli`
- [ ] All `fleetcredits-qt` → `fleetcredits-qt`
- [ ] All `CBitcoinAddress` → `CFleetCreditsAddress` (or keep as CDogecoinAddress alias)

### Fleet Credits References to Update  
- [ ] All `fleetcredits` → `fleetcredits` (lowercase, careful with word boundaries)
- [ ] All `Fleet Credits` → `Fleet Credits`
- [ ] All `FC` → `FC`
- [ ] All `fc` → `fc` (lowercase currency)

### CDogecoinAddress Usage
Currently used in 26+ files. Options:
1. Keep `CDogecoinAddress` everywhere (simpler, backward compatible)
2. Add `typedef CDogecoinAddress CFleetCreditsAddress;` and update new code
3. Full rename (complex, many files)

**Recommendation**: Keep CDogecoinAddress but add typedef alias.

### File Paths & Directories
- [ ] `.fleetcredits/` → `.fleetcredits/` (data directory)
- [ ] All path references in code
- [ ] Build system path references

### Makefile Updates Needed
- [ ] Update `src/Makefile.am` SOURCES to use new filenames (or keep old names)
- [ ] Update any hardcoded paths

---

## 🔍 Files Needing Manual Review

### High Priority
1. **Wallet Files** - Many CDogecoinAddress usages
2. **QT GUI Files** - Display strings, UI elements
3. **RPC Files** - Help text, response messages
4. **Test Files** - Test descriptions, assertions

### Medium Priority  
5. **Documentation** - README, INSTALL, doc/*.md
6. **Contrib Files** - Package definitions, scripts
7. **Build Scripts** - autogen.sh, configure.ac dependencies

### Lower Priority
8. **Locale Files** - Translation strings (can update later)
9. **Generated Files** - MOC, UI headers (regenerate after changes)

---

## 📋 Conversion Script Status

### Created Scripts
1. `devtesting/scripts/conversion/branding_conversion.py` - Basic find/replace
2. `devtesting/scripts/conversion/comprehensive_conversion.py` - File rename + content (needs Unicode fix)

### Script Improvements Needed
- Fix Unicode encoding for Windows console
- Add better file rename logic
- Handle Makefile.am SOURCES updates
- Skip generated files properly

---

## 🎯 Strategy

### Phase 1: Keep Source Filenames (Current)
- Source files can keep old names (fleetcreditsd.cpp)
- Build system maps them to new binary names (fleetcreditsd)
- Easier transition, less file renaming

### Phase 2: Update Content Systematically
- Use comprehensive script to update all content
- Manual review of critical files
- Test after each major change

### Phase 3: Optional File Renames
- After everything works, optionally rename source files
- Update Makefiles accordingly
- Cleaner but more complex

---

## 📝 Notes

### Class Names
- `CDogecoinAddress` - Keep as-is for now, add typedef
- `CDogecoinSecret` - Keep as-is
- All function names with `Fleet Credits` prefix can get Fleet Credits aliases

### Compatibility
- Legacy aliases maintained throughout
- Allows gradual migration
- Can remove legacy names in future release

### Currency Unit
- Display name: "Fleet Credits" or "FC"
- Code: "FC" 
- Symbol: TBD (could be FC or custom symbol)

---

**Next Steps**: Continue with RPC files, wallet files, and QT files systematically.

