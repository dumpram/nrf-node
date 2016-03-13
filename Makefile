# Put your stlink folder here so make burn will work.
STLINK=~/arm-workspace/stlink

# Put your source files here (or *.c, etc)
SRCS=src/*.cpp

# Put your inc files here (or *.h, etc)
INCS=inc/

# Binaries will be generated with this name (.elf, .bin, .hex, etc)
PROJ_NAME=nrf-node

# Put your STM32F4 library code directory here
STM_COMMON=lib/stm32

# Standard peripheral library
STD_PERIPH_LIB=$(STM_COMMON)/STM32F10x_StdPeriph_Driver

# Linker script
STM_LD=stm32f103_flash.ld

# Standard peripheral sources
STD_PERIPH_SRC = $(STD_PERIPH_LIB)/src/stm32f10x_spi.c
STD_PERIPH_SRC += $(STD_PERIPH_LIB)/src/stm32f10x_tim.c
STD_PERIPH_SRC += $(STD_PERIPH_LIB)/src/stm32f10x_gpio.c
STD_PERIPH_SRC += $(STD_PERIPH_LIB)/src/stm32f10x_rcc.c
STD_PERIPH_SRC += $(STD_PERIPH_LIB)/src/stm32f10x_usart.c
# Normally you shouldn't need to change anything below this line!
################################################################################

CC=arm-none-eabi-g++
OBJCOPY=arm-none-eabi-objcopy

# Compiler flags
CFLAGS  = -g -O2 -Wall -T$(STM_LD) -std=c++11 --specs=nosys.specs
CFLAGS += -D USE_STDPERIPH_DRIVER -D SERIAL_DEBUG -D STM32F10X_LD
CFLAGS += -mcpu=cortex-m3 -mthumb

# User includes
CFLAGS += -I$(INCS)

# Include files from STM libraries
CFLAGS += -I$(STM_COMMON)/CMSIS/CM3/DeviceSupport/ST/STM32F10x
CFLAGS += -I$(STM_COMMON)/CMSIS/CM3/CoreSupport

# Include from standard peripheral library
CFLAGS += -I$(STD_PERIPH_LIB)/inc

# Size optimization
CFLAGS+=-fno-exceptions
CFLAGS+=-fno-builtin
CFLAGS+=-flto
CFLAGS+=-fno-rtti
CFLAGS+=--specs=nano.specs

# add startup file to build
SRCS += startup_stm32f10x_md.s

# Standard peripheral library src
SRCS += $(STD_PERIPH_SRC)

.PHONY: proj

all: proj

proj: $(PROJ_NAME).elf

$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@
	$(OBJCOPY) -O ihex $(PROJ_NAME).elf $(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(PROJ_NAME).elf $(PROJ_NAME).bin

clean:
	rm -f *.o $(PROJ_NAME).elf $(PROJ_NAME).hex $(PROJ_NAME).bin

# Flash the STM32
burn: proj
	$(STLINK)/st-flash write $(PROJ_NAME).bin 0x8000000
