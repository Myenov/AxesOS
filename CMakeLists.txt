cmake_minimum_required(VERSION 3.20)

project(AxesOS C ASM_NASM)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(LOAD_DIR ${SOURCE_DIR}/load)
set(KERNEL_DIR ${SOURCE_DIR}/kernel)
set(DRIVERS_DIR ${SOURCE_DIR}/drivers)
set(INT_DIR ${DRIVERS_DIR}/int)
set(KEYBOARD_DIR ${DRIVERS_DIR}/keyboard)
set(VIDEO_DIR ${DRIVERS_DIR}/video)
set(ALLOC_DIR ${DRIVERS_DIR}/alloc)
set(CLOCK_DIR ${DRIVERS_DIR}/clock)

set(BOOTLOADER_BIN ${CMAKE_BINARY_DIR}/bootloader.bin)
set(TTS_OBJ       ${CMAKE_BINARY_DIR}/tts_init.o)
set(ISR_OBJ       ${CMAKE_BINARY_DIR}/isr.o)
set(IDT_OBJ       ${CMAKE_BINARY_DIR}/idt.o)
set(ISR_C_OBJ     ${CMAKE_BINARY_DIR}/isr_c.o)
set(PIC_OBJ       ${CMAKE_BINARY_DIR}/pic.o)
set(TOCLICK_OBJ   ${CMAKE_BINARY_DIR}/toclick.o)
set(VIDEO_OBJ     ${CMAKE_BINARY_DIR}/video.o)
set(ALLOC_OBJ     ${CMAKE_BINARY_DIR}/allocator.o)
set(CLOCK_OBJ     ${CMAKE_BINARY_DIR}/clock.o)
set(KERNEL_OBJ    ${CMAKE_BINARY_DIR}/kernel.o)
set(KERNEL_BIN    ${CMAKE_BINARY_DIR}/kernel.bin)
set(DISK_IMAGE    ${CMAKE_BINARY_DIR}/AxesOS.img)

# Общие флаги для gcc
set(COMMON_CFLAGS
    -ffreestanding -nostdlib -fno-stack-protector -fno-pie -fno-pic
    -mno-red-zone -mcmodel=large -mno-mmx -mno-sse -mno-sse2 -O0
)

# -------- Bootloader --------
add_custom_command(
    OUTPUT ${BOOTLOADER_BIN}
    COMMAND nasm -f bin ${LOAD_DIR}/bootloader.asm -o ${BOOTLOADER_BIN}
    DEPENDS ${LOAD_DIR}/bootloader.asm
    COMMENT "Compiling bootloader"
)

# -------- TTS init (asm) --------
add_custom_command(
    OUTPUT ${TTS_OBJ}
    COMMAND nasm -f elf64 ${LOAD_DIR}/TTS.init.asm -o ${TTS_OBJ}
    DEPENDS ${LOAD_DIR}/TTS.init.asm
    COMMENT "Compiling TTS init"
)

# -------- ISR (asm) --------
add_custom_command(
    OUTPUT ${ISR_OBJ}
    COMMAND nasm -f elf64 ${INT_DIR}/isr.asm -o ${ISR_OBJ}
    DEPENDS ${INT_DIR}/isr.asm
    COMMENT "Compiling ISR assembly"
)

