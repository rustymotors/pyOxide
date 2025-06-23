# pyOxide Feature Specification

**Version:** 1.0.0
**License:** GPL v3.0
**Last Updated:** 2025-06-23

This document serves as the comprehensive feature specification for the pyOxide project. It details all current capabilities, endpoints, models, CLI commands, and technical specifications.

---

## Project Overview

pyOxide is a modern Python HTTP server with embedded Django integration, featuring:
- **Modular Architecture**: Separate HTTP, command, and server management components
- **Embedded Django**: Full Django admin interface and ORM capabilities
- **Custom Authentication**: Secure user management with session handling
- **Jinja2 Templating**: All HTML responses use templates
- **GPL v3.0 Compliance**: Full open-source licensing with required notices
- **Development Tools**: Comprehensive testing, linting, and code quality tools

---

## Core Components

### 1. Server Management (`src/server_manager.py`)
- **HTTP Server**: Manages HTTP server lifecycle on port 3000
- **TCP Server**: Manages TCP server lifecycle on port 8226
- **Status Tracking**: Real-time server status monitoring
- **Auto-Start**: Servers start automatically on application launch

### 2. Command Handler (`src/command_handler.py`)
Interactive CLI with commands:
- `help` - Show available commands
- `hello` - Display greeting with GPL notice
- `info` - Show project information
- `license` - Display GPL v3.0 license information
- `start` - Start HTTP and TCP servers
- `status` - Check server status
- `stop` - Stop all servers
- `quit` - Exit application

### 3. HTTP Handler (`src/http_handler.py`)
- **Route Management**: URL routing for both custom and Django endpoints
- **Jinja2 Integration**: Template-based HTML responses
- **Django WSGI**: Seamless Django integration for admin interface
- **Static File Serving**: CSS, JavaScript, and image assets
- **JSON API**: RESTful endpoints for status and data

### 4. Django Integration (`src/django_integration.py`)
- **WSGI Application**: Embedded Django app
- **Admin Interface**: Full Django admin at `/admin/`
- **Custom Models**: Authentication, configuration, and logging
- **Database**: SQLite backend with migrations
- **Static Files**: Django admin CSS/JS assets

---

## HTTP Endpoints

### Core Application Routes
| Endpoint | Method | Description | Response Type |
|----------|--------|-------------|---------------|
| `/` | GET | Main homepage with endpoint directory | HTML (Jinja2) |
| `/test` | GET | **Interactive test page for all routes** | HTML (Jinja2) |
| `/license` | GET | GPL v3.0 license information | HTML (Jinja2) |
| `/status` | GET | Server status information | JSON |
| `/health` | GET | Health check endpoint | JSON |
| `/api/info` | GET | API information and capabilities | JSON |
| `/AuthLogin` | GET | Authentication login (query params) | Plain Text |
| `/api/echo` | POST | Echo back JSON data | JSON |

### Django Routes (Embedded)
| Endpoint | Method | Description | Response Type |
|----------|--------|-------------|---------------|
| `/admin/` | GET/POST | Django admin interface | HTML (Django) |
| `/admin/login/` | GET/POST | Django admin login | HTML (Django) |
| `/dashboard/` | GET | Custom admin dashboard | HTML (Django) |
| `/api/django/` | GET | Django API information | JSON |
| `/api/django/data/` | GET/POST | Data management operations | JSON |
| `/api/django/users/` | GET | User management API | JSON |
| `/static/*` | GET | Static files (CSS, JS, images) | Static Files |

### Custom Authentication
- **Endpoint**: `/AuthLogin`
- **Method**: GET with query parameters
- **Parameters**: `username`, `password`
- **Response Format**: Plain text with `reasoncode`, `reasontext`, `reasonurl`
- **Session Management**: Creates/updates unique session tickets
- **Security**: Secure password hashing with Django's built-in functions

---

## Database Models

### Authentication Models (`src/django_app/pyoxide_admin/models/auth.py`)

#### AuthUsers
Custom user authentication model:
```python
customer_id: AutoField (Primary Key)
username: CharField(150, unique=True)
password_hash: CharField(128)  # Secure Django hashing
email: EmailField
is_active: BooleanField(default=True)
created_at: DateTimeField(auto_now_add=True)
updated_at: DateTimeField(auto_now=True)
last_login: DateTimeField(null=True)
```

**Methods:**
- `set_password(raw_password)` - Secure password hashing
- `check_password(raw_password)` - Password verification
- `authenticate(password)` - Full authentication check
- `create_user(username, password, email)` - User creation helper

