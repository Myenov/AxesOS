cmake_minimum_required(VERSION 3.20)
project(AxesOS C ASM_NASM)

set(SOURCE_DIR    ${CMAKE_SOURCE_DIR}/src)
set(LOAD_DIR      ${SOURCE_DIR}/load)
set(APP_DIR       ${SOURCE_DIR}/app)
set(KERNEL_DIR    ${SOURCE_DIR}/kernel)
set(DRIVERS_DIR   ${SOURCE_DIR}/drivers)
set(INT_DIR       ${DRIVERS_DIR}/int)
set(KEYBOARD_DIR  ${DRIVERS_DIR}/keyboard)
set(VIDEO_DIR     ${DRIVERS_DIR}/video)
set(ALLOC_DIR     ${DRIVERS_DIR}/alloc)
set(CLOCK_DIR     ${DRIVERS_DIR}/clock)
set(SIGN_DIR      ${DRIVERS_DIR}/sign)
set(APIC_DIR      ${DRIVERS_DIR}/apic)
set(HPET_DIR      ${APIC_DIR}/hpet)
set(XSDT_DIR      ${APIC_DIR}/xsdt)
set(TERMINAL_DIR  ${APP_DIR}/terminal)

set(BOOTLOADER_BIN ${CMAKE_BINARY_DIR}/bootloader.bin)
set(STAGE2_BIN     ${CMAKE_BINARY_DIR}/bootloader.stage2.bin)
set(TTS_OBJ        ${CMAKE_BINARY_DIR}/tts_init.o)
set(ISR_OBJ        ${CMAKE_BINARY_DIR}/isr.o)
set(IDT_OBJ        ${CMAKE_BINARY_DIR}/idt.o)
set(ISR_C_OBJ      ${CMAKE_BINARY_DIR}/isr_c.o)
set(PIC_OBJ        ${CMAKE_BINARY_DIR}/pic.o)
set(TOCLICK_OBJ    ${CMAKE_BINARY_DIR}/toclick.o)
set(VIDEO_OBJ      ${CMAKE_BINARY_DIR}/video.o)
set(ALLOCATOR_OBJ  ${CMAKE_BINARY_DIR}/allocator.o)
set(ALLOC_OBJ      ${CMAKE_BINARY_DIR}/alloc.o)
set(CLOCK_OBJ      ${CMAKE_BINARY_DIR}/clock.o)
set(SIGN_OBJ       ${CMAKE_BINARY_DIR}/signature_search.o)
set(HPET_OBJ       ${CMAKE_BINARY_DIR}/hpet.o)
set(HSEARCH_OBJ    ${CMAKE_BINARY_DIR}/headers_search.o)
set(KERNEL_OBJ     ${CMAKE_BINARY_DIR}/kernel.o)
set(TERMINAL_OBJ   ${CMAKE_BINARY_DIR}/terminal.o)
set(KERNEL_BIN     ${CMAKE_BINARY_DIR}/kernel.bin)
set(DISK_IMAGE     ${CMAKE_BINARY_DIR}/AxesOS.img)

set(COMMON_CFLAGS
    -ffreestanding -nostdlib -fno-stack-protector -fno-pie -fno-pic
    -mno-red-zone -mcmodel=large -mno-mmx -mno-sse -mno-sse2 -O0
)

add_custom_command(
    OUTPUT ${BOOTLOADER_BIN}
    COMMAND nasm -f bin -I${LOAD_DIR}/ ${LOAD_DIR}/bootloader.asm -o ${BOOTLOADER_BIN}
    DEPENDS ${LOAD_DIR}/bootloader.asm ${LOAD_DIR}/bootloader.inc
    COMMENT "Compiling stage1"
)

add_custom_command(
    OUTPUT ${STAGE2_BIN}
    COMMAND nasm -f bin -I${LOAD_DIR}/ ${LOAD_DIR}/bootloader.stage2.asm -o ${STAGE2_BIN}
    DEPENDS ${LOAD_DIR}/bootloader.stage2.asm ${LOAD_DIR}/bootloader.inc
    COMMENT "Compiling stage2"
)

add_custom_command(
    OUTPUT ${TTS_OBJ}
    COMMAND nasm -f elf64 ${LOAD_DIR}/TTS.init.asm -o ${TTS_OBJ}
    DEPENDS ${LOAD_DIR}/TTS.init.asm
    COMMENT "Compiling TTS init"
)

add_custom_command(
    OUTPUT ${ISR_OBJ}
    COMMAND nasm -f elf64 ${INT_DIR}/isr.asm -o ${ISR_OBJ}
    DEPENDS ${INT_DIR}/isr.asm
    COMMENT "Compiling ISR assembly"
)

