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
Session Key Decryption Module

This module provides functionality to decrypt session keys from NPS LOGIN_REQUEST
packets using RSA private key decryption, based on the Motor City Online protocol.
"""

import logging
import struct
from pathlib import Path
from typing import Any, Dict, Optional, Tuple

logger = logging.getLogger(__name__)

# Import cryptography with fallback
try:
    from cryptography.hazmat.primitives import hashes, serialization
    from cryptography.hazmat.primitives.asymmetric import padding, rsa

    crypto_available = True
except ImportError:
    crypto_available = False
    serialization = None  # type: ignore
    padding = None  # type: ignore
    rsa = None  # type: ignore
    hashes = None  # type: ignore


class SessionKeyDecryptor:
    """Handles decryption of session keys from NPS LOGIN_REQUEST packets."""

    def __init__(self, private_key_path: Optional[str] = None) -> None:
        """
        Initialize the session key decryptor.

        Args:
            private_key_path: Path to the RSA private key file. If None, uses
                default path.
        """
        self.logger = logging.getLogger(f"{__name__}.{self.__class__.__name__}")

        if not crypto_available:
            raise ImportError(
                "cryptography library is required for session key decryption. "
                "Install with: pip install cryptography"
            )

        # Default to data/private_key.pem
        if private_key_path is None:
            private_key_path = str(
                Path(__file__).parent.parent / "data" / "private_key.pem"
            )

        self.private_key_path = private_key_path
        self.private_key = self._load_private_key()

    def _load_private_key(self) -> Any:
        """Load the RSA private key from file."""
        try:
            with open(self.private_key_path, "rb") as key_file:
                private_key = serialization.load_pem_private_key(  # type: ignore
                    key_file.read(),
                    password=None,  # No password for MCO keys
                )

            if not isinstance(private_key, rsa.RSAPrivateKey):  # type: ignore
                raise ValueError("Private key is not an RSA key")

            self.logger.info(f"Loaded RSA private key from {self.private_key_path}")
            return private_key

        except Exception as e:
            self.logger.error(f"Failed to load private key: {e}")
            raise

    def decrypt_session_key(self, encrypted_data: bytes) -> Optional[bytes]:
        """
        Decrypt a session key using RSA private key.

        Based on CUSTOM1_PROTOCOL.md, the encrypted data should be 128 bytes.
        We'll try multiple padding schemes as the documentation mentions OAEP
        but older implementations might use PKCS1v15.

        Args:
            encrypted_data: The encrypted session key bytes (should be 128 bytes)

        Returns:
            Decrypted session key bytes, or None if decryption fails
        """
        try:
            if len(encrypted_data) != 128:
                self.logger.warning(
                    f"Expected 128 bytes for RSA decryption, got {len(encrypted_data)}"
                )

            # Try OAEP padding first (as mentioned in documentation)
            try:
                decrypted = self.private_key.decrypt(  # type: ignore
                    encrypted_data,
                    padding.OAEP(  # type: ignore
                        mgf=padding.MGF1(algorithm=hashes.SHA1()),  # type: ignore
                        algorithm=hashes.SHA1(),  # type: ignore
                        label=None,
                    ),
                )

                self.logger.info(
                    f"Successfully decrypted with OAEP padding ({len(decrypted)} bytes)"
                )
                return self._parse_decrypted_session_key(decrypted)

            except Exception as oaep_error:
                self.logger.debug(f"OAEP decryption failed: {oaep_error}")

                # Try PKCS1v15 padding as fallback (older systems)
                try:
                    decrypted = self.private_key.decrypt(  # type: ignore
                        encrypted_data, padding.PKCS1v15()  # type: ignore
                    )

                    self.logger.info(
                        f"Successfully decrypted with PKCS1v15 padding "
                        f"({len(decrypted)} bytes)"
                    )
                    return self._parse_decrypted_session_key(decrypted)

                except Exception as pkcs_error:
                    self.logger.error("Both OAEP and PKCS1v15 decryption failed")
                    self.logger.debug(f"OAEP error: {oaep_error}")
                    self.logger.debug(f"PKCS1v15 error: {pkcs_error}")
                    return None

        except Exception as e:
            self.logger.error(f"Failed to decrypt session key: {e}")
            return None

    def _parse_decrypted_session_key(self, decrypted: bytes) -> Optional[bytes]:
        """
        Parse the decrypted RSA blob to extract the session key.

        According to CUSTOM1_PROTOCOL.md, the decrypted blob contains:
        - 2 bytes: session key length (big-endian)
        - N bytes: session key
        - 4 bytes: session key expiration (big-endian uint32)

        However, we may need to handle different formats.
        """
        try:
            self.logger.info(f"Analyzing decrypted blob: {len(decrypted)} bytes")
            self.logger.info(f"Decrypted hex: {decrypted.hex()}")

            if len(decrypted) < 6:
                self.logger.warning(
                    "Decrypted data too short for expected format "
                    "(need at least 6 bytes)"
                )
                # Maybe the entire blob is the session key?
                if len(decrypted) == 32:
                    self.logger.info(
                        "Decrypted data is exactly 32 bytes - "
                        "treating as direct session key"
                    )
                    return decrypted
                elif len(decrypted) >= 16:
                    self.logger.info(
                        f"Decrypted data is {len(decrypted)} bytes - "
                        "treating as direct session key"
                    )
                    return decrypted
                else:
                    self.logger.error(
                        "Decrypted data too short to be a valid session key"
                    )
                    return None

            # Try to parse according to the protocol spec
            try:
                # Extract session key length
                session_key_length = struct.unpack(">H", decrypted[0:2])[0]
                self.logger.debug(
                    f"Session key length from header: {session_key_length}"
                )

                # Check if this looks reasonable - MCO session keys are typically 16-32
                # bytes
                if session_key_length > 100 or session_key_length < 8:
                    self.logger.warning(
                        f"Session key length {session_key_length} seems unreasonable"
                    )
                    # Treat the whole blob as the session key
                    self.logger.info(
                        f"Treating entire {len(decrypted)} byte blob as session key"
                    )
                    return decrypted

                if len(decrypted) < 2 + session_key_length + 4:
                    self.logger.warning(
                        "Decrypted data too short for session key + expiration"
                    )
                    # Maybe there's no expiration field?
                    if len(decrypted) >= 2 + session_key_length:
                        session_key = decrypted[2 : 2 + session_key_length]
                        self.logger.info(
                            f"Extracted {len(session_key)} byte session key "
                            "(no expiration)"
                        )
                        return session_key
                    else:
                        self.logger.error(
                            f"Not enough data for session key of length "
                            f"{session_key_length}"
                        )
                        return None

                # Extract session key
                session_key = decrypted[2 : 2 + session_key_length]

                # Extract expiration (for logging)
                expiration = struct.unpack(
                    ">I", decrypted[2 + session_key_length : 2 + session_key_length + 4]
                )[0]
                self.logger.info(f"Session key expiration timestamp: {expiration}")

                return session_key

            except struct.error as e:
                self.logger.warning(f"Failed to parse header: {e}")
                # Maybe the whole thing is just a session key
                if len(decrypted) >= 16:  # Reasonable minimum for a session key
                    self.logger.info(
                        f"Treating entire {len(decrypted)} byte blob as session key"
                    )
                    return decrypted
                else:
                    return None

        except Exception as e:
            self.logger.error(f"Failed to parse decrypted session key: {e}")
            return None

    def extract_and_decrypt_from_parsed_packet(
        self, packet_analysis: Dict[str, Any]
    ) -> Optional[Tuple[bytes, str]]:
        """
        Extract and decrypt session key from a parsed LOGIN_REQUEST packet.

        This uses the direct session_key_data field from the CLI output.

        Args:
            packet_analysis: The analysis dict containing session_key_data

        Returns:
            Tuple of (decrypted_session_key, session_ticket) or None if extraction fails
        """
        try:
            # Extract session ticket - check multiple possible locations
            session_ticket = ""

            # First try to get from extracted strings
            if (
                "extracted_strings" in packet_analysis
                and packet_analysis["extracted_strings"]
            ):
                # First extracted string is often the session ticket (40 chars)
                for string in packet_analysis["extracted_strings"]:
                    if len(string) == 40:  # Session tickets are typically 40 hex chars
                        session_ticket = string
                        self.logger.info(
                            f"Extracted session ticket from strings: {session_ticket}"
                        )
                        break

            # Extract session key data - this should be directly available
            if "session_key_data" not in packet_analysis:
                self.logger.error("No session_key_data field in packet analysis")
                return None

            session_key_hex = packet_analysis["session_key_data"]

            if not isinstance(session_key_hex, str):
                self.logger.error(
                    f"session_key_data should be string, got {type(session_key_hex)}"
                )
                return None

            # Clean the hex string
            session_key_hex = session_key_hex.strip()

            self.logger.info(
                f"Found session key data: {len(session_key_hex)} hex chars"
            )
            self.logger.debug(f"Session key hex: {session_key_hex[:64]}...")

            if len(session_key_hex) == 256:  # 128 bytes = 256 hex chars
                # This is exactly what we expect for RSA encrypted data
                self.logger.info(
                    "Session key data is 256 hex chars (128 bytes) - perfect for RSA"
                )

                # Convert hex string to binary (should be 128 bytes)
                try:
                    encrypted_data = bytes.fromhex(session_key_hex)
                    self.logger.info(
                        f"Converted to {len(encrypted_data)} bytes for RSA decryption"
                    )

                    # Decrypt the session key
                    decrypted_key = self.decrypt_session_key(encrypted_data)
                    if decrypted_key is not None:
                        self.logger.info(
                            "Successfully decrypted session key from parsed packet"
                        )
                        return decrypted_key, session_ticket
                    else:
                        self.logger.error("Failed to decrypt session key")
                        return None

                except ValueError as e:
                    self.logger.error(f"Invalid hex string: {e}")
                    return None

            elif len(session_key_hex) == 512:  # 256 bytes = 512 hex chars
                # This would be a larger container, take first 128 bytes
                self.logger.info(
                    "Session key data is 512 hex chars, taking first 128 bytes for RSA"
                )

                try:
                    full_data = bytes.fromhex(session_key_hex)

                    if len(full_data) != 256:
                        self.logger.error(
                            f"Expected 256 bytes after hex conversion, got "
                            f"{len(full_data)}"
                        )
                        return None

                    # Take first 128 bytes for RSA decryption
                    encrypted_data = full_data[:128]
                    self.logger.info(
                        f"Using first {len(encrypted_data)} bytes for RSA decryption"
                    )

                    # Decrypt the session key
                    decrypted_key = self.decrypt_session_key(encrypted_data)
                    if decrypted_key is not None:
                        self.logger.info(
                            "Successfully decrypted session key from parsed packet"
                        )
                        return decrypted_key, session_ticket
                    else:
                        self.logger.error("Failed to decrypt session key")
                        return None

                except ValueError as e:
                    self.logger.error(f"Invalid hex string: {e}")
                    return None
            else:
                self.logger.error(
                    f"Expected 256 or 512 hex characters, got {len(session_key_hex)}"
                )
                return None

        except Exception as e:
            self.logger.error(f"Failed to extract session key from parsed packet: {e}")
            return None

    def extract_and_decrypt_from_login_packet(
        self, packet_payload: bytes
    ) -> Optional[Tuple[bytes, str]]:
        """
        Extract and decrypt session key from a LOGIN_REQUEST packet payload.

        Based on CUSTOM1_PROTOCOL.md, we need to look for Field2 - a 256-character
        ASCII hex string representing 128 bytes of RSA-encrypted data.

        Args:
            packet_payload: The LOGIN_REQUEST packet payload

        Returns:
            Tuple of (decrypted_session_key, session_ticket) or None if extraction fails
        """
        try:
            # First, extract session ticket (for context)
            if len(packet_payload) < 42:
                self.logger.error("Packet payload too short")
                return None

            session_ticket_length = struct.unpack(">H", packet_payload[0:2])[0]
            session_ticket_data = packet_payload[2 : 2 + session_ticket_length]
            session_ticket = session_ticket_data.hex()

            # Look for Field2 - a 256-character hex string in the packet
            # Convert payload to string form to search for hex patterns
            payload_hex = packet_payload.hex().upper()

            # Field2 should be a 256-character (128 bytes) hex string
            # Look for patterns that could be this field
            for i in range(0, len(payload_hex) - 255, 2):  # Step by 2 (hex chars)
                potential_field2 = payload_hex[i : i + 256]

                # Check if this looks like a valid hex string (all hex chars)
                if all(c in "0123456789ABCDEF" for c in potential_field2):
                    try:
                        # Convert hex string to binary
                        encrypted_data = bytes.fromhex(potential_field2)

                        if len(encrypted_data) == 128:
                            self.logger.info(
                                f"Found potential Field2 at offset {i // 2}"
                            )
                            self.logger.debug(f"Field2 hex: {potential_field2}")

                            # Try to decrypt it
                            decrypted_key = self.decrypt_session_key(encrypted_data)
                            if decrypted_key is not None:
                                self.logger.info(
                                    "Successfully decrypted session key from Field2"
                                )
                                return decrypted_key, session_ticket

                    except Exception as e:
                        # This potential field didn't work, try the next one
                        self.logger.debug(f"Field2 candidate at {i // 2} failed: {e}")
                        continue

            # Alternative approach: look for the session key container from our packet
            # analysis
            self.logger.info("Field2 not found, trying session key container approach")
            return self._extract_from_session_key_container(
                packet_payload, session_ticket
            )

        except Exception as e:
            self.logger.error(f"Failed to extract session key from packet: {e}")
            return None

    def _extract_from_session_key_container(
        self, packet_payload: bytes, session_ticket: str
    ) -> Optional[Tuple[bytes, str]]:
        """
        Fallback method to extract from the session key container we identified
        in packet analysis.
        """
        try:
            # Skip V0-style header (2 bytes at offset 42)
            # Container 2: Encrypted session key (starts at offset 44)
            session_key_offset = 44
            if len(packet_payload) < session_key_offset + 2:
                self.logger.error("Packet payload too short for session key container")
                return None

            # Extract session key container length
            session_key_length = struct.unpack(
                ">H", packet_payload[session_key_offset : session_key_offset + 2]
            )[0]
            session_key_start = session_key_offset + 2
            session_key_end = session_key_start + session_key_length

            if len(packet_payload) < session_key_end:
                self.logger.error("Packet payload too short for session key data")
                return None

            encrypted_session_key = packet_payload[session_key_start:session_key_end]

            self.logger.info(
                f"Extracted encrypted session key from container "
                f"({len(encrypted_session_key)} bytes)"
            )

            # If this is exactly 128 bytes, try direct decryption
            if len(encrypted_session_key) == 128:
                decrypted_key = self.decrypt_session_key(encrypted_session_key)
                if decrypted_key is not None:
                    return decrypted_key, session_ticket

            # If it's 256 bytes, it might be hex-encoded
            if len(encrypted_session_key) == 256:
                try:
                    # Try to interpret as hex string
                    hex_string = encrypted_session_key.decode("ascii")
                    if all(c in "0123456789ABCDEFabcdef" for c in hex_string):
                        binary_data = bytes.fromhex(hex_string)
                        if len(binary_data) == 128:
                            self.logger.info(
                                "Found hex-encoded 128-byte encrypted data"
                            )
                            decrypted_key = self.decrypt_session_key(binary_data)
                            if decrypted_key is not None:
                                return decrypted_key, session_ticket
                except Exception:
                    pass

            self.logger.error("Could not decrypt session key from container")
            return None

        except Exception as e:
            self.logger.error(f"Failed to extract from session key container: {e}")
            return None

    def validate_session_key(self, session_key: bytes) -> bool:
        """
        Validate that a decrypted session key looks reasonable.

        Args:
            session_key: The decrypted session key bytes

        Returns:
            True if the session key appears valid
        """
        # Basic validation - MCO session keys should be 16-32 bytes (flexible range)
        if len(session_key) < 16 or len(session_key) > 32:
            self.logger.warning(
                f"Session key length {len(session_key)} outside expected range "
                "16-32 bytes"
            )
            return False

        # Check for null key (all zeros)
        if session_key == b"\x00" * len(session_key):
            self.logger.warning("Session key is all zeros")
            return False

        # Check for obvious patterns that might indicate failed decryption
        if len(set(session_key)) < 4:  # Too few unique bytes
            self.logger.warning("Session key has too few unique bytes")
            return False

        self.logger.info(
            f"Session key validation passed: {len(session_key)} bytes with good entropy"
        )
        return True


def main() -> None:
    """Demo function to test session key decryption."""
    if not crypto_available:
        print("❌ Cryptography library not available")
        print("Install with: pip install cryptography")
        return

    print("🔐 Session Key Decryptor Demo")
    print("=" * 50)

    try:
        # Initialize decryptor
        decryptor = SessionKeyDecryptor()
        print(f"✅ Loaded private key from {decryptor.private_key_path}")

        # Use known working data from our test
        print("\n🔍 Testing with confirmed working session key data")

        # Create analysis dict with known working data
        analysis = {
            "session_key_data": (
                "23A989B1C7DBBEF76A410E0AA6FBB579E0222A52C9FCE1C6D0E291B10B7867E6"
                "7D632C65A8FB33C7A2C495CB4FCFD5C1F117B8EFC41C151582D0F5E6E3A2F46"
                "243D9A0747B5063881E50A2506BF22DCFE6CFF961B1E4926943C3B52EA0AC56"
                "D08F2F52B276603FA7CC479F07D89660E579E4207399670BE6DAAA2DAC0BFE82E9"
            ),
            "extracted_strings": ["a986ea58099a5a37a597d0059870a314850954ca"],
        }

        # Extract and decrypt session key
        result = decryptor.extract_and_decrypt_from_parsed_packet(analysis)

        if result:
            decrypted_key, session_ticket = result
            print("✅ Successfully decrypted session key!")
            print(f"   Session Ticket: {session_ticket}")
            print(f"   Decrypted Key: {decrypted_key.hex()}")
            print(f"   Key Length: {len(decrypted_key)} bytes")
            print(f"   Valid: {decryptor.validate_session_key(decrypted_key)}")
        else:
            print("❌ Failed to decrypt session key")

    except Exception as e:
        print(f"❌ Error: {e}")


if __name__ == "__main__":
    main()
