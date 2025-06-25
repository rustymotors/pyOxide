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
Tests for the NPS packet parser.
"""

from src.packet_parser import NPSPacket, NPSPacketParser


class TestNPSPacketParser:
    """Test cases for the NPS packet parser."""

    def test_parse_login_request_packet(self):
        """Test parsing a real LOGIN_REQUEST packet."""
        # Sample packet received on port 8226
        sample_packet = (
            "0501014401010000000001440028663433336338383364333363613630636661"
            "3866653633656365316363666430313431386433616200000100344139"
            "4544323635334442303242334542383830343730414231463745433030"
            "304231423331433635354445314431363131443744313834373943353034"
            "3236433236414437303236454435334438464144394437373945453643"
            "413138353846454645463039343144333530394545313444304136384342"
            "334534393532423135334530463736464438323037313139343338463537"
            "4439303341363441373842353137343535444238364645464530433334"
            "3845383141314632393245343031454437354532384237333043464237"
            "38303935463742383645423833364344373041373346324546394344"
            "3030303037314242304546413246383746373200043231373646454131433139"
        )

        parser = NPSPacketParser()
        packet = parser.parse_packet(sample_packet)

        # Verify packet was parsed successfully
        assert packet is not None
        assert isinstance(packet, NPSPacket)

        # Check header values
        assert packet.header.msg_id == 0x0501  # LOGIN_REQUEST
        assert packet.header.format_version == "v1"
        assert packet.header.length == 324

        # Check payload
        assert len(packet.payload) == 312
        assert packet.checksum == 0x31433139

    def test_analyze_login_request_payload(self):
        """Test analysis of LOGIN_REQUEST payload."""
        sample_packet = (
            "0501014401010000000001440028663433336338383364333363613630636661"
            "3866653633656365316363666430313431386433616200000100344139"
            "4544323635334442303242334542383830343730414231463745433030"
            "304231423331433635354445314431363131443744313834373943353034"
            "3236433236414437303236454435334438464144394437373945453643"
            "413138353846454645463039343144333530394545313444304136384342"
            "334534393532423135334530463736464438323037313139343338463537"
            "4439303341363441373842353137343535444238364645464530433334"
            "3845383141314632393245343031454437354532384237333043464237"
            "38303935463742383645423833364344373041373346324546394344"
            "3030303037314242304546413246383746373200043231373646454131433139"
        )

        parser = NPSPacketParser()
        packet = parser.parse_packet(sample_packet)

        assert packet is not None
        analysis = parser.analyze_payload(packet)

        # Check analysis results
        assert analysis["message_type"] == "LOGIN_REQUEST"
        assert analysis["payload_length"] == 312
        assert "extracted_strings" in analysis
        assert len(analysis["extracted_strings"]) >= 2  # Should find at least 2 strings
        assert "generic_parsing" in analysis

    def test_invalid_packet_handling(self):
        """Test handling of invalid packet data."""
        parser = NPSPacketParser()

        # Test empty packet
        assert parser.parse_packet("") is None

        # Test too short packet
        assert parser.parse_packet("050101") is None

        # Test invalid hex
        assert parser.parse_packet("INVALID_HEX") is None

    def test_message_type_identification(self):
        """Test identification of different message types."""
        parser = NPSPacketParser()

        # Test known message types
        assert parser.MESSAGE_TYPES[0x0501] == "LOGIN_REQUEST"
        assert parser.MESSAGE_TYPES[0x0145] == "LOGIN_RESPONSE"
        assert parser.MESSAGE_TYPES[0x0101] == "HEARTBEAT"

    def test_large_packet_handling(self):
        """Test handling of large packets."""
        parser = NPSPacketParser()

        # Create a large packet (simulate real-world scenario)
        # Ensure hex string has even length
        large_hex = "0501014401010000000001440" + "0" + "00" * 1000 + "12345678"
        packet = parser.parse_packet(large_hex)

        # Should still parse successfully
        assert packet is not None
        assert packet.header.msg_id == 0x0501

    def test_login_request_structured_parsing(self):
        """Test structured parsing of LOGIN_REQUEST payload."""
        # Sample packet with known LOGIN_REQUEST structure
        sample_packet = (
            "0501014401010000000001440028663433336338383364333363613630636661"
            "3866653633656365316363666430313431386433616200000100344139"
            "4544323635334442303242334542383830343730414231463745433030"
            "304231423331433635354445314431363131443744313834373943353034"
            "3236433236414437303236454435334438464144394437373945453643"
            "413138353846454645463039343144333530394545313444304136384342"
            "334534393532423135334530463736464438323037313139343338463537"
            "4439303341363441373842353137343535444238364645464530433334"
            "3845383141314632393245343031454437354532384237333043464237"
            "38303935463742383645423833364344373041373346324546394344"
            "3030303037314242304546413246383746373200043231373646454131433139"
        )

        parser = NPSPacketParser()
        packet = parser.parse_packet(sample_packet)

        assert packet is not None
        assert packet.header.msg_id == 0x0501  # LOGIN_REQUEST

        # Analyze payload with structured parser
        analysis = parser.analyze_payload(packet)

        assert analysis["type"] == "LOGIN_REQUEST"
        assert "structured_fields" in analysis

        structured = analysis["structured_fields"]

        # Verify session ticket field
        assert "session_ticket_container" in structured
        assert structured["session_ticket_container"]["data_length"] == 40
        assert (
            structured["session_ticket_container"]["data_hex"]
            == "66343333633838336433336361363063666138666536336563653163636664"
            "303134313864336162"
        )

        # Verify session key container
        assert "session_key_container" in structured
        assert structured["session_key_container"]["total_length"] == 258  # 2 + 256
        assert structured["session_key_container"]["key_data_length"] == 256

        # Verify session key data
        assert "session_key_data" in structured
        assert structured["session_key_data"]["length"] == 256

        # Verify remaining data
        assert "remaining_data" in structured
        assert structured["remaining_data"]["length"] == 10

    def test_login_request_parser_class(self):
        """Test the LoginRequestParser class directly."""
        from src.packet_parser import LoginRequestParser

        # Create sample payload (excluding header)
        sample_payload_hex = (
            "002866343333633838336433336361363063666138666536336563653163636664"
            "303134313864336162"
            "000001003441394544323635334442303242334542383830343730414231463745"
            "433030304231423331433635354445314431363131443744313834373943353034"
            "3236433236414437303236454435334438464144394437373945453643"
            "413138353846454645463039343144333530394545313444304136384342"
            "334534393532423135334530463736464438323037313139343338463537"
            "4439303341363441373842353137343535444238364645464530433334"
            "3845383141314632393245343031454437354532384237333043464237"
            "38303935463742383645423833364344373041373346324546394344"
            "3030303037314242304546413246383746373200043231373646454131"
        )

        payload_bytes = bytes.fromhex(sample_payload_hex)

        parser = LoginRequestParser()
        result = parser.parse(payload_bytes, "v1")

        assert result is not None
        assert len(result.session_ticket_data) == 40
        assert len(result.session_key_container) == 258
        assert len(result.remaining_data) == 10
        assert result.v0_header == b"\x00\x00"
