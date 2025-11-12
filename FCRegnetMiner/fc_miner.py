#!/usr/bin/env python3
"""
Fleet Credits Regtest Miner
A simple mining tool for Fleet Credits regtest networks.
"""

import argparse
import json
import time
import sys
import requests
from typing import Optional, Dict, Any

class FleetCreditsMiner:
    def __init__(self, rpc_url: str = "http://127.0.0.1:42068", 
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
                timeout=30
            )
            response.raise_for_status()
            result = response.json()
            
            if "error" in result and result["error"]:
                raise Exception(f"RPC Error: {result['error']}")
            
            return result.get("result")
        except requests.exceptions.RequestException as e:
            raise Exception(f"RPC connection failed: {e}")
    
    def get_blockchain_info(self) -> Dict[str, Any]:
        """Get current blockchain information."""
        return self.rpc_call("getblockchaininfo")
    
    def get_network_info(self) -> Dict[str, Any]:
        """Get network information."""
        return self.rpc_call("getnetworkinfo")
    
    def generate_blocks(self, num_blocks: int) -> list:
        """Generate (mine) blocks."""
        return self.rpc_call("generate", [num_blocks])
    
    def get_block_count(self) -> int:
        """Get current block count."""
        return self.rpc_call("getblockcount")
    
    def get_balance(self) -> float:
        """Get wallet balance."""
        return self.rpc_call("getbalance")
    
    def mine_blocks(self, num_blocks: int, show_stats: bool = True) -> list:
        """Mine blocks and optionally show statistics."""
        if show_stats:
            print(f"⛏️  Mining {num_blocks} block(s)...")
            start_height = self.get_block_count()
            start_balance = self.get_balance()
        
        start_time = time.time()
        block_hashes = self.generate_blocks(num_blocks)
        elapsed = time.time() - start_time
        
        if show_stats:
            end_height = self.get_block_count()
            end_balance = self.get_balance()
            
            print(f"✅ Mined {len(block_hashes)} block(s) in {elapsed:.2f} seconds")
            print(f"   Block height: {start_height} → {end_height}")
            print(f"   Balance: {start_balance:.8f} FC → {end_balance:.8f} FC")
            print(f"   Reward: {end_balance - start_balance:.8f} FC")
            if len(block_hashes) > 0:
                print(f"   First block: {block_hashes[0][:16]}...")
        
        return block_hashes
    
    def auto_mine(self, interval: float = 1.0, max_blocks: Optional[int] = None):
        """Continuously mine blocks."""
        print(f"🤖 Auto-mining mode (interval: {interval}s)")
        if max_blocks:
            print(f"   Will mine up to {max_blocks} blocks")
        print("   Press Ctrl+C to stop\n")
        
        total_mined = 0
        try:
            while True:
                if max_blocks and total_mined >= max_blocks:
                    print(f"\n✅ Reached maximum blocks ({max_blocks})")
                    break
                
                self.mine_blocks(1, show_stats=True)
                total_mined += 1
                
                if interval > 0:
                    time.sleep(interval)
        except KeyboardInterrupt:
            print(f"\n\n⏹️  Stopped after mining {total_mined} block(s)")
            print(f"   Final balance: {self.get_balance():.8f} FC")
            print(f"   Final height: {self.get_block_count()}")

def main():
    parser = argparse.ArgumentParser(
        description="Fleet Credits Regtest Miner",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s mine 10              # Mine 10 blocks
  %(prog)s auto                 # Auto-mine continuously
  %(prog)s auto --interval 2    # Auto-mine with 2 second intervals
  %(prog)s --rpc-url http://127.0.0.1:42068 mine 5  # Regtest (default)
  %(prog)s --rpc-url http://96.126.123.195:42068 --rpc-user user --rpc-password pass mine 5  # Remote regtest
        """
    )
    
    parser.add_argument(
        "--rpc-url",
        default="http://127.0.0.1:42068",
        help="RPC URL (default: http://127.0.0.1:42068 for regtest). For remote: http://IP:PORT"
    )
    parser.add_argument(
        "--rpc-user",
        help="RPC username (if required)"
    )
    parser.add_argument(
        "--rpc-password",
        help="RPC password (if required)"
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
        default=1.0,
        help="Seconds between blocks (default: 1.0)"
    )
    auto_parser.add_argument(
        "--max-blocks",
        type=int,
        help="Maximum number of blocks to mine (default: unlimited)"
    )
    
    # Status command
    status_parser = subparsers.add_parser("status", help="Show mining status")
    
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
        print(f"🔗 Connected to Fleet Credits node")
        print(f"   Network: {info.get('chain', 'unknown')}")
        print(f"   Blocks: {info.get('blocks', 0)}")
        print(f"   Difficulty: {info.get('difficulty', 0):.2f}")
        print()
    except Exception as e:
        print(f"❌ Failed to connect to Fleet Credits node: {e}")
        print()
        print("📋 To fix this, start the Fleet Credits daemon:")
        print()
        print("   Option 1: Start daemon in foreground (see output):")
        print("   cd /mnt/d/seriousprojects/FleetCredits/fleetcredits")
        print("   ./src/fleetcreditsd -regtest -mocktime=1776643200 -printtoconsole")
        print()
        print("   Option 2: Start daemon in background:")
        print("   cd /mnt/d/seriousprojects/FleetCredits/fleetcredits")
        print("   ./src/fleetcreditsd -regtest -mocktime=1776643200 -daemon")
        print()
        print("   Option 3: Use Qt client (includes daemon):")
        print("   cd /mnt/d/seriousprojects/FleetCredits/fleetcredits")
        print("   ./src/qt/fleetcredits-qt -regtest -mocktime=1776643200")
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
            print(f"📊 Mining Status")
            print(f"   Block Height: {info.get('blocks', 0)}")
            print(f"   Difficulty: {info.get('difficulty', 0):.2f}")
            print(f"   Balance: {balance:.8f} FC")
            print(f"   Network: {info.get('chain', 'unknown')}")
    except Exception as e:
        print(f"❌ Error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()

