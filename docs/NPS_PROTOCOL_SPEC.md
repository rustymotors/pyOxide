# NPS (Network Play System) Protocol Specification

> **Based on analysis of Motor City Online / EA Network Play System header files**

## Protocol Overview

### Protocol Name
**Name**: NPS (Network Play System) Protocol / Motor City Online Protocol

### Version
**Version**: 1.0 (based on spec1 headers)
**Revision Date**: 2025-06-24

### Purpose
**Description**: Electronic Arts Network Play System protocol used for multiplayer game lobbies, user authentication, and communication channels. Originally used for Motor City Online and other EA games.

**Use Cases**:
- User authentication and login to game servers
- Lobby system for multiplayer games
- Communication channels (chat, game data)
- Player status and presence management
- Game server management and monitoring

## Connection Details

### Transport Layer
- **Protocol**: TCP (primary), UDP (for some data channels)
- **Default Port(s)**:
     - 8226 - Login Server (initial packet unencrypted, subsequent traffic DES-CBC encrypted)
     - 8228 - Persona Server (all traffic unencrypted)
     - 7003 - Lobby Server (initial packet unencrypted, subsequent traffic DES-CBC encrypted)
     - 43300 - MCOTS Server (encrypted with session key, different encryption method)
- **Encryption**:
  - Session key extracted from RSA-encrypted Field2 in NPS_USER_LOGIN packet
  - DES-CBC encryption (blank IV) for Login Server and Lobby Server traffic
  - Initial responses to NPS_USER_LOGIN and NPS_LOGIN remain unencrypted
  - All Persona Server traffic unencrypted
- **Authentication**: Required - EA account authentication with hashed passwords

### Connection Flow
1. **Client connects to Login Server**: TCP connection to port 8226
2. **Session key exchange**: NPS_USER_LOGIN packet with RSA-encrypted session key in Field2
3. **Login Server response**: Unencrypted response, subsequent traffic encrypted with DES-CBC
4. **Client connects to Lobby Server**: TCP connection to port 7003
5. **Lobby authentication**: NPS_LOGIN packet (unencrypted), subsequent traffic encrypted
6. **Channel management**: Client can join/create communication channels
7. **Data exchange**: Game messages, chat, status updates (encrypted)

## Packet Structure

### General Packet Format

```
[Header][Payload]
```

### Header Format

| Field Name | Size (bytes) | Type | Description | Example |
|------------|--------------|------|-------------|---------|
| Opcode | 2 | NPS_OPCODE (uint16) | Message type identifier | 0x0100 |
| Length | 2 | NPS_MSGLEN (uint16) | Payload length in bytes | 0x0040 |

**Total Header Size**: 4 bytes (NPS_HEADER_LENGTH)

### Payload Format

**Structure**: Variable-length based on message type

| Field Name | Size (bytes) | Type | Description | Required |
|------------|--------------|------|-------------|----------|
| Message Data | Variable | Varies by opcode | Opcode-specific payload | Yes |

## Data Types and Encoding

### Primitive Types
- **uint8**: 1 byte unsigned integer
- **uint16**: 2 byte unsigned integer, little-endian (Windows-centric)
- **uint32**: 4 byte unsigned integer, little-endian
- **NPS_LOGICAL**: 2 byte boolean (0=FALSE, 1=TRUE)
- **string**: null-terminated ASCII strings
- **timestamp**: time_t (Unix timestamp)

### NPS-Specific Types
- **NPS_USERID**: 4 bytes - User ID (persona ID)
- **NPS_CUSTOMERID**: 4 bytes - Global customer ID across NPS
- **NPS_COMMID**: 4 bytes - Communication channel ID
- **NPS_SERVID**: 4 bytes - Server ID
- **NPS_OPCODE**: 2 bytes - Message opcode
- **NPS_MSGLEN**: 2 bytes - Message length

### String Encoding
- **Character Set**: ASCII
- **Null Termination**: Yes
- **Length Prefix**: No (fixed-length fields with null termination)
- **Maximum Length**: Varies by field type (see NPS length constants)

### String Length Constants
```cpp
NPS_USERNAME_LEN         = 32
NPS_PASSWORD_LEN         = 32
NPS_EMAIL_LEN            = 64
NPS_HOSTNAME_LEN         = 64
NPS_USERDATA_LEN         = 64
NPS_RIFF_NAME_LEN        = 32
NPS_HASHED_KEY_LEN       = 16
NPS_SESSION_KEY_LEN      = 32
NPS_CHAT_MESSAGE_LEN     = 256
NPS_SYSTEM_MESSAGE_LEN   = 256
```

