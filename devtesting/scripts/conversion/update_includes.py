#!/usr/bin/env python3
"""
Fleet Credits Include Updater
Updates all #include statements from dogecoin* to fleetcredits* naming convention
"""

import os
import re
import sys

# Mapping of old includes to new includes
INCLUDE_MAPPINGS = {
    'dogecoin.h': 'fleetcredits.h',
    'dogecoin-fees.h': 'fleetcredits-fees.h',
    'dogecoin.cpp': 'fleetcredits.cpp',
    'dogecoingui.h': 'fleetcreditsgui.h',
    'dogecoinunits.h': 'fleetcreditsunits.h',
    'dogecoinaddressvalidator.h': 'fleetcreditsaddressvalidator.h',
    'dogecoinamountfield.h': 'fleetcreditsamountfield.h',
    'dogecoinstrings.cpp': 'fleetcreditsstrings.cpp',
    'dogecoin-config.h': 'fleetcredits-config.h',
}

def update_includes_in_file(filepath):
    """Update includes in a single file"""
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        original_content = content
        updated = False
        
        # Update each include pattern
        for old_include, new_include in INCLUDE_MAPPINGS.items():
            # Pattern for #include "filename" or #include <filename>
            pattern = r'(#include\s+["<])(' + re.escape(old_include) + r')([">])'
            replacement = r'\1' + new_include + r'\3'
            
            new_content = re.sub(pattern, replacement, content)
            if new_content != content:
                content = new_content
                updated = True
        
        if updated:
            with open(filepath, 'w', encoding='utf-8') as f:
                f.write(content)
            return True
        return False
    except Exception as e:
        print(f"Error processing {filepath}: {e}", file=sys.stderr)
        return False

def main():
    if len(sys.argv) < 2:
        base_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
        src_dir = os.path.join(base_dir, 'src')
    else:
        src_dir = sys.argv[1]
    
    if not os.path.exists(src_dir):
        print(f"Error: Directory not found: {src_dir}", file=sys.stderr)
        sys.exit(1)
    
    updated_count = 0
    processed_count = 0
    
    # Process all .cpp and .h files
    for root, dirs, files in os.walk(src_dir):
        # Skip build directories
        if '.deps' in root or '.o' in root or '.moc' in root:
            continue
        
        for filename in files:
            if filename.endswith(('.cpp', '.h')):
                filepath = os.path.join(root, filename)
                processed_count += 1
                if update_includes_in_file(filepath):
                    updated_count += 1
                    print(f"Updated: {filepath}")
    
    print(f"\nProcessed {processed_count} files, updated {updated_count} files")

if __name__ == '__main__':
    main()

