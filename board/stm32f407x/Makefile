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
CFLAGS		+=	-lm
CFLAGS		+=	-std=gnu11 -g -Os

CFLAGS		+=	-mthumb
CFLAGS		+=	-Wno-incompatible-pointer-types
CFLAGS		+=	-Wno-unused-but-set-variable
CFLAGS		+=	-Wno-unused-variable
CFLAGS		+=	-mcpu=cortex-m4
CFLAGS		+=	-mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS		+=	-D"assert_param(expr)=((void)0)"
CFLAGS		+=	-D STM32F40XX -DUSE_STDPERIPH_DRIVER
CFLAGS		+=	-nostartfiles

#------------------------------------------------------------------------#
# Link script
CFLAGS		+=	-Wl,-Tdolphin-os.ld

#------------------------------------------------------------------------#
# Libraries
STM32_LIBS	=	libs/STM32F4xx_StdPeriph_Driver
CFLAGS		+=	-I$(STM32_LIBS)/inc
CFLAGS		+=	-Ilibs/CMSIS/Include
CFLAGS		+=	-Ilibs/CMSIS/Device/ST/STM32F4xx/Include

CFLAGS		+=	-I./
CFLAGS		+=	-I./src/main
CFLAGS		+=	-I./src/modules
CFLAGS		+=	-I./src/modules/led
CFLAGS		+=	-I./src/modules/hmc5883l
CFLAGS		+=	-I./src/libs
CFLAGS		+=	-I./src/drv
CFLAGS		+=	-I./src/dev
CFLAGS		+=	-I./src/include

KERNEL_PATH	=	../../kernel
CFLAGS		+=	-I$(KERNEL_PATH)/kernel
CFLAGS		+=	-I$(KERNEL_PATH)/lib
CFLAGS		+=	-I$(KERNEL_PATH)/libc
CFLAGS		+=	-I$(KERNEL_PATH)/math
CFLAGS		+=	-I$(KERNEL_PATH)/mm
CFLAGS		+=	-I$(KERNEL_PATH)/fs
CFLAGS		+=	-I$(KERNEL_PATH)/drv

#------------------------------------------------------------------------#
# StdPeriph
SRC			=	$(STM32_LIBS)/src/misc.c \
				$(STM32_LIBS)/src/stm32f4xx_rcc.c \
				$(STM32_LIBS)/src/stm32f4xx_gpio.c \
				$(STM32_LIBS)/src/stm32f4xx_tim.c \
				$(STM32_LIBS)/src/stm32f4xx_usart.c \
				$(STM32_LIBS)/src/stm32f4xx_dma.c \
				$(STM32_LIBS)/src/stm32f4xx_exti.c \
				$(STM32_LIBS)/src/stm32f4xx_i2c.c

#------------------------------------------------------------------------#
# Setup system clock
SRC			+=	./src/main/system_stm32f4xx.c

#------------------------------------------------------------------------#
# Main function
SRC			+=	./src/main/main.c

#------------------------------------------------------------------------#
# Hardware function
SRC			+=	./src/drv/led.c \
				./src/drv/sysclk.c \
				./src/drv/serial1.c \
				./src/drv/hmc5883l.c \
				./src/dev/std.c

#------------------------------------------------------------------------#
# Libs
SRC			+=	./src/libs/buff_s.c