add_custom_command(
    OUTPUT ${IDT_OBJ}
    COMMAND gcc ${COMMON_CFLAGS} -I${SOURCE_DIR} -I${INT_DIR}
            -c ${INT_DIR}/IDT.c -o ${IDT_OBJ}
    DEPENDS ${INT_DIR}/IDT.c ${INT_DIR}/IDT.h
    COMMENT "Compiling IDT"
)

add_custom_command(
    OUTPUT ${ISR_C_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${INT_DIR} -I${KEYBOARD_DIR} -I${VIDEO_DIR}
            -c ${INT_DIR}/isr.c -o ${ISR_C_OBJ}
    DEPENDS ${INT_DIR}/isr.c ${INT_DIR}/isr.h ${INT_DIR}/IDT.h ${KEYBOARD_DIR}/toclick.h
    COMMENT "Compiling ISR C code"
)

add_custom_command(
    OUTPUT ${PIC_OBJ}
    COMMAND gcc ${COMMON_CFLAGS} -I${SOURCE_DIR} -I${INT_DIR}
            -c ${INT_DIR}/pic.c -o ${PIC_OBJ}
    DEPENDS ${INT_DIR}/pic.c ${INT_DIR}/pic.h
    COMMENT "Compiling PIC"
)

add_custom_command(
    OUTPUT ${TOCLICK_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${KEYBOARD_DIR} -I${VIDEO_DIR}
            -c ${KEYBOARD_DIR}/toclick.c -o ${TOCLICK_OBJ}
    DEPENDS ${KEYBOARD_DIR}/toclick.c ${KEYBOARD_DIR}/toclick.h ${VIDEO_DIR}/video.h
    COMMENT "Compiling keyboard handler"
)

add_custom_command(
    OUTPUT ${VIDEO_OBJ}
    COMMAND gcc ${COMMON_CFLAGS} -I${SOURCE_DIR} -I${VIDEO_DIR}
            -c ${VIDEO_DIR}/video.c -o ${VIDEO_OBJ}
    DEPENDS ${VIDEO_DIR}/video.c ${VIDEO_DIR}/video.h
    COMMENT "Compiling video driver"
)

add_custom_command(
    OUTPUT ${ALLOCATOR_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${ALLOC_DIR} -I${VIDEO_DIR} -I${DRIVERS_DIR}
            -c ${ALLOC_DIR}/allocator.c -o ${ALLOCATOR_OBJ}
    DEPENDS ${ALLOC_DIR}/allocator.c ${ALLOC_DIR}/allocator.h
            ${DRIVERS_DIR}/str.h ${VIDEO_DIR}/video.h
    COMMENT "Compiling allocator"
)

add_custom_command(
    OUTPUT ${ALLOC_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${ALLOC_DIR} -I${DRIVERS_DIR}
            -c ${ALLOC_DIR}/alloc.c -o ${ALLOC_OBJ}
    DEPENDS ${ALLOC_DIR}/alloc.c ${ALLOC_DIR}/alloc.h ${ALLOC_DIR}/allocator.h
    COMMENT "Compiling alloc wrappers"
)

add_custom_command(
    OUTPUT ${CLOCK_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${CLOCK_DIR} -I${DRIVERS_DIR} -I${APIC_DIR} -I${HPET_DIR}
            -c ${CLOCK_DIR}/clock.c -o ${CLOCK_OBJ}
    DEPENDS ${CLOCK_DIR}/clock.c ${CLOCK_DIR}/clock.h ${DRIVERS_DIR}/outb_and_inb.h
            ${HPET_DIR}/hpet.h
    COMMENT "Compiling clock driver"
)

add_custom_command(
    OUTPUT ${SIGN_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${SIGN_DIR} -I${DRIVERS_DIR}
            -c ${SIGN_DIR}/signature_search.c -o ${SIGN_OBJ}
    DEPENDS ${SIGN_DIR}/signature_search.c ${SIGN_DIR}/signature_search.h
    COMMENT "Compiling signature search"
)

add_custom_command(
    OUTPUT ${HSEARCH_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${APIC_DIR} -I${XSDT_DIR} -I${DRIVERS_DIR}
            -c ${APIC_DIR}/headers_search.c -o ${HSEARCH_OBJ}
    DEPENDS ${APIC_DIR}/headers_search.c ${APIC_DIR}/headers_search.h
            ${XSDT_DIR}/xsdt.h ${APIC_DIR}/ACPI.h
    COMMENT "Compiling ACPI headers search"
)

add_custom_command(
    OUTPUT ${HPET_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${APIC_DIR} -I${HPET_DIR} -I${XSDT_DIR} -I${DRIVERS_DIR}
            -c ${HPET_DIR}/hpet.c -o ${HPET_OBJ}
    DEPENDS ${HPET_DIR}/hpet.c ${HPET_DIR}/hpet.h
            ${APIC_DIR}/ACPI.h ${XSDT_DIR}/xsdt.h
    COMMENT "Compiling HPET"
)

add_custom_command(
    OUTPUT ${KERNEL_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${KERNEL_DIR} -I${VIDEO_DIR} -I${INT_DIR}
            -I${KEYBOARD_DIR} -I${ALLOC_DIR} -I${DRIVERS_DIR} -I${CLOCK_DIR}
            -I${SIGN_DIR} -I${APIC_DIR} -I${HPET_DIR} -I${XSDT_DIR}
            -c ${KERNEL_DIR}/kernel.c -o ${KERNEL_OBJ}
    DEPENDS ${KERNEL_DIR}/kernel.c
            ${VIDEO_DIR}/video.h ${INT_DIR}/isr.h ${INT_DIR}/pic.h
            ${KEYBOARD_DIR}/toclick.h ${ALLOC_DIR}/allocator.h ${ALLOC_DIR}/alloc.h
            ${DRIVERS_DIR}/str.h ${CLOCK_DIR}/clock.h ${SIGN_DIR}/signature_search.h
            ${APIC_DIR}/headers_search.h ${XSDT_DIR}/xsdt.h
    COMMENT "Compiling kernel"
)

add_custom_command(
    OUTPUT ${TERMINAL_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${APP_DIR} -I${TERMINAL_DIR}
            -I${DRIVERS_DIR} -I${ALLOC_DIR} -I${VIDEO_DIR}
            -I${KEYBOARD_DIR} -I${CLOCK_DIR} -I${SIGN_DIR}
            -c ${TERMINAL_DIR}/main.c -o ${TERMINAL_OBJ}
    DEPENDS ${TERMINAL_DIR}/main.c
            ${DRIVERS_DIR}/alloc/alloc.h ${DRIVERS_DIR}/video/video.h
            ${DRIVERS_DIR}/keyboard/toclick.h ${DRIVERS_DIR}/clock/clock.h
    COMMENT "Compiling terminal"
)

add_custom_command(
    OUTPUT ${KERNEL_BIN}
    COMMAND ld -T ${CMAKE_SOURCE_DIR}/linker.ld -o ${KERNEL_BIN}
            ${KERNEL_OBJ}
            ${VIDEO_OBJ}
            ${IDT_OBJ}
            ${ISR_C_OBJ}
            ${ISR_OBJ}
            ${PIC_OBJ}
            ${TOCLICK_OBJ}
            ${ALLOCATOR_OBJ}
            ${ALLOC_OBJ}
            ${CLOCK_OBJ}
            ${SIGN_OBJ}
            ${HPET_OBJ}
            ${HSEARCH_OBJ}
            ${TTS_OBJ}
            ${TERMINAL_OBJ}
    DEPENDS ${KERNEL_OBJ} ${VIDEO_OBJ} ${IDT_OBJ} ${ISR_C_OBJ} ${ISR_OBJ}
            ${PIC_OBJ} ${TOCLICK_OBJ} ${ALLOCATOR_OBJ} ${ALLOC_OBJ}
            ${CLOCK_OBJ} ${SIGN_OBJ} ${HPET_OBJ} ${HSEARCH_OBJ}
            ${TTS_OBJ} ${TERMINAL_OBJ}
            ${CMAKE_SOURCE_DIR}/linker.ld
    COMMENT "Linking kernel"
)

add_custom_command(
    OUTPUT ${DISK_IMAGE}
    COMMAND dd if=/dev/zero of=${DISK_IMAGE} bs=512 count=2880 status=none
    COMMAND dd if=${BOOTLOADER_BIN} of=${DISK_IMAGE} bs=512 seek=0   conv=notrunc status=none
    COMMAND dd if=${STAGE2_BIN}     of=${DISK_IMAGE} bs=512 seek=1   conv=notrunc status=none
    COMMAND dd if=${KERNEL_BIN}     of=${DISK_IMAGE} bs=512 seek=129 conv=notrunc status=none
    DEPENDS ${BOOTLOADER_BIN} ${STAGE2_BIN} ${KERNEL_BIN}
    COMMENT "Creating disk image"
)

add_custom_target(image ALL DEPENDS ${DISK_IMAGE} COMMENT "Building OS image")