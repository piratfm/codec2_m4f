
# Sources
SRCS = main.c stm32f4xx_it.c system_stm32f4xx.c newlib_stubs.c 


# Discovery board specific files
SRCS += stm32f4_discovery.c stm32f4_discovery_audio_codec.c stm32f4_discovery_audio_mic.c

# Project name

PROJ_NAME=codec2_m4f
OUTPATH=build

###################################################

# Check for valid float argument
# NOTE that you have to run make clan after
# changing these as hardfloat and softfloat are not
# binary compatible
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
override FLOAT_TYPE = hard
#override FLOAT_TYPE = soft
endif
endif

###################################################

#########################
# Defines from eclipse
#########################
#STM32F4XX
#USE_STDPERIPH_DRIVER
#MEDIA_USB_KEY
#ARM_MATH_CM4
#ARM_MATH_MATRIX_CHECK
#ARM_MATH_ROUNDING
#__FPU_PRESENT=1
#__FPU_USED=1
#__VFP_FP__
#UNALIGNED_SUPPORT_DISABLE

#########################
# UNDefines from eclipse
#########################
#__SOFTFP__


BINPATH=~/sat/bin
CC=$(BINPATH)/arm-none-eabi-gcc
OBJCOPY=$(BINPATH)/arm-none-eabi-objcopy
SIZE=$(BINPATH)/arm-none-eabi-size
FLASHPROG=$(BINPATH)/st-flash

CFLAGS  = -std=gnu99 -g -O2 -Wall -Tstm32_flash.ld
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -nostartfiles -mcpu=cortex-m4

CFLAGS += -fno-strict-aliasing -ffunction-sections
CFLAGS += -DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -DUNALIGNED_SUPPORT_DISABLE

ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -D__FPU_PRESENT="1" -D__FPU_USED="1" -D__VFP_FP__
else
CFLAGS += -msoft-float
endif

###################################################

vpath %.c src
vpath %.a lib

ROOT=$(shell pwd)

# Includes
CFLAGS += -Iinc -Ilib/Core/cmsis -Ilib/Core/stm32

# Flags
CFLAGS += -DUSE_STDPERIPH_DRIVER

# Library paths
LIBPATHS = -Llib/StdPeriph
LIBPATHS += -Llib/PDM_filter
LIBPATHS += -Llib/DSP_Lib
LIBPATHS += -Llib/Codec2_Lib

# Libraries to link
LIBS = -lm -lstdperiph -lpdm_filter -larm_math -lcodec2m

# Extra includes
CFLAGS += -Ilib/StdPeriph/inc
CFLAGS += -Ilib/PDM_filter
CFLAGS += -Ilib/DSP_Lib/inc
CFLAGS += -Ilib/Codec2_Lib/inc

# add startup file to build
SRCS += lib/startup_stm32f4xx.S

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj flash

all: lib proj
	$(SIZE) $(OUTPATH)/$(PROJ_NAME).elf

lib:
	$(MAKE) -C lib FLOAT_TYPE=$(FLOAT_TYPE)

proj: 	$(OUTPATH)/$(PROJ_NAME).elf

$(OUTPATH)/$(PROJ_NAME).elf: $(SRCS)
	mkdir $(OUTPATH)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBPATHS) $(LIBS)
	$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin

clean:
	rm -f *.o
	rm -f $(OUTPATH)/$(PROJ_NAME).elf
	rm -f $(OUTPATH)/$(PROJ_NAME).hex
	rm -f $(OUTPATH)/$(PROJ_NAME).bin
	$(MAKE) clean -C lib # Remove this line if you don't want to clean the libs as well
	
flash: proj
	$(FLASHPROG) write $(OUTPATH)/$(PROJ_NAME).bin 0x8000000