#------------------------------------------------------------------------#
# Modules
SRC			+=	./src/modules/led/led_task.c \
				./src/modules/hmc5883l/hmc5883l_task.c

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
				$(KERNEL_PATH)/kernel/top.c \
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
				$(KERNEL_PATH)/lib/k_printf.c \
				$(KERNEL_PATH)/math/__sin.c \
				$(KERNEL_PATH)/math/__cos.c \
				$(KERNEL_PATH)/math/lib_acos.c \
				$(KERNEL_PATH)/math/lib_acosf.c \
				$(KERNEL_PATH)/math/lib_acosh.c \
				$(KERNEL_PATH)/math/lib_acoshf.c \
				$(KERNEL_PATH)/math/lib_acoshl.c \
				$(KERNEL_PATH)/math/lib_acosl.c \
				$(KERNEL_PATH)/math/lib_asin.c \
				$(KERNEL_PATH)/math/lib_asinf.c \
				$(KERNEL_PATH)/math/lib_asinh.c \
				$(KERNEL_PATH)/math/lib_asinhf.c \
				$(KERNEL_PATH)/math/lib_asinhl.c \
				$(KERNEL_PATH)/math/lib_asinl.c \
				$(KERNEL_PATH)/math/lib_atan2.c \
				$(KERNEL_PATH)/math/lib_atan2f.c \
				$(KERNEL_PATH)/math/lib_atan2l.c \
				$(KERNEL_PATH)/math/lib_atan.c \
				$(KERNEL_PATH)/math/lib_atanf.c \
				$(KERNEL_PATH)/math/lib_atanh.c \
				$(KERNEL_PATH)/math/lib_atanhf.c \
				$(KERNEL_PATH)/math/lib_atanhl.c \
				$(KERNEL_PATH)/math/lib_atanl.c \
				$(KERNEL_PATH)/math/lib_ceil.c \
				$(KERNEL_PATH)/math/lib_ceilf.c \
				$(KERNEL_PATH)/math/lib_ceill.c \
				$(KERNEL_PATH)/math/lib_copysign.c \
				$(KERNEL_PATH)/math/lib_copysignf.c \
				$(KERNEL_PATH)/math/lib_copysignl.c \
				$(KERNEL_PATH)/math/lib_cos.c \
				$(KERNEL_PATH)/math/lib_cosf.c \
				$(KERNEL_PATH)/math/lib_cosh.c \
				$(KERNEL_PATH)/math/lib_coshf.c \
				$(KERNEL_PATH)/math/lib_coshl.c \
				$(KERNEL_PATH)/math/lib_cosl.c \
				$(KERNEL_PATH)/math/lib_erf.c \
				$(KERNEL_PATH)/math/lib_erff.c \
				$(KERNEL_PATH)/math/lib_erfl.c \
				$(KERNEL_PATH)/math/lib_exp.c \
				$(KERNEL_PATH)/math/lib_expf.c \
				$(KERNEL_PATH)/math/lib_expl.c \
				$(KERNEL_PATH)/math/lib_fabs.c \
				$(KERNEL_PATH)/math/lib_fabsf.c \
				$(KERNEL_PATH)/math/lib_fabsl.c \
				$(KERNEL_PATH)/math/lib_floor.c \
				$(KERNEL_PATH)/math/lib_floorf.c \
				$(KERNEL_PATH)/math/lib_floorl.c \
				$(KERNEL_PATH)/math/lib_fmod.c \
				$(KERNEL_PATH)/math/lib_fmodf.c \
				$(KERNEL_PATH)/math/lib_fmodl.c \
				$(KERNEL_PATH)/math/lib_frexp.c \
				$(KERNEL_PATH)/math/lib_frexpf.c \
				$(KERNEL_PATH)/math/lib_frexpl.c \
				$(KERNEL_PATH)/math/lib_gamma.c \
				$(KERNEL_PATH)/math/lib_ldexp.c \
				$(KERNEL_PATH)/math/lib_ldexpf.c \
				$(KERNEL_PATH)/math/lib_ldexpl.c \
				$(KERNEL_PATH)/math/lib_lgamma.c \
				$(KERNEL_PATH)/math/lib_libexpi.c \
				$(KERNEL_PATH)/math/lib_libexpif.c \
				$(KERNEL_PATH)/math/lib_libsqrtapprox.c \
				$(KERNEL_PATH)/math/lib_log10.c \
				$(KERNEL_PATH)/math/lib_log10f.c \
				$(KERNEL_PATH)/math/lib_log10l.c \
				$(KERNEL_PATH)/math/lib_log2.c \
				$(KERNEL_PATH)/math/lib_log2f.c \
				$(KERNEL_PATH)/math/lib_log2l.c \
				$(KERNEL_PATH)/math/lib_log.c \
				$(KERNEL_PATH)/math/lib_logf.c \
				$(KERNEL_PATH)/math/lib_logl.c \
				$(KERNEL_PATH)/math/lib_modf.c \
				$(KERNEL_PATH)/math/lib_modff.c \
				$(KERNEL_PATH)/math/lib_modfl.c \
				$(KERNEL_PATH)/math/lib_pow.c \
				$(KERNEL_PATH)/math/lib_powf.c \
				$(KERNEL_PATH)/math/lib_powl.c \
				$(KERNEL_PATH)/math/lib_rint.c \
				$(KERNEL_PATH)/math/lib_rintf.c \
				$(KERNEL_PATH)/math/lib_rintl.c \
				$(KERNEL_PATH)/math/lib_round.c \
				$(KERNEL_PATH)/math/lib_roundf.c \
				$(KERNEL_PATH)/math/lib_roundl.c \
				$(KERNEL_PATH)/math/lib_sin.c \
				$(KERNEL_PATH)/math/lib_sinf.c \
				$(KERNEL_PATH)/math/lib_sinh.c \
				$(KERNEL_PATH)/math/lib_sinhf.c \
				$(KERNEL_PATH)/math/lib_sinhl.c \
				$(KERNEL_PATH)/math/lib_sinl.c \
				$(KERNEL_PATH)/math/lib_sqrt.c \
				$(KERNEL_PATH)/math/lib_sqrtf.c \
				$(KERNEL_PATH)/math/lib_sqrtl.c \
				$(KERNEL_PATH)/math/lib_tan.c \
				$(KERNEL_PATH)/math/lib_tanf.c \
				$(KERNEL_PATH)/math/lib_tanh.c \
				$(KERNEL_PATH)/math/lib_tanhf.c \
				$(KERNEL_PATH)/math/lib_tanhl.c \
				$(KERNEL_PATH)/math/lib_tanl.c \
				$(KERNEL_PATH)/math/lib_trunc.c \
				$(KERNEL_PATH)/math/lib_truncf.c \
				$(KERNEL_PATH)/math/lib_truncl.c



STARTUP		=	./src/main/startup_stm32f40xx.s

STARTUP_OBJ	=	startup_stm32f40xx.o

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
	#@st-flash write $(BIN_FILE) 0x8000000
	jlink -commanderscript loadbin_cmd.jlink

clean:
	@rm -rvf *.o *.bin *.code *.elf *.hex *.info
