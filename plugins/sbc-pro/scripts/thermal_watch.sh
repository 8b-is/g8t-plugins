#!/usr/bin/env bash
# thermal_watch.sh — sample throttle flags, thermal zones, and clocks at 1 Hz.
# Prints the first throttle transition. No Python.
set -u

INTERVAL="${1:-1}"

prev=""
while true; do
    throttled=""
    if command -v vcgencmd >/dev/null 2>&1; then
        throttled="$(vcgencmd get_throttled 2>/dev/null || echo 'n/a')"
    fi

    temps=""
    for z in /sys/class/thermal/thermal_zone*/temp; do
        [ -e "$z" ] && temps="$temps $(basename "$(dirname "$z")"):$(cat "$z")"
    done

    line="throttled=$throttled temps=$temps"
    if [ "$line" != "$prev" ]; then
        echo "$(date -Is) $line"
        prev="$line"
    fi

    sleep "$INTERVAL"
done
