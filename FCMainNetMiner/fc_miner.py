#!/usr/bin/env python3
"""
Fleet Credits Mainnet Miner
A mining tool for Fleet Credits mainnet networks.
"""

import argparse
import json
import time
import sys
import requests
from typing import Optional, Dict, Any

class FleetCreditsMiner:
    def __init__(self, rpc_url: str = "http://127.0.0.1:22555", 
                 rpc_user: Optional[str] = None, 
                 rpc_password: Optional[str] = None):
        self.rpc_url = rpc_url.rstrip('/')  # Remove trailing slash
        self.rpc_user = rpc_user
        self.rpc_password = rpc_password
        self.session = requests.Session()
        if rpc_user and rpc_password:
            self.session.auth = (rpc_user, rpc_password)
    
    def rpc_call(self, method: str, params: list = None) -> Dict[str, Any]:
        """Make an RPC call to Fleet Credits daemon."""
        if params is None:
            params = []
        
        payload = {
            "jsonrpc": "2.0",
            "id": 1,
            "method": method,
            "params": params
        }
        
        try:
            response = self.session.post(
                self.rpc_url,
                json=payload,
                headers={"Content-Type": "application/json"},
                timeout=3600  # 60 minutes for mainnet mining (real Scrypt takes time)
            )
            response.raise_for_status()
            result = response.json()
            
            if "error" in result and result["error"]:
                error_msg = result["error"]
                if isinstance(error_msg, dict):
                    error_str = f"Code {error_msg.get('code', 'unknown')}: {error_msg.get('message', str(error_msg))}"
                else:
                    error_str = str(error_msg)
                raise Exception(f"RPC Error: {error_str}")
            
            return result.get("result")
        except requests.exceptions.Timeout:
            raise Exception(f"RPC call timed out after 30 minutes. Mining may take longer - try mining directly on server.")
        except requests.exceptions.RequestException as e:
            raise Exception(f"RPC connection failed: {e}")
    
    def get_blockchain_info(self) -> Dict[str, Any]:
        """Get current blockchain information."""
        return self.rpc_call("getblockchaininfo")
    
    def get_network_info(self) -> Dict[str, Any]:
        """Get network information."""
        return self.rpc_call("getnetworkinfo")
    
    def generate_blocks(self, num_blocks: int, maxtries: int = 5000000) -> list:
        """Generate (mine) blocks."""
        # Default maxtries is 5,000,000 for better block finding
        # Lower values cause early exit and return 0 blocks
        # Higher values give more chances to find blocks, especially with low difficulty
        return self.rpc_call("generate", [num_blocks, maxtries])
    
    def get_block_count(self) -> int:
        """Get current block count."""
        return self.rpc_call("getblockcount")
    
    def get_balance(self) -> float:
        """Get wallet balance."""
        return self.rpc_call("getbalance")
    
    def verify_mining_setup(self) -> bool:
        """Verify that mining is properly configured."""
        print("🔍 Verifying mining setup...")
        issues = []
        
        # Check blockchain connection
        try:
            info = self.get_blockchain_info()
            print(f"   ✅ Connected to network: {info.get('chain', 'unknown')}")
            print(f"   ✅ Current block height: {info.get('blocks', 0)}")
        except Exception as e:
            issues.append(f"Blockchain connection failed: {e}")
            print(f"   ❌ Blockchain connection failed: {e}")
        
        # Check wallet
        try:
            balance = self.get_balance()
            print(f"   ✅ Wallet accessible, balance: {balance:.8f} FC")
        except Exception as e:
            issues.append(f"Wallet access failed: {e}")
            print(f"   ❌ Wallet access failed: {e}")
        
        # Check mining info
        try:
            mining_info = self.rpc_call("getmininginfo")
            difficulty = mining_info.get('difficulty', 0)
            hashrate = mining_info.get('networkhashps', 0)
            print(f"   ✅ Mining info available")
            print(f"      Difficulty: {difficulty:.2f}")
            print(f"      Network hashrate: {hashrate:.0f} H/s")
            if difficulty > 1000:
                print(f"      ⚠️  High difficulty - mining will be slow!")
        except Exception as e:
            issues.append(f"Mining info unavailable: {e}")
            print(f"   ⚠️  Mining info unavailable: {e}")
        
        # Test generate call (with 0 blocks to just test the call)
        try:
            # This should return an empty array, not an error
            test_result = self.rpc_call("generate", [0, 1])
            if isinstance(test_result, list):
                print(f"   ✅ Generate RPC call works correctly")
            else:
                issues.append(f"Generate RPC returned unexpected format: {type(test_result)}")
                print(f"   ⚠️  Generate RPC returned unexpected format")
        except Exception as e:
            issues.append(f"Generate RPC test failed: {e}")
            print(f"   ❌ Generate RPC test failed: {e}")
        
        if issues:
            print(f"\n⚠️  Found {len(issues)} issue(s):")
            for issue in issues:
                print(f"   - {issue}")
            return False
        else:
            print(f"\n✅ All checks passed! Mining should work.")
            return True
    
    def mine_blocks(self, num_blocks: int, show_stats: bool = True) -> list:
        """Mine blocks and optionally show statistics."""
        if show_stats:
            print(f"⛏️  Mining {num_blocks} block(s) on mainnet...")
            start_height = self.get_block_count()
            start_balance = self.get_balance()
            
            # Check mining prerequisites
            try:
                mining_info = self.rpc_call("getmininginfo")
                difficulty = mining_info.get('difficulty', 0)
                network_hashrate = mining_info.get('networkhashps', 0)
                print(f"   Current difficulty: {difficulty:.2f}")
                if network_hashrate > 0:
                    print(f"   Network hashrate: {network_hashrate:.0f} H/s")
                else:
                    print(f"   Network hashrate: 0 H/s (need 120+ blocks for accurate calculation)")
                    print(f"   Note: With only {mining_info.get('blocks', 0)} blocks, hashrate can't be calculated yet")
            except:
                pass  # Ignore if getmininginfo fails
        
        start_time = time.time()
        try:
            # Use high maxtries for real mining, but allow early success
            # If difficulty is very low, blocks will be found quickly
            # If difficulty is high, this gives enough tries to find blocks
            block_hashes = self.generate_blocks(num_blocks, maxtries=5000000)
        except Exception as e:
            if show_stats:
                print(f"❌ Mining failed: {e}")
                print(f"   This may be normal - real Scrypt mining can take hours")
                print(f"   Try mining directly on server for better performance")
            raise
        
        elapsed = time.time() - start_time
        
        if show_stats:
            end_height = self.get_block_count()
            end_balance = self.get_balance()
            
            if len(block_hashes) == 0:
                print(f"⚠️  Mined 0 block(s) in {elapsed:.2f} seconds")
                print(f"   Block height unchanged: {start_height}")
                print(f"   This means no valid block was found within maxtries")
                print(f"   This is NORMAL for real Scrypt mining - it can take hours")
                print(f"   Try: Mine directly on server, or increase maxtries, or wait longer")
            else:
                print(f"✅ Mined {len(block_hashes)} block(s) in {elapsed:.2f} seconds")
                print(f"   Block height: {start_height} → {end_height}")
                print(f"   Balance: {start_balance:.8f} FC → {end_balance:.8f} FC")
                print(f"   Reward: {end_balance - start_balance:.8f} FC")
                if len(block_hashes) > 0:
                    print(f"   First block: {block_hashes[0][:16]}...")
        
        return block_hashes
    
    def auto_mine(self, interval: float = 10.0, max_blocks: Optional[int] = None):
        """Continuously mine blocks."""
        print(f"🤖 Auto-mining mode on mainnet (interval: {interval}s)")
        print(f"   ⚠️  WARNING: This is REAL mining with REAL difficulty!")
        print(f"   ⚠️  Each block may take MINUTES or HOURS to find!")
        if max_blocks:
            print(f"   Will mine up to {max_blocks} blocks")
        print("   Press Ctrl+C to stop\n")
        
        total_mined = 0
        total_attempts = 0
        try:
            while True:
                if max_blocks and total_mined >= max_blocks:
                    print(f"\n✅ Reached maximum blocks ({max_blocks})")
                    break
                
                total_attempts += 1
                try:
                    block_hashes = self.mine_blocks(1, show_stats=True)
                    if len(block_hashes) > 0:
                        total_mined += len(block_hashes)
                        current_height = self.get_block_count()
                        print(f"   Progress: {total_mined} block(s) mined this session in {total_attempts} attempt(s)")
                        print(f"   Current blockchain height: {current_height}")
                    else:
                        print(f"   Progress: {total_mined} block(s) mined this session in {total_attempts} attempt(s) (0 this time)")
                except Exception as e:
                    print(f"   Attempt {total_attempts} failed: {e}")
                    print(f"   Continuing...")
                
                if interval > 0:
                    time.sleep(interval)
        except KeyboardInterrupt:
            print(f"\n\n⏹️  Stopped after {total_attempts} attempt(s)")
            print(f"   Blocks mined this session: {total_mined}")
            print(f"   Current block height: {self.get_block_count()}")
            print(f"   Available balance: {self.get_balance():.8f} FC")
            print(f"   Note: Check wallet history for total blocks ever mined")
            print(f"   Note: Coinbase rewards need 30 confirmations to mature")

