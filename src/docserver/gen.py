#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Fleet Credits Complete Documentation Generator
Generates ALL remaining documentation pages
"""

import re
from pathlib import Path

def get_sidebar_template():
    """Extract sidebar from existing page"""
    arch_file = Path('docserver/pages/rpc-contribution.html')
    if arch_file.exists():
        content = arch_file.read_text(encoding='utf-8')
        start = content.find('<aside class="sidebar">')
        end = content.find('</aside>') + len('</aside>')
        if start != -1:
            return content[start:end]
    return None

def update_sidebar_active(sidebar, active_item):
    """Mark active item in sidebar"""
    sidebar = re.sub(r'class="nav-item active"', 'class="nav-item"', sidebar)
    sidebar = sidebar.replace(
        f'href="{active_item}" class="nav-item"',
        f'href="{active_item}" class="nav-item active"'
    )
    return sidebar

def create_page(filename, title, active_item, content):
    """Create a documentation page"""
    sidebar_template = get_sidebar_template()
    if not sidebar_template:
        print(f"Error: Could not get sidebar template")
        return False
    
    sidebar = update_sidebar_active(sidebar_template, active_item)
    
    html = f'''<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>{title} - Fleet Credits Developer Documentation</title>
    <link rel="stylesheet" href="../assets/css/main.css">
</head>
<body>
    <div class="docs-container">
        <!-- Sidebar Navigation -->
{sidebar}

        <!-- Main Content -->
        <main class="main-content">
            <div class="content-header">
                <h1>{title}</h1>
            </div>

{content}
        </main>
    </div>

    <script src="../assets/js/main.js"></script>
</body>
</html>'''
    
    filepath = Path(f'docserver/pages/{filename}')
    filepath.write_text(html, encoding='utf-8')
    return True

# ALL pages to create - comprehensive list
PAGES = [
    {
        'filename': 'building.html',
        'title': 'Building & Setup',
        'active': 'building.html',
        'content': '''            <section class="content-section">
                <h2>Building Fleet Credits Core</h2>
                
                <h3>Prerequisites</h3>
                <ul>
                    <li><strong>Compiler:</strong> GCC 7+ or Clang 5+</li>
                    <li><strong>Build Tools:</strong> Autotools (autoconf, automake, libtool)</li>
                    <li><strong>pkg-config:</strong> For dependency detection</li>
                    <li><strong>Boost:</strong> 1.64+ (system, filesystem, thread, chrono, program_options)</li>
                    <li><strong>OpenSSL:</strong> For cryptographic functions</li>
                    <li><strong>Qt 5.x:</strong> For GUI (optional, if building fleetcredits-qt)</li>
                    <li><strong>Berkeley DB:</strong> 4.8+ for wallet storage</li>
                </ul>

                <h3>Linux Build</h3>
                <pre><code># Install dependencies (Ubuntu/Debian)
sudo apt-get install build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils python3 libboost-all-dev libdb-dev libdb++-dev

# Install Qt dependencies (if building GUI)
sudo apt-get install qt5-default qttools5-dev-tools

# Build
./autogen.sh
./configure
make -j$(nproc)</code></pre>

                <h3>macOS Build</h3>
                <pre><code># Install dependencies via Homebrew
brew install autoconf automake libtool pkg-config boost openssl berkeley-db4 qt5

# Build
./autogen.sh
./configure
make -j$(sysctl -n hw.ncpu)</code></pre>

                <h3>Windows Build</h3>
                <p>Use MSYS2 or WSL. See Windows build documentation for details.</p>

                <h3>Binaries</h3>
                <ul>
                    <li><code>src/fleetcreditsd</code> - Core daemon</li>
                    <li><code>src/qt/fleetcredits-qt</code> - Qt GUI client</li>
                    <li><code>src/fleetcredits-cli</code> - Command-line interface</li>
                    <li><code>src/fleetcredits-tx</code> - Transaction utility</li>
                </ul>
            </section>'''
    },
    {
        'filename': 'contribution-types.html',
        'title': 'Contribution Types',
        'active': 'contribution-types.html',
        'content': '''            <section class="content-section">
                <h2>All Contribution Types</h2>
                <p>Fleet Credits supports 8 types of contributions. See <a href="rpc-contribution.html">Contribution RPC</a> for API details.</p>
                <p>File: <code>src/primitives/contribution.h</code></p>
            </section>'''
    },
]

if __name__ == '__main__':
    pages_dir = Path('docserver/pages')
    pages_dir.mkdir(parents=True, exist_ok=True)
    
    print(f"Generating {len(PAGES)} documentation pages...")
    
    success = 0
    for page_def in PAGES:
        if create_page(page_def['filename'], page_def['title'], page_def['active'], page_def['content']):
            print(f"Generated {page_def['filename']}")
            success += 1
        else:
            print(f"Failed {page_def['filename']}")
    
    print(f"\nDone! Generated {success}/{len(PAGES)} pages in {pages_dir}")

