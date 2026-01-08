#!/bin/bash

# 服务状态监控脚本

echo "BSP Service Monitor"
echo "==================="

# 检查 io-service
echo -n "io-service: "
if [ -f /tmp/io-service.pid ]; then
    PID=$(cat /tmp/io-service.pid)
    if ps -p $PID > /dev/null 2>&1; then
        echo "RUNNING (PID: $PID)"
        
        # 检查端口
        echo -n "  Port 5555: "
        if netstat -tuln 2>/dev/null | grep -q ":5555 "; then
            echo "LISTENING"
        else
            echo "NOT LISTENING"
        fi
    else
        echo "NOT RUNNING (stale PID file)"
    fi
else
    echo "NOT RUNNING"
fi

echo ""
echo "Check completed"

