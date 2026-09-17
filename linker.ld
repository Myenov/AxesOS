OUTPUT_FORMAT(binary)
ENTRY(kernel_main)

SECTIONS
{
    . = 0x10000;

    .text : {
        *(.text)
        *(.text.*)
    }

    . = ALIGN(512);
    .rodata : {
        *(.rodata)
        *(.rodata.*)
    }

    . = ALIGN(512);
    .data : {
        *(.data)
        *(.data.*)
    }

    . = ALIGN(512);
    .bss : {
        *(.bss)
        *(.bss.*)
        *(COMMON)
    }

    . = ALIGN(512);
}