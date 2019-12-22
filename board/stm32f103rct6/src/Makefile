# src Makefile
include ../Makefile.common

OS_PATH		= ../../../src

CFLAGSlib	+=	-I$(OS_PATH)/kernel
CFLAGSlib	+=	-I$(OS_PATH)/lib

CFLAGSlib	+=	-Iinclude
CFLAGSlib	+=	-Imain
CFLAGSlib	+=	-Imodules

all: src

src: mkdir objs app.a

mkdir:
	mkdir -p release

objs:
	$(CC) $(CFLAGSlib) -o $(PATH_TARGET)/main.o -c main/main.c
	$(CC) $(CFLAGSlib) -o $(PATH_TARGET)/startup.o -c main/startup_stm32f10x_md.s
	$(CC) $(CFLAGSlib) -o $(PATH_TARGET)/led.o -c modules/led.c
	$(CC) $(CFLAGSlib) -o $(PATH_TARGET)/timer.o -c modules/timer.c
	$(CC) $(CFLAGSlib) -o $(PATH_TARGET)/timer4.o -c modules/timer4.c
	$(CC) $(CFLAGSlib) -o $(PATH_TARGET)/core.o -c $(OS_PATH)/kernel/core.c
	$(CC) $(CFLAGSlib) -o $(PATH_TARGET)/pcb.o -c $(OS_PATH)/kernel/pcb.c
	$(CC) $(CFLAGSlib) -o $(PATH_TARGET)/sche.o -c $(OS_PATH)/kernel/sche.c
	$(CC) $(CFLAGSlib) -o $(PATH_TARGET)/list.o -c $(OS_PATH)/lib/list.c

app.a:
	$(AR) cr $(PATH_TARGET)/app.a $(PATH_TARGET)/startup.o \
		$(PATH_TARGET)/main.o \
		$(PATH_TARGET)/led.o \
		$(PATH_TARGET)/timer.o \
		$(PATH_TARGET)/timer4.o \
		$(PATH_TARGET)/core.o \
		$(PATH_TARGET)/pcb.o \
		$(PATH_TARGET)/sche.o \
		$(PATH_TARGET)/list.o
	
clean:
	rm -f $(PATH_TARGET)/*
	
