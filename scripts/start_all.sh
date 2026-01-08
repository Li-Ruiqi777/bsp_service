#!/bin/bash

# 启动所有服务脚本

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SERVICE_DIR="$(dirname "$SCRIPT_DIR")"

echo "Starting BSP services..."

# 启动 io-service
if [ -f "$SERVICE_DIR/build/io-service" ]; then
    echo "Starting io-service..."
    $SERVICE_DIR/build/io-service $SERVICE_DIR/configs/io_service.json &
    echo $! > /tmp/io-service.pid
    echo "io-service started with PID: $(cat /tmp/io-service.pid)"
else
    echo "Error: io-service not found at $SERVICE_DIR/build/io-service"
    exit 1
fi

echo "All services started"

