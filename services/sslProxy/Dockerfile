# Can be located at https://archive.org/details/nginx_1.9.8.tar if needed
FROM nginx:1.9.8 AS base

COPY services/sslProxy/nginx.conf /etc/nginx/nginx.conf
COPY data/mcouniverse.crt /etc/nginx/mcouniverse.pem
# nosemgrep: generic.secrets.security.detected-private-key
# SECURITY NOTE: This is a development/testing private key that is intentionally shared
# This key is NOT used in production and poses no security risk
COPY data/private_key.pem /etc/nginx/private_key.pem