# -------- IDT.c --------
add_custom_command(
    OUTPUT ${IDT_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${INT_DIR}
            -c ${INT_DIR}/IDT.c -o ${IDT_OBJ}
    DEPENDS ${INT_DIR}/IDT.c ${INT_DIR}/IDT.h
    COMMENT "Compiling IDT"
)

# -------- isr.c --------
add_custom_command(
    OUTPUT ${ISR_C_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${INT_DIR} -I${KEYBOARD_DIR} -I${VIDEO_DIR}
            -c ${INT_DIR}/isr.c -o ${ISR_C_OBJ}
    DEPENDS ${INT_DIR}/isr.c ${INT_DIR}/isr.h ${INT_DIR}/IDT.h ${KEYBOARD_DIR}/toclick.h
    COMMENT "Compiling ISR C code"
)

# -------- pic.c --------
add_custom_command(
    OUTPUT ${PIC_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${INT_DIR}
            -c ${INT_DIR}/pic.c -o ${PIC_OBJ}
    DEPENDS ${INT_DIR}/pic.c ${INT_DIR}/pic.h
    COMMENT "Compiling PIC"
)

# -------- toclick.c --------
add_custom_command(
    OUTPUT ${TOCLICK_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${KEYBOARD_DIR} -I${VIDEO_DIR}
            -c ${KEYBOARD_DIR}/toclick.c -o ${TOCLICK_OBJ}
    DEPENDS ${KEYBOARD_DIR}/toclick.c ${KEYBOARD_DIR}/toclick.h ${VIDEO_DIR}/video.h
    COMMENT "Compiling keyboard handler"
)

# -------- video.c --------
add_custom_command(
    OUTPUT ${VIDEO_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${VIDEO_DIR}
            -c ${VIDEO_DIR}/video.c -o ${VIDEO_OBJ}
    DEPENDS ${VIDEO_DIR}/video.c ${VIDEO_DIR}/video.h
    COMMENT "Compiling video driver"
)

# -------- allocator.c --------
add_custom_command(
    OUTPUT ${ALLOC_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${ALLOC_DIR} -I${VIDEO_DIR} -I${DRIVERS_DIR}
            -c ${ALLOC_DIR}/allocator.c -o ${ALLOC_OBJ}
    DEPENDS ${ALLOC_DIR}/allocator.c ${ALLOC_DIR}/allocator.h
            ${DRIVERS_DIR}/str.h ${VIDEO_DIR}/video.h
    COMMENT "Compiling allocator"
)

# -------- clock.c --------
add_custom_command(
    OUTPUT ${CLOCK_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${CLOCK_DIR} -I${DRIVERS_DIR}
            -c ${CLOCK_DIR}/clock.c -o ${CLOCK_OBJ}
    DEPENDS ${CLOCK_DIR}/clock.c ${CLOCK_DIR}/clock.h ${DRIVERS_DIR}/outb_and_inb.h
    COMMENT "Compiling clock driver"
)

# -------- kernel.c --------
add_custom_command(
    OUTPUT ${KERNEL_OBJ}
    COMMAND gcc ${COMMON_CFLAGS}
            -I${SOURCE_DIR} -I${KERNEL_DIR} -I${VIDEO_DIR} -I${INT_DIR}
            -I${KEYBOARD_DIR} -I${ALLOC_DIR}
            -c ${KERNEL_DIR}/kernel.c -o ${KERNEL_OBJ}
    DEPENDS ${KERNEL_DIR}/kernel.c ${VIDEO_DIR}/video.h ${INT_DIR}/isr.h
            ${INT_DIR}/pic.h ${KEYBOARD_DIR}/toclick.h ${ALLOC_DIR}/allocator.h
    COMMENT "Compiling kernel"
)

# -------- Линковка --------
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
            ${ALLOC_OBJ}
            ${CLOCK_OBJ}
            ${TTS_OBJ}
    DEPENDS ${KERNEL_OBJ} ${VIDEO_OBJ} ${IDT_OBJ} ${ISR_C_OBJ} ${ISR_OBJ}
            ${PIC_OBJ} ${TOCLICK_OBJ} ${ALLOC_OBJ} ${CLOCK_OBJ} ${TTS_OBJ}
            ${CMAKE_SOURCE_DIR}/linker.ld
    COMMENT "Linking kernel"
)

# -------- Образ диска --------
add_custom_command(
    OUTPUT ${DISK_IMAGE}
    COMMAND dd if=/dev/zero of=${DISK_IMAGE} bs=512 count=2880 status=none
    COMMAND dd if=${BOOTLOADER_BIN} of=${DISK_IMAGE} bs=512 count=1 conv=notrunc status=none
    COMMAND dd if=${KERNEL_BIN} of=${DISK_IMAGE} bs=512 seek=1 conv=notrunc status=none
    DEPENDS ${BOOTLOADER_BIN} ${KERNEL_BIN}
    COMMENT "Creating disk image"
)

# -------- Цели --------
add_custom_target(image ALL
    DEPENDS ${DISK_IMAGE}
    COMMENT "Building OS image"
)

add_custom_target(run
    COMMAND qemu-system-x86_64 -drive file=${DISK_IMAGE},format=raw,if=ide,index=0 -no-reboot -no-shutdown
    DEPENDS ${DISK_IMAGE}
    COMMENT "Running in QEMU"
)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E remove -f
            ${BOOTLOADER_BIN} ${TTS_OBJ} ${ISR_OBJ} ${IDT_OBJ} ${ISR_C_OBJ}
            ${PIC_OBJ} ${TOCLICK_OBJ} ${VIDEO_OBJ} ${ALLOC_OBJ} ${CLOCK_OBJ}
            ${KERNEL_OBJ} ${KERNEL_BIN} ${DISK_IMAGE}
    COMMENT "Cleaning all generated files"
)