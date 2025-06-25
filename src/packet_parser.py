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
NPS Packet Parser for pyOxide

This module provides functionality to parse Network Protocol Server (NPS) packets
received on various ports, including the authentication and game server protocols.
"""

import logging
import struct
from dataclasses import dataclass
from typing import Any, Dict, List, Optional

logger = logging.getLogger(__name__)


@dataclass
class NPSPacketHeader:
    """NPS packet header structure."""

    format_version: str  # "v0" or "v1"
    msg_id: int
    length: int
    version: Optional[int] = None  # Only present in v1
    reserved: Optional[int] = None  # Only present in v1
    length_checksum: Optional[int] = None  # Only present in v1


@dataclass
class NPSPacket:
    """Complete NPS packet structure."""

    header: NPSPacketHeader
    payload: bytes
    checksum: Optional[int] = None


@dataclass
class LoginRequestPayload:
    """
    LOGIN_REQUEST (0x0501) payload structure based on confirmed field analysis.

    Structure:
    1. Session ticket container (42 bytes total: 2-byte length prefix +
       40 bytes session ticket data)
    2. V0-style header (2 bytes: msgId=0x0000 + length) - rare container
       structure with empty message ID
    3. Encrypted session key container (258 bytes total: 2-byte length prefix +
       256 bytes encrypted data)
    4. Remaining data (variable length: client info, version data, etc.)

    The V0-style header at offset 42 is a special container structure that
    mimics a v0 header with an empty message ID (0x0000), used to separate
    the session ticket from the session key.
    """

    session_ticket_container: bytes  # Full container: length prefix + data
    session_ticket_data: bytes  # Just the session ticket data (no prefix)
    v0_header: bytes  # V0-style header (msgId=0x0000, length) for session key
    session_key_container: bytes  # Full container: length prefix + encrypted data
    session_key_data: bytes  # Just the encrypted session key data (no prefix)
    remaining_data: bytes  # Any remaining payload data after session key


class LoginRequestParser:
    """Parser specifically for LOGIN_REQUEST payloads using confirmed field offsets."""

    def parse(
        self, payload: bytes, packet_format: str = "v1"
    ) -> Optional[LoginRequestPayload]:
        """
        Parse a LOGIN_REQUEST payload into structured data based on actual
        observed structure.

        Args:
            payload: The raw payload bytes
            packet_format: The packet format ("v0" or "v1") to determine
                header size

        Returns:
            LoginRequestPayload object or None if parsing fails
        """
        try:
            # Based on actual payload analysis:
            # - Container 1 (sessionTicket): 40 bytes at offset 0
            #   (with 2-byte length prefix)
            # - Short: 2 bytes at offset 42
            # - Container 2 (sessionKey): 256 bytes at offset 44
            #   (with 2-byte length prefix)
            # - Remaining data: starts around offset 302

            # Ensure we have minimum required data
            if len(payload) < 46:  # Need enough for containers + short + start
                return None

            # Extract first container (session ticket area)
            container1_length = struct.unpack(">H", payload[0:2])[0]
            if container1_length + 2 > len(payload):
                return None

            session_ticket_container = payload[
                0 : container1_length + 2
            ]  # Include length prefix
            session_ticket_data = payload[2 : container1_length + 2]  # Just the data

            # Extract V0-style header (msgId=0x0000, length for session key container)
            v0_header_offset = container1_length + 2
            if v0_header_offset + 2 > len(payload):
                return None
            v0_header = payload[v0_header_offset : v0_header_offset + 2]

            # Extract second container (session key area)
            container2_offset = v0_header_offset + 2
            if container2_offset + 2 > len(payload):
                return None

            container2_length = struct.unpack(
                ">H", payload[container2_offset : container2_offset + 2]
            )[0]
            container2_end = container2_offset + 2 + container2_length

            if container2_end > len(payload):
                return None

            session_key_container = payload[
                container2_offset:container2_end
            ]  # Include length prefix
            session_key_data = payload[
                container2_offset + 2 : container2_end
            ]  # Just the data (without length prefix)
            remaining_data = payload[container2_end:]

            return LoginRequestPayload(
                session_ticket_container=session_ticket_container,
                session_ticket_data=session_ticket_data,
                v0_header=v0_header,
                session_key_container=session_key_container,
                session_key_data=session_key_data,
                remaining_data=remaining_data,
            )

        except (struct.error, IndexError) as e:
            logger.error(f"Error parsing LOGIN_REQUEST payload: {e}")
            return None


class NPSPacketParser:
    """Parser for NPS protocol packets."""

    # Known message types (based on NPS protocol analysis)
    MESSAGE_TYPES = {
        0x0501: "LOGIN_REQUEST",  # Based on your packet analysis
        0x0145: "LOGIN_RESPONSE",
        0x0101: "HEARTBEAT",
        0x0102: "HANDSHAKE",
        0x0201: "USER_LOGIN",
        0x0202: "USER_AUTH",
    }

    def __init__(self) -> None:
        self.logger = logging.getLogger(f"{__name__}.{self.__class__.__name__}")

    def parse_packet(self, hex_data: str) -> Optional[NPSPacket]:
        """
        Parse a hex string into an NPS packet structure.

        Args:
            hex_data: Hex string representation of the packet

        Returns:
            NPSPacket object or None if parsing fails
        """
        try:
            # Remove any whitespace and convert to bytes
            clean_hex = hex_data.replace(" ", "").replace("\n", "").strip()
            packet_bytes = bytes.fromhex(clean_hex)

            self.logger.info(f"Parsing packet of {len(packet_bytes)} bytes")

            # Determine header format and size
            # Try to parse as much as we can to determine format
            header_bytes = packet_bytes[: min(12, len(packet_bytes))]
            header = self._parse_header(header_bytes)
            if not header:
                return None

            # Determine actual header size based on format
            header_size = 4 if header.format_version == "v0" else 12

            # Make sure we have enough bytes for the detected header format
            if len(packet_bytes) < header_size:
                self.logger.error(
                    f"Packet too short for {header.format_version} header "
                    f"({header_size} bytes needed)"
                )
                return None

            # Extract payload
            payload_start = header_size
            payload_length = (
                header.length - header_size if header.length > header_size else 0
            )

            if (
                payload_length > 0
                and len(packet_bytes) >= payload_start + payload_length
            ):
                payload = packet_bytes[payload_start : payload_start + payload_length]
            else:
                payload = (
                    packet_bytes[payload_start:-4] if len(packet_bytes) > 16 else b""
                )

            # Extract checksum (last 4 bytes)
            checksum = None
            if len(packet_bytes) >= 4:
                checksum = struct.unpack(">I", packet_bytes[-4:])[0]

            packet = NPSPacket(header=header, payload=payload, checksum=checksum)

            self.logger.info(f"Parsed packet: {self._packet_summary(packet)}")

            return packet

        except Exception as e:
            self.logger.error(f"Error parsing packet: {e}")
            return None

    def _parse_header(self, header_bytes: bytes) -> Optional[NPSPacketHeader]:
        """Parse the packet header - supports both v0 and v1 formats (big-endian)."""
        try:
            # Try v0 format first: msgId (u16be), length (u16be) - 4 bytes total
            if len(header_bytes) >= 4:
                msg_id, length = struct.unpack(">HH", header_bytes[:4])

                # Check if this looks like v0 format
                # v0 packets would be shorter and not have the v1 version pattern
                if len(header_bytes) == 4:
                    return NPSPacketHeader(
                        format_version="v0", msg_id=msg_id, length=length
                    )

            # Try v1 format: msgId (u16be), length (u16be), version (u16be),
            # reserved (u16be), length_checksum (u32be)
            if len(header_bytes) >= 12:
                msg_id, length, version, reserved = struct.unpack(
                    ">HHHH", header_bytes[:8]
                )
                length_checksum = struct.unpack(">I", header_bytes[8:12])[0]

                # Check if this looks like v1 format
                # Version should be 0x0101 and length should match length_checksum
                if version == 0x0101 and length == length_checksum:
                    return NPSPacketHeader(
                        format_version="v1",
                        msg_id=msg_id,
                        length=length,
                        version=version,
                        reserved=reserved,
                        length_checksum=length_checksum,
                    )

                # If version is 0x0101 but lengths don't match, still might be v1
                elif version == 0x0101:
                    self.logger.warning(
                        f"v1 format detected but length mismatch: "
                        f"{length} != {length_checksum}"
                    )
                    return NPSPacketHeader(
                        format_version="v1",
                        msg_id=msg_id,
                        length=length,
                        version=version,
                        reserved=reserved,
                        length_checksum=length_checksum,
                    )

                # If we have 12+ bytes but version isn't 0x0101, assume it's still v1
                # (maybe version field has different meaning than expected)
                else:
                    self.logger.info(
                        f"Assuming v1 format with unexpected version: 0x{version:04X}"
                    )
                    return NPSPacketHeader(
                        format_version="v1",
                        msg_id=msg_id,
                        length=length,
                        version=version,
                        reserved=reserved,
                        length_checksum=length_checksum,
                    )

        except struct.error as e:
            self.logger.error(f"Error parsing header: {e}")
            return None

        return None

    def _packet_summary(self, packet: NPSPacket) -> str:
        """Generate a summary string for the packet."""
        msg_type_name = self.MESSAGE_TYPES.get(packet.header.msg_id, "UNKNOWN")
        return (
            f"Format: {packet.header.format_version}, "
            f"MsgID: 0x{packet.header.msg_id:04X} ({msg_type_name}), "
            f"Length: {packet.header.length}, "
            f"Payload: {len(packet.payload)} bytes, "
            f"Checksum: 0x{packet.checksum:08X}"
            if packet.checksum
            else "No checksum"
        )

    def analyze_payload(self, packet: NPSPacket) -> Dict[str, Any]:
        """
        Analyze the packet payload based on message type.

        Args:
            packet: The parsed NPS packet

        Returns:
            Dictionary containing analyzed payload data
        """
        analysis = {
            "message_type": self.MESSAGE_TYPES.get(packet.header.msg_id, "UNKNOWN"),
            "payload_length": len(packet.payload),
            "raw_payload": packet.payload.hex().upper(),
        }

        if packet.header.msg_id == 0x0501:  # LOGIN_REQUEST
            analysis.update(
                self._analyze_login_request(
                    packet.payload, packet.header.format_version
                )
            )
        elif packet.header.msg_id == 0x0201:  # USER_LOGIN
            analysis.update(self._analyze_user_login(packet.payload))
        else:
            # Generic payload analysis
            analysis.update(self._analyze_generic_payload(packet.payload))

        return analysis

    def _analyze_login_request(
        self, payload: bytes, packet_format: str = "v1"
    ) -> Dict[str, Any]:
        """
        Analyze LOGIN_REQUEST payload using confirmed field structure.

        LOGIN_REQUEST (0x0501) payload structure based on confirmed offsets:
        - sessionTicket: packet bytes 14-48 (34 bytes)
        - sessionKey: starts at packet byte 52 (length-prefixed container)
        """
        analysis: Dict[str, Any] = {"type": "LOGIN_REQUEST"}

        try:
            # Use the specific LOGIN_REQUEST parser with confirmed offsets
            login_parser = LoginRequestParser()
            structured_data = login_parser.parse(payload, packet_format)

            if structured_data:
                analysis["structured_fields"] = {
                    "session_ticket_container": {
                        "total_length": len(structured_data.session_ticket_container),
                        "data_length": len(structured_data.session_ticket_data),
                        "hex": structured_data.session_ticket_container.hex().upper(),
                        "data_hex": structured_data.session_ticket_data.hex().upper(),
                        "ascii": self._try_ascii(structured_data.session_ticket_data),
                    },
                    "v0_header": {
                        "length": len(structured_data.v0_header),
                        "hex": structured_data.v0_header.hex().upper(),
                        "msgid": (
                            struct.unpack(">H", structured_data.v0_header)[0]
                            if len(structured_data.v0_header) >= 2
                            else 0
                        ),
                        "description": "V0-style header (msgId=0x0000 empty msg)",
                    },
                    "session_key_container": {
                        "total_length": len(structured_data.session_key_container),
                        "key_data_length": len(structured_data.session_key_data),
                        "hex_preview": (
                            structured_data.session_key_container[:64].hex().upper()
                            + "..."
                            if len(structured_data.session_key_container) > 32
                            else structured_data.session_key_container.hex().upper()
                        ),
                    },
                    "session_key_data": {
                        "length": len(structured_data.session_key_data),
                        "hex_preview": (
                            structured_data.session_key_data[:32].hex().upper() + "..."
                            if len(structured_data.session_key_data) > 16
                            else structured_data.session_key_data.hex().upper()
                        ),
                        "ascii": self._try_ascii(structured_data.session_key_data),
                    },
                    "remaining_data": {
                        "length": len(structured_data.remaining_data),
                        "hex": structured_data.remaining_data.hex().upper(),
                        "ascii": self._try_ascii(structured_data.remaining_data),
                    },
                }
            else:
                analysis["error"] = "Failed to parse with structured parser"

            # Also include generic parsing for comparison
            parsed_data = self._parse_nps_payload(payload)
            analysis["generic_parsing"] = parsed_data

            # Include raw string extraction
            strings = self._extract_strings(payload)
            if strings:
                analysis["extracted_strings"] = strings

        except Exception as e:
            analysis["error"] = str(e)

        return analysis

    def _parse_nps_payload(self, payload: bytes) -> Dict[str, Any]:
        """Parse NPS payload using known data types."""
        parsed: Dict[str, Any] = {"fields": [], "offset": 0}
        offset = 0
        field_num = 1

        while offset < len(payload) - 1:  # Leave room for at least 1 byte
            try:
                field_info = f"Field {field_num} (offset {offset})"

                # Try to parse as container (u16 length prefix)
                if offset + 2 <= len(payload):
                    container_length = struct.unpack(
                        ">H", payload[offset : offset + 2]
                    )[0]
                    if container_length > 0 and offset + 2 + container_length <= len(
                        payload
                    ):
                        container_data = payload[
                            offset + 2 : offset + 2 + container_length
                        ]
                        parsed["fields"].append(
                            {
                                "field": field_info,
                                "type": "container",
                                "length": container_length,
                                "data": container_data.hex().upper(),
                                "ascii_attempt": self._try_ascii(container_data),
                            }
                        )
                        offset += 2 + container_length
                        field_num += 1
                        continue

                # Try to parse as u16-prefixed string
                if offset + 2 <= len(payload):
                    str_length = struct.unpack(">H", payload[offset : offset + 2])[0]
                    if (
                        str_length > 0
                        and str_length < 1000
                        and offset + 2 + str_length <= len(payload)
                    ):
                        string_data = payload[offset + 2 : offset + 2 + str_length]
                        # Check if it's mostly ASCII
                        if self._is_mostly_ascii(string_data):
                            parsed["fields"].append(
                                {
                                    "field": field_info,
                                    "type": "u16_string",
                                    "length": str_length,
                                    "data": string_data.decode(
                                        "ascii", errors="ignore"
                                    ),
                                    "raw_hex": string_data.hex().upper(),
                                }
                            )
                            offset += 2 + str_length
                            field_num += 1
                            continue

                # Try to parse as short (u16)
                if offset + 2 <= len(payload):
                    short_val = struct.unpack(">H", payload[offset : offset + 2])[0]
                    parsed["fields"].append(
                        {
                            "field": field_info,
                            "type": "short",
                            "value": short_val,
                            "hex": f"0x{short_val:04X}",
                        }
                    )
                    offset += 2
                    field_num += 1
                    continue

                # Try to parse as long (u32)
                if offset + 4 <= len(payload):
                    long_val = struct.unpack(">I", payload[offset : offset + 4])[0]
                    parsed["fields"].append(
                        {
                            "field": field_info,
                            "type": "long",
                            "value": long_val,
                            "hex": f"0x{long_val:08X}",
                        }
                    )
                    offset += 4
                    field_num += 1
                    continue

                # If we can't parse anything sensible, try null-terminated string
                null_pos = payload.find(b"\x00", offset)
                if null_pos != -1 and null_pos > offset:
                    string_data = payload[offset:null_pos]
                    if self._is_mostly_ascii(string_data):
                        parsed["fields"].append(
                            {
                                "field": field_info,
                                "type": "null_terminated_string",
                                "data": string_data.decode("ascii", errors="ignore"),
                                "raw_hex": string_data.hex().upper(),
                            }
                        )
                        offset = null_pos + 1  # Skip the null byte
                        field_num += 1
                        continue

                # If nothing else works, take a single byte and move on
                byte_val = payload[offset]
                parsed["fields"].append(
                    {
                        "field": field_info,
                        "type": "byte",
                        "value": byte_val,
                        "hex": f"0x{byte_val:02X}",
                    }
                )
                offset += 1
                field_num += 1

            except (struct.error, IndexError):
                # If we can't parse any more, stop
                break

        parsed["total_parsed"] = offset
        parsed["remaining_bytes"] = len(payload) - offset

        return parsed

    def _is_mostly_ascii(self, data: bytes) -> bool:
        """Check if data is mostly printable ASCII."""
        if len(data) == 0:
            return False

        ascii_count = sum(1 for b in data if 32 <= b <= 126)
        return ascii_count / len(data) > 0.7  # At least 70% ASCII

    def _try_ascii(self, data: bytes) -> str:
        """Try to decode as ASCII, return hex if it fails."""
        try:
            decoded = data.decode("ascii", errors="ignore")
            if self._is_mostly_ascii(data):
                return f"'{decoded}'"
            else:
                return f"(non-ASCII: {data.hex().upper()})"
        except Exception:
            return f"(binary: {data.hex().upper()})"

    def _analyze_user_login(self, payload: bytes) -> Dict[str, Any]:
        """Analyze USER_LOGIN payload."""
        return {"type": "USER_LOGIN", "hex_data": payload.hex().upper()}

    def _analyze_generic_payload(self, payload: bytes) -> Dict[str, Any]:
        """Generic payload analysis."""
        analysis: Dict[str, Any] = {}

        # Look for ASCII strings
        strings = self._extract_strings(payload)
        if strings:
            analysis["strings"] = strings

        # Look for patterns
        analysis["patterns"] = self._find_patterns(payload)

        return analysis

    def _extract_strings(self, data: bytes, min_length: int = 4) -> List[str]:
        """Extract ASCII strings from binary data."""
        strings: List[str] = []
        current_string = ""

        for byte in data:
            if 32 <= byte <= 126:  # Printable ASCII
                current_string += chr(byte)
            else:
                if len(current_string) >= min_length:
                    strings.append(current_string)
                current_string = ""

        if len(current_string) >= min_length:
            strings.append(current_string)

        return strings

    def _find_hex_patterns(self, data: bytes) -> Dict[str, str]:
        """Find common hex patterns in the data."""
        patterns = {}

        # Look for UUIDs or session tokens (32+ hex chars)
        hex_str = data.hex().upper()

        # Find long hex sequences that might be tokens
        import re

        long_sequences = re.findall(r"[0-9A-F]{32,}", hex_str)

        for i, seq in enumerate(long_sequences):
            patterns[f"hex_sequence_{i+1}"] = seq

        return patterns

    def _find_patterns(self, data: bytes) -> Dict[str, Any]:
        """Find various patterns in the data."""
        patterns: Dict[str, Any] = {}

        # Look for repeated bytes
        from collections import Counter

        byte_counts = Counter(data)
        most_common = byte_counts.most_common(3)
        patterns["common_bytes"] = [
            {"byte": f"0x{b:02X}", "count": c} for b, c in most_common
        ]

        # Look for null-terminated strings
        null_splits = data.split(b"\x00")
        non_empty_splits = [s for s in null_splits if s and len(s) > 2]
        if non_empty_splits:
            patterns["null_terminated_segments"] = [
                s.hex().upper() for s in non_empty_splits
            ]

        return patterns


def main() -> None:
    """Demo function to test the parser."""
    # Sample packet from the user
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

    if packet:
        print("=== NPS Packet Analysis ===")
        print(f"Header: {packet.header}")
        print(f"Payload Length: {len(packet.payload)} bytes")
        print(
            f"Checksum: 0x{packet.checksum:08X}" if packet.checksum else "No checksum"
        )

        analysis = parser.analyze_payload(packet)
        print("\n=== Payload Analysis ===")
        for key, value in analysis.items():
            print(f"{key}: {value}")
    else:
        print("Failed to parse packet")


if __name__ == "__main__":
    main()
