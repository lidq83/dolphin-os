#------------------------------------------------------------------------#
# Output files
ELF_FILE	=	dolphin-os.elf
BIN_FILE	=	dolphin-os.bin
HEX_FILE	=	dolphin-os.hex
INFO_FILE	=	dolphin-os.info
CODE_FILE	=	dolphin-os.code

#------------------------------------------------------------------------#
# Cross Compiler
CC			=	arm-none-eabi-gcc
OBJCOPY		=	arm-none-eabi-objcopy
OBJDUMP		=	arm-none-eabi-objdump
READELF		=	arm-none-eabi-readelf

#------------------------------------------------------------------------#
# Flags
CFLAGS		+=	-g
CFLAGS		+=	-mthumb
CFLAGS		+=	-Wno-incompatible-pointer-types
CFLAGS		+=	-Wno-unused-but-set-variable
CFLAGS		+=	-Wno-unused-variable
CFLAGS		+=	-mcpu=cortex-m3
CFLAGS		+=	-I./
CFLAGS		+=	-D"assert_param(expr)=((void)0)"
CFLAGS		+=	-D STM32F10X_MD
CFLAGS		+=	-nostartfiles

#------------------------------------------------------------------------#
# Link script
CFLAGS		+=	-Wl,-Tstm32_flash.ld

KERNEL_PATH	=	../../src
CFLAGS		+=	-I./src
CFLAGS		+=	-I$(KERNEL_PATH)/kernel
CFLAGS		+=	-I$(KERNEL_PATH)/lib
CFLAGS		+=	-I$(KERNEL_PATH)/mm
CFLAGS		+=	-I$(KERNEL_PATH)/fs

#------------------------------------------------------------------------#
# Main function
SRC			+=	./src/main.c \
				./src/k_printf.c \
				./src/string.c \
				./src/systick.c \
				./src/vector.c

SRC			+=	$(KERNEL_PATH)/kernel/core.c \
				$(KERNEL_PATH)/kernel/pcb.c \
				$(KERNEL_PATH)/kernel/sche.c \
				$(KERNEL_PATH)/kernel/sem.c \
				$(KERNEL_PATH)/kernel/switch.S \
				$(KERNEL_PATH)/mm/mm.c \
				$(KERNEL_PATH)/fs/vfs.c \
				$(KERNEL_PATH)/fs/fs.c \
				$(KERNEL_PATH)/fs/fcntl.c \
				$(KERNEL_PATH)/lib/slist.c

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
	qemu-system-arm -M lm3s6965evb --kernel $(BIN_FILE) -nographic

debug:$(BIN_FILE) $(HEX_FILE) $(INFO_FILE) $(CODE_FILE)
	qemu-system-arm -M lm3s6965evb --kernel $(BIN_FILE) -nographic -s -S

clean:
	@rm -rvf *.o *.bin *.code *.elf *.hex *.info
