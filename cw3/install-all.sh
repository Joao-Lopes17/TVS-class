#!/bin/bash

# Exit on errors
set -e

#https://stackoverflow.com/questions/59895/how-do-i-get-the-directory-where-a-bash-script-is-located-from-within-the-script
SCRIPT_DIR=$(cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd)

TVSAPP="tvsapp"
TVSCTL_SRV="tvsctld"
TVSCTL_CLI="tvsctl"

INSTALL_DIR="/opt/isel/tvs"
USR_DIR="/usr/local"
RUN_DIR="/run/isel"
SYSTEMD_DIR="/etc/systemd/system"
NGINX_SITES_AVAILABLE_PATH="etc/nginx/sites-available"
NGINX_SITES_ENABLED_PATH="etc/nginx/sites-enabled"

USER="isel"
GROUP="tvsgrp"

# Ensure user and group exist
if ! id -u "$USER" >/dev/null 2>&1; then
    echo "User $USER does not exist. Creating user..."
    useradd -m -s /bin/bash "$USER"
fi
if ! getent group "$GROUP" >/dev/null 2>&1; then
    echo "Group $GROUP does not exist. Creating group..."
    groupadd "$GROUP"
    usermod -aG "$GROUP" "$USER"
fi

# Ensure base directories exist
mkdir -p "$INSTALL_DIR/$TVSAPP" "$INSTALL_DIR/$TVSCTL_SRV"

# Install TVS Control Client
echo "Installing TVS Control Client..."
make -C $SCRIPT_DIR/$TVSCTL_CLI/src -f "Makefile"
# Copy the new binary
rm -f $USR_DIR/bin/$TVSCTL_CLI
cp -f $SCRIPT_DIR/$TVSCTL_CLI/bin/$TVSCTL_CLI $USR_DIR/bin
# Set client permissions
chown :$GROUP $USR_DIR/bin/$TVSCTL_CLI
chmod 0750 $USR_DIR/bin/$TVSCTL_CLI

# Install TVS App
echo "Installing TVS App..."
rm -rf $INSTALL_DIR/$TVSAPP
mkdir -p "$INSTALL_DIR/$TVSAPP"
cp -r "$SCRIPT_DIR/$TVSAPP/app" "$INSTALL_DIR/$TVSAPP"
npm install --prefix "$INSTALL_DIR/$TVSAPP/app"

# Install TVS Control Daemon
echo "Installing TVS Control Daemon..."
make -C "$SCRIPT_DIR/$TVSCTL_SRV/src" -f "Makefile"
# Stop the service and socket before copying the new binary
if systemctl is-active --quiet $TVSCTL_SRV.service; then
    echo "Stopping $TVSCTL_SRV service..."
    systemctl stop $TVSCTL_SRV.service
fi
if systemctl is-active --quiet $TVSCTL_SRV.socket; then
    echo "Stopping $TVSCTL_SRV socket..."
    systemctl stop $TVSCTL_SRV.socket
fi
# Copy the new binary and scripts
rm -rf $INSTALL_DIR/$TVSCTL_SRV
mkdir -p "$INSTALL_DIR/$TVSCTL_SRV"
cp -r $SCRIPT_DIR/$TVSCTL_SRV/bin $INSTALL_DIR/$TVSCTL_SRV

# Set up systemd services
echo "Setting up systemd services..."
cp -f $SCRIPT_DIR/$TVSAPP/etc/service/*.service "$SYSTEMD_DIR"
cp -f $SCRIPT_DIR/$TVSCTL_SRV/etc/service/*.service "$SYSTEMD_DIR"
cp -f $SCRIPT_DIR/$TVSCTL_SRV/etc/service/*.socket "$SYSTEMD_DIR"

# Configure Nginx
cp -f "$SCRIPT_DIR/$TVSCTL_SRV/$NGINX_SITES_AVAILABLE_PATH/$TVSAPP" "/$NGINX_SITES_AVAILABLE_PATH"
ln -s -f "/$NGINX_SITES_AVAILABLE_PATH/$TVSAPP" "/$NGINX_SITES_ENABLED_PATH/$TVSAPP"
if [ -f "/$NGINX_SITES_ENABLED_PATH/default" ]; then
    echo "Nginx default configuration already enabled, skipping"
else
    ln -s -f "/$NGINX_SITES_AVAILABLE_PATH/default" "/$NGINX_SITES_ENABLED_PATH/default"
fi

#Launching systemd services
echo "Launching systemd services..."
systemctl daemon-reload
if systemctl is-active --quiet nginx; then
    echo "Launching nginx service"
    systemctl reload nginx
else
    echo "Restarting nginx service"
    systemctl start nginx
fi
systemctl enable $TVSCTL_SRV.socket
systemctl start $TVSCTL_SRV.socket

echo "Installation completed successfully!"
