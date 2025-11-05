# Fleet Credits Test Files Conversion - Complete

## Summary
All test files have been renamed and updated to use Fleet Credits naming convention.

## Files Renamed

### Test Framework Files
- `test/test_dogecoin.h` → `test/test_fleetcredits.h`
- `test/test_dogecoin.cpp` → `test/test_fleetcredits.cpp`
- `test/dogecoin_tests.cpp` → `test/fleetcredits_tests.cpp`

## Test Suite Updates

### Test Suite Names
- `BOOST_FIXTURE_TEST_SUITE(dogecoin_tests, TestingSetup)` → `BOOST_FIXTURE_TEST_SUITE(fleetcredits_tests, TestingSetup)`

### Test Class References
All test files updated to use Fleet Credits classes:
- `CDogecoinAddress` → `CFleetCreditsAddress`
- `CDogecoinSecret` → `CFleetCreditsSecret`
- `CDogecoinExtKey` → `CFleetCreditsExtKey`
- `CDogecoinExtPubKey` → `CFleetCreditsExtPubKey`

### Test Includes
All test files updated:
- `#include "test/test_dogecoin.h"` → `#include "test/test_fleetcredits.h"`

## Makefile Updates

### `src/Makefile.test.include`
- Updated `TESTS` and `bin_PROGRAMS` to reference `test/test_fleetcredits`
- Updated `TEST_BINARY` to `test/test_fleetcredits$(EXEEXT)`
- Updated `BITCOIN_TESTS` to include `test/fleetcredits_tests.cpp`
- Updated test source references to `test/test_fleetcredits.cpp` and `test/test_fleetcredits.h`
- Updated all `test_test_dogecoin_*` variables to `test_test_fleetcredits_*`
- Updated `LIBDOGECOIN_*` references to `LIBFLEETCREDITS_*` in test linking
- Updated fuzzy test references to `test_test_fleetcredits_fuzzy_*`
- Updated clean targets to reference `test_test_fleetcredits_OBJECTS`

## Test Data Files

### `test/data/bitcoin-util-test.json`
- Updated all `./dogecoin-tx` references to `./fleetcredits-tx`

## QT Resource Updates

### `src/qt/fleetcredits.cpp`
- Updated `Q_INIT_RESOURCE(dogecoin)` → `Q_INIT_RESOURCE(fleetcredits)`
- Updated `Q_INIT_RESOURCE(dogecoin_locale)` → `Q_INIT_RESOURCE(fleetcredits_locale)`

## Header Guard Updates

### `test/test_fleetcredits.h`
- Updated header guard: `FLEETCREDITS_TEST_TEST_FLEETCREDITS_H`

## Files Updated

All test files in `src/test/` directory:
- `arith_uint256_tests.cpp`
- `auxpow_tests.cpp`
- `base58_tests.cpp`
- `base64_tests.cpp`
- `bip32_tests.cpp`
- `bloom_tests.cpp`
- `key_tests.cpp`
- `script_tests.cpp`
- `fleetcredits_tests.cpp`
- ... and all other test files

## Next Steps

1. Test compilation to verify all test files compile correctly
2. Run test suite to ensure functionality is preserved
3. Continue with remaining conversion tasks

## Notes

- Generated resource files (`qrc_fleetcredits.cpp`, `qrc_fleetcredits_locale.cpp`) still contain references to `dogecoin` in comments - these are auto-generated and will be updated when resources are regenerated
- Translation files (`*.ts`) still reference `dogecoin` - these can be updated later if needed
- Test data files may contain legacy references that don't affect functionality

