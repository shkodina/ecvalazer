###############################################################################
# Makefile for the project ecva_chan_01
###############################################################################

## General Flags
PROJECT = ecva_chan_01
MCU = atmega8
TARGET = ecva_chan_01.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=ecva_chan_01.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = ecva_chan_01.o fftest.o ffft.o uart_logger.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) ecva_chan_01.hex ecva_chan_01.eep ecva_chan_01.lss size

## Compile
ecva_chan_01.o: ../ecva_chan_01.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

fftest.o: ../fftest.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ffft.o: ../ffft.S
	$(CC) $(INCLUDES) $(ASMFLAGS) -c  $<

uart_logger.o: ../uart_logger.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) ecva_chan_01.elf dep/* ecva_chan_01.hex ecva_chan_01.eep ecva_chan_01.lss ecva_chan_01.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