## Packet Types

### Client to Server Commands (NPS_LOBBYCLIENT_COMMANDS)

#### Packet Type: NPS_LOGIN
- **Opcode**: 0x0100
- **Description**: User login authentication - FIRST packet sent to Lobby Server
- **Direction**: Client→Lobby Server (Port 7003)
- **Encryption**: Unencrypted initially, subsequent traffic encrypted with DES-CBC

**Payload Structure**:
```
Offset | Size | Type         | Field Name      | Description
-------|------|--------------|-----------------|-------------
0x00   | 4    | NPS_USERID   | UserId         | User ID (persona ID)
0x04   | 32   | char[]       | UserName       | Username (null-terminated)
0x24   | 64   | char[]       | UserData       | User controllable data
0x64   | 4    | NPS_CUSTOMERID| CustomerId    | Global customer ID
0x68   | 16   | char[]       | keyHash        | Hashed authentication key
0x78   | 64   | char[]       | HostName       | Computer hostname
0xB8   | 16   | char[]       | MyIpAddr       | Client IP address
0xC8   | 4    | uint32       | Flags          | Initial user flags
0xCC   | 33   | char[]       | Version        | Client version string
```

#### Packet Type: NPS_GET_USER_LIST
- **Opcode**: 0x0101
- **Description**: Request list of users on server
- **Direction**: Client→Server

#### Packet Type: NPS_SET_MY_USER_DATA
- **Opcode**: 0x0103
- **Description**: Update user's data blob
- **Direction**: Client→Server

#### Packet Type: NPS_OPEN_COMM_CHANNEL
- **Opcode**: 0x0106
- **Description**: Create or join a communication channel
- **Direction**: Client→Server

**Payload Structure**:
```
Offset | Size | Type         | Field Name      | Description
-------|------|--------------|-----------------|-------------
0x00   | 4    | uint32       | Protocol       | NPSTCP or NPSUDP
0x04   | 4    | NPS_COMMID   | CommId         | Channel ID (0 for auto-assign)
0x08   | 4    | uint32       | ChannelFlags   | Channel behavior flags
0x0C   | 32   | char[]       | Riff           | Channel name/identifier
0x2C   | 4    | int          | Port           | Socket port (0 for auto)
0x30   | 32   | char[]       | Password       | Private channel password
0x50   | 4    | long         | SKU            | Game SKU identifier
0x54   | 4    | long         | SendRate       | Rate limiting value
0x58   | 256  | char[]       | ChannelData    | User-settable channel info
0x158  | 2    | uint16       | MaxReadyPlayers| Maximum players allowed
0x15A  | 2    | short        | ChannelType    | Public/Private/Opaque
```

### Server to Client Commands (NPS_LOBBYSERVER_COMMANDS)

#### Packet Type: NPS_LOGIN_RESP
- **Opcode**: 0x0120
- **Description**: Response to login attempt
- **Direction**: Server→Client

#### Packet Type: NPS_USER_JOINED
- **Opcode**: 0x0203
- **Description**: Notification that a user joined the server
- **Direction**: Server→Client

**Payload Structure**:
```
Offset | Size | Type         | Field Name      | Description
-------|------|--------------|-----------------|-------------
0x00   | 4    | NPS_USERID   | UserId         | User ID that joined
0x04   | 32   | char[]       | UserName       | Username
0x24   | 64   | char[]       | UserData       | User's data blob
```

#### Packet Type: NPS_USER_LEFT
- **Opcode**: 0x0202
- **Description**: Notification that a user left the server
- **Direction**: Server→Client

#### Packet Type: NPS_CHANNEL_GRANTED
- **Opcode**: 0x0214
- **Description**: Confirmation of successful channel join
- **Direction**: Server→Client

#### Packet Type: NPS_HEARTBEAT
- **Opcode**: 0x0217
- **Description**: Keep-alive message (hidden from user callbacks)
- **Direction**: Bidirectional

### Login Server Commands (NPS_LOGINCLIENT_COMMANDS)

#### Packet Type: NPS_USER_LOGIN
- **Opcode**: 0x0501
- **Description**: EA account authentication - FIRST packet sent to Login Server
- **Direction**: Client→Login Server (Port 8226)
- **Encryption**: Unencrypted (contains encrypted session key in Field2)

**Special Note**: Field2 in this packet contains a 256-character ASCII hex string representing an RSA-encrypted blob containing the session key used for subsequent encrypted communications.

#### Packet Type: NPS_GAME_LOGIN
- **Opcode**: 0x0502
- **Description**: Game-specific login
- **Direction**: Client→Login Server

