# Bare metal, linker script, exception vectors, MMU

## Confirm the boot stage first

On Broadcom (Pi), VideoCore firmware runs first and releases ARM. There is no ARM-side
"bare metal from reset" — the firmware has already set up clocks, PLLs, and the mailbox.
Any bare-metal answer must state what firmware already did.

## Linker script

- Place `.text` in executable, read-only memory.
- `.data` and `.bss` in RAM; `.bss` zeroed by boot code (not the linker).
- Stack pointer initialised before `main`; 16-byte aligned.
- Exception vectors at the configured vector base (VBAR_EL1 for EL1, VBAR_EL2 for EL2).

## Exception vectors

- Synchronous exceptions, IRQ, FIQ, SError, each at EL0/EL1/EL2/EL3 offsets.
- Vector table must be aligned and contiguous (typically 2 KB).
- On AArch64, `VBAR_ELx` points to the table; write it early in boot.

## MMU

- Enable stage-1 translation via `SCTLR_EL1.M` after setting up translation tables.
- TCR_EL1 controls page size, TTBR0/TTBR1 split, ASID.
- MAIR_EL1 defines memory attributes (device vs normal, cacheable vs not).
- Invalidate TLBs after changing tables (`TLBI VMALLE1`, `DSB ISH`, `ISB`).

## Rule

If the user wants "bare metal HDMI" on Pi, the honest answer is: VideoCore owns HDMI, the
mailbox is an RPC to a closed blob, and there is no ARM-side HDMI/DSI PHY. Hand off to
`sbc-pro` for the board-level fact.
