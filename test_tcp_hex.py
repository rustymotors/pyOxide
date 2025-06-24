#!/usr/bin/env python3
"""
Test TCP client to verify hex dumping functionality.
"""

import socket
import sys


def test_tcp_server():
    """Test the TCP server with various data patterns."""
    try:
        # Connect to TCP server
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.settimeout(5.0)
        sock.connect(("localhost", 8226))
        print("Connected to TCP server on port 8226")

        # Test data with leading zeros and various byte patterns
        test_patterns = [
            bytes([0x00, 0x01, 0x02, 0x03]),  # Leading zeros
            bytes([0xFF, 0xFE, 0xFD, 0xFC]),  # High bytes
            b"Hello World!",  # ASCII text
            bytes([0x00, 0x0A, 0x0D, 0x00]),  # Mixed zeros and control chars
            bytes(range(16)),  # 0x00 through 0x0F
        ]

        for i, data in enumerate(test_patterns):
            print(f"\nSending test pattern {i+1}: {data.hex().upper()}")
            sock.send(data)

            # Receive echo
            response = sock.recv(len(data))
            print(f"Received echo: {response.hex().upper()}")

            if response != data:
                print(f"ERROR: Echo mismatch!")

        sock.close()
        print("\nTest completed successfully!")

    except Exception as e:
        print(f"Test failed: {e}")
        return False

    return True


if __name__ == "__main__":
    test_tcp_server()