#### AuthSessions
Session tracking with unique tickets:
```python
customer_id: ForeignKey(AuthUsers)
ticket: CharField(40, unique=True)  # 40-char hex token
created_at: DateTimeField(auto_now_add=True)
updated_at: DateTimeField(auto_now=True)
expires_at: DateTimeField
is_active: BooleanField(default=True)
```

**Methods:**
- `generate_ticket()` - Create unique 40-character hex token
- `create_session(user, expires_at)` - New session creation
- `get_or_create_session(user, expires_at)` - Update existing or create new

### Configuration Model (`src/django_app/pyoxide_admin/models/config.py`)

#### Configuration
Key-value configuration storage:
```python
key: CharField(100, unique=True)
value: TextField
description: TextField
created_at: DateTimeField(auto_now_add=True)
updated_at: DateTimeField(auto_now=True)
```

### Logging Models (`src/django_app/pyoxide_admin/models/logging.py`)

#### ServerStatus
Server event logging:
```python
timestamp: DateTimeField(auto_now_add=True)
level: CharField(20)  # INFO, WARNING, ERROR, DEBUG
source: CharField(50)  # Component name
message: TextField
```

#### APIRequest
HTTP request tracking:
```python
timestamp: DateTimeField(auto_now_add=True)
method: CharField(10)  # GET, POST, etc.
path: CharField(200)
status_code: IntegerField
response_time_ms: IntegerField
ip_address: GenericIPAddressField
user_agent: TextField
```

---

## Django Admin Interface

### Access Credentials
- **URL**: `http://localhost:3000/admin/`
- **Username**: `admin`
- **Password**: `admin123`

### Registered Models
All models are registered with custom admin interfaces:
- **AuthUsers**: User management with secure password handling
- **AuthSessions**: Session tracking and management
- **Configuration**: System configuration management
- **ServerStatus**: Server event logging
- **APIRequest**: HTTP request tracking

### Admin Features
- **List Views**: Filtered and searchable model lists
- **Detail Views**: Full CRUD operations
- **Search**: Full-text search across relevant fields
- **Filters**: Date, status, and category filters
- **Read-only Fields**: Protected system fields
- **Custom Actions**: Bulk operations where applicable

---

## Templates (Jinja2)

### Template Structure
All templates extend `base.html` for consistent styling:

#### `templates/base.html`
- Modern CSS Grid layout
- Responsive design
- GPL v3.0 compliance footer
- Navigation structure

#### `templates/home.html`
- Welcome page with endpoint directory
- Server status display
- Feature highlights
- Interactive endpoint links

#### `templates/test_pages.html`
- **Comprehensive test interface**
- Direct links to all endpoints
- Login credentials display
- Testing instructions and tips

#### `templates/auth_login.html`
- Authentication login form
- Error handling display
- Security notices

#### `templates/license.html`
- Complete GPL v3.0 license text
- Compliance information
- Source code links

#### `templates/404.html`
- Custom error page
- Helpful navigation links
- Consistent branding

---

## CLI Commands

### Interactive Commands
Available through the command handler:

```bash
# User management
adduser  # Add a new authentication user interactively

# Display help
help

# Show greeting with GPL notice
hello

# Project information
info

# GPL v3.0 license details
license

# Server management
start    # Start HTTP and TCP servers
status   # Show server status
stop     # Stop all servers

# Exit application
quit
```

### User Management with `adduser`

The `adduser` command provides an interactive way to create new authentication users from the command line.

#### Usage
```bash
# Start the pyOxide application
python -m src.main

# In the interactive CLI, run:
adduser
```

#### Interactive Prompts
The command will prompt for:
1. **Username**: Unique identifier (required)
2. **Password**: User password (required)
3. **Email**: Email address (optional)

#### Example Session
```
> adduser

==================================================
ADD NEW USER
==================================================
Enter username: johndoe
Enter password: secretpassword123
Enter email (optional): john@example.com
✅ User 'johndoe' created successfully!
   Customer ID: 5
   Email: john@example.com
   Created: 2025-06-23 12:34:26.796961+00:00
   Active: True
```

#### Features
- **Validation**: Checks for empty usernames/passwords
- **Duplicate Prevention**: Prevents creating users with existing usernames
- **Secure Password Hashing**: Uses Django's built-in password hashing
- **Optional Email**: Email field is optional during creation
- **Immediate Feedback**: Shows success/error messages with details
- **Auto-Activation**: New users are automatically set to active status

#### Error Handling
```
# Empty username
❌ Username cannot be empty.

# Duplicate username
❌ User 'johndoe' already exists.

# Empty password
❌ Password cannot be empty.

# Django not available
❌ Django models not available. Make sure Django is properly set up.

# Database error
❌ Error creating user: [specific error message]
```

#### Alternative User Creation Methods
Besides the CLI `adduser` command, users can also be created via:

