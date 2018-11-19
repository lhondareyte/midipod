#
#


MCU          	= atmega8u2
ARCH         	= AVR8
F_CPU        	= 16000000
HZ		= $(F_CPU)
F_USB        	= $(F_CPU)
OPTIMIZATION 	= s
TARGET       	= midipod
CCPATH          :=/usr/local/CrossPack-AVR/bin
CC              :=$(CCPATH)/avr-gcc
OBJCOPY         :=$(CCPATH)/avr-objcopy

SRC          	= $(wildcard *.c) $(LUFA_SRC_USB) $(LUFA_SRC_USBCLASS)
LUFA_PATH    	= ./LUFA
CC_FLAGS     	=  -DUSE_LUFA_CONFIG_HEADER -IConfig/ 
LD_FLAGS     	= 

# Default target
all:

# Include LUFA build script makefiles
include $(LUFA_PATH)/Build/lufa_core.mk
include $(LUFA_PATH)/Build/lufa_sources.mk
include $(LUFA_PATH)/Build/lufa_build.mk
include $(LUFA_PATH)/Build/lufa_cppcheck.mk
include $(LUFA_PATH)/Build/lufa_doxygen.mk
include $(LUFA_PATH)/Build/lufa_dfu.mk
include $(LUFA_PATH)/Build/lufa_hid.mk
include $(LUFA_PATH)/Build/lufa_avrdude.mk
include $(LUFA_PATH)/Build/lufa_atprogram.mk

load: all
	@avrdude -q -p $(MCU) -c stk500v2 -P /dev/cu.usbmodem2221 -i 5 -U flash:w:$(TARGET).hex

bin: all
	@printf "Generating $(FIRMWARE).bin ..."
	@$(OBJCOPY) -R .eeprom -O binary $(TARGET).elf  \
                $(TARGET).bin
	@echo "done."

