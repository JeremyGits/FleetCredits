#!/usr/bin/env python3
"""
Fleet Credits Content-Only Conversion Script
Updates all text content without renaming files
Handles: bitcoin/dogecoin → fleetcredits, BTC/DOGE → FC
"""

import os
import re
from pathlib import Path

# Content replacements - order matters!
REPLACEMENTS = [
    # Currency codes first (before word boundaries break them)
    (r'\bDOGE\b', 'FC', 'Currency: DOGE -> FC'),
    (r'\bDoge\b', 'FC', 'Currency: Doge -> FC'),
    (r'\bdoge\b', 'fc', 'Currency: doge -> fc'),
    (r'\bBTC\b', 'FC', 'Currency: BTC -> FC'),
    
    # Binary names
    (r'\bdogecoind\b', 'fleetcreditsd', 'Binary: daemon'),
    (r'\bdogecoin-cli\b', 'fleetcredits-cli', 'Binary: CLI'),
    (r'\bdogecoin-qt\b', 'fleetcredits-qt', 'Binary: QT'),
    (r'\bdogecoin-tx\b', 'fleetcredits-tx', 'Binary: TX'),
    (r'\bbitcoind\b', 'fleetcreditsd', 'Binary: bitcoin daemon'),
    (r'\bbitcoin-cli\b', 'fleetcredits-cli', 'Binary: bitcoin CLI'),
    (r'\bbitcoin-qt\b', 'fleetcredits-qt', 'Binary: bitcoin QT'),
    
    # Package/library names
    (r'\bDogecoin Core\b', 'Fleet Credits Core', 'Package display'),
    (r'\bDOGECOIN\b', 'FLEETCREDITS', 'Package uppercase'),
    (r'\bBitcoin Core\b', 'Fleet Credits Core', 'Bitcoin package'),
    (r'\bBITCOIN\b', 'FLEETCREDITS', 'Bitcoin uppercase'),
    
    # Display names (after binaries to avoid partial matches)
    (r'\bDogecoin\b', 'Fleet Credits', 'Display name'),
    (r'\bBitcoin\b', 'Fleet Credits', 'Bitcoin display'),
    
    # Lowercase (be careful - use word boundaries)
    (r'\bdogecoin\b', 'fleetcredits', 'Package lowercase'),
    (r'\bbitcoin\b', 'fleetcredits', 'Bitcoin lowercase'),
    
    # Config includes
    (r'config/dogecoin-config\.h', 'config/fleetcredits-config.h', 'Config include'),
    (r'config/bitcoin-config\.h', 'config/fleetcredits-config.h', 'Bitcoin config'),
    
    # File references
    (r'dogecoin\.conf', 'fleetcredits.conf', 'Config file'),
    (r'dogecoind\.pid', 'fleetcreditsd.pid', 'PID file'),
    (r'\.dogecoin', '.fleetcredits', 'Data directory'),
    
    # Library/function names
    (r'libdogecoin', 'libfleetcredits', 'Library prefix'),
    (r'dogecoinconsensus', 'fleetcreditsconsensus', 'Consensus library'),
    (r'GetDogecoin', 'GetFleetCredits', 'Function prefix'),
    
    # Macros
    (r'DOGECOIN_CONFIG_H', 'FLEETCREDITS_CONFIG_H', 'Config macro'),
    (r'BITCOIN_CONFIG_H', 'FLEETCREDITS_CONFIG_H', 'Bitcoin config macro'),
    (r'BUILD_DOGECOIN_INTERNAL', 'BUILD_FLEETCREDITS_INTERNAL', 'Build macro'),
    (r'BUILD_BITCOIN_INTERNAL', 'BUILD_FLEETCREDITS_INTERNAL', 'Bitcoin build macro'),
    
    # URLs
    (r'github\.com/dogecoin/dogecoin', 'github.com/fleetcredits/fleetcredits', 'GitHub URL'),
    (r'dogecoin\.com', 'fleetcredits.org', 'Website'),
    (r'github\.com/bitcoin/bitcoin', 'github.com/fleetcredits/fleetcredits', 'Bitcoin GitHub'),
    
    # Client name
    (r'"Shibetoshi"', '"Fleet Credits"', 'Client name string'),
    (r'Shibetoshi', 'Fleet Credits', 'Client name var'),
    (r'bitcoin-core', 'fleetcredits-core', 'Translation context'),
]

SKIP_PATTERNS = [
    r'\.o$', r'\.a$', r'\.lo$', r'\.la$',
    r'\.pyc$', r'\.qm$', r'\.qrc$',
    r'moc_', r'ui_', r'\.moc$',
    r'\.git', r'\.svn',
]

PROCESS_EXTENSIONS = {
    '.cpp', '.h', '.hpp', '.c', '.cc',
    '.md', '.txt', '.rst',
    '.ac', '.am', '.in', '.sh',
    '.json', '.cmake', '.pro', '.rc',
    '.ts', '.ui',
}

def should_process(filepath):
    if not filepath.is_file():
        return False
    
    name = str(filepath)
    for pattern in SKIP_PATTERNS:
        if re.search(pattern, name, re.IGNORECASE):
            return False
    
    if filepath.suffix.lower() in PROCESS_EXTENSIONS:
        return True
    
    return False

def convert_content(filepath):
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        original = content
        changes = []
        
        for pattern, replacement, desc in REPLACEMENTS:
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
    except Exception:
        return []

def main():
    # Find base_dir by looking for fleetcredits directory relative to script location
    script_dir = Path(__file__).resolve().parent
    # Go up from devtesting/scripts/conversion to fleetcredits root
    base_dir = script_dir.parent.parent.parent.resolve()
    
    if not base_dir.exists() or base_dir.name != 'fleetcredits':
        # Fallback: try current directory
        cwd = Path.cwd()
        if (cwd / 'src').exists() and (cwd / 'configure.ac').exists():
            base_dir = cwd
        else:
            print(f"Error: Could not find fleetcredits root directory")
            print(f"  Tried: {script_dir.parent.parent.parent}")
            print(f"  Current dir: {cwd}")
            return
    
    print(f"Fleet Credits Content Conversion")
    print(f"Base: {base_dir}\n")
    
    converted = 0
    total = 0
    
    for filepath in base_dir.rglob('*'):
        if not should_process(filepath):
            continue
        
        total += 1
        changes = convert_content(filepath)
        
        if changes:
            converted += 1
            rel = filepath.relative_to(base_dir)
            print(f"[OK] {rel}")
            if len(changes) <= 5:
                for c in changes:
                    print(f"     - {c}")
            else:
                for c in changes[:3]:
                    print(f"     - {c}")
                print(f"     ... and {len(changes)-3} more")
    
    print(f"\nProcessed {total} files")
    print(f"Modified {converted} files")
    print("\nDone!")

if __name__ == '__main__':
    main()

