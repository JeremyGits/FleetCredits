# Fleet Credits Documentation Pages Generator
# This script generates all remaining documentation pages

import os
from pathlib import Path

# Read sidebar template from existing page
def get_sidebar_template():
    arch_file = Path('docserver/pages/architecture.html')
    if arch_file.exists():
        content = arch_file.read_text(encoding='utf-8')
        start = content.find('<aside class="sidebar">')
        end = content.find('</aside>') + len('</aside>')
        if start != -1:
            return content[start:end]
    return None

def create_page(filename, title, active_item, content_html):
    """Create a documentation page"""
    sidebar = get_sidebar_template()
    if not sidebar:
        print(f"Error: Could not get sidebar template")
        return False
    
    # Update active item in sidebar
    sidebar_lines = sidebar.split('\n')
    for i, line in enumerate(sidebar_lines):
        if f'href="{active_item}"' in line and 'class="nav-item"' in line:
            sidebar_lines[i] = line.replace('class="nav-item"', 'class="nav-item active"')
            # Remove active from other items
            for j, other_line in enumerate(sidebar_lines):
                if j != i and 'class="nav-item active"' in other_line:
                    sidebar_lines[j] = other_line.replace(' active', '')
    sidebar = '\n'.join(sidebar_lines)
    
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

{content_html}
        </main>
    </div>

    <script src="../assets/js/main.js"></script>
</body>
</html>'''
    
    filepath = Path(f'docserver/pages/{filename}')
    filepath.write_text(html, encoding='utf-8')
    return True

# Page definitions - I'll create key ones now, you can expand this
pages_to_create = [
    {
        'filename': 'building.html',
        'title': 'Building & Setup',
        'active': 'building.html',
        'content': '''            <section class="content-section">
                <h2>Building Fleet Credits Core</h2>
                <h3>Prerequisites</h3>
                <ul>
                    <li>GCC 7+ or Clang 5+</li>
                    <li>Autotools (autoconf, automake, libtool)</li>
                    <li>pkg-config</li>
                    <li>Boost libraries (1.64+)</li>
                    <li>OpenSSL</li>
                    <li>Qt 5.x (for GUI)</li>
                </ul>

                <h3>Linux Build</h3>
                <pre><code>./autogen.sh
./configure
make -j$(nproc)</code></pre>

                <h3>Windows Build</h3>
                <p>See <a href="windows-build.html">Windows Build Guide</a> for details.</p>

                <h3>Binaries</h3>
                <ul>
                    <li><code>fleetcreditsd</code> - Core daemon</li>
                    <li><code>fleetcredits-qt</code> - Qt GUI client</li>
                    <li><code>fleetcredits-cli</code> - Command-line interface</li>
                    <li><code>fleetcredits-tx</code> - Transaction utility</li>
                </ul>
            </section>'''
    },
    # Add more page definitions here...
]

if __name__ == '__main__':
    pages_dir = Path('docserver/pages')
    pages_dir.mkdir(parents=True, exist_ok=True)
    
    print(f"Generating {len(pages_to_create)} documentation pages...")
    
    for page_def in pages_to_create:
        if create_page(page_def['filename'], page_def['title'], page_def['active'], page_def['content']):
            print(f"Generated {page_def['filename']}")
        else:
            print(f"Failed to generate {page_def['filename']}")
    
    print(f"\nDone! Pages generated in {pages_dir}")

