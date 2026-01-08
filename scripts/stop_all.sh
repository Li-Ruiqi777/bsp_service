#!/bin/bash

# 停止所有服务脚本

echo "Stopping BSP services..."

# 停止 io-service
if [ -f /tmp/io-service.pid ]; then
    PID=$(cat /tmp/io-service.pid)
    if ps -p $PID > /dev/null 2>&1; then
        echo "Stopping io-service (PID: $PID)..."
        kill $PID
        rm /tmp/io-service.pid
        echo "io-service stopped"
    else
        echo "io-service is not running"
        rm /tmp/io-service.pid
    fi
else
    echo "io-service PID file not found"
fi

echo "All services stopped"

