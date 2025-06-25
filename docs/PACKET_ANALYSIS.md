# NPS Packet Analysis

This document describes the Network Protocol Server (NPS) packet structure and analysis tools available in pyOxide.

## Packet Structure

Based on analysis of packets received on port 8226, the NPS protocol appears to use the following packet structure:

### Header Format

```
Offset | Size | Field        | Description
-------|------|--------------|-------------------------------------
0x00   | 2    | Magic        | Protocol identifier (0x0501)
0x02   | 2    | Message Type | Type of message (e.g., 0x0144 for LOGIN_REQUEST)
0x04   | 2    | Sub Type     | Message subtype
0x06   | 2    | Flags        | Protocol flags
0x08   | 2    | Sequence     | Sequence number
0x0A   | 2    | Length       | Total packet length including header
```

### Message Types

| Code   | Name           | Description                    |
|--------|----------------|--------------------------------|
| 0x0144 | LOGIN_REQUEST  | Client login attempt           |
| 0x0145 | LOGIN_RESPONSE | Server login response          |
| 0x0101 | HEARTBEAT      | Keep-alive message             |
| 0x0102 | HANDSHAKE      | Connection initialization      |
| 0x0201 | USER_LOGIN     | User authentication            |
| 0x0202 | USER_AUTH      | User authorization             |

## Sample Packet Analysis

Here's analysis of a real LOGIN_REQUEST packet received on port 8226:

```
Raw Hex: 0501014401010000000001440028663433336338383364333363613630636661386665363365636531636366643031343138643361620000010034413945443236353344423032423345423838303437304142314637454330303042314233314336353544453144313631314437443138343739433530343236433236414437303236454435334438464144394437373945453643413138353846454645463039343144333530394545313444304136384342334534393532423135334530463736464438323037313139343338463537443930334136344137384235313734353544423836464546453043333438453831413146323932453430314544373545323842373330434642373830393546374238364542383336434437304137334632454639434430303030373142423045464132463837463732000432313736FEA31C19

Header Analysis:
- Magic Number: 0x0501 (NPS protocol identifier)
- Message Type: 0x0144 (LOGIN_REQUEST)
- Sub Type: 0x0101
- Flags: 0x0000
- Sequence: 0
- Length: 324 bytes
- Checksum: 0x31433139 (last 4 bytes)

Payload Analysis:
- Payload Length: 312 bytes
- Contains encrypted session tokens/credentials
- Includes hash values that appear to be authentication related
- String patterns suggest session IDs and cryptographic tokens
```

## Analysis Tools

### 1. NPSPacketParser Class

The main parser class provides:

- `parse_packet(hex_data)` - Parse hex string into structured packet
- `analyze_payload(packet)` - Analyze packet payload based on message type
- String extraction from binary data
- Hex pattern recognition
- Message type identification

### 2. Command Line Tool

Use `analyze_packet.py` for interactive packet analysis:

```bash
# Analyze from hex string
python analyze_packet.py -s "0501014401010000..."

# Analyze from file
python analyze_packet.py -f packet_data.txt

# Use built-in sample
python analyze_packet.py --sample
```

### 3. Integration with pyOxide Server

The packet parser can be integrated with the main server to analyze incoming packets in real-time:

```python
from src.packet_parser import NPSPacketParser

parser = NPSPacketParser()

# In your packet handler
def handle_packet(raw_data):
    packet = parser.parse_packet(raw_data.hex())
    if packet:
        analysis = parser.analyze_payload(packet)
        # Process based on message type
        if packet.header.message_type == 0x0144:  # LOGIN_REQUEST
            handle_login_request(packet, analysis)
```

## Security Considerations

- Packets contain encrypted authentication data
- Session tokens should be handled securely
- Packet analysis should not log sensitive data in production
- Implement proper validation before processing packet contents

## Future Development

Potential enhancements:

1. **Encryption Support** - Decrypt payload content where possible
2. **Protocol Validation** - Verify packet checksums and integrity
3. **Real-time Analysis** - Integrate with server for live packet monitoring
4. **Extended Message Types** - Support for additional NPS message types
5. **Binary Export** - Export parsed packets to structured formats (JSON, XML)

## Testing

Run the packet parser tests:

```bash
hatch run test tests/test_packet_parser.py
```

The test suite covers:
- Basic packet parsing
- Payload analysis
- Error handling
- Large packet support
- Message type identification
