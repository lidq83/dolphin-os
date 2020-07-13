#------------------------------------------------------------------------#
# Output files
ELF_FILE	=	dolphin-os.elf
BIN_FILE	=	dolphin-os.bin
HEX_FILE	=	dolphin-os.hex
INFO_FILE	=	dolphin-os.info
CODE_FILE	=	dolphin-os.code

#------------------------------------------------------------------------#
# OS Arch
OS_ARCH		=	arm/armv7-m

#------------------------------------------------------------------------#
# Cross Compiler
CC			=	arm-none-eabi-gcc
OBJCOPY		=	arm-none-eabi-objcopy
OBJDUMP		=	arm-none-eabi-objdump
READELF		=	arm-none-eabi-readelf

#------------------------------------------------------------------------#
# Flags
CFLAGS		+=	-g -Os
CFLAGS		+=	-mthumb
CFLAGS		+=	-Wno-incompatible-pointer-types
CFLAGS		+=	-Wno-unused-but-set-variable
CFLAGS		+=	-Wno-int-conversion
CFLAGS		+=	-Wno-unused-variable
CFLAGS		+=	-mcpu=cortex-m3
CFLAGS		+=	-I./
CFLAGS		+=	-D"assert_param(expr)=((void)0)"
CFLAGS		+=	-D STM32F10X_MD
CFLAGS		+=	-nostartfiles

#------------------------------------------------------------------------#
# Link script
CFLAGS		+=	-Wl,-Tdolphin-os.ld

KERNEL_PATH	=	../../kernel
CFLAGS		+=	-I./src
CFLAGS		+=	-I$(KERNEL_PATH)/kernel
CFLAGS		+=	-I$(KERNEL_PATH)/lib
CFLAGS		+=	-I$(KERNEL_PATH)/libc
CFLAGS		+=	-I$(KERNEL_PATH)/mm
CFLAGS		+=	-I$(KERNEL_PATH)/fs
CFLAGS		+=	-I$(KERNEL_PATH)/drv

#------------------------------------------------------------------------#
# Main function
SRC			+=	./src/main.c \
				./src/systick.c \
				./src/vector.c \
				./src/stddev.c \
				./src/buff_s.c

#------------------------------------------------------------------------#
# OS Arch
SRC			+=	$(KERNEL_PATH)/arch/$(OS_ARCH)/stack.c \
				$(KERNEL_PATH)/arch/$(OS_ARCH)/switch.S

#------------------------------------------------------------------------#
# OS Kernel
SRC			+=	$(KERNEL_PATH)/kernel/core.c \
				$(KERNEL_PATH)/kernel/pcb.c \
				$(KERNEL_PATH)/kernel/sche.c \
				$(KERNEL_PATH)/kernel/sem.c \
				$(KERNEL_PATH)/mm/mm.c \
				$(KERNEL_PATH)/mm/mm_addfreechunk.c \
				$(KERNEL_PATH)/mm/mm_shrinkchunk.c \
				$(KERNEL_PATH)/mm/mm_free.c \
				$(KERNEL_PATH)/mm/mm_initialize.c \
				$(KERNEL_PATH)/mm/mm_malloc.c \
				$(KERNEL_PATH)/mm/mm_realloc.c \
				$(KERNEL_PATH)/mm/mm_sem.c \
				$(KERNEL_PATH)/mm/mm_size2ndx.c \
				$(KERNEL_PATH)/fs/vfs.c \
				$(KERNEL_PATH)/fs/fs.c \
				$(KERNEL_PATH)/fs/fcntl.c \
				$(KERNEL_PATH)/libc/lib_libnoflush.c \
				$(KERNEL_PATH)/libc/lib_memoutstream.c \
				$(KERNEL_PATH)/libc/lib_nulloutstream.c \
				$(KERNEL_PATH)/libc/lib_libvsprintf.c \
				$(KERNEL_PATH)/libc/lib_vsprintf.c \
				$(KERNEL_PATH)/libc/lib_vsnprintf.c \
				$(KERNEL_PATH)/libc/lib_snprintf.c \
				$(KERNEL_PATH)/libc/lib_dtoa.c \
				$(KERNEL_PATH)/libc/lib_strtod.c \
				$(KERNEL_PATH)/libc/lib_strtof.c \
				$(KERNEL_PATH)/libc/lib_strtol.c \
				$(KERNEL_PATH)/libc/lib_strtoll.c \
				$(KERNEL_PATH)/libc/lib_strtoul.c \
				$(KERNEL_PATH)/libc/lib_strtoull.c \
				$(KERNEL_PATH)/libc/lib_skipspace.c \
				$(KERNEL_PATH)/libc/lib_checkbase.c \
				$(KERNEL_PATH)/libc/lib_isbasedigit.c \
				$(KERNEL_PATH)/lib/slist.c \
				$(KERNEL_PATH)/lib/k_printf.c

all:$(BIN_FILE) $(HEX_FILE) $(INFO_FILE) $(CODE_FILE)

$(BIN_FILE):$(ELF_FILE)
	$(OBJCOPY) -O binary $^ $@

$(HEX_FILE):$(ELF_FILE)
	$(OBJCOPY) -O ihex $^ $@

$(INFO_FILE):$(ELF_FILE)
	$(READELF) -a $^ > $@

$(CODE_FILE):$(ELF_FILE)
	$(OBJDUMP) -S $^ > $@

$(ELF_FILE):$(SRC)
	$(CC) $(CFLAGS)	$^ -o $@

run:$(BIN_FILE) $(HEX_FILE) $(INFO_FILE) $(CODE_FILE)
	qemu-system-arm -M lm3s6965evb --kernel $(BIN_FILE) -monitor stdio

debug:$(BIN_FILE) $(HEX_FILE) $(INFO_FILE) $(CODE_FILE)
	qemu-system-arm -M lm3s6965evb --kernel $(BIN_FILE) -monitor stdio -s -S

clean:
	@rm -rvf *.o *.bin *.code *.elf *.hex *.info
