#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Fleet Credits Documentation Generator
Generates all remaining documentation pages
"""

import re
from pathlib import Path

def get_sidebar_template():
    """Extract sidebar from existing page"""
    arch_file = Path('docserver/pages/architecture.html')
    if arch_file.exists():
        content = arch_file.read_text(encoding='utf-8')
        start = content.find('<aside class="sidebar">')
        end = content.find('</aside>') + len('</aside>')
        if start != -1:
            return content[start:end]
    return None

def update_sidebar_active(sidebar, active_item):
    """Mark active item in sidebar"""
    # Remove all active classes
    sidebar = re.sub(r'class="nav-item active"', 'class="nav-item"', sidebar)
    # Add active to specified item
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

# All pages to create
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
make -j$(nproc)

# Install (optional)
sudo make install</code></pre>

                <h3>macOS Build</h3>
                <pre><code># Install dependencies via Homebrew
brew install autoconf automake libtool pkg-config boost openssl berkeley-db4 qt5

# Build
./autogen.sh
./configure
make -j$(sysctl -n hw.ncpu)</code></pre>

                <h3>Windows Build</h3>
                <p>See <a href="windows-build.html">Windows Build Guide</a> for detailed instructions.</p>
                <p>Basic steps:</p>
                <ol>
                    <li>Install MSYS2</li>
                    <li>Install build dependencies via pacman</li>
                    <li>Run autogen.sh and configure</li>
                    <li>Build with make</li>
                </ol>

                <h3>Binaries</h3>
                <p>After successful build, you'll find:</p>
                <ul>
                    <li><code>src/fleetcreditsd</code> - Core daemon</li>
                    <li><code>src/qt/fleetcredits-qt</code> - Qt GUI client</li>
                    <li><code>src/fleetcredits-cli</code> - Command-line interface</li>
                    <li><code>src/fleetcredits-tx</code> - Transaction utility</li>
                </ul>

                <h3>First Run</h3>
                <pre><code># Create data directory
mkdir -p ~/.fleetcredits

# Create config file
cat > ~/.fleetcredits/fleetcredits.conf << EOF
rpcuser=your_username
rpcpassword=your_password
rpcport=22555
server=1
rpcallowip=127.0.0.1
EOF

# Start daemon
./fleetcreditsd

# Or start GUI
./fleetcredits-qt</code></pre>
            </section>'''
    },
    {
        'filename': 'contribution-types.html',
        'title': 'Contribution Types',
        'active': 'contribution-types.html',
        'content': '''            <section class="content-section">
                <h2>All Contribution Types</h2>
                <p>Fleet Credits supports 8 types of contributions, each with specific verification requirements and reward structures.</p>

                <div class="api-endpoint">
                    <h3>1. CODE_CONTRIBUTION (0x01)</h3>
                    <p><strong>Description:</strong> Open-source code commits to public repositories.</p>
                    <p><strong>Proof Data:</strong> GitHub commit hash, GitLab commit hash, or similar.</p>
                    <p><strong>Verification:</strong> Automated via GitHub API or similar service.</p>
                    <p><strong>Bonus Level:</strong> LOW to HIGH based on code quality and impact.</p>
                    <p><strong>MWEB Required:</strong> No</p>
                </div>

                <div class="api-endpoint">
                    <h3>2. CHARITABLE_ACT (0x02)</h3>
                    <p><strong>Description:</strong> Verified charitable donations or community service.</p>
                    <p><strong>Proof Data:</strong> Receipt hash, attestation, or verification link.</p>
                    <p><strong>Verification:</strong> Oracle consensus (3-of-5) required.</p>
                    <p><strong>Bonus Level:</strong> MEDIUM to HIGH based on impact.</p>
                    <p><strong>MWEB Required:</strong> No</p>
                </div>

                <div class="api-endpoint">
                    <h3>3. CREATIVE_WORK (0x03)</h3>
                    <p><strong>Description:</strong> Art, writing, music, or other creative content.</p>
                    <p><strong>Proof Data:</strong> IPFS hash of the creative work.</p>
                    <p><strong>Verification:</strong> Community validation or oracle consensus.</p>
                    <p><strong>Bonus Level:</strong> LOW to MEDIUM.</p>
                    <p><strong>MWEB Required:</strong> No</p>
                </div>

                <div class="api-endpoint">
                    <h3>4. EDUCATIONAL_CONTENT (0x04)</h3>
                    <p><strong>Description:</strong> Teaching materials, tutorials, courses, or educational resources.</p>
                    <p><strong>Proof Data:</strong> IPFS hash or URL to educational content.</p>
                    <p><strong>Verification:</strong> Automated checks for engagement metrics.</p>
                    <p><strong>Bonus Level:</strong> LOW to MEDIUM.</p>
                    <p><strong>MWEB Required:</strong> No</p>
                </div>

                <div class="api-endpoint">
                    <h3>5. CODE_MENTORSHIP (0x05)</h3>
                    <p><strong>Description:</strong> Code reviews and mentorship between mentor and mentee.</p>
                    <p><strong>Proof Data:</strong> Feedback hash, improvement commit hash, approval status.</p>
                    <p><strong>Verification:</strong> Both mentor and mentee signatures required.</p>
                    <p><strong>Bonus Level:</strong> MEDIUM to HIGH.</p>
                    <p><strong>MWEB Required:</strong> No</p>
                    <p><strong>Special Fields:</strong> mentor, mentee, mentor_signature, mentee_signature, approval_status, feedback_hash</p>
                </div>

                <div class="api-endpoint">
                    <h3>6. AI_VALIDATION (0x06)</h3>
                    <p><strong>Description:</strong> Human validation of AI model outputs.</p>
                    <p><strong>Proof Data:</strong> AI model ID, accuracy score, validation report hash.</p>
                    <p><strong>Verification:</strong> Accuracy score >= 0.8 required.</p>
                    <p><strong>Bonus Level:</strong> CRITICAL (1.25x multiplier).</p>
                    <p><strong>MWEB Required:</strong> No</p>
                    <p><strong>Special Fields:</strong> ai_model_id, accuracy_score, validation_report_hash, human_override_hash</p>
                </div>

                <div class="api-endpoint">
                    <h3>7. DATA_LABELING (0x07)</h3>
                    <p><strong>Description:</strong> Dataset labeling with inter-annotator agreement.</p>
                    <p><strong>Proof Data:</strong> Dataset ID, labels hash (IPFS), inter-annotator agreement score.</p>
                    <p><strong>Verification:</strong> Inter-annotator agreement >= 0.7, minimum 3 annotators.</p>
                    <p><strong>Bonus Level:</strong> MEDIUM to HIGH.</p>
                    <p><strong>MWEB Required:</strong> No</p>
                    <p><strong>Special Fields:</strong> dataset_id, labels_count, labels_hash, inter_annotator_agreement, annotator_count</p>
                </div>

                <div class="api-endpoint">
                    <h3>8. ETHICAL_REVIEW (0x08)</h3>
                    <p><strong>Description:</strong> Ethical review of AI systems or proposals.</p>
                    <p><strong>Proof Data:</strong> Review report hash, ethical rationale.</p>
                    <p><strong>Verification:</strong> Oracle consensus or expert review.</p>
                    <p><strong>Bonus Level:</strong> HIGH to CRITICAL.</p>
                    <p><strong>MWEB Required:</strong> Yes (mandatory for privacy)</p>
                    <p><strong>Special Fields:</strong> ethical_rationale, review_report_hash</p>
                </div>

                <h2>Contribution Structure</h2>
                <pre><code>class CContributionTransaction {
    uint256 tx_id;
    CPubKey contributor;
    ContributionType contrib_type;
    CProofData proof_data;
    uint64_t timestamp;
    uint32_t bonus_level;
    bool requires_mweb;
    vector<unsigned char> signature;
};</code></pre>
            </section>'''
    },
]

if __name__ == '__main__':
    pages_dir = Path('docserver/pages')
    pages_dir.mkdir(parents=True, exist_ok=True)
    
    print(f"Generating {len(PAGES)} documentation pages...")
    
    for page_def in PAGES:
        if create_page(page_def['filename'], page_def['title'], page_def['active'], page_def['content']):
            print(f"Generated {page_def['filename']}")
        else:
            print(f"Failed {page_def['filename']}")
    
    print(f"\nDone! Generated pages in {pages_dir}")

