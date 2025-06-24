# Protocol Specification Template

> **Instructions**: Fill out this template with the specific details of your binary protocol. Remove sections that don't apply and add custom sections as needed.

## Protocol Overview

### Protocol Name
**Name**: [e.g., MCO Universe Protocol, Custom Game Protocol, etc.]

### Version
**Version**: [e.g., 1.0, 2.3, etc.]
**Revision Date**: [YYYY-MM-DD]

### Purpose
**Description**: [Brief description of what this protocol is used for]

**Use Cases**:
- [Primary use case]
- [Secondary use case]
- [Additional use cases...]

## Connection Details

### Transport Layer
- **Protocol**: [TCP/UDP]
- **Default Port(s)**: [e.g., 8226, 8228, etc.]
- **Encryption**: [None/TLS/Custom/etc.]
- **Authentication**: [None/Required/Optional]

### Connection Flow
1. [Step 1: e.g., Client connects to server]
2. [Step 2: e.g., Authentication handshake]
3. [Step 3: e.g., Protocol negotiation]
4. [Step 4: e.g., Data exchange begins]

## Packet Structure

### General Packet Format

```
[Header][Payload][Footer (optional)]
```

### Header Format

| Field Name | Size (bytes) | Type | Description | Example |
|------------|--------------|------|-------------|---------|
| [Field 1] | [Size] | [uint8/uint16/etc.] | [Description] | [0x01] |
| [Field 2] | [Size] | [uint8/uint16/etc.] | [Description] | [0x00FF] |
| [Field 3] | [Size] | [uint8/uint16/etc.] | [Description] | [0x12345678] |

**Total Header Size**: [X] bytes

### Payload Format

**Structure**: [Fixed-length/Variable-length/Mixed]

| Field Name | Size (bytes) | Type | Description | Required |
|------------|--------------|------|-------------|----------|
| [Field 1] | [Size/Variable] | [Type] | [Description] | [Yes/No] |
| [Field 2] | [Size/Variable] | [Type] | [Description] | [Yes/No] |

### Footer Format (if applicable)

| Field Name | Size (bytes) | Type | Description |
|------------|--------------|------|-------------|
| [Field 1] | [Size] | [Type] | [Description] |

## Packet Types

### Command Packets

#### Packet Type: [COMMAND_NAME]
- **Opcode**: [0x01]
- **Description**: [What this command does]
- **Direction**: [Client→Server / Server→Client / Bidirectional]

**Payload Structure**:
```
Offset | Size | Type   | Field Name | Description
-------|------|--------|------------|-------------
0x00   | 2    | uint16 | [field]    | [description]
0x02   | 4    | uint32 | [field]    | [description]
```

**Example**:
```hex
01 00 FF FF 12 34 56 78
```

#### Packet Type: [ANOTHER_COMMAND]
- **Opcode**: [0x02]
- **Description**: [What this command does]
- **Direction**: [Client→Server / Server→Client / Bidirectional]

[Continue for all packet types...]

### Response Packets

#### Response Type: [RESPONSE_NAME]
- **Opcode**: [0x81]
- **Description**: [Response to which command]
- **Direction**: [Server→Client]

**Payload Structure**:
[Similar format as above]

### Data Packets

#### Data Type: [DATA_NAME]
- **Opcode**: [0x10]
- **Description**: [What data this contains]
- **Direction**: [Bidirectional]

[Continue for all data types...]

## Data Types and Encoding

### Primitive Types
- **uint8**: 1 byte unsigned integer
- **uint16**: 2 byte unsigned integer, [little-endian/big-endian]
- **uint32**: 4 byte unsigned integer, [little-endian/big-endian]
- **string**: [null-terminated/length-prefixed/fixed-length]
- **timestamp**: [Unix timestamp/Custom format]

### Custom Types

#### Type: [CUSTOM_TYPE_NAME]
**Size**: [Variable/Fixed] bytes
**Description**: [What this type represents]

**Structure**:
```
Offset | Size | Type   | Field Name | Description
-------|------|--------|------------|-------------
0x00   | 1    | uint8  | [field]    | [description]
0x01   | 4    | uint32 | [field]    | [description]
```

### String Encoding
- **Character Set**: [UTF-8/ASCII/UTF-16/etc.]
- **Null Termination**: [Yes/No]
- **Length Prefix**: [None/1 byte/2 bytes/4 bytes]
- **Maximum Length**: [X] characters

## Protocol Flow Examples

### Example 1: [Scenario Name]

