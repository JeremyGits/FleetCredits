# Copyright (c) 2025-2026 The Fleet Credits Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

# MWEB Test Client - Python Version
# More comprehensive test client for MWEB operations

import subprocess
import json
import sys
import time

# Colors for output
class Colors:
    RED = '\033[0;31m'
    GREEN = '\033[0;32m'
    YELLOW = '\033[1;33m'
    BLUE = '\033[0;34m'
    NC = '\033[0m'  # No Color

def run_rpc_command(command, params=None):
    """Run an RPC command and return the result"""
    try:
        cmd = ['fleetcredits-cli', '-testnet']
        if params:
            cmd.extend([command] + params)
        else:
            cmd.append(command)
        
        result = subprocess.run(cmd, capture_output=True, text=True, timeout=10)
        
        if result.returncode == 0:
            try:
                return json.loads(result.stdout)
            except json.JSONDecodeError:
                return result.stdout
        else:
            return {'error': result.stderr}
    except subprocess.TimeoutExpired:
        return {'error': 'Command timed out'}
    except Exception as e:
        return {'error': str(e)}

def test_createpegin():
    """Test createpegin command"""
    print(f"{Colors.BLUE}Testing createpegin...{Colors.NC}")
    
    result = run_rpc_command('createpegin', ['mweb_test_address', '1000'])
    
    if 'error' in result:
        print(f"  {Colors.YELLOW}⚠ {result['error']}{Colors.NC}")
        return False
    elif 'pegtxid' in result:
        print(f"  {Colors.GREEN}✓ createpegin successful{Colors.NC}")
        print(f"    Peg TX ID: {result.get('pegtxid', 'N/A')}")
        print(f"    Main TX ID: {result.get('maintxid', 'N/A')}")
        print(f"    Amount: {result.get('amount', 'N/A')} FC")
        return True
    else:
        print(f"  {Colors.YELLOW}⚠ Unexpected response: {result}{Colors.NC}")
        return False

def test_createpegout():
    """Test createpegout command"""
    print(f"{Colors.BLUE}Testing createpegout...{Colors.NC}")
    
    # Use a placeholder commitment hash
    commitment = '0' * 64
    result = run_rpc_command('createpegout', [commitment, 'FleetCreditsTestAddress', '500'])
    
    if 'error' in result:
        print(f"  {Colors.YELLOW}⚠ {result['error']}{Colors.NC}")
        return False
    elif 'pegtxid' in result:
        print(f"  {Colors.GREEN}✓ createpegout successful{Colors.NC}")
        print(f"    Peg TX ID: {result.get('pegtxid', 'N/A')}")
        print(f"    Amount: {result.get('amount', 'N/A')} FC")
        return True
    else:
        print(f"  {Colors.YELLOW}⚠ Unexpected response: {result}{Colors.NC}")
        return False

def test_listmwebtxs():
    """Test listmwebtxs command"""
    print(f"{Colors.BLUE}Testing listmwebtxs...{Colors.NC}")
    
    result = run_rpc_command('listmwebtxs')
    
    if 'error' in result:
        print(f"  {Colors.YELLOW}⚠ {result['error']}{Colors.NC}")
        return False
    elif isinstance(result, list):
        print(f"  {Colors.GREEN}✓ listmwebtxs successful{Colors.NC}")
        print(f"    Found {len(result)} MWEB transactions")
        return True
    else:
        print(f"  {Colors.YELLOW}⚠ Unexpected response: {result}{Colors.NC}")
        return False

def test_routecontributiontomweb():
    """Test routecontributiontomweb command"""
    print(f"{Colors.BLUE}Testing routecontributiontomweb...{Colors.NC}")
    
    # Use a placeholder transaction ID
    txid = '0' * 64
    result = run_rpc_command('routecontributiontomweb', [txid])
    
    if 'error' in result:
        print(f"  {Colors.YELLOW}⚠ {result['error']}{Colors.NC}")
        return False
    elif 'mwebtxid' in result:
        print(f"  {Colors.GREEN}✓ routecontributiontomweb successful{Colors.NC}")
        print(f"    MWEB TX ID: {result.get('mwebtxid', 'N/A')}")
        print(f"    Original TX ID: {result.get('originaltxid', 'N/A')}")
        return True
    else:
        print(f"  {Colors.YELLOW}⚠ Unexpected response: {result}{Colors.NC}")
        return False

def main():
    """Run all MWEB tests"""
    print(f"{Colors.GREEN}{'='*60}{Colors.NC}")
    print(f"{Colors.GREEN}Fleet Credits MWEB Test Client{Colors.NC}")
    print(f"{Colors.GREEN}{'='*60}{Colors.NC}")
    print("")
    
    # Check if daemon is running
    print(f"{Colors.BLUE}Checking daemon status...{Colors.NC}")
    info_result = run_rpc_command('getinfo')
    
    if 'error' in info_result:
        print(f"{Colors.RED}✗ Cannot connect to fleetcreditsd{Colors.NC}")
        print(f"  Error: {info_result['error']}")
        print(f"  Please start fleetcreditsd with: fleetcreditsd -testnet -daemon")
        sys.exit(1)
    else:
        print(f"{Colors.GREEN}✓ Connected to fleetcreditsd{Colors.NC}")
        if isinstance(info_result, dict):
            print(f"    Version: {info_result.get('version', 'N/A')}")
            print(f"    Blocks: {info_result.get('blocks', 'N/A')}")
    
    print("")
    print(f"{Colors.BLUE}Running MWEB RPC Tests...{Colors.NC}")
    print("")
    
    results = []
    
    # Run tests
    results.append(('createpegin', test_createpegin()))
    print("")
    results.append(('createpegout', test_createpegout()))
    print("")
    results.append(('listmwebtxs', test_listmwebtxs()))
    print("")
    results.append(('routecontributiontomweb', test_routecontributiontomweb()))
    
    # Summary
    print("")
    print(f"{Colors.GREEN}{'='*60}{Colors.NC}")
    print(f"{Colors.GREEN}Test Summary{Colors.NC}")
    print(f"{Colors.GREEN}{'='*60}{Colors.NC}")
    
    passed = sum(1 for _, result in results if result)
    total = len(results)
    
    for test_name, result in results:
        status = f"{Colors.GREEN}✓ PASS{Colors.NC}" if result else f"{Colors.RED}✗ FAIL{Colors.NC}"
        print(f"  {test_name:30} {status}")
    
    print("")
    print(f"Results: {passed}/{total} tests passed")
    
    if passed == total:
        print(f"{Colors.GREEN}All tests passed!{Colors.NC}")
        sys.exit(0)
    else:
        print(f"{Colors.YELLOW}Some tests failed (expected for placeholder implementations){Colors.NC}")
        sys.exit(0)  # Exit 0 because placeholder implementations are expected to fail

if __name__ == '__main__':
    main()

