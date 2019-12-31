TOOL_CHAIN = arm-none-eabi-
CC 		= ${TOOL_CHAIN}gcc
AS 		= ${TOOL_CHAIN}as
LD 		= ${TOOL_CHAIN}ld
OBJCOPY = ${TOOL_CHAIN}objcopy
OBJDUMP = $(TOOL_CHAIN)objdump

CFLAGS	 =	-Wall -g -mlittle-endian -mthumb -nostdlib -ffreestanding
CFLAGS	+=	-mcpu=cortex-m3
CFLAGS	+=	-I./
LDFLAGS	:= 	-g
objs 	:= 	vector.o main.o debug.o systick.o string.o

# os 部分
CFLAGS		+=-D GCC_BUILD

CFLAGS		+=-I./../../src/lib
CFLAGS		+=-I./../../src/kernel/

KERNEL_DIR	=../../kernel
	
KERNEL_PATH	=	../../src

objs	+=	$(KERNEL_PATH)/kernel/core.o \
			$(KERNEL_PATH)/kernel/pcb.o \
			$(KERNEL_PATH)/kernel/sche.o \
			$(KERNEL_PATH)/kernel/switch.o \
			$(KERNEL_PATH)/kernel/mm.o \
			$(KERNEL_PATH)/kernel/sem.o \
			$(KERNEL_PATH)/lib/slist.o

lidqos.bin: $(objs) $(SRC)
	${LD} -T lidqos.ld -o lidqos.elf $^
	${OBJCOPY} -O binary -S lidqos.elf $@
	${OBJDUMP} -D -m arm lidqos.elf > lidqos.dis

run: $(objs)
	${LD} -T lidqos.ld -o lidqos.elf $^
	${OBJCOPY} -O binary -S lidqos.elf lidqos.bin
	${OBJDUMP} -D -m arm lidqos.elf > lidqos.dis
	qemu-system-arm -M lm3s6965evb --kernel lidqos.bin -nographic

debug: $(objs)
	${LD} -T lidqos.ld -o lidqos.elf $^
	${OBJCOPY} -O binary -S lidqos.elf lidqos.bin
	${OBJDUMP} -D -m arm lidqos.elf > lidqos.dis
	qemu-system-arm -M lm3s6965evb --kernel lidqos.bin -nographic -s -S

%.o:%.c 
	${CC} $(CFLAGS) -c -o $@ $<

%.o:%.s
	${CC} $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o *.elf *.bin *.dis
