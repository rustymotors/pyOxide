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
Test script for session key extraction and decryption.
"""

import sys
import logging
from pathlib import Path

# Add src to path so we can import our modules
sys.path.insert(0, str(Path(__file__).parent / "src"))

from session_key_decryptor import SessionKeyDecryptor
from packet_parser import NPSPacketParser

# Fresh LOGIN_REQUEST packet for testing
SAMPLE_PACKET = (
    "0501014401010000000001440028613938366561353830393961356133376135393764303035393837306133313438353039353463610000010032334139383942314337444242454637364134313045304141364642423537394530323232413532433946434531433644304532393142313042373836374536374436333243363541384642333343374132433439354342344643464435433146313137423845464334314331353135383244304635453645334132463436323433443941303734374235303633383831453530413235303642463232444346453643464639363142314534393236393433433342353245413041433536443038463246353242323736363033464137434334373946303744383936363045353739453432303733393936373042453644414141324441433042464538324539000432313736FEA31C19"
)

def main():
    """Test session key extraction and decryption."""
    logging.basicConfig(level=logging.INFO, format='%(levelname)s: %(message)s')

    print("🔑 Testing Session Key Extraction and Decryption")
    print("=" * 60)

    try:
        # Parse the packet to get structured data
        parser = NPSPacketParser()
        packet_bytes = bytes.fromhex(SAMPLE_PACKET)

        print(f"📦 Parsing packet of {len(packet_bytes)} bytes...")

        # First parse the packet structure
        packet = parser.parse_packet(SAMPLE_PACKET)
        if not packet:
            print("❌ Failed to parse packet structure")
            return 1

        # Then analyze the payload
        analysis = parser.analyze_payload(packet)

        if not analysis:
            print("❌ Failed to analyze packet payload")
            return 1

        print(f"✅ Packet parsed successfully")
        print(f"📋 Message Type: {analysis.get('message_type', 'Unknown')}")

        # Look for session key data in structured fields
        session_key_hex = None
        if 'structured_fields' in analysis:
            structured = analysis['structured_fields']
            if 'session_key_data' in structured:
                session_key_info = structured['session_key_data']
                if 'ascii' in session_key_info:
                    session_key_hex = session_key_info['ascii'].strip("'")
                    print(f"🔍 Session Key Data: {len(session_key_hex)} hex chars")
                    print(f"    Preview: {session_key_hex[:64]}...")

        if not session_key_hex:
            print("❌ No session_key_data found in structured fields")
            if 'structured_fields' in analysis:
                print("Available structured fields:", list(analysis['structured_fields'].keys()))
            return 1

        # Create a simplified analysis dict for the decryptor
        simplified_analysis = {
            'session_key_data': session_key_hex,
            'extracted_strings': analysis.get('extracted_strings', [])
        }

        # Try to decrypt the session key
        decryptor = SessionKeyDecryptor()
        print(f"\n🔐 Attempting session key decryption...")

        result = decryptor.extract_and_decrypt_from_parsed_packet(simplified_analysis)

        if result:
            session_key, session_ticket = result
            print(f"✅ Session key decryption successful!")
            print(f"🎫 Session Ticket: {session_ticket}")
            print(f"🔑 Decrypted Session Key: {len(session_key)} bytes")
            print(f"    Hex: {session_key.hex()}")

            # Validate the session key
            if decryptor.validate_session_key(session_key):
                print(f"✅ Session key validation passed")
            else:
                print(f"⚠️  Session key validation failed")
        else:
            print(f"❌ Session key decryption failed")
            return 1

        print(f"\n🎉 Test completed successfully!")
        return 0

    except Exception as e:
        print(f"❌ Test failed with error: {e}")
        import traceback
        traceback.print_exc()
        return 1

if __name__ == "__main__":
    sys.exit(main())
