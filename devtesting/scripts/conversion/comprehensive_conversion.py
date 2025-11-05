#!/usr/bin/env python3
"""
Fleet Credits Comprehensive Conversion Script
Handles file renaming and content replacement systematically
"""

import os
import re
from pathlib import Path
import shutil

# File renaming patterns: (old_pattern, new_pattern, description)
FILE_RENAMES = [
    # Config files
    (r'dogecoin-config\.h', 'fleetcredits-config.h', 'Config header'),
    (r'dogecoin-config\.h\.in', 'fleetcredits-config.h.in', 'Config header template'),
    
    # Source files
    (r'dogecoind\.cpp', 'fleetcreditsd.cpp', 'Daemon source'),
    (r'dogecoind-res\.rc', 'fleetcreditsd-res.rc', 'Daemon resource'),
    (r'dogecoin-cli\.cpp', 'fleetcredits-cli.cpp', 'CLI source'),
    (r'dogecoin-cli-res\.rc', 'fleetcredits-cli-res.rc', 'CLI resource'),
    (r'dogecoin-tx\.cpp', 'fleetcredits-tx.cpp', 'TX tool source'),
    (r'dogecoin-tx-res\.rc', 'fleetcredits-tx-res.rc', 'TX tool resource'),
    (r'dogecoin-fees\.cpp', 'fleetcredits-fees.cpp', 'Fees source'),
    (r'dogecoin-fees\.h', 'fleetcredits-fees.h', 'Fees header'),
    (r'dogecoin\.cpp', 'fleetcredits.cpp', 'Main source'),
    (r'dogecoin\.h', 'fleetcredits.h', 'Main header'),
    (r'dogecoin\.moc', 'fleetcredits.moc', 'MOC file'),
    
    # Script files
    (r'dogecoinconsensus\.cpp', 'fleetcreditsconsensus.cpp', 'Consensus script'),
    (r'dogecoinconsensus\.h', 'fleetcreditsconsensus.h', 'Consensus header'),
    (r'libdogecoinconsensus\.pc', 'libfleetcreditsconsensus.pc', 'Package config'),
    (r'libdogecoinconsensus\.pc\.in', 'libfleetcreditsconsensus.pc.in', 'Package config template'),
    
    # Test files
    (r'test_dogecoin\.cpp', 'test_fleetcredits.cpp', 'Test main'),
    (r'test_dogecoin\.h', 'test_fleetcredits.h', 'Test header'),
    (r'test_dogecoin_fuzzy\.cpp', 'test_fleetcredits_fuzzy.cpp', 'Fuzzy test'),
    (r'dogecoin_tests\.cpp', 'fleetcredits_tests.cpp', 'Tests'),
    
    # Config files
    (r'dogecoin\.conf', 'fleetcredits.conf', 'Config file'),
    
    # QT resource files
    (r'qrc_dogecoin\.cpp', 'qrc_fleetcredits.cpp', 'QT resource'),
    (r'qrc_dogecoin_locale\.cpp', 'qrc_fleetcredits_locale.cpp', 'QT locale resource'),
    (r'dogecoin-qt-res\.rc', 'fleetcredits-qt-res.rc', 'QT resource file'),
    
    # QT locale files (pattern)
    (r'dogecoin_([^.]+)\.ts', r'fleetcredits_\1.ts', 'QT translation source'),
    (r'dogecoin_([^.]+)\.qm', r'fleetcredits_\1.qm', 'QT translation compiled'),
    
    # MOC files
    (r'moc_dogecoin', 'moc_fleetcredits', 'MOC generated'),
    (r'dogecoinamountfield\.moc', 'fleetcreditsamountfield.moc', 'Amount field MOC'),
    
    # Icons and resources
    (r'dogecoin\.png', 'fleetcredits.png', 'PNG icon'),
    (r'dogecoin\.ico', 'fleetcredits.ico', 'ICO icon'),
    (r'dogecoin\.icns', 'fleetcredits.icns', 'ICNS icon'),
    (r'dogecoin\.svg', 'fleetcredits.svg', 'SVG icon'),
    (r'dogecoin_testnet\.ico', 'fleetcredits_testnet.ico', 'Testnet icon'),
]

