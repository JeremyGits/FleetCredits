#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Fleet Credits Documentation Generator
Generates all documentation pages from codebase structure
"""

import os
import sys
from pathlib import Path

# Set UTF-8 encoding for stdout
if sys.platform == 'win32':
    sys.stdout.reconfigure(encoding='utf-8')

# Common sidebar (will be inserted into each page)
# We'll read from existing architecture.html and extract the sidebar pattern

def get_sidebar_template():
    """Get sidebar template from existing page"""
    arch_file = Path('docserver/pages/architecture.html')
    if arch_file.exists():
        content = arch_file.read_text(encoding='utf-8')
        # Extract sidebar section
        start = content.find('<aside class="sidebar">')
        end = content.find('</aside>') + len('</aside>')
        if start != -1 and end != -1:
            return content[start:end]
    return None

def update_sidebar_active(sidebar, active_item):
    """Update sidebar to mark active item"""
    # Replace active class for the specified item
    sidebar = sidebar.replace(f'href="{active_item}" class="nav-item"', 
                             f'href="{active_item}" class="nav-item active"')
    # Remove active from all other items
    sidebar = re.sub(r'href="[^"]*" class="nav-item active"', 
                    lambda m: m.group(0).replace(' active', '') if active_item not in m.group(0) else m.group(0),
                    sidebar)
    return sidebar

import re

def generate_page(filename, title, active_item, content):
    """Generate a documentation page"""
    sidebar_template = get_sidebar_template()
    if not sidebar_template:
        print("Error: Could not find sidebar template. Please ensure architecture.html exists.")
        return None
    
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
    
    return html

# Page content definitions
PAGES = {
    'network-params.html': {
        'title': 'Network Parameters',
        'active': 'network-params.html',
        'content': '''            <section class="content-section">
                <h2>Network Configuration</h2>
                <p>Fleet Credits operates on three networks: Mainnet, Testnet, and Regtest.</p>

                <h3>Mainnet</h3>
                <table>
                    <tr><th>Parameter</th><th>Value</th></tr>
                    <tr><td>P2P Port</td><td>22556</td></tr>
                    <tr><td>RPC Port</td><td>22555</td></tr>
                    <tr><td>Message Start</td><td>0xfc, 0xc1, 0xb7, 0xdc</td></tr>
                    <tr><td>Genesis Timestamp</td><td>January 1, 2026 00:00:00 UTC</td></tr>
                    <tr><td>Block Time</td><td>60 seconds</td></tr>
                    <tr><td>Block Reward</td><td>10,000 FC (fixed)</td></tr>
                </table>

                <h3>Testnet</h3>
                <table>
                    <tr><th>Parameter</th><th>Value</th></tr>
                    <tr><td>P2P Port</td><td>44556</td></tr>
                    <tr><td>RPC Port</td><td>44555</td></tr>
                    <tr><td>Message Start</td><td>0xfc, 0xc1, 0xb7, 0xdc</td></tr>
                    <tr><td>Genesis Timestamp</td><td>January 1, 2026 00:00:00 UTC</td></tr>
                    <tr><td>Block Time</td><td>60 seconds</td></tr>
                </table>

                <h3>Regtest</h3>
                <table>
                    <tr><th>Parameter</th><th>Value</th></tr>
                    <tr><td>P2P Port</td><td>18444</td></tr>
                    <tr><td>RPC Port</td><td>18332</td></tr>
                    <tr><td>Difficulty</td><td>Minimum (instant blocks)</td></tr>
                </table>

                <h3>Base58 Address Prefixes</h3>
                <table>
                    <tr><th>Type</th><th>Mainnet</th><th>Testnet</th></tr>
                    <tr><td>Pubkey Address</td><td>0x23</td><td>0x71</td></tr>
                    <tr><td>Script Address</td><td>0x16</td><td>0xc4</td></tr>
                    <tr><td>Secret Key</td><td>0x9e</td><td>0xf1</td></tr>
                </table>

                <h3>Configuration File</h3>
                <p>Edit <code>~/.fleetcredits/fleetcredits.conf</code>:</p>
                <pre><code>rpcuser=your_username
rpcpassword=your_password
rpcport=22555
rpcallowip=127.0.0.1
server=1</code></pre>
            </section>'''
    },
}

if __name__ == '__main__':
    pages_dir = Path('docserver/pages')
    pages_dir.mkdir(parents=True, exist_ok=True)
    
    print(f"Generating {len(PAGES)} documentation pages...")
    
    for filename, page_data in PAGES.items():
        html = generate_page(filename, page_data['title'], page_data['active'], page_data['content'])
        if html:
            filepath = pages_dir / filename
            filepath.write_text(html, encoding='utf-8')
            print(f"Generated {filename}")
    
    print(f"\nAll pages generated in {pages_dir}")