1. **Django Admin Interface** (`/admin/`)
   - Web-based user management
   - Full CRUD operations
   - Advanced filtering and search

2. **Django Shell** (programmatic)
   ```python
   from src.django_app.pyoxide_admin.models import AuthUsers
   user = AuthUsers.create_user(
       username="testuser",
       password="password123",
       email="test@example.com"
   )
   ```

3. **Direct Model Creation** (advanced)
   ```python
   user = AuthUsers(username="testuser", email="test@example.com")
   user.set_password("password123")
   user.save()
   ```

### Development Commands (VS Code Tasks)
```bash
# Application
Run Python Application

# Testing
Run Tests
Run Tests with Coverage
Coverage Report Only
Coverage Status

# Code Quality
Format Code           # Black + isort
Type Check           # MyPy
Lint Code            # Flake8
Setup Pre-commit Hooks
Run Pre-commit on All Files

# Documentation
View Changelog
```

---

## Authentication Flow

### Login Process (`/AuthLogin`)
1. **Request**: GET with `username` and `password` query parameters
2. **Validation**: Check user exists and password matches
3. **Session Management**: Create or update session with new ticket
4. **Response**: Plain text with status codes

### Response Formats
```
# Success
reasoncode=0
reasontext=Login successful
reasonurl=

# Missing parameters
reasoncode=1
reasontext=Missing username or password
reasonurl=

# Invalid credentials
reasoncode=2
reasontext=Invalid username or password
reasonurl=

# Database error
reasoncode=3
reasontext=Database error
reasonurl=
```

### Session Security
- **Unique Tickets**: 40-character hex tokens (160 bits entropy)
- **Single Session**: One active session per user
- **Automatic Updates**: New ticket on each login
- **Expiration**: 30-day default expiration
- **Secure Hashing**: Django's built-in password hashing

---

## Development Workflow

### Code Quality Tools
- **Black**: Code formatting (88-character line length)
- **isort**: Import sorting with Black profile
- **Flake8**: Linting and style checking
- **MyPy**: Static type checking
- **Bandit**: Security analysis
- **pre-commit**: Git hooks for automated quality checks

### Testing Strategy
- **pytest**: Test framework with fixtures
- **Coverage**: Code coverage tracking (~58% current)
- **Integration Tests**: HTTP endpoint testing
- **Unit Tests**: Component-level testing
- **Django Tests**: Admin and model testing

### Documentation Requirements
- **CHANGELOG.md**: Keep a Changelog format
- **FEATURES.md**: This document (update for all changes)
- **DEVELOPMENT.md**: Development guidelines
- **GPL Headers**: All source files must include GPL v3.0 headers

---

## License Compliance (GPL v3.0)

### Required Components
- **License Text**: Complete GPL v3.0 in `LICENSE` file
- **Source Headers**: GPL header in all Python source files
- **CLI Command**: `license` command for compliance
- **HTTP Endpoint**: `/license` endpoint for web access
- **Console Notice**: GPL notice on application startup
- **Copyright**: `Copyright (C) 2025 rustymotors`

### Compliance Features
- **Distribution Rights**: Source code availability
- **Modification Rights**: Clear licensing terms
- **Warranty Disclaimer**: Included in all notices
- **Source Access**: GitHub repository links
- **License Display**: Web and CLI access to terms

---

## Technical Specifications

### Dependencies
```toml
# Core
python = "^3.8"
django = "^4.2"
jinja2 = "^3.1"

# Development
pytest = "^7.0"
black = "^23.0"
flake8 = "^6.0"
mypy = "^1.0"
pre-commit = "^3.0"
```

### Architecture
- **Modular Design**: Separate concerns across components
- **WSGI Integration**: Django embedded via WSGI
- **Template Separation**: Jinja2 for custom, Django for admin
- **Database**: SQLite with Django ORM
- **Static Files**: Django admin assets + custom CSS

### Performance
- **HTTP Server**: Python's built-in `http.server`
- **TCP Server**: Custom TCP socket server
- **Database**: SQLite for development, PostgreSQL-ready
- **Static Files**: Direct file serving
- **Session Storage**: Database-backed sessions

### Security
- **Password Hashing**: Django's PBKDF2 with SHA256
- **Session Tokens**: Cryptographically secure random tokens
- **Input Validation**: Parameter validation and sanitization
- **SQL Injection**: Django ORM protection
- **XSS Protection**: Template auto-escaping

---

## Deployment Considerations

### Development Setup
```bash
# Clone repository
git clone https://github.com/rustymotors/pyOxide.git

# Install dependencies
pip install -r requirements.txt

# Setup pre-commit hooks
pre-commit install

# Run migrations
python -m src.django_integration migrate

# Start application
python -m src.main
```

