# Pre-GitHub Cleanup Checklist

This document outlines what needs to be cleaned before pushing to GitHub.

## Files/Directories to DELETE

### 1. Temporary Development Scripts (Root Level)
- `build_dogecoin.sh`
- `build-daemon.sh`
- `build-qt-client.sh`
- `build_windows.sh`
- `build-qt-client.sh`
- `BUILD_QT_NOW.sh`
- `BUILD_QT_SIMPLE.sh`
- `check-daemon.sh`
- `check-qt-build.sh`
- `clean_and_regenerate_build.sh`
- `cleanup-remaining-references.sh`
- `CLEAR_ALL_DATA.sh`
- `CLEAR_OLD_DATA.sh`
- `clear-regtest.sh`
- `debug-daemon.gdb`
- `debug-qt-client.sh`
- `debug-qt-gdb.sh`
- `debug-qt-simple.sh`
- `fix-and-rebuild-qt.sh`
- `fix_locale_references.sh`
- `fix_test_includes.sh`
- `FIX_MAKEFILE_NOW.sh`
- `rename-locale-files.sh`
- `rename_remaining_bitcoin_files.sh`
- `rename_locale_files.py`
- `scan_bitcoin_references.sh`
- `start-two-nodes.sh`
- `test-daemon.sh`
- `test_modern_ui.cpp`
- `test_modern_ui.sh`
- `testing-platform.sh`
- `UNCOMMENT_ALL_QT.sh`
- `update_makefile_references.sh`
- `verify_modern_ui.py`
- `setup_build_env.sh`

### 2. Temporary Development Documentation (Root Level)
All the "fix" and "debug" markdown files that were created during development:
- `ALL_FIXED_NOW.md`
- `BUILD_DAEMON.md`
- `BUILD_NOW.md`
- `BUILD_QT_CLIENT.md`
- `CHECK_QT_BUILD.md`
- `CLEANUP_SUMMARY.md`
- `COMPLETE_CLEANUP_DOGECOIN_BITCOIN.md`
- `COMPLETE_STARTUP_FIX.md`
- `COMPREHENSIVE_FILE_AUDIT.md`
- `COMPREHENSIVE_FIX.md`
- `connect-qt-client.md`
- `DAEMON_BUILD_LOCATION.md`
- `DEBUG_QT_CLIENT.md`
- `DIAGNOSE_NO_OUTPUT.md`
- `FIX_LOCALE_BUILD.md`
- `FIX_OPTIONS_DIALOG.md`
- `FIX_QT_BUILD.md`
- `FIX_QT_CRASH.md`
- `FIX_RPC_CONNECTION.md`
- `FIX_RPC_WARMUP.md`
- `GDB_DEBUG.md`
- `JUST_RUN_THIS.md`
- `LAUNCH_COMMANDS.md`
- `LIVE_STREAM_TESTS.md`
- `MODERN_UI_README.md`
- `MWEB_TESTING_GUIDE.md`
- `OPEN_PORT_18444.md`
- `OPTIONS_DIALOG_SUMMARY.md`
- `PEER_CONNECTION_EXPLAINED.md`
- `PEER_MANAGEMENT_COMPLETE_FIX.md`
- `PEER_MANAGEMENT_FIXES.md`
- `PEER_MANAGEMENT_ISSUES.md`
- `PEER_TO_PEER_NETWORKING.md`
- `QT_BUILD_FIX.md`
- `QT_CLIENT_DAEMON_ARCHITECTURE.md`
- `QT_CLIENT_DAEMON_EXPLAINED.md`
- `QT_CLIENT_RPC_MODE.md`
- `QT_CLIENT_SERVER_MODE.md`
- `QT_M4_FIX.md`
- `QUICK_FIX_QT_BUILD.md`
- `QUICK_FIX_QT.md`
- `QUICK_RUN_CLIENT_TEST.md`
- `READY_FOR_RECOMPILATION.md`
- `REAL_FIX.md`
- `REMOVE_DOGECOIN_BITCOIN.md`
- `RENAME_LOCALE_NOW.md`
- `RUN_QT_CLIENT.md`
- `RUN_QT_WITH_TEST_DIR.md`
- `SETUP_TWO_NODES.md`
- `TEST_DAEMON_CONSOLE.md`
- `TEST_RUNNING_DAEMON.md`

### 3. Test Data Directories
- `client-test/`
- `node1/`
- `node2/`
- `test-data/`

### 4. Build Artifacts & Logs
- `build.log`
- `build_output.log`
- `config.log` (already in .gitignore, but delete if present)
- `config.status` (already in .gitignore, but delete if present)
- `configure~` (backup file)
- `aclocal.m4` (already in .gitignore, but delete if present)
- `autom4te.cache/` (already in .gitignore, but delete if present)
- `libtool` (already in .gitignore, but delete if present)
- `Makefile` (already in .gitignore, but delete if present)
- `configure` (already in .gitignore, but delete if present)

### 5. Old Dogecoin/Bitcoin Files
- `dogecoin action bubble for splash.xcf`
- `dogecoin-rebrand.bundle`
- `libdogecoinconsensus.pc` (if generated, should be in .gitignore)
- `libdogecoinconsensus.pc.in` (should be removed if not needed)

### 6. Qt-Client Directory (if not needed)
- `qt-client/` - This appears to be a separate build attempt. If the main Qt client is in `src/qt/`, this can be removed.

### 7. Development Testing Directory
- `devtesting/` - Development testing scripts, can be removed or kept if you want to preserve test scripts

## Files/Directories to KEEP

### Essential Documentation
- `README.md`
- `INSTALL.md`
- `BUILD_GUIDE.md`
- `CONTRIBUTING.md`
- `SECURITY.md`
- `COPYING`
- `docs/` - Project documentation
- `doc/` - Original Bitcoin/Dogecoin documentation (may want to update)

### Essential Scripts
- `autogen.sh`
- `genbuild.sh`
- `contrib/` - Contribution scripts
- `depends/` - Build dependencies
- `qa/` - Quality assurance tests

### Essential Directories
- `src/` - Source code
- `share/` - Shared resources
- `build-aux/` - Build auxiliary files
- `contrib/` - Contribution scripts
- `depends/` - Build dependencies
- `qa/` - Quality assurance tests

## .gitignore Updates Needed

The `.gitignore` should be updated to include:
- `test-data/`
- `client-test/`
- `node1/`
- `node2/`
- `*.xcf` (GIMP files)
- `*.bundle` (Git bundle files)
- `qt-client/` (if removing it)

## After Cleanup

1. Update `.gitignore` with the new entries
2. Run `git status` to verify what will be committed
3. Review `README.md` to ensure it's accurate
4. Check that all dogecoin/bitcoin references in essential files are updated
5. Verify that `docs/` contains only relevant documentation

