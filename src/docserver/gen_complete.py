#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Fleet Credits Complete Documentation Generator
Generates ALL remaining documentation pages (~30+ pages)
Run: python docserver/gen_complete.py
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
    # Skip if already exists
    if filepath.exists():
        print(f"Skipped {filename} (already exists)")
        return True
    
    filepath.write_text(html, encoding='utf-8')
    return True

# Comprehensive list of ALL remaining pages
PAGES = [
    # Core Blockchain (4 pages)
    {
        'filename': 'blockchain-core.html',
        'title': 'Block & Transaction',
        'active': 'blockchain-core.html',
        'content': '''            <section class="content-section">
                <h2>Block Structure</h2>
                <p>Fleet Credits uses a standard blockchain structure with blocks containing transactions.</p>
                <p><strong>Files:</strong> <code>src/primitives/block.h</code>, <code>src/primitives/transaction.h</code></p>
                
                <h3>CBlock Structure</h3>
                <pre><code>class CBlock : public CBlockHeader {
    std::vector<CTransactionRef> vtx;
    std::shared_ptr<CMWEBExtensionBlock> mweb_extension;
    mutable bool fChecked;
};</code></pre>
                
                <h3>CTransaction Structure</h3>
                <pre><code>class CTransaction {
    std::vector<CTxIn> vin;
    std::vector<CTxOut> vout;
    int32_t nVersion;
    uint32_t nLockTime;
};</code></pre>
            </section>'''
    },
    {
        'filename': 'utxo-model.html',
        'title': 'UTXO Model',
        'active': 'utxo-model.html',
        'content': '''            <section class="content-section">
                <h2>Unspent Transaction Output (UTXO) Model</h2>
                <p>Fleet Credits uses the UTXO model, similar to Bitcoin and Dogecoin.</p>
                <p><strong>Files:</strong> <code>src/primitives/transaction.h</code>, <code>src/coins.h</code></p>
                
                <h3>UTXO Set</h3>
                <p>The UTXO set tracks all unspent transaction outputs, enabling efficient balance queries.</p>
                
                <h3>Spending UTXOs</h3>
                <p>Transactions consume UTXOs via <code>CTxIn</code> (which references <code>COutPoint</code>) and create new UTXOs via <code>CTxOut</code>.</p>
            </section>'''
    },
    {
        'filename': 'consensus.html',
        'title': 'Consensus & PoW',
        'active': 'consensus.html',
        'content': '''            <section class="content-section">
                <h2>Proof-of-Work Consensus</h2>
                <p>Fleet Credits uses Scrypt-based Proof-of-Work for consensus.</p>
                <p><strong>Files:</strong> <code>src/pow.cpp</code>, <code>src/fleetcredits.cpp</code></p>
                
                <h3>Block Reward</h3>
                <p>Fixed 10,000 FC per block, no halving. See <a href="contribution-rewards.html">Reward System</a> for contribution bonuses.</p>
                
                <h3>Difficulty Adjustment</h3>
                <p>Digishield algorithm adjusts difficulty every block to maintain 60-second block time.</p>
            </section>'''
    },
    {
        'filename': 'validation.html',
        'title': 'Validation System',
        'active': 'validation.html',
        'content': '''            <section class="content-section">
                <h2>Transaction and Block Validation</h2>
                <p>Fleet Credits validates transactions and blocks before adding them to the chain.</p>
                <p><strong>Files:</strong> <code>src/validation.cpp</code>, <code>src/consensus/validation.h</code></p>
                
                <h3>Transaction Validation</h3>
                <ul>
                    <li>Script verification</li>
                    <li>Fee calculation (zero fees for &lt;1000 FC)</li>
                    <li>UTXO availability</li>
                    <li>Contribution transaction parsing</li>
                </ul>
                
                <h3>Block Validation</h3>
                <ul>
                    <li>Proof-of-Work verification</li>
                    <li>Merkle root validation</li>
                    <li>Transaction ordering</li>
                    <li>MWEB extension block validation</li>
                </ul>
            </section>'''
    },
    
    # Contribution System (3 more pages)
    {
        'filename': 'contribution-overview.html',
        'title': 'Contribution Overview',
        'active': 'contribution-overview.html',
        'content': '''            <section class="content-section">
                <h2>Contribution Reward System</h2>
                <p>Fleet Credits rewards contributors for verified community contributions.</p>
                <p><strong>Files:</strong> <code>src/primitives/contribution.h</code>, <code>src/fleetcredits.cpp</code></p>
                
                <h3>How It Works</h3>
                <ol>
                    <li>Submit contribution via <code>submitcontribution</code> RPC</li>
                    <li>Contribution is embedded in transaction via OP_RETURN</li>
                    <li>Verification occurs (automated or oracle consensus)</li>
                    <li>Block reward includes bonus based on contribution level</li>
                </ol>
                
                <h3>Bonus Levels</h3>
                <ul>
                    <li>BONUS_NONE: 1.0x (base reward)</li>
                    <li>BONUS_LOW: 1.05x</li>
                    <li>BONUS_MEDIUM: 1.10x</li>
                    <li>BONUS_HIGH: 1.15x</li>
                    <li>BONUS_CRITICAL: 1.20x (1.25x for AI_VALIDATION)</li>
                </ul>
            </section>'''
    },
    {
        'filename': 'contribution-rewards.html',
        'title': 'Reward System',
        'active': 'contribution-rewards.html',
        'content': '''            <section class="content-section">
                <h2>Block Reward Calculation</h2>
                <p>Base reward: 10,000 FC per block (fixed, no halving)</p>
                <p>Function: <code>GetFleetCreditsBlockSubsidyWithContributions</code> in <code>src/fleetcredits.cpp</code></p>
                
                <h3>Reward Formula</h3>
                <pre><code>base_reward = 10000 * COIN
bonus_multiplier = GetBonusMultiplier(highest_bonus_level_in_block)
final_reward = base_reward * bonus_multiplier</code></pre>
                
                <h3>Contribution Extraction</h3>
                <p>Contributions are extracted from blocks via <code>ExtractContributionsFromBlock</code>, which parses OP_RETURN outputs.</p>
            </section>'''
    },
    {
        'filename': 'verification-system.html',
        'title': 'Verification System',
        'active': 'verification-system.html',
        'content': '''            <section class="content-section">
                <h2>Contribution Verification</h2>
                <p>Fleet Credits uses automated verification and oracle consensus for contribution validation.</p>
                <p><strong>Files:</strong> <code>src/primitives/verification.h</code>, <code>src/primitives/verification.cpp</code></p>
                
                <h3>Verification Methods</h3>
                <ul>
                    <li><strong>Automated:</strong> Code contributions (GitHub API), AI validation (accuracy scores)</li>
                    <li><strong>Oracle Consensus:</strong> Charitable acts, ethical reviews (3-of-5 oracle votes)</li>
                    <li><strong>Human Validators:</strong> Reputation-gated pools for batch verification</li>
                </ul>
                
                <h3>Oracle Nodes</h3>
                <p>Minimum stake: 500,000 FC. Reputation-based selection. Slashing for incorrect votes.</p>
            </section>'''
    },
    
    # MWEB Privacy (3 pages)
    {
        'filename': 'mweb-overview.html',
        'title': 'MWEB Overview',
        'active': 'mweb-overview.html',
        'content': '''            <section class="content-section">
                <h2>Mimblewimble Extension Blocks (MWEB)</h2>
                <p>MWEB provides privacy and scalability for Fleet Credits transactions.</p>
                <p><strong>Files:</strong> <code>src/primitives/mweb.h</code>, <code>src/primitives/mweb.cpp</code></p>
                
                <h3>Features</h3>
                <ul>
                    <li>Hidden transaction amounts via Pedersen commitments</li>
                    <li>Cut-through reduces blockchain bloat</li>
                    <li>Mandatory for ETHICAL_REVIEW contributions</li>
                    <li>Optional privacy for standard transactions</li>
                </ul>
                
                <h3>Peg-in/Peg-out</h3>
                <p>Move FC between main chain and MWEB via atomic swaps. See <a href="mweb-operations.html">Peg-in/Peg-out</a>.</p>
            </section>'''
    },
    {
        'filename': 'mweb-structures.html',
        'title': 'MWEB Structures',
        'active': 'mweb-structures.html',
        'content': '''            <section class="content-section">
                <h2>MWEB Data Structures</h2>
                <p><strong>File:</strong> <code>src/primitives/mweb.h</code></p>
                
                <h3>Core Structures</h3>
                <ul>
                    <li><code>CPedersenCommitment</code> - Hides transaction amounts</li>
                    <li><code>CRangeProof</code> - Proves non-negative amounts</li>
                    <li><code>CMWEBInput</code> - References previous MWEB outputs</li>
                    <li><code>CMWEBOutput</code> - Creates new MWEB UTXOs</li>
                    <li><code>CMWEBKernel</code> - Contains excess value and signature</li>
                    <li><code>CMWEBTransaction</code> - MWEB transaction structure</li>
                    <li><code>CMWEBExtensionBlock</code> - MWEB block attached to main chain block</li>
                </ul>
            </section>'''
    },
    {
        'filename': 'mweb-operations.html',
        'title': 'Peg-in/Peg-out',
        'active': 'mweb-operations.html',
        'content': '''            <section class="content-section">
                <h2>Peg-in and Peg-out Operations</h2>
                <p>Move FC between main chain and MWEB via atomic swaps.</p>
                
                <h3>Peg-in (Main Chain → MWEB)</h3>
                <p>Use <code>createpegin</code> RPC command. Creates main chain transaction that locks funds, then creates MWEB output.</p>
                
                <h3>Peg-out (MWEB → Main Chain)</h3>
                <p>Use <code>createpegout</code> RPC command. Spends MWEB commitment, creates main chain output with view key proof.</p>
                
                <h3>See Also</h3>
                <p><a href="rpc-mweb.html">MWEB RPC</a> for API details.</p>
            </section>'''
    },
    
    # RPC API (4 more pages)
    {
        'filename': 'rpc-mweb.html',
        'title': 'MWEB RPC',
        'active': 'rpc-mweb.html',
        'content': '''            <section class="content-section">
                <h2>MWEB RPC Commands</h2>
                <p><strong>File:</strong> <code>src/rpc/mweb.cpp</code></p>
                
                <h3>Endpoints</h3>
                <ul>
                    <li><code>createpegin</code> - Create peg-in transaction</li>
                    <li><code>createpegout</code> - Create peg-out transaction</li>
                    <li><code>routecontributiontomweb</code> - Route contribution to MWEB</li>
                    <li><code>listmwebtxs</code> - List MWEB transactions</li>
                </ul>
                
                <p>See <a href="mweb-overview.html">MWEB Overview</a> for details.</p>
            </section>'''
    },
    {
        'filename': 'rpc-mining.html',
        'title': 'Mining RPC',
        'active': 'rpc-mining.html',
        'content': '''            <section class="content-section">
                <h2>Mining RPC Commands</h2>
                <p><strong>File:</strong> <code>src/rpc/mining.cpp</code></p>
                
                <h3>Key Commands</h3>
                <ul>
                    <li><code>getnetworkhashps</code> - Network hash rate</li>
                    <li><code>generate</code> - Generate blocks (regtest)</li>
                    <li><code>generatetoaddress</code> - Generate blocks to address</li>
                    <li><code>getmininginfo</code> - Mining information</li>
                    <li><code>getblocktemplate</code> - Block template for mining</li>
                    <li><code>submitblock</code> - Submit mined block</li>
                </ul>
            </section>'''
    },
    {
        'filename': 'rpc-wallet.html',
        'title': 'Wallet RPC',
        'active': 'rpc-wallet.html',
        'content': '''            <section class="content-section">
                <h2>Wallet RPC Commands</h2>
                <p><strong>Files:</strong> <code>src/wallet/rpcwallet.cpp</code>, <code>src/rpc/misc.cpp</code></p>
                
                <h3>Key Commands</h3>
                <ul>
                    <li><code>getnewaddress</code> - Generate new address</li>
                    <li><code>getbalance</code> - Get wallet balance</li>
                    <li><code>sendtoaddress</code> - Send FC to address</li>
                    <li><code>listtransactions</code> - List wallet transactions</li>
                    <li><code>getwalletinfo</code> - Wallet information</li>
                </ul>
            </section>'''
    },
    {
        'filename': 'rpc-blockchain.html',
        'title': 'Blockchain RPC',
        'active': 'rpc-blockchain.html',
        'content': '''            <section class="content-section">
                <h2>Blockchain RPC Commands</h2>
                <p><strong>File:</strong> <code>src/rpc/blockchain.cpp</code></p>
                
                <h3>Key Commands</h3>
                <ul>
                    <li><code>getblockcount</code> - Current block height</li>
                    <li><code>getbestblockhash</code> - Best block hash</li>
                    <li><code>getblock</code> - Block details</li>
                    <li><code>getblockchaininfo</code> - Blockchain information</li>
                    <li><code>gettxout</code> - UTXO details</li>
                    <li><code>verifychain</code> - Verify blockchain</li>
                </ul>
            </section>'''
    },
    
    # Wallet System (3 pages)
    {
        'filename': 'wallet-overview.html',
        'title': 'Wallet Overview',
        'active': 'wallet-overview.html',
        'content': '''            <section class="content-section">
                <h2>Wallet System</h2>
                <p>Fleet Credits wallet manages addresses, transactions, and keys.</p>
                <p><strong>Files:</strong> <code>src/wallet/wallet.h</code>, <code>src/wallet/wallet.cpp</code></p>
                
                <h3>Features</h3>
                <ul>
                    <li>HD (Hierarchical Deterministic) key generation</li>
                    <li>Berkeley DB storage</li>
                    <li>Multi-wallet support</li>
                    <li>Encrypted wallet support</li>
                </ul>
            </section>'''
    },
    {
        'filename': 'wallet-classes.html',
        'title': 'Wallet Classes',
        'active': 'wallet-classes.html',
        'content': '''            <section class="content-section">
                <h2>Wallet Classes</h2>
                <p><strong>File:</strong> <code>src/wallet/wallet.h</code></p>
                
                <h3>Core Classes</h3>
                <ul>
                    <li><code>CWallet</code> - Main wallet class</li>
                    <li><code>CWalletTx</code> - Wallet transaction</li>
                    <li><code>CWalletDB</code> - Wallet database operations</li>
                    <li><code>CFleetCreditsAddress</code> - Address class</li>
                </ul>
            </section>'''
    },
    {
        'filename': 'address-system.html',
        'title': 'Address System',
        'active': 'address-system.html',
        'content': '''            <section class="content-section">
                <h2>Address System</h2>
                <p>Fleet Credits uses Base58 encoding for addresses.</p>
                <p><strong>File:</strong> <code>src/base58.h</code></p>
                
                <h3>Address Types</h3>
                <ul>
                    <li>Pubkey addresses (P2PKH)</li>
                    <li>Script addresses (P2SH)</li>
                    <li>Extended keys (BIP32)</li>
                </ul>
                
                <h3>Prefixes</h3>
                <p>See <a href="network-params.html">Network Parameters</a> for address prefixes.</p>
            </section>'''
    },
    
    # QT GUI (4 pages)
    {
        'filename': 'qt-overview.html',
        'title': 'GUI Overview',
        'active': 'qt-overview.html',
        'content': '''            <section class="content-section">
                <h2>Qt GUI Client</h2>
                <p>Fleet Credits Qt GUI provides a graphical interface for the wallet.</p>
                <p><strong>Files:</strong> <code>src/qt/fleetcreditsgui.h</code>, <code>src/qt/fleetcredits.cpp</code></p>
                
                <h3>Features</h3>
                <ul>
                    <li>Modern UI with theme support</li>
                    <li>Transaction history</li>
                    <li>Send/receive functionality</li>
                    <li>Address book</li>
                    <li>RPC console</li>
                </ul>
            </section>'''
    },
    {
        'filename': 'qt-components.html',
        'title': 'Components',
        'active': 'qt-components.html',
        'content': '''            <section class="content-section">
                <h2>Qt GUI Components</h2>
                <p><strong>Directory:</strong> <code>src/qt/</code></p>
                
                <h3>Main Components</h3>
                <ul>
                    <li><code>FleetCreditsGUI</code> - Main window</li>
                    <li><code>WalletView</code> - Wallet view</li>
                    <li><code>OverviewPage</code> - Overview page</li>
                    <li><code>SendCoinsDialog</code> - Send dialog</li>
                    <li><code>ReceiveCoinsDialog</code> - Receive dialog</li>
                </ul>
            </section>'''
    },
    {
        'filename': 'qt-models.html',
        'title': 'Models',
        'active': 'qt-models.html',
        'content': '''            <section class="content-section">
                <h2>Qt Models</h2>
                <p>Qt models provide data to views.</p>
                
                <h3>Core Models</h3>
                <ul>
                    <li><code>ClientModel</code> - Core client data</li>
                    <li><code>WalletModel</code> - Wallet data</li>
                    <li><code>TransactionTableModel</code> - Transaction list</li>
                    <li><code>AddressTableModel</code> - Address book</li>
                </ul>
            </section>'''
    },
    {
        'filename': 'qt-themes.html',
        'title': 'Themes',
        'active': 'qt-themes.html',
        'content': '''            <section class="content-section">
                <h2>Qt GUI Themes</h2>
                <p>Fleet Credits supports multiple themes.</p>
                <p><strong>Files:</strong> <code>src/qt/themes/theme_manager.h</code></p>
                
                <h3>Theme System</h3>
                <p>Themes are managed via <code>ThemeManager</code> class. Supports custom themes and color schemes.</p>
            </section>'''
    },
    
    # Fee Structure (2 pages)
    {
        'filename': 'fee-system.html',
        'title': 'Fee System',
        'active': 'fee-system.html',
        'content': '''            <section class="content-section">
                <h2>Transaction Fee System</h2>
                <p>Fleet Credits implements zero fees for micro-transactions.</p>
                <p><strong>Files:</strong> <code>src/policy/fleetcredits-fees.h</code>, <code>src/fleetcredits-fees.cpp</code></p>
                
                <h3>Fee Rules</h3>
                <ul>
                    <li>Zero fees for transactions with total output &lt; 1000 FC</li>
                    <li>Standard fees for larger transactions</li>
                    <li>Fee calculation via <code>GetFleetCreditsMinFee</code></li>
                </ul>
            </section>'''
    },
    {
        'filename': 'community-reserve.html',
        'title': 'Community Reserve',
        'active': 'community-reserve.html',
        'content': '''            <section class="content-section">
                <h2>Community Reserve</h2>
                <p>A portion of non-zero fees is allocated to a community reserve for public goods.</p>
                
                <h3>Allocation</h3>
                <p>Managed by multisig wallet. Community governance determines spending.</p>
            </section>'''
    },
    
    # Testing (2 pages)
    {
        'filename': 'testing-overview.html',
        'title': 'Testing Overview',
        'active': 'testing-overview.html',
        'content': '''            <section class="content-section">
                <h2>Testing Fleet Credits</h2>
                <p>Fleet Credits includes comprehensive test suites.</p>
                
                <h3>Test Types</h3>
                <ul>
                    <li>Unit tests</li>
                    <li>Integration tests</li>
                    <li>MWEB tests</li>
                    <li>Contribution system tests</li>
                </ul>
                
                <h3>Running Tests</h3>
                <pre><code>make check
test/test_fleetcredits</code></pre>
            </section>'''
    },
    {
        'filename': 'test-structures.html',
        'title': 'Test Structures',
        'active': 'test-structures.html',
        'content': '''            <section class="content-section">
                <h2>Test Structures</h2>
                <p><strong>Directory:</strong> <code>src/test/</code></p>
                
                <h3>Test Files</h3>
                <ul>
                    <li><code>fleetcredits_tests.cpp</code> - Core tests</li>
                    <li><code>mweb_tests.cpp</code> - MWEB tests</li>
                    <li><code>mweb_test.h</code> - MWEB test utilities</li>
                </ul>
            </section>'''
    },
    
    # Reference (3 pages)
    {
        'filename': 'erd-diagram.html',
        'title': 'ERD Diagram',
        'active': 'erd-diagram.html',
        'content': '''            <section class="content-section">
                <h2>Entity-Relationship Diagram</h2>
                <p>Complete ERD for Fleet Credits ecosystem.</p>
                <p><strong>File:</strong> <code>fleetcredits/docs/ERD/globaldiagram.md</code></p>
                
                <p>The ERD includes all entities: blocks, transactions, contributions, MWEB structures, oracles, validators, governance, etc.</p>
            </section>'''
    },
    {
        'filename': 'naming-conventions.html',
        'title': 'Naming Conventions',
        'active': 'naming-conventions.html',
        'content': '''            <section class="content-section">
                <h2>Naming Conventions</h2>
                <p>Fleet Credits follows specific naming conventions.</p>
                <p><strong>File:</strong> <code>fleetcredits/docs/NAMING_CONVENTIONS.md</code></p>
                
                <h3>Binary Names</h3>
                <ul>
                    <li><code>fleetcreditsd</code> - Daemon</li>
                    <li><code>fleetcredits-cli</code> - CLI</li>
                    <li><code>fleetcredits-qt</code> - GUI</li>
                    <li><code>fleetcredits-tx</code> - Transaction utility</li>
                </ul>
            </section>'''
    },
    {
        'filename': 'api-reference.html',
        'title': 'Complete API Reference',
        'active': 'api-reference.html',
        'content': '''            <section class="content-section">
                <h2>Complete API Reference</h2>
                <p>Complete reference for all Fleet Credits APIs.</p>
                
                <h3>RPC APIs</h3>
                <ul>
                    <li><a href="rpc-overview.html">RPC Overview</a></li>
                    <li><a href="rpc-contribution.html">Contribution RPC</a></li>
                    <li><a href="rpc-mweb.html">MWEB RPC</a></li>
                    <li><a href="rpc-mining.html">Mining RPC</a></li>
                    <li><a href="rpc-wallet.html">Wallet RPC</a></li>
                    <li><a href="rpc-blockchain.html">Blockchain RPC</a></li>
                </ul>
            </section>'''
    },
]

if __name__ == '__main__':
    pages_dir = Path('docserver/pages')
    pages_dir.mkdir(parents=True, exist_ok=True)
    
    print(f"Generating {len(PAGES)} documentation pages...")
    
    success = 0
    skipped = 0
    for page_def in PAGES:
        if create_page(page_def['filename'], page_def['title'], page_def['active'], page_def['content']):
            if Path(f'docserver/pages/{page_def["filename"]}').exists():
                success += 1
            else:
                skipped += 1
    
    print(f"\nDone! Generated {success} new pages, skipped {skipped} existing pages")
    print(f"Total pages in docserver/pages/: {len(list(Path('docserver/pages').glob('*.html')))}")