def main():
    parser = argparse.ArgumentParser(
        description="Fleet Credits Mainnet Miner",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s mine 10              # Mine 10 blocks
  %(prog)s auto                 # Auto-mine continuously
  %(prog)s auto --interval 120 # Auto-mine with 2 minute intervals
  %(prog)s --rpc-url http://127.0.0.1:22555 --rpc-user user --rpc-password pass mine 5
        """
    )
    
    parser.add_argument(
        "--rpc-url",
        default="http://127.0.0.1:22555",
        help="RPC URL (default: http://127.0.0.1:22555 for mainnet). For remote: http://IP:22555"
    )
    parser.add_argument(
        "--rpc-user",
        help="RPC username (required if RPC auth is enabled)"
    )
    parser.add_argument(
        "--rpc-password",
        help="RPC password (required if RPC auth is enabled)"
    )
    
    subparsers = parser.add_subparsers(dest="command", help="Command to execute")
    
    # Mine command
    mine_parser = subparsers.add_parser("mine", help="Mine specific number of blocks")
    mine_parser.add_argument("blocks", type=int, help="Number of blocks to mine")
    
    # Auto command
    auto_parser = subparsers.add_parser("auto", help="Auto-mine blocks continuously")
    auto_parser.add_argument(
        "--interval",
        type=float,
        default=10.0,
        help="Seconds between mining attempts (default: 10.0 for faster operation)"
    )
    auto_parser.add_argument(
        "--max-blocks",
        type=int,
        help="Maximum number of blocks to mine (default: unlimited)"
    )
    
    # Status command
    status_parser = subparsers.add_parser("status", help="Show mining status")
    
    # Verify command
    verify_parser = subparsers.add_parser("verify", help="Verify mining setup and RPC connection")
    
    args = parser.parse_args()
    
    if not args.command:
        parser.print_help()
        sys.exit(1)
    
    # Create miner instance
    miner = FleetCreditsMiner(
        rpc_url=args.rpc_url,
        rpc_user=args.rpc_user,
        rpc_password=args.rpc_password
    )
    
    # Test connection
    try:
        info = miner.get_blockchain_info()
        print(f"🔗 Connected to Fleet Credits mainnet node")
        print(f"   Network: {info.get('chain', 'unknown')}")
        print(f"   Blocks: {info.get('blocks', 0)}")
        print(f"   Difficulty: {info.get('difficulty', 0):.2f}")
        print()
    except Exception as e:
        print(f"❌ Failed to connect to Fleet Credits node: {e}")
        print()
        print("📋 To fix this, start the Fleet Credits daemon on mainnet:")
        print()
        print("   Option 1: Start daemon in foreground (see output):")
        print("   cd /mnt/d/seriousprojects/FleetCredits/fleetcredits")
        print("   ./src/fleetcreditsd -printtoconsole")
        print()
        print("   Option 2: Start daemon in background:")
        print("   cd /mnt/d/seriousprojects/FleetCredits/fleetcredits")
        print("   ./src/fleetcreditsd -daemon")
        print()
        print("   Option 3: Use Qt client (includes daemon):")
        print("   cd /mnt/d/seriousprojects/FleetCredits/fleetcredits")
        print("   ./src/qt/fleetcredits-qt")
        print()
        print(f"   Then try running the miner again.")
        print(f"   RPC URL: {args.rpc_url}")
        sys.exit(1)
    
    # Execute command
    try:
        if args.command == "mine":
            miner.mine_blocks(args.blocks)
        elif args.command == "auto":
            miner.auto_mine(interval=args.interval, max_blocks=args.max_blocks)
        elif args.command == "status":
            info = miner.get_blockchain_info()
            balance = miner.get_balance()
            print(f"📊 Mining Status (Mainnet)")
            print(f"   Block Height: {info.get('blocks', 0)}")
            print(f"   Difficulty: {info.get('difficulty', 0):.2f}")
            print(f"   Balance: {balance:.8f} FC")
            print(f"   Network: {info.get('chain', 'unknown')}")
        elif args.command == "verify":
            if not miner.verify_mining_setup():
                sys.exit(1)
    except Exception as e:
        print(f"❌ Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()

