#------------------------------------------------------------------------#
# OS Arch
OS_ARCH		=	arm/armv7-m

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
CFLAGS		+=	-g -Os
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
CFLAGS		+=	-Wl,-Tdolphin-os.ld

#------------------------------------------------------------------------#
# Libraries
STM32_LIBS	=	libs/STM32F10x_StdPeriph_Driver
CFLAGS		+=	-I$(STM32_LIBS)/inc
CFLAGS		+=	-Ilibs/CMSIS/Include
CFLAGS		+=	-Ilibs/CMSIS/Device/ST/STM32F10x/Include

KERNEL_PATH	=	../../kernel
CFLAGS		+=	-I./src/include
CFLAGS		+=	-I./src/main
CFLAGS		+=	-I./src/libs
CFLAGS		+=	-I./src/drv
CFLAGS		+=	-I./src/dev
CFLAGS		+=	-I./src/modules
CFLAGS		+=	-I./src/modules/led
CFLAGS		+=	-I./src/modules/debug
CFLAGS		+=	-I$(KERNEL_PATH)/kernel
CFLAGS		+=	-I$(KERNEL_PATH)/lib
CFLAGS		+=	-I$(KERNEL_PATH)/libc
CFLAGS		+=	-I$(KERNEL_PATH)/mm
CFLAGS		+=	-I$(KERNEL_PATH)/fs
CFLAGS		+=	-I$(KERNEL_PATH)/drv

#------------------------------------------------------------------------#
# StdPeriph
SRC			=	$(STM32_LIBS)/src/misc.c \
				$(STM32_LIBS)/src/stm32f10x_rcc.c \
				$(STM32_LIBS)/src/stm32f10x_gpio.c \
				$(STM32_LIBS)/src/stm32f10x_tim.c \
				$(STM32_LIBS)/src/stm32f10x_usart.c \
				$(STM32_LIBS)/src/stm32f10x_dma.c \
				$(STM32_LIBS)/src/stm32f10x_exti.c \
				$(STM32_LIBS)/src/stm32f10x_i2c.c

#------------------------------------------------------------------------#
# Setup system clock
SRC			+=	./src/main/system_stm32f10x.c

#------------------------------------------------------------------------#
# Main function
SRC			+=	./src/main/main.c \
				./src/modules/led/led_task.c \
				./src/modules/debug/debug_task.c

#------------------------------------------------------------------------#
# Libs
SRC			+=	./src/libs/buff_s.c

#------------------------------------------------------------------------#
# Drv
SRC			+=	./src/drv/led.c \
				./src/drv/sysclk.c \
				./src/drv/serial1.c \
				./src/drv/serial2.c \
				./src/drv/virtual_uart.c

#------------------------------------------------------------------------#
# Dev
SRC			+=	./src/dev/ttyS1.c \
				./src/dev/ttyS2.c \
				./src/dev/std.c

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
				$(KERNEL_PATH)/mm/mm_mallinfo.c \
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

STARTUP		=	./src/main/startup_stm32f10x_md.s

STARTUP_OBJ	=	startup_stm32f10x_md.o

all:$(BIN_FILE) $(HEX_FILE) $(INFO_FILE) $(CODE_FILE)

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

$(ELF_FILE):$(SRC) $(STARTUP_OBJ)
	$(CC) $(CFLAGS)	$^ -o $@

install:
	jlink -commanderscript loadbin_cmd.jlink

clean:
	@rm -rvf *.o *.bin *.code *.elf *.hex *.info