# Content replacements
CONTENT_REPLACEMENTS = [
    # Binary/executable names
    (r'\bdogecoind\b', 'fleetcreditsd', 'Binary: daemon'),
    (r'\bdogecoin-cli\b', 'fleetcredits-cli', 'Binary: CLI'),
    (r'\bdogecoin-qt\b', 'fleetcredits-qt', 'Binary: QT GUI'),
    (r'\bdogecoin-tx\b', 'fleetcredits-tx', 'Binary: TX tool'),
    
    # Package/library names
    (r'\bdogecoin-core\b', 'fleetcredits-core', 'Package name'),
    (r'\bDogecoin Core\b', 'Fleet Credits Core', 'Package display name'),
    (r'\bDOGECOIN\b', 'FLEETCREDITS', 'Package uppercase'),
    (r'\bdogecoin\b', 'fleetcredits', 'Package lowercase (be careful)'),
    
    # Currency codes
    (r'\bDOGE\b', 'FC', 'Currency code uppercase'),
    (r'\bDoge\b', 'FC', 'Currency code mixed'),
    (r'\bdoge\b', 'fc', 'Currency code lowercase'),
    
    # BTC references
    (r'\bBTC\b', 'FC', 'Bitcoin currency code'),
    (r'\bBitcoin\b', 'Fleet Credits', 'Bitcoin name'),
    (r'\bbitcoin\b', 'fleetcredits', 'Bitcoin lowercase'),
    (r'\bbitcoind\b', 'fleetcreditsd', 'Bitcoin daemon'),
    (r'\bbitcoin-cli\b', 'fleetcredits-cli', 'Bitcoin CLI'),
    (r'\bbitcoin-qt\b', 'fleetcredits-qt', 'Bitcoin QT'),
    
    # Config includes
    (r'config/dogecoin-config\.h', 'config/fleetcredits-config.h', 'Config include'),
    (r'config/bitcoin-config\.h', 'config/fleetcredits-config.h', 'Bitcoin config include'),
    
    # File references
    (r'dogecoin\.conf', 'fleetcredits.conf', 'Config file reference'),
    (r'dogecoind\.pid', 'fleetcreditsd.pid', 'PID file reference'),
    (r'\.dogecoin', '.fleetcredits', 'Data directory'),
    
    # Library names
    (r'libdogecoin', 'libfleetcredits', 'Library prefix'),
    (r'dogecoinconsensus', 'fleetcreditsconsensus', 'Consensus library'),
    
    # Function/class names
    (r'GetDogecoin', 'GetFleetCredits', 'Function prefix'),
    (r'CDogecoin', 'CFleetCredits', 'Class prefix'),
    
    # Macros/defines
    (r'DOGECOIN_CONFIG_H', 'FLEETCREDITS_CONFIG_H', 'Config header guard'),
    (r'BITCOIN_CONFIG_H', 'FLEETCREDITS_CONFIG_H', 'Bitcoin config guard'),
    (r'BUILD_DOGECOIN_INTERNAL', 'BUILD_FLEETCREDITS_INTERNAL', 'Build macro'),
    (r'ALLOW_DOGECOIN_EXPERIMENTAL', 'ALLOW_FLEETCREDITS_EXPERIMENTAL', 'Experimental macro'),
    
    # URLs
    (r'github\.com/dogecoin/dogecoin', 'github.com/fleetcredits/fleetcredits', 'GitHub URL'),
    (r'dogecoin\.com', 'fleetcredits.org', 'Website URL'),
    (r'github\.com/bitcoin/bitcoin', 'github.com/fleetcredits/fleetcredits', 'Bitcoin GitHub'),
    
    # Client name
    (r'"Shibetoshi"', '"Fleet Credits"', 'Client name'),
    (r'Shibetoshi', 'Fleet Credits', 'Client name var'),
]

def should_rename_file(filepath):
    """Check if file should be renamed"""
    name = filepath.name
    # Skip if already renamed
    if 'fleetcredits' in name.lower() and 'dogecoin' not in name.lower():
        return False, None
    
    # Check against rename patterns
    for old_pattern, new_pattern, desc in FILE_RENAMES:
        if re.search(old_pattern, name):
            new_name = re.sub(old_pattern, new_pattern, name)
            return True, new_name
    
    return False, None

def rename_file(filepath, new_name):
    """Rename a file"""
    try:
        new_path = filepath.parent / new_name
        if new_path.exists():
            print(f"  ⚠ Skipping: {new_name} already exists")
            return False
        filepath.rename(new_path)
        return True
    except Exception as e:
        print(f"  ✗ Error renaming {filepath.name}: {e}")
        return False

def convert_file_content(filepath):
    """Convert content of a file"""
    try:
        # Skip binary files
        if filepath.suffix in ['.png', '.ico', '.icns', '.qm', '.o', '.a', '.lo']:
            return []
        
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        original = content
        changes = []
        
        for pattern, replacement, desc in CONTENT_REPLACEMENTS:
            try:
                new_content = re.sub(pattern, replacement, content)
                if new_content != content:
                    changes.append(desc)
                    content = new_content
            except re.error:
                pass
        
        if content != original:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(content)
            return changes
        return []
    
    except (UnicodeDecodeError, PermissionError, OSError):
        return []
    except Exception as e:
        return []

def main():
    base_dir = Path('fleetcredits').resolve()
    
    if not base_dir.exists():
        print(f"Error: {base_dir} not found")
        return
    
    print(f"Fleet Credits Comprehensive Conversion")
    print(f"Base directory: {base_dir}\n")
    
    # Phase 1: Rename files
    print("Phase 1: Renaming files...")
    renamed_count = 0
    
    for filepath in base_dir.rglob('*'):
        if not filepath.is_file():
            continue
        
        should_rename, new_name = should_rename_file(filepath)
        if should_rename:
            if rename_file(filepath, new_name):
                print(f"  [OK] Renamed: {filepath.name} -> {new_name}")
                renamed_count += 1
    
    print(f"\nRenamed {renamed_count} files\n")
    
    # Phase 2: Convert content
    print("Phase 2: Converting file content...")
    converted_count = 0
    
    for filepath in base_dir.rglob('*'):
        if not filepath.is_file():
            continue
        
        changes = convert_file_content(filepath)
        if changes:
            converted_count += 1
            rel_path = filepath.relative_to(base_dir)
            print(f"  [OK] {rel_path}")
            for change in changes[:3]:  # Show first 3 changes
                print(f"    - {change}")
            if len(changes) > 3:
                print(f"    ... and {len(changes) - 3} more")
    
    print(f"\nConverted content in {converted_count} files")
    print(f"\nConversion complete!")

if __name__ == '__main__':
    main()

