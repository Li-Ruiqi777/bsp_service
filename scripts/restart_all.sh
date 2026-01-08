#!/bin/bash

# 重启所有服务脚本

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "Restarting BSP services..."

# 先停止
$SCRIPT_DIR/stop_all.sh

# 等待一下
sleep 1

# 再启动
$SCRIPT_DIR/start_all.sh

echo "All services restarted"

