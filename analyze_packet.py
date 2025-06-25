#!/usr/bin/env python3

# Copyright (C) 2025 rustymotors
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

"""
NPS Packet Analyzer CLI Tool

Command-line tool for analyzing NPS protocol packets received by the pyOxide server.
"""

import argparse
import sys
from pathlib import Path

# Add src to path for imports
sys.path.insert(0, str(Path(__file__).parent / "src"))

from packet_parser import NPSPacket, NPSPacketParser  # type: ignore


def analyze_packet_from_file(file_path: str) -> None:
    """Analyze a packet from a file containing hex data."""
    try:
        with open(file_path) as f:
            hex_data = f.read().strip()

        parser = NPSPacketParser()
        packet = parser.parse_packet(hex_data)

        if packet:
            print_packet_analysis(packet, parser)
        else:
            print("❌ Failed to parse packet from file")

    except FileNotFoundError:
        print(f"❌ File not found: {file_path}")
    except Exception as e:
        print(f"❌ Error reading file: {e}")


def analyze_packet_from_string(hex_data: str) -> None:
    """Analyze a packet from a hex string."""
    parser = NPSPacketParser()
    packet = parser.parse_packet(hex_data)

    if packet:
        print_packet_analysis(packet, parser)
    else:
        print("❌ Failed to parse packet from string")


def print_packet_analysis(packet: NPSPacket, parser: NPSPacketParser) -> None:
    """Print a detailed analysis of the packet."""
    print("🔍 NPS Packet Analysis")
    print("=" * 50)

    # Header information
    print("📋 Header Information:")
    print(f"  Format: {packet.header.format_version}")
    print(f"  Message ID: 0x{packet.header.msg_id:04X}")
    print(f"  Length: {packet.header.length} bytes")

    if packet.header.format_version == "v1":
        print(f"  Version: 0x{packet.header.version:04X}")
        print(f"  Reserved: 0x{packet.header.reserved:04X}")
        print(f"  Length Checksum: {packet.header.length_checksum}")

    if packet.checksum:
        print(f"  Packet Checksum: 0x{packet.checksum:08X}")

    print()

    # Payload analysis
    analysis = parser.analyze_payload(packet)
    print("🔬 Payload Analysis:")
    print(f"  Message Type: {analysis.get('message_type', 'UNKNOWN')}")
    print(f"  Payload Length: {analysis['payload_length']} bytes")

    # Show structured LOGIN_REQUEST analysis if present
    if 'structured_fields' in analysis:
        structured = analysis['structured_fields']
        print("\n🏗️  Structured LOGIN_REQUEST Analysis (confirmed structure):")

        # Session ticket container
        if 'session_ticket_container' in structured:
            st_container = structured['session_ticket_container']
            print(f"  Session Ticket Container ({st_container['total_length']} bytes):")
            print(f"    Data Length: {st_container['data_length']} bytes")
            print(f"    Data Hex: {st_container['data_hex']}")
            print(f"    ASCII: {st_container['ascii']}")

        # V0 header (formerly unknown middle)
        if 'v0_header' in structured:
            v0_header = structured['v0_header']
            print(f"  V0-Style Header ({v0_header['length']} bytes):")
            print(f"    Hex: {v0_header['hex']}")
            print(f"    Message ID: 0x{v0_header['msgid']:04X}")
            print(f"    Description: {v0_header['description']}")

        # Session key container
        if 'session_key_container' in structured:
            sk_container = structured['session_key_container']
            print(f"  Session Key Container ({sk_container['total_length']} bytes):")
            print(f"    Key Data Length: {sk_container['key_data_length']} bytes")
            print(f"    Preview: {sk_container['hex_preview']}")

        # Session key data (without length prefix)
        if 'session_key_data' in structured:
            sk_data = structured['session_key_data']
            print(f"  Session Key Data ({sk_data['length']} bytes):")
            print(f"    Preview: {sk_data['hex_preview']}")
            print(f"    ASCII: {sk_data['ascii']}")

        # Remaining data
        if 'remaining_data' in structured and structured['remaining_data']['length'] > 0:
            remaining = structured['remaining_data']
            print(f"  Remaining Data ({remaining['length']} bytes):")
            print(f"    Hex: {remaining['hex']}")
            print(f"    ASCII: {remaining['ascii']}")

    # Show generic parsing for comparison
    if analysis.get('generic_parsing'):
        parsed = analysis['generic_parsing']
        print("\n🔧 Generic NPS Structure Analysis:")
        for field in parsed['fields']:
            field_info = field['field']
            if field['type'] == 'container':
                print(f"  {field_info}: Container (length={field['length']})")
                print(f"    Content: {field['ascii_attempt']}")
            elif field['type'] == 'u16_string':
                print(f"  {field_info}: String (length={field['length']})")
                print(f"    Value: '{field['data']}'")
            elif field['type'] == 'null_terminated_string':
                print(f"  {field_info}: Null-terminated String")
                print(f"    Value: '{field['data']}'")
            elif field['type'] == 'short':
                print(f"  {field_info}: Short = {field['value']} ({field['hex']})")
            elif field['type'] == 'long':
                print(f"  {field_info}: Long = {field['value']} ({field['hex']})")
            elif field['type'] == 'byte':
                print(f"  {field_info}: Byte = {field['value']} ({field['hex']})")

        print(f"\n  📊 Parsing Summary: {parsed['total_parsed']}/{len(packet.payload)} bytes parsed")
        if parsed['remaining_bytes'] > 0:
            print(f"  ⚠️  {parsed['remaining_bytes']} bytes remaining unparsed")

    # Show extracted strings
    if analysis.get('extracted_strings'):
        print("\n📝 Extracted Strings:")
        for i, string in enumerate(analysis['extracted_strings'], 1):
            print(f"  {i}. {string}")

    # Show other patterns (legacy compatibility)
    if 'patterns' in analysis:
        patterns = analysis['patterns']
        if 'common_bytes' in patterns:
            print("\n📊 Most Common Bytes:")
            for byte_info in patterns['common_bytes']:
                print(f"  {byte_info['byte']}: {byte_info['count']} occurrences")

    print()
    print("📦 Raw Payload (first 128 bytes):")
    raw_hex = analysis['raw_payload']
    # Show first 128 bytes in a nice format
    for i in range(0, min(256, len(raw_hex)), 32):  # 256 chars = 128 bytes
        chunk = raw_hex[i:i+32]
        print(f"  {chunk}")

    if len(raw_hex) > 256:
        print(f"  ... ({len(raw_hex)//2 - 128} more bytes)")


