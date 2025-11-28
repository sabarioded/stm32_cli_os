/* Entry */
ENTRY(Reset_Handler)

/* ---------------- Memory map (STM32L476RG) ----------------
   FLASH  : 1 MB  at 0x0800_0000
   SRAM1  : 96 KB at 0x2000_0000  (data/bss + heap + stack)
   SRAM2  : 32 KB at 0x1000_0000
------------------------------------------------------------ */
MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
  SRAM1 (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
  SRAM2 (rwx) : ORIGIN = 0x10000000, LENGTH = 32K
}

/* Stack pointer at top of SRAM1 */
_estack = ORIGIN(SRAM1) + LENGTH(SRAM1);

/* Size hints */
__max_stack_size = 0x00001000;  /* 4 KB stack guard (tune as needed) */

/* ---- Code & RO data in FLASH ---- */
SECTIONS
{
  .text :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);

    *(.text) *(.text.*)
    *(.glue_7) *(.glue_7t)
    *(.init) *(.fini)
    *(.rodata) *(.rodata.*)

    . = ALIGN(4);
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;

    . = ALIGN(4);
    _etext = .;
  } > FLASH

  /* C/C++ preinit array in FLASH */
  .preinit_array :
  {
    . = ALIGN(4);
    __preinit_array_start = .;
    KEEP(*(.preinit_array*))
    __preinit_array_end = .;
  } > FLASH


  .init_array :
  {
    . = ALIGN(4);
    __init_array_start = .;
    KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
    KEEP(*(.init_array*))
    __init_array_end = .;
  } > FLASH

  .fini_array :
  {
    . = ALIGN(4);
    __fini_array_start = .;
    KEEP(*(SORT_BY_INIT_PRIORITY(.fini_array.*)))
    KEEP(*(.fini_array*))
    __fini_array_end = .;
  } > FLASH

  /* ---- Initialized data in SRAM1, load from FLASH ---- */
  .data : AT ( _etext )
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data) *(.data.*)
    . = ALIGN(4);
    _edata = .;
  } > SRAM1
  _sidata = LOADADDR(.data);

  /* ---- Zero-init data in SRAM1 ---- */
  .bss (NOLOAD) :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss) *(.bss.*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > SRAM1

/* ---- Fixed 8KB heap in SRAM1 ---- */
.heap (NOLOAD) :
{
  . = ALIGN(8);
  __heap_start__ = .;
  . += 0x2000;          /* 8 KB heap */
  __heap_end__ = .;
} > SRAM1


  /DISCARD/ :
  {
    *(.comment) *(.note*) *(.gnu*) *(.eh_frame*)
  }
}
