#!/bin/bash

# Pre-GitHub Cleanup Script for Fleet Credits
# This script removes temporary development files before pushing to GitHub

set -e

echo "=== Fleet Credits Pre-GitHub Cleanup ==="
echo ""
echo "This script will remove:"
echo "  - Temporary development scripts"
echo "  - Temporary documentation files"
echo "  - Test data directories"
echo "  - Build artifacts and logs"
echo "  - Old dogecoin/bitcoin files"
echo ""
read -p "Continue? (y/N): " -n 1 -r
echo
if [[ ! $REPLY =~ ^[Yy]$ ]]; then
    echo "Cleanup cancelled."
    exit 1
fi

# Get script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
cd "$SCRIPT_DIR"

echo ""
echo "Starting cleanup..."

# 1. Remove temporary development scripts
echo "Removing temporary development scripts..."
rm -f build_dogecoin.sh
rm -f build-daemon.sh
rm -f build-qt-client.sh
rm -f build_windows.sh
rm -f BUILD_QT_NOW.sh
rm -f BUILD_QT_SIMPLE.sh
rm -f check-daemon.sh
rm -f check-qt-build.sh
rm -f clean_and_regenerate_build.sh
rm -f cleanup-remaining-references.sh
rm -f CLEAR_ALL_DATA.sh
rm -f CLEAR_OLD_DATA.sh
rm -f clear-regtest.sh
rm -f debug-daemon.gdb
rm -f debug-qt-client.sh
rm -f debug-qt-gdb.sh
rm -f debug-qt-simple.sh
rm -f fix-and-rebuild-qt.sh
rm -f fix_locale_references.sh
rm -f fix_test_includes.sh
rm -f FIX_MAKEFILE_NOW.sh
rm -f rename-locale-files.sh
rm -f rename_remaining_bitcoin_files.sh
rm -f rename_locale_files.py
rm -f scan_bitcoin_references.sh
rm -f start-two-nodes.sh
rm -f test-daemon.sh
rm -f test_modern_ui.cpp
rm -f test_modern_ui.sh
rm -f testing-platform.sh
rm -f UNCOMMENT_ALL_QT.sh
rm -f update_makefile_references.sh
rm -f verify_modern_ui.py
rm -f setup_build_env.sh
echo "✓ Removed temporary scripts"

# 2. Remove temporary development documentation
echo "Removing temporary development documentation..."
rm -f ALL_FIXED_NOW.md
rm -f BUILD_DAEMON.md
rm -f BUILD_NOW.md
rm -f BUILD_QT_CLIENT.md
rm -f CHECK_QT_BUILD.md
rm -f CLEANUP_SUMMARY.md
rm -f COMPLETE_CLEANUP_DOGECOIN_BITCOIN.md
rm -f COMPLETE_STARTUP_FIX.md
rm -f COMPREHENSIVE_FILE_AUDIT.md
rm -f COMPREHENSIVE_FIX.md
rm -f connect-qt-client.md
rm -f DAEMON_BUILD_LOCATION.md
rm -f DEBUG_QT_CLIENT.md
rm -f DIAGNOSE_NO_OUTPUT.md
rm -f FIX_LOCALE_BUILD.md
rm -f FIX_OPTIONS_DIALOG.md
rm -f FIX_QT_BUILD.md
rm -f FIX_QT_CRASH.md
rm -f FIX_RPC_CONNECTION.md
rm -f FIX_RPC_WARMUP.md
rm -f GDB_DEBUG.md
rm -f JUST_RUN_THIS.md
rm -f LAUNCH_COMMANDS.md
rm -f LIVE_STREAM_TESTS.md
rm -f MODERN_UI_README.md
rm -f MWEB_TESTING_GUIDE.md
rm -f OPEN_PORT_18444.md
rm -f OPTIONS_DIALOG_SUMMARY.md
rm -f PEER_CONNECTION_EXPLAINED.md
rm -f PEER_MANAGEMENT_COMPLETE_FIX.md
rm -f PEER_MANAGEMENT_FIXES.md
rm -f PEER_MANAGEMENT_ISSUES.md
rm -f PEER_TO_PEER_NETWORKING.md
rm -f QT_BUILD_FIX.md
rm -f QT_CLIENT_DAEMON_ARCHITECTURE.md
rm -f QT_CLIENT_DAEMON_EXPLAINED.md
rm -f QT_CLIENT_RPC_MODE.md
rm -f QT_CLIENT_SERVER_MODE.md
rm -f QT_M4_FIX.md
rm -f QUICK_FIX_QT_BUILD.md
rm -f QUICK_FIX_QT.md
rm -f QUICK_RUN_CLIENT_TEST.md
rm -f READY_FOR_RECOMPILATION.md
rm -f REAL_FIX.md
rm -f REMOVE_DOGECOIN_BITCOIN.md
rm -f RENAME_LOCALE_NOW.md
rm -f RUN_QT_CLIENT.md
rm -f RUN_QT_WITH_TEST_DIR.md
rm -f SETUP_TWO_NODES.md
rm -f TEST_DAEMON_CONSOLE.md
rm -f TEST_RUNNING_DAEMON.md
echo "✓ Removed temporary documentation"

# 3. Remove test data directories
echo "Removing test data directories..."
rm -rf client-test/
rm -rf node1/
rm -rf node2/
rm -rf test-data/
echo "✓ Removed test data directories"

# 4. Remove build artifacts and logs
echo "Removing build artifacts and logs..."
rm -f build.log
rm -f build_output.log
rm -f configure~
echo "✓ Removed build artifacts"

# 5. Remove old dogecoin/bitcoin files
echo "Removing old dogecoin/bitcoin files..."
rm -f "dogecoin action bubble for splash.xcf"
rm -f dogecoin-rebrand.bundle
rm -f libdogecoinconsensus.pc
rm -f libdogecoinconsensus.pc.in
echo "✓ Removed old dogecoin/bitcoin files"

# 6. Remove qt-client directory if it exists (optional - uncomment if needed)
# echo "Removing qt-client directory..."
# rm -rf qt-client/
# echo "✓ Removed qt-client directory"

# 7. Remove devtesting directory (optional - uncomment if needed)
# echo "Removing devtesting directory..."
# rm -rf devtesting/
# echo "✓ Removed devtesting directory"

echo ""
echo "=== Cleanup Complete ==="
echo ""
echo "Next steps:"
echo "1. Update .gitignore (see PRE_GITHUB_CLEANUP.md)"
echo "2. Review git status: git status"
echo "3. Check README.md for accuracy"
echo "4. Verify no dogecoin/bitcoin references remain in essential files"
echo ""