### Production Considerations
- **Secret Key**: Change Django SECRET_KEY for production
- **Database**: Migrate to PostgreSQL for production
- **Static Files**: Use CDN or web server for static assets
- **HTTPS**: Enable SSL/TLS encryption
- **Logging**: Configure production logging
- **Monitoring**: Add health checks and metrics

---

## Future Enhancements

### Planned Features
- **REST API**: Full RESTful API for all models
- **User Registration**: Public user registration endpoint
- **Password Reset**: Email-based password recovery
- **Rate Limiting**: API request throttling
- **Caching**: Redis-based caching layer
- **WebSocket**: Real-time communication support

### Scalability Improvements
- **Database Pooling**: Connection pooling for performance
- **Load Balancing**: Multi-instance deployment
- **Microservices**: Service decomposition options
- **Container Support**: Docker containerization
- **Cloud Deployment**: AWS/GCP deployment guides

---

## Adding New Features

When implementing new features, follow this process:

### 1. Feature Planning
- [ ] Add feature description to this specification
- [ ] Document expected endpoints, models, or CLI commands
- [ ] Update the feature table with new capabilities

### 2. Implementation
- [ ] Follow GPL v3.0 compliance (headers in new files)
- [ ] Use modular architecture patterns
- [ ] Implement with comprehensive type hints
- [ ] Create Jinja2 templates for any HTML responses

### 3. Testing
- [ ] Write comprehensive pytest tests
- [ ] Ensure all existing tests continue to pass
- [ ] Verify pre-commit hooks pass
- [ ] Test in embedded Django context

### 4. Documentation
- [ ] Update this feature specification
- [ ] Update CHANGELOG.md with detailed changes
- [ ] Update README.md if user-facing changes
- [ ] Add VS Code tasks if applicable

### 5. Quality Assurance
- [ ] Run full test suite with coverage
- [ ] Verify type checking passes
- [ ] Ensure security scans pass
- [ ] Test CLI and web interfaces

### 6. Commit
- [ ] Use descriptive commit messages
- [ ] Reference feature spec updates
- [ ] Include testing verification
- [ ] Note GPL compliance

---

## Changelog Integration

This document must be updated whenever:
- New endpoints are added
- Database models are modified
- CLI commands are changed
- Authentication logic is updated
- Templates are added or modified
- Django admin is enhanced
- Dependencies are updated

All changes should be documented in `CHANGELOG.md` and reflected in this specification.

---

## Project Structure

pyOxide follows a well-organized directory structure for maintainability and clarity:

### Directory Layout
```
pyOxide/
├── src/                    # Source code
│   ├── django_app/        # Django application
│   ├── command_handler.py # CLI command handling
│   ├── http_handler.py    # HTTP request handling
│   ├── server_manager.py  # Server lifecycle management
│   └── main.py           # Application entry point
├── tests/                 # Unit tests
│   ├── integration/       # Integration tests
│   ├── test_*.py         # Component tests
│   └── conftest.py       # Test configuration
├── examples/              # Example scripts and demos
│   ├── demo_adduser.py   # CLI command demonstration
│   ├── test_adduser.py   # User creation examples
│   └── README.md         # Examples documentation
├── templates/             # Jinja2 HTML templates
├── data/                  # Data files and database
│   ├── db.sqlite3        # SQLite database
│   └── certificates/     # SSL certificates
├── reports/               # Generated reports
│   ├── htmlcov/          # HTML coverage reports
│   ├── coverage.xml      # XML coverage reports
│   └── bandit-report*.json # Security scan reports
├── scripts/               # Utility scripts
├── services/              # Docker services
└── docs/                  # Documentation
    ├── README.md         # Documentation index
    ├── FEATURES.md       # This file - feature specification
    ├── QUICK_REFERENCE.md # Commands and API reference
    └── development/      # Developer documentation
        ├── DEVELOPMENT.md    # Development workflow
        ├── TESTING.md        # Testing guide
        └── ORG_PUBLISHING_GUIDE.md # Publishing guide
```

### Key Principles
- **Separation of Concerns**: Each directory has a specific purpose
- **Clean Root**: Minimal files in project root for clarity
- **Generated Files**: Reports and artifacts in dedicated directories
- **Examples**: Demonstration scripts separate from tests
- **Documentation**: All docs in one place or at root level

### File Organization
- **Source Code**: `src/` - All application code
- **Tests**: `tests/` - Unit tests, `tests/integration/` - Integration tests
- **Examples**: `examples/` - Demo scripts and usage examples
- **Data**: `data/` - Database and configuration files
- **Reports**: `reports/` - Generated coverage and security reports
- **Templates**: `templates/` - Jinja2 HTML templates
