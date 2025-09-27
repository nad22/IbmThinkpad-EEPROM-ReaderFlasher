#!/usr/bin/env python3
"""
Quick EEPROM Paste Converter
============================

Ultra-simple converter: Just paste your hex dump and get C array!

Usage:
1. Run: python quick_converter.py
2. Paste hex data (Ctrl+V)
3. Press Enter twice
4. Copy the result!
"""

import re

def convert_paste():
    print("🚀 Quick EEPROM Hex to C Array Converter")
    print("=" * 50)
    print("Paste your Arduino hex dump here:")
    print("(Press Enter twice when done)")
    print()

    # Collect input
    lines = []
    empty_count = 0
    
    while True:
        try:
            line = input()
            if not line.strip():
                empty_count += 1
                if empty_count >= 2:
                    break
            else:
                empty_count = 0
                lines.append(line)
        except KeyboardInterrupt:
            print("\nBye!")
            return

    # Parse hex data
    hex_content = '\n'.join(lines)
    bytes_data = []
    
    for line in lines:
        line = line.strip()
        if not line or '====' in line:
            continue
            
        # Extract hex bytes from line like: 0x00: 53 45 52 23 ...
        match = re.match(r'0x[0-9A-Fa-f]+:\s+(.+)', line)
        if match:
            hex_bytes = match.group(1).strip().split()
            for hex_byte in hex_bytes:
                if len(hex_byte) == 2:
                    try:
                        bytes_data.append(int(hex_byte, 16))
                    except:
                        pass

    if not bytes_data:
        print("❌ No valid hex data found!")
        return

    # Pad or truncate to 256 bytes
    while len(bytes_data) < 256:
        bytes_data.append(0x00)
    bytes_data = bytes_data[:256]

    # Generate C array
    print(f"\n✅ Converted {len([b for b in bytes_data if b != 0])} non-zero bytes")
    print("=" * 50)
    print()
    print("// Copy this array into main.cpp:")
    print("const byte backup_blockX[256] PROGMEM = {")
    
    for i in range(0, 256, 16):
        line_bytes = bytes_data[i:i+16]
        hex_values = [f"0x{b:02X}" for b in line_bytes]
        print("  " + ", ".join(hex_values) + ("," if i < 240 else ""))
    
    print("};")
    print()
    print("🎯 Replace 'X' with your block number (1-4)")
    print("💡 Example: backup_block1, backup_block2, etc.")

if __name__ == "__main__":
    convert_paste()