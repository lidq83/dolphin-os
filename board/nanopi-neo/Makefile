#------------------------------------------------------------------------#
# OS Arch
OS_ARCH		=	arm/armv7-a

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


# CFLAGS	 =	-Wall -g -march=armv7-a -marm -msoft-float 
				#-Wall -g -gdwarf-2 -O0 -DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB
# CFLAGS	+=	-I./
# LDFLAGS	:= 	-g -Map ygos.map
# objs 	:=  main.o  

# -march=armv7-a -marm -msoft-float -nostartfiles -Wl,--gc-sections,-Map=rtthread.map,-cref,-u,system_vectors

#------------------------------------------------------------------------#
# Flags
CFLAGS		+=	-march=armv7-a
CFLAGS		+=	-mthumb -marm -msoft-float 
CFLAGS		+=	-Wno-incompatible-pointer-types
CFLAGS		+=	-Wno-unused-but-set-variable
CFLAGS		+=	-Wno-unused-variable
CFLAGS		+=	-nostartfiles
CFLAGS		+=	-g
CFLAGS		+=	-I./
CFLAGS		+=	-DHAVE_CCONFIG_H -DGCC_BUILD -DLINUX -DRT_USING_NEWLIB

#------------------------------------------------------------------------#
# Link script
CFLAGS		+=	-Wl,-Tdolphin-os.ld

KERNEL_PATH	=	../../src
CFLAGS		+=	-I./src/include
CFLAGS		+=	-I./src/main
CFLAGS		+=	-I./src/hardware
CFLAGS		+=	-I./src/modules
CFLAGS		+=	-I$(KERNEL_PATH)/kernel
CFLAGS		+=	-I$(KERNEL_PATH)/lib
CFLAGS		+=	-I$(KERNEL_PATH)/mm
CFLAGS		+=	-I$(KERNEL_PATH)/fs
CFLAGS		+=	-I$(KERNEL_PATH)/drv

#------------------------------------------------------------------------#
# Main function
SRC			+=	./src/main/main.c \
				./src/main/task.c

#------------------------------------------------------------------------#
# OS Arch
SRC			+=	$(KERNEL_PATH)/arch/$(OS_ARCH)/stack.c \
				$(KERNEL_PATH)/arch/$(OS_ARCH)/start.S \
				$(KERNEL_PATH)/arch/$(OS_ARCH)/vector.S \
				$(KERNEL_PATH)/arch/$(OS_ARCH)/switch.S

#------------------------------------------------------------------------#
# OS Kernel
SRC			+=	$(KERNEL_PATH)/kernel/core.c \
				$(KERNEL_PATH)/kernel/pcb.c \
				$(KERNEL_PATH)/kernel/sche.c \
				$(KERNEL_PATH)/kernel/sem.c \
				$(KERNEL_PATH)/mm/mm.c \
				$(KERNEL_PATH)/fs/vfs.c \
				$(KERNEL_PATH)/fs/fs.c \
				$(KERNEL_PATH)/fs/fcntl.c \
				$(KERNEL_PATH)/lib/slist.c \
				$(KERNEL_PATH)/lib/k_string.c \
				$(KERNEL_PATH)/lib/k_printf.c \
				$(KERNEL_PATH)/lib/k_scanf.c

# all:$(BIN_FILE) $(HEX_FILE) $(INFO_FILE) $(CODE_FILE)
all:
	@echo "Dolphin-OS do not support Cortex-A cpu yet."

$(BIN_FILE):$(ELF_FILE)
	$(OBJCOPY) -O binary $^ $@

$(HEX_FILE):$(ELF_FILE)
	$(OBJCOPY) -O ihex $^ $@

$(INFO_FILE):$(ELF_FILE)
	$(READELF) -a $^ > $@

$(CODE_FILE):$(ELF_FILE)
	$(OBJDUMP) -S $^ > $@

$(STARTUP_OBJ):$(STARTUP)
	$(CC) $(CFLAGS) $^ -c $(STARTUP)

$(ELF_FILE):$(SRC)
	$(CC) $(CFLAGS)	$^ -o $@

# run: $(BIN_FILE)
# 	qemu-system-arm -M mcimx6ul-evk -kernel $(ELF_FILE) -serial stdio

# debug: $(BIN_FILE)
# 	qemu-system-arm -M mcimx6ul-evk -kernel $(ELF_FILE) -s -S

# install:

clean:
	@rm -rvf *.o *.bin *.code *.elf *.hex *.info