**Description**: [What happens in this scenario]

```
Client                    Server
  |                         |
  |-- [Packet Name] ------->|
  |                         |
  |<----- [Response] -------|
  |                         |
```

**Packet Details**:
1. **[Packet Name]**: [Hex dump and explanation]
2. **[Response]**: [Hex dump and explanation]

### Example 2: [Another Scenario]

[Similar format...]

## Error Handling

### Error Codes

| Code | Name | Description | Recovery Action |
|------|------|-------------|-----------------|
| 0x00 | SUCCESS | Operation successful | Continue |
| 0x01 | [ERROR_NAME] | [Description] | [Action] |
| 0x02 | [ERROR_NAME] | [Description] | [Action] |

### Error Packet Format

**Opcode**: [0xFF]

| Field Name | Size | Type | Description |
|------------|------|------|-------------|
| Error Code | 1 | uint8 | Error code from table above |
| Error Message | Variable | string | Human-readable error description |

## Implementation Notes

### Parsing Considerations
- **Byte Order**: [Little-endian/Big-endian/Mixed]
- **Alignment**: [Required/Not required]
- **Padding**: [Used/Not used]
- **Buffer Size**: Recommended minimum [X] bytes

### Validation Rules
- [Rule 1: e.g., Header magic number must be 0x1234]
- [Rule 2: e.g., Packet length must not exceed 65535 bytes]
- [Rule 3: e.g., Opcode must be valid]

### Security Considerations
- [Consideration 1: e.g., Validate all length fields]
- [Consideration 2: e.g., Sanitize string inputs]
- [Consideration 3: e.g., Rate limiting recommendations]

## Testing Data

### Valid Test Packets

#### Test Case 1: [Test Name]
**Description**: [What this tests]
**Raw Data**:
```hex
01 00 FF FF 12 34 56 78 9A BC DE F0
```
**Expected Result**: [What should happen when parsing this]

#### Test Case 2: [Test Name]
**Raw Data**:
```hex
[hex data]
```
**Expected Result**: [Expected parsing result]

### Invalid Test Packets

#### Invalid Case 1: [Test Name]
**Description**: [What makes this invalid]
**Raw Data**:
```hex
[invalid hex data]
```
**Expected Result**: [Should fail with specific error]

## References and Resources

### Related Documentation
- [Link to official protocol documentation]
- [Link to reference implementation]
- [Link to protocol specification RFC/standard]

### Tools for Analysis
- **Packet Capture**: [Wireshark filters, tcpdump commands]
- **Hex Editors**: [Recommended tools]
- **Protocol Analyzers**: [Specialized tools]

### Example Implementations
- [Language]: [Link to example implementation]
- [Language]: [Link to another implementation]

## Revision History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | [YYYY-MM-DD] | [Name] | Initial specification |
| 1.1 | [YYYY-MM-DD] | [Name] | [Description of changes] |

---

## Implementation Checklist

- [ ] Basic packet structure parsing
- [ ] Header validation
- [ ] Payload extraction
- [ ] Packet type identification
- [ ] Error handling
- [ ] String encoding/decoding
- [ ] Endianness handling
- [ ] Length validation
- [ ] Test packet parsing
- [ ] Integration tests
- [ ] Performance testing
- [ ] Security validation

## Parser Architecture Notes

### Suggested Class Structure

```python
class ProtocolParser:
    """Main parser class for [Protocol Name]"""

    def parse_packet(self, data: bytes) -> ParsedPacket:
        """Parse raw bytes into structured packet"""
        pass

    def validate_header(self, header: bytes) -> bool:
        """Validate packet header"""
        pass

    def extract_payload(self, packet: bytes) -> bytes:
        """Extract payload from packet"""
        pass

    def identify_packet_type(self, opcode: int) -> PacketType:
        """Identify packet type from opcode"""
        pass

class ParsedPacket:
    """Represents a parsed packet"""

    def __init__(self, packet_type: PacketType, payload: dict):
        self.packet_type = packet_type
        self.payload = payload
        self.timestamp = time.time()
```

### Integration Points with pyOxide

- **TCP Server**: Integrate with existing `server_manager.py` TCP servers
- **Logging**: Use existing Django models for packet logging
- **Configuration**: Store protocol settings in Django configuration
- **Testing**: Add parser tests to existing test suite
- **CLI**: Add packet parsing commands to command handler

---

**Note**: Remove this template header and fill in the actual protocol details. Add sections specific to your protocol as needed.
