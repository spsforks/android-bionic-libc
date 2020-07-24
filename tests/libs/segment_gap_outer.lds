SECTIONS {
  # This starts off fairly normal: rodata, text, dynamic, data, bss with
  # appropriate alignment between them.
  . = SIZEOF_HEADERS;
  .rodata : {*(.rodata.*)}
  . = ALIGN(0x1000);
  .text : {*(.text.*)}
  . = ALIGN(0x1000);
  .dynamic : {}
  . = ALIGN(0x1000);
  .data : {*(.data.*)}
  .bss : {}

  # Now create the gap. We need a text segment first to prevent the linker from
  # merging .bss with .bss.end_of_gap.
  . = ALIGN(0x1000);
  .text_before_start_of_gap : {
    *(.text_before_start_of_gap);
  }

  # Place end_of_gap at the end of the gap.
  . = 0x1000000;
  .bss.end_of_gap : {
    *(.bss.*end_of_gap*);
  }
}