## Protocol Flow Examples

### Example 1: User Login Sequence

**Description**: Complete user authentication and server join process

```
Client                    Server
  |                         |
  |-- NPS_LOGIN (0x0100) -->|
  |                         |
  |<-- NPS_LOGIN_RESP ------|
  |    (0x0120)             |
  |                         |
  |<-- NPS_USER_JOINED -----|
  |    (0x0203)             |
```

**Packet Details**:
1. **NPS_LOGIN**: Client sends credentials, user data, version info
2. **NPS_LOGIN_RESP**: Server responds with success/failure and session info
3. **NPS_USER_JOINED**: Server notifies all clients of new user

### Example 2: Channel Creation and Join

**Description**: Creating a game lobby channel

```
Client                    Server
  |                         |
  |-- NPS_OPEN_COMM_CHANNEL |
  |    (0x0106) ----------->|
  |                         |
  |<-- NPS_CHANNEL_GRANTED--|
  |    (0x0214)             |
  |                         |
  |<-- NPS_USER_JOINED_CHANNEL
  |    (0x020C)             |
```

## Error Handling

### Error Codes (NPSSTATUS)

| Code | Name | Description | Recovery Action |
|------|------|-------------|-----------------|
| 0 | NPS_OK | Operation successful | Continue |
| -1 | NPS_ERR | General error | Check error details |
| -2 | NPS_TIMEOUT | Operation timed out | Retry operation |
| -3 | NPS_ERR_DUP_USER | User already online | Use different username |
| -4 | NPS_ERR_SERVER_FULL | Server at capacity | Try different server |
| -6 | NPS_NOT_CONNECTED | Not connected to server | Establish connection first |
| -127 | NPS_LOGIN_USER_BANNED | User account banned | Contact support |

### Error Response Packets

**Opcode**: Various (depends on failed operation)

Common error responses include:
- **NPS_DUP_USER** (0x020A): Duplicate user already online
- **NPS_SERVER_FULL** (0x020B): Too many users on server
- **NPS_CHANNEL_DENIED** (0x0213): Incorrect password for private channel

## Implementation Notes

### Parsing Considerations
- **Byte Order**: Little-endian (LE) for all NPS packet fields and values. MCOTS packets use big-endian (BE).
- **Alignment**: Structure packing may be required
- **Padding**: Fixed-length string fields, null-terminated
- **Buffer Size**: Recommended minimum 2048 bytes for largest messages

### Validation Rules
- **Header validation**: Opcode must be valid, length must not exceed buffer
- **String validation**: All strings must be null-terminated within field bounds
- **User ID validation**: Must be non-zero for valid users
- **Channel validation**: Channel IDs must be positive for existing channels

### Security Considerations
- **Authentication**: Always validate hashed passwords, never plain text
- **Session keys**: Use NPS_SessionKey for encrypted communications
- **Rate limiting**: Implement per-user message rate limiting
- **Input validation**: Sanitize all user-provided strings and data

## Questions for Implementation

> **Note**: These questions need clarification from additional documentation or reverse engineering:

1. **Session Key Details**: ✅ **RESOLVED** - The session key is extracted from Field2 in the NPS_USER_LOGIN packet (first packet to Login Server on port 8226). Field2 contains a 256-character ASCII hex string representing an RSA-encrypted blob (with OAEP padding). When decrypted, it yields: session key length (2 bytes, big-endian), session key data (N bytes), and expiration timestamp (4 bytes, big-endian). This session key is used with DES-CBC encryption (blank IV) for all traffic after NPS_USER_LOGIN (port 8226) and NPS_LOGIN (port 7003). The initial responses to these packets remain unencrypted, and all Persona Server traffic (port 8228) is unencrypted. The same key is also used for MCOTS server communication (port 43300) but with different encryption.

2. **Port Assignment**: ✅ **RESOLVED** - Ports are confirmed correct: 8226 (Login Server), 8228 (Persona Server), 7003 (Lobby Server), and 43300 (MCOTS Server).

3. **Channel Types**: ✅ **RESOLVED** - Channel types are clearly defined: NPS_PRIVATE_CHANNEL (password-protected, requires password to join), NPS_PUBLIC_CHANNEL (open channel, anyone can join), NPS_OPAQUE_CHANNEL (users can enter but can only read messages if they have the password).

4. **Game Data Format**: ⚠️ **PARTIALLY RESOLVED** - Game data within channels uses: ChannelData[256] for user-settable channel information, GameSpecific[512] blobs in user profiles, and game message opcodes (0x80-0x88) including NPS_SEND_ALL, NPS_SEND_GAME_READY_LIST, and NPS_GAME_MESSAGE. Channels track ready players and game server state. Specific Motor City Online message payload structures will be analyzed during parser implementation phase.

