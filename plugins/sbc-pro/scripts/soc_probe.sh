#!/usr/bin/env bash
# soc_probe.sh — emit the version block every sbc-pro answer depends on.
# No Python. POSIX shell only.
set -u

echo "== model =="
cat /proc/device-tree/model 2>/dev/null || echo "n/a"
echo

echo "== kernel =="
uname -a
echo

echo "== firmware =="
if command -v vcgencmd >/dev/null 2>&1; then
    vcgencmd version
else
    echo "no vcgencmd"
fi
echo

echo "== mesa / GL =="
if command -v glxinfo >/dev/null 2>&1; then
    glxinfo -B 2>/dev/null | grep -i "OpenGL version" || echo "no OpenGL version line"
else
    echo "no glxinfo"
fi
echo

echo "== vulkan =="
if command -v vulkaninfo >/dev/null 2>&1; then
    vulkaninfo --summary 2>/dev/null | head -40 || echo "vulkaninfo produced no summary"
else
    echo "no vulkaninfo"
fi
echo

echo "== gpu dmesg =="
dmesg 2>/dev/null | grep -iE 'v3d|panfrost|mali|drm' | tail -20 || echo "no gpu dmesg lines"
echo

echo "== thermal zones =="
for z in /sys/class/thermal/thermal_zone*/temp; do
    [ -e "$z" ] && echo "$z: $(cat "$z")"
done || echo "no thermal zones"
echo

echo "== throttle =="
if command -v vcgencmd >/dev/null 2>&1; then
    vcgencmd get_throttled
else
    echo "n/a"
fi
