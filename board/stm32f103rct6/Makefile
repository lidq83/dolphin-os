# src Makefile
include ../Makefile.common

PATH_HARDWARE=hardware
PATH_MAIN=main
CFLAGSlib+=-Iinclude

all: src

src: mkdir objs app.a

mkdir:
	mkdir -p release

objs:
	$(CC) $(CFLAGSlib) -o $(PATH_TARGET)/main.o -c $(PATH_MAIN)/main.c
	$(CC) $(CFLAGSlib) -o $(PATH_TARGET)/startup.o -c $(PATH_MAIN)/startup_stm32f10x_md.s
	$(CC) $(CFLAGSlib) -o $(PATH_TARGET)/led.o -c $(PATH_HARDWARE)/led.c
	$(CC) $(CFLAGSlib) -o $(PATH_TARGET)/timer.o -c $(PATH_HARDWARE)/timer.c
	$(CC) $(CFLAGSlib) -o $(PATH_TARGET)/timer4.o -c $(PATH_HARDWARE)/timer4.c
	

app.a:
	$(AR) cr $(PATH_TARGET)/app.a $(PATH_TARGET)/startup.o \
		$(PATH_TARGET)/main.o \
		$(PATH_TARGET)/led.o \
		$(PATH_TARGET)/timer.o \
		$(PATH_TARGET)/timer4.o
	
clean:
	rm -f $(PATH_TARGET)/*
	