5. **UDP Usage**: ✅ **RESOLVED** - Both TCP and UDP are supported with protocol constants: NPSTCP (33) for TCP only, NPSUDP (44) for UDP only, NPSUDPPREF (55) for UDP preferred, NPSTCP_UDPPREF (66) for TCP with UDP preference. UDP has a maximum packet size of 512 bytes and is used for real-time game data where packet loss is acceptable.

6. **SRP Authentication**: ✅ **RESOLVED** - SRP (Secure Remote Password) is fully implemented with complete handshake sequence: NPS_SRP_USERNAME → NPS_SRP_NGS → NPS_SRP_A → NPS_SRP_B → NPS_SRP_USER_VERIFY → NPS_SRP_SERVER_VERIFY.

7. **Binary Compatibility**: ✅ **RESOLVED** - All NPS packet fields and values use little-endian (LE) byte order. MCOTS packets use big-endian (BE) byte order. Cross-platform endianness handling is required for proper protocol implementation.

## Testing Data

### Valid Test Packets

#### Test Case 1: Basic Login
**Description**: Minimum valid login packet
**Raw Data**:
```hex
00 01 40 00  // Header: opcode=0x0100, length=0x0040
01 00 00 00  // UserId: 1
74 65 73 74 00 00 00 00 00 00 00 00 00 00 00 00  // UserName: "test" + padding
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
// ... (rest of NPS_LoginInfo structure)
```
**Expected Result**: Server should process login and respond with NPS_LOGIN_RESP

### Invalid Test Packets

#### Invalid Case 1: Malformed Header
**Description**: Invalid opcode
**Raw Data**:
```hex
FF FF 10 00  // Invalid opcode 0xFFFF
// ... payload
```
**Expected Result**: Should fail with unknown opcode error

## References and Resources

### Related Documentation
- EA Network Play System original documentation (if available)
- Motor City Online protocol analysis
- NPSTypes.h, NPSDll_Types.h header files

### Tools for Analysis
- **Packet Capture**: Wireshark with custom NPS dissector
- **Hex Editors**: HxD, Hex Fiend for packet analysis
- **Protocol Analyzers**: Custom tools for NPS packet parsing

## Implementation Checklist

- [ ] Basic packet structure parsing (header + payload)
- [ ] Header validation (opcode, length)
- [ ] Opcode-to-handler mapping
- [ ] Login packet parsing (NPS_LOGIN)
- [ ] User data structure handling
- [ ] Channel management packets
- [ ] String encoding/decoding with proper null termination
- [ ] Little-endian integer handling
- [ ] Error packet generation
- [ ] Session key integration
- [ ] Heartbeat mechanism
- [ ] Rate limiting per user
- [ ] Security validation (password hashing)

## Parser Architecture Notes

### Suggested Class Structure

```python
class NPSProtocolParser:
    """Main parser class for NPS Protocol"""

    def parse_packet(self, data: bytes) -> NPSPacket:
        """Parse raw bytes into structured NPS packet"""
        pass

    def validate_header(self, header: bytes) -> bool:
        """Validate NPS packet header (opcode + length)"""
        pass

    def extract_payload(self, packet: bytes, expected_length: int) -> bytes:
        """Extract payload from packet using header length"""
        pass

    def identify_packet_type(self, opcode: int) -> NPSMessageType:
        """Map opcode to message type enum"""
        pass

class NPSPacket:
    """Represents a parsed NPS packet"""

    def __init__(self, opcode: int, payload: dict, raw_data: bytes):
        self.opcode = opcode
        self.payload = payload
        self.raw_data = raw_data
        self.timestamp = time.time()

class NPSMessageType(Enum):
    """NPS Message type enumeration"""
    LOGIN = 0x0100
    LOGIN_RESP = 0x0120
    USER_JOINED = 0x0203
    OPEN_COMM_CHANNEL = 0x0106
    # ... etc
```

### Integration Points with pyOxide

- **TCP Server**: Integrate with existing `server_manager.py` TCP servers on ports 8226/8228
- **Logging**: Use existing Django models to log NPS packets and user sessions
- **Configuration**: Store NPS server settings in Django configuration
- **Testing**: Add NPS parser tests to existing test suite
- **CLI**: Add NPS packet parsing commands to command handler
- **Authentication**: Integrate with existing AuthUsers model for account management

---

**Implementation Status**: Specification complete based on header file analysis. Ready for parser implementation.
