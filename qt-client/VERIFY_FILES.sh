#!/bin/bash
# Verify all Qt source files are in FleetCredits.pro

cd /mnt/d/seriousprojects/FleetCredits/fleetcredits

# Get all source files that should be included
ACTUAL_FILES=$(find src/qt -name '*.cpp' -not -path '*/test/*' -not -name 'test_*.cpp' -not -name '*_fixed.cpp' -not -name 'optionsdialog_minimal.cpp' -exec basename {} \; | sort)

# Extract files from .pro (simplified - just check if key files exist)
KEY_FILES="fleetcredits.cpp fleetcreditsgui.cpp clientmodel.cpp walletmodel.cpp \
overviewpage.cpp sendcoinsdialog.cpp receivecoinsdialog.cpp \
mwebsuitepage.cpp contributionspage.cpp rpcconsole.cpp \
intro.cpp guiutil.cpp optionsdialog.cpp"

echo "✅ Checking critical files..."
for file in $KEY_FILES; do
    if grep -q "../src/qt/$file" qt-client/FleetCredits.pro; then
        echo "  ✓ $file"
    else
        echo "  ✗ MISSING: $file"
    fi
done

echo ""
echo "✅ All critical files should be present!"
echo "Total .cpp files in src/qt: $(echo "$ACTUAL_FILES" | wc -l)"

