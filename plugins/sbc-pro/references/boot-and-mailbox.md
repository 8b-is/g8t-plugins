# Boot chain and mailbox

## Boot order on Broadcom (Pi)

1. **VideoCore firmware** runs first (GPU boots first).
2. VideoCore loads `kernel8.img` / `vmlinuz` and releases ARM cores.
3. ARM kernel boots; peripherals initialized after firmware.

## Mailbox property interface

- Channel 8 (property channel).
- 16-byte-aligned buffer in ARM-visible memory.
- Cache clean/invalidate required around buffer use.
- `0xC0000000` uncached alias for VideoCore.
- It is an **RPC to a closed binary**, not a driver you write.

## Hard rule #4

Do not describe mailbox framebuffer setup as "writing a video driver." There is no ARM-side
HDMI/DSI PHY to program on Broadcom SoCs.

## U-Boot vs firmware

- When U-Boot is present, it still runs after VideoCore; firmware remains the boot master.
- `config.txt` is parsed by VideoCore firmware, not by the ARM kernel.

## Common failure domains

| Symptom | Likely owner |
|---|---|
| No HDMI at all | VideoCore firmware / `config.txt` |
| Kernel panics on peripheral access | Device tree / wrong DTB |
| GPU compute fails | Mesa version / driver stack |
| Undervoltage / throttling | Power supply / thermal |
