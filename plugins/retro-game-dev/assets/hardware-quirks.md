# Hardware quirks — quick reference

| Platform | Quirk | Status |
|---|---|---|
| NES | 8 sprites/scanline; sprite-0 hit for splits | VALID |
| NES | 64 sprites total; OAM DMA in vblank | VALID |
| NES | Mapper determines PRG/CHR banking; no mapper = 32K PRG + 8K CHR | VALID |
| SNES | Two sprite sizes at a time; 32 sprites/line | VALID |
| SNES | HDMA for per-scanline effects | VALID |
| SNES | VRAM write only in vblank/forced blank | VALID |
| Genesis | 20 sprites/line; 80 total; 320×224 or 256×224 | VALID |
| Genesis | Z80 co-processor for sound; 68000 main | VALID |
| GB | 10 sprites/line; 40 total; 4 shades | VALID |
| GB | MBC required above 32K ROM / 8K RAM | VALID |
| GB | VRAM access restricted during mode 3 | VALID |
