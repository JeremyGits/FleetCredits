#!/bin/bash
# Build script for Fleet Credits Qt Client

set -e

echo "🚀 Building Fleet Credits Qt Client..."

# Check if we're in the right directory
if [ ! -f "FleetCredits.pro" ]; then
    echo "❌ Error: FleetCredits.pro not found!"
    echo "   Please run this script from the qt-client directory"
    exit 1
fi

# Check if core libraries exist
if [ ! -f "../src/libfleetcredits_server.a" ]; then
    echo "⚠️  Warning: Core libraries not found!"
    echo "   Building core libraries first..."
    cd ../src
    if [ ! -f "Makefile" ]; then
        echo "   Configuring..."
        ../configure --without-gui
    fi
    echo "   Building core libraries..."
    make -j$(nproc)
    cd ../qt-client
    echo "✅ Core libraries built!"
fi

# Generate Makefile
echo "📝 Generating Makefile from FleetCredits.pro..."
qmake FleetCredits.pro

# Build
echo "🔨 Building Qt client..."
make -j$(nproc)

# Check if binary was created
if [ -f "../bin/fleetcredits-qt" ]; then
    echo "✅ Build successful! Executable: ../bin/fleetcredits-qt"
else
    echo "❌ Build failed! Check errors above."
    exit 1
fi

echo ""
echo "🎉 Done! Run with: ../bin/fleetcredits-qt"

