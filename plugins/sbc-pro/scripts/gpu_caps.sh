#!/usr/bin/env bash
# gpu_caps.sh — parse vulkaninfo/eglinfo into a capability table.
# Flags features that require a non-default stack.
set -u

echo "== Vulkan summary =="
if command -v vulkaninfo >/dev/null 2>&1; then
    vulkaninfo --summary 2>/dev/null || echo "vulkaninfo failed"
else
    echo "no vulkaninfo"
fi
echo

echo "== EGL / GLES =="
if command -v eglinfo >/dev/null 2>&1; then
    eglinfo 2>/dev/null | grep -iE 'client api|version|vendor' | head -20 || echo "eglinfo produced nothing"
elif command -v glxinfo >/dev/null 2>&1; then
    glxinfo -B 2>/dev/null | head -20 || echo "glxinfo produced nothing"
else
    echo "no eglinfo or glxinfo"
fi
echo

echo "== Stack flags =="
if command -v vulkaninfo >/dev/null 2>&1 && vulkaninfo --summary 2>/dev/null | grep -qi 'panvk'; then
    echo "[flag] PanVK active — Vulkan via reverse-engineered Mali driver (not vendor blob)"
elif command -v vulkaninfo >/dev/null 2>&1 && vulkaninfo --summary 2>/dev/null | grep -qi 'v3dv'; then
    echo "[flag] V3DV active — Vulkan via Mesa VideoCore driver"
else
    echo "[flag] no Vulkan ICD detected — install Mesa or vendor Vulkan driver"
fi

if command -v glxinfo >/dev/null 2>&1 && glxinfo -B 2>/dev/null | grep -qi 'panfrost'; then
    echo "[flag] Panfrost active — OpenGL via reverse-engineered Mali driver"
elif command -v glxinfo >/dev/null 2>&1 && glxinfo -B 2>/dev/null | grep -qi 'v3d'; then
    echo "[flag] V3D active — OpenGL via Mesa VideoCore driver"
else
    echo "[flag] no OpenGL driver detected — install Mesa"
fi
