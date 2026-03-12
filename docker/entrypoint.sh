#!/bin/bash
set -e

echo "[entrypoint] Starting iox-roudi in background..."
source /ros2_ws/install/setup.bash
iox-roudi &
IOX_PID=$!
echo "[entrypoint] iox-roudi started (PID $IOX_PID)"

exec "$@"