def main():
    """Main CLI function."""
    parser = argparse.ArgumentParser(
        description="Analyze NPS protocol packets",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Analyze packet from hex string
  python analyze_packet.py -s "0501014401010000..."

  # Analyze packet from file
  python analyze_packet.py -f packet_data.txt

  # Use sample packet for testing
  python analyze_packet.py --sample
        """
    )

    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument('-s', '--string', help='Hex string to analyze')
    group.add_argument('-f', '--file', help='File containing hex data')
    group.add_argument('--sample', action='store_true', help='Use sample packet for testing')

    args = parser.parse_args()

    if args.sample:
        # Use the sample packet provided by the user
        sample_packet = ("0501014401010000000001440028663433336338383364333363613630636661"
                        "3866653633656365316363666430313431386433616200000100344139"
                        "4544323635334442303242334542383830343730414231463745433030"
                        "304231423331433635354445314431363131443744313834373943353034"
                        "3236433236414437303236454435334438464144394437373945453643"
                        "413138353846454645463039343144333530394545313444304136384342"
                        "334534393532423135334530463736464438323037313139343338463537"
                        "4439303341363441373842353137343535444238364645464530433334"
                        "3845383141314632393245343031454437354532384237333043464237"
                        "38303935463742383645423833364344373041373346324546394344"
                        "3030303037314242304546413246383746373200043231373646454131433139")

        print("🧪 Analyzing sample packet...")
        analyze_packet_from_string(sample_packet)

    elif args.string:
        analyze_packet_from_string(args.string)

    elif args.file:
        analyze_packet_from_file(args.file)


if __name__ == "__main__":
    main()
