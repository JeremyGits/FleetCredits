#!/usr/bin/env python3
"""
Fleet Credits Conversion Script
Systematic find/replace for branding conversion
"""

import os
import re
from pathlib import Path

# Define replacements: (pattern, replacement, description)
REPLACEMENTS = [
    # Binary names
    (r'\bdogecoind\b', 'fleetcreditsd', 'Binary: daemon'),
    (r'\bdogecoin-cli\b', 'fleetcredits-cli', 'Binary: CLI'),
    (r'\bdogecoin-qt\b', 'fleetcredits-qt', 'Binary: QT GUI'),
    (r'\bdogecoin-tx\b', 'fleetcredits-tx', 'Binary: tx tool'),
    
    # Package names
    (r'\bdogecoin-core\b', 'fleetcredits-core', 'Package name'),
    (r'\bdogecoin\b', 'fleetcredits', 'Package name lowercase'),
    
    # Currency codes
    (r'\bDOGE\b', 'FC', 'Currency code uppercase'),
    (r'\bdoge\b', 'fc', 'Currency code lowercase'),
    
    # Display names
    (r'\bDogecoin\b', 'Fleet Credits', 'Display name'),
    (r'\bDOGECOIN\b', 'FLEET CREDITS', 'Display name uppercase'),
    
    # Config file names
    (r'dogecoin\.conf', 'fleetcredits.conf', 'Config file'),
    (r'dogecoin-config\.h', 'fleetcredits-config.h', 'Config header'),
    
    # Paths
    (r'/\.dogecoin/', '/.fleetcredits/', 'Data directory'),
    (r'\\\.dogecoin\\', '\\.fleetcredits\\', 'Data directory Windows'),
    
    # Copyright
    (r'Copyright \(c\) 2022 The Dogecoin Core developers', 
     'Copyright (c) 2022-2024 The Dogecoin Core developers\n// Copyright (c) 2025-2026 The Fleet Credits Core developers',
     'Copyright notice'),
]

# Files to skip (binary, generated, etc.)
SKIP_PATTERNS = [
    r'\.o$', r'\.a$', r'\.lo$', r'\.la$',
    r'\.pyc$', r'\.pyo$', r'__pycache__',
    r'\.git', r'\.svn', r'\.DS_Store',
    r'moc_', r'ui_', r'\.moc$',
    r'\.qrc$', r'\.qm$',
]

# Extensions to process
PROCESS_EXTENSIONS = {
    '.cpp', '.h', '.hpp', '.c', '.cc',
    '.md', '.txt', '.rst',
    '.ac', '.am', '.in', '.sh',
    '.json', '.cmake', '.pro',
    '.rc', '.ico', '.png', '.svg',
}

def should_process_file(filepath):
    """Check if file should be processed"""
    path_str = str(filepath)
    
    # Skip hidden files
    if any(part.startswith('.') for part in filepath.parts):
        if '.git' in path_str or '.svn' in path_str:
            return False
    
    # Skip binary/generated patterns
    for pattern in SKIP_PATTERNS:
        if re.search(pattern, path_str, re.IGNORECASE):
            return False
    
    # Check extension
    if filepath.suffix.lower() in PROCESS_EXTENSIONS:
        return True
    
    # Check if no extension but might be script
    if not filepath.suffix and filepath.name in ['configure', 'Makefile', 'INSTALL']:
        return True
    
    return False

def convert_file(filepath):
    """Convert a single file"""
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        original_content = content
        changes = []
        
        for pattern, replacement, description in REPLACEMENTS:
            try:
                new_content = re.sub(pattern, replacement, content)
                if new_content != content:
                    changes.append(description)
                    content = new_content
            except re.error as e:
                # Skip files with regex issues (like binary files or special formats)
                pass
        
        if content != original_content:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(content)
            return changes
        return []
    
    except (UnicodeDecodeError, PermissionError, OSError) as e:
        # Skip binary files, locked files, etc.
        return []
    except Exception as e:
        # Only print unexpected errors
        if "bad escape" not in str(e).lower():
            print(f"Error processing {filepath}: {e}")
        return []

def main():
    # Get script directory and find fleetcredits root
    script_dir = Path(__file__).parent.absolute()
    # Script is in fleetcredits/devtesting/scripts/conversion/
    # So base_dir is 4 levels up
    base_dir = script_dir.parent.parent.parent.parent
    
    # If we're running from within fleetcredits directory, use current directory
    if Path.cwd().name == 'fleetcredits' and Path('src').exists():
        base_dir = Path.cwd()
    # If base_dir doesn't exist or isn't fleetcredits, try current directory
    elif not base_dir.exists() or base_dir.name != 'fleetcredits':
        if Path('fleetcredits').exists():
            base_dir = Path('fleetcredits')
        elif Path('src').exists():
            base_dir = Path.cwd()
        else:
            print(f"Error: Cannot find fleetcredits directory")
            print(f"Current directory: {Path.cwd()}")
            print(f"Script directory: {script_dir}")
            print(f"Attempted base_dir: {base_dir}")
            return
    
    if not base_dir.exists():
        print(f"Error: {base_dir} directory not found")
        print(f"Current directory: {Path.cwd()}")
        print(f"Script directory: {script_dir}")
        return
    
    print(f"Using base directory: {base_dir}")
    print(f"Current working directory: {Path.cwd()}")
    
    total_files = 0
    converted_files = 0
    
    for filepath in base_dir.rglob('*'):
        try:
            if not filepath.is_file():
                continue
        except (OSError, PermissionError):
            # Skip files we can't access
            continue
        
        if not should_process_file(filepath):
            continue
        
        total_files += 1
        changes = convert_file(filepath)
        
        if changes:
            converted_files += 1
            rel_path = filepath.relative_to(base_dir)
            print(f"✓ {rel_path}")
            for change in changes:
                print(f"  - {change}")
    
    print(f"\nConversion complete:")
    print(f"  Total files processed: {total_files}")
    print(f"  Files modified: {converted_files}")

if __name__ == '__main__':
    main()

