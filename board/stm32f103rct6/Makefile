# general Makefile

include Makefile.common

LDFLAGS=$(COMMONFLAGS) -fno-exceptions -ffunction-sections -fdata-sections -L$(LIBDIR) -nostartfiles -Wl,--gc-sections,-Tlinker.ld
LDLIBS+=-lm
LDLIBS+=-lstm32

all: mkdir stm32libs src
	$(CC) -o $(PATH_TARGET)/$(PROGRAM).elf $(LDFLAGS) \
		-Wl,--whole-archive \
		src/$(PATH_TARGET)/app.a \
		-Wl,--no-whole-archive \
		$(LDLIBS)
	$(OBJCOPY) -O ihex $(PATH_TARGET)/$(PROGRAM).elf $(PATH_TARGET)/$(PROGRAM).hex
	$(OBJCOPY) -O binary $(PATH_TARGET)/$(PROGRAM).elf $(PATH_TARGET)/$(PROGRAM).bin
	arm-none-eabi-readelf -a $(PATH_TARGET)/$(PROGRAM).elf > $(PATH_TARGET)/$(PROGRAM).info_elf
	arm-none-eabi-size -d -B -t $(PATH_TARGET)/$(PROGRAM).elf > $(PATH_TARGET)/$(PROGRAM).info_size
	arm-none-eabi-objdump -S $(PATH_TARGET)/$(PROGRAM).elf > $(PATH_TARGET)/$(PROGRAM).info_code
	arm-none-eabi-nm -t d -S --size-sort -s $(PATH_TARGET)/$(PROGRAM).elf > $(PATH_TARGET)/$(PROGRAM).info_symbol

.PHONY: mkdir stm32libs src clean

stm32libs:
	cd stm32libs && make

mkdir:
	mkdir -p $(PATH_TARGET)

src:
	$(MAKE) -C src $@
	
install:
	jlink -commanderscript loadbin_cmd.jlink

clean:
	$(MAKE) -C src $@
	rm -f $(PATH_TARGET)/*

cleanstm32libs:
	cd stm32libs && make clean
