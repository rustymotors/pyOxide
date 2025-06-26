# pyOxide Features

## 🔐 Session Key Decryption System

### RSA Session Key Decryption
- **RSA Private Key Management**: Load and manage RSA private keys for session decryption
- **Multi-Padding Support**: Automatic fallback between OAEP and PKCS1v15 padding schemes
- **Session Key Extraction**: Parse encrypted session keys from NPS LOGIN_REQUEST packets
- **Key Validation**: Comprehensive validation of decrypted session keys (length, entropy, format)
- **Protocol Integration**: Seamless integration with existing NPS packet parser

### Session Management
- **Session Ticket Extraction**: Automatic extraction of session tickets from login packets
- **Key Format Parsing**: Support for Motor City Online session key format (length + key + expiration)
- **Encryption Protocol Support**: Full compliance with MCO RSA encryption protocol
- **Real-time Decryption**: Live session key decryption during packet processing

## 🔍 Network Protocol Analysis

### NPS Packet Parser
- **Multi-format Support**: Parse both v0 and v1 NPS packet formats
- **Message Type Detection**: Automatic identification of LOGIN_REQUEST, HEARTBEAT, and other message types
- **Structured Analysis**: Extract session tickets, session keys, and other protocol fields
- **Command-line Interface**: Interactive packet analysis tool (`analyze_packet.py`)

### Packet Structure Analysis
- **Header Parsing**: Complete NPS packet header analysis (magic, length, checksums)
- **Payload Extraction**: Automatic extraction of strings and hex patterns from packet payloads
- **Field Identification**: Structured field analysis for LOGIN_REQUEST packets
- **Data Validation**: Comprehensive packet integrity and format validation

## 🌐 HTTP Server System

### Web Interface
- **Django Integration**: Full Django web framework integration for admin interface
- **Template System**: Jinja2 template engine for dynamic HTML generation
- **Static File Serving**: CSS, JavaScript, and asset management
- **Authentication Pages**: Login forms and session management

### API Endpoints
- **Status Monitoring**: Server health and status endpoints
- **Shard Management**: Game world/shard information and configuration
- **License Information**: GPL v3.0 compliance and license display
- **Test Interface**: Comprehensive testing and debugging interface

## 🎮 Motor City Online Protocol Support

### Game Server Emulation
- **Shard System**: Multi-world game server architecture
- **User Authentication**: Complete login and session management
- **Protocol Compliance**: Full support for original MCO network protocol
- **Legacy Compatibility**: Support for original game client connections

### Network Services
- **Multi-port Support**: Handle connections on multiple game service ports
- **SSL/TLS Support**: Encrypted connections and certificate management
- **Session Management**: Player session tracking and authentication
- **Command Processing**: Game command parsing and execution

## 🛠️ Development Tools

### Code Quality
- **Type Checking**: Full MyPy type annotation support
- **Code Formatting**: Black and isort integration
- **Linting**: Flake8 code quality checks
- **Security Scanning**: Bandit security analysis

### Testing Framework
- **Unit Tests**: Comprehensive pytest test suite
- **Integration Tests**: Full system integration testing
- **Coverage Reporting**: HTML and XML coverage reports
- **CI/CD Integration**: GitHub Actions workflow automation

### Documentation
- **API Documentation**: Complete module and function documentation
- **Protocol Specifications**: Detailed MCO protocol documentation
- **Development Guides**: Setup and contribution guidelines
- **Change Tracking**: Detailed changelog and version management

## 📋 Configuration Management

### Environment Configuration
- **Database Settings**: SQLite database configuration and management
- **SSL Certificate Management**: TLS certificate and key configuration
- **Port Configuration**: Multi-service port binding and management
- **Development Settings**: Debug mode and development tool configuration

### Project Structure
- **Modular Architecture**: Clean separation of concerns across modules
- **GPL v3.0 Compliance**: Full open-source license compliance
- **Package Management**: Modern Python packaging with pyproject.toml
- **Dependency Management**: Clean dependency specification and management
