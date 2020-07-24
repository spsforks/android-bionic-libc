SECTIONS {
  # This starts off fairly normal: rodata, text, dynamic, data, bss with
  # appropriate alignment between them.
  . = SIZEOF_HEADERS;
  .rodata : {*(.rodata .rodata.*)}
  . = ALIGN(0x1000);
  .text : {*(.text .text.*)}
  . = ALIGN(0x1000);
  .dynamic : {*(.dynamic)}
  . = ALIGN(0x1000);
  .data : {*(.data .data.*)}
  .bss : {*(.bss .bss.*)}

  # Now create the gap. We need a text segment first to prevent the linker from
  # merging .bss with .bss.end_of_gap.
  . = ALIGN(0x1000);
  .custom_text : {
    *(.custom_text.text_before_start_of_gap);
  }

  # Place end_of_gap at the end of the gap.
  . = 0x1000000;
  .custom_bss : {
    *(.custom_bss.end_of_gap);
  }
}
