# Development SSL Certificates

⚠️ **SECURITY NOTICE** ⚠️

## Test/Development Keys - Not for Production

The files in this directory contain **development/testing SSL certificates and private keys** that are:

- ✅ **Intentionally shared** in version control
- ✅ **Safe for public repositories** 
- ✅ **Only used for local development**
- ❌ **NEVER used in production environments**

## Files

- `mcouniverse.crt` - Development SSL certificate
- `private_key.pem` - Development private key (**intentionally public**)
- `pub.key` - Development public key

## Security Scanner Exceptions

These files are marked as safe for automated security scanners:

```
# nosemgrep: generic.secrets.security.detected-private-key
# gitleaks:allow
# truffleHog:allow
```

## For Production

In production environments, these files should be:
1. Replaced with real SSL certificates from a Certificate Authority
2. Stored securely using secret management systems
3. Never committed to version control

## Purpose

These development certificates enable HTTPS testing in local Docker containers and development environments without requiring real SSL certificates.
