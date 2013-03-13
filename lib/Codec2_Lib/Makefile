BINPATH=~/sat/bin

CC=$(BINPATH)/arm-none-eabi-gcc
AR=$(BINPATH)/arm-none-eabi-ar

###################################################

# Check for valid float argument
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
override FLOAT_TYPE = hard
#override FLOAT_TYPE = soft
endif
endif

###################################################

vpath %.c codec2/codebooks codec2 fdmdv math_helpers

#CFLAGS  = -g -O2 -Wall
CFLAGS  = -g -O3 -Wall
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m4 -fno-strict-aliasing
CFLAGS += -DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DARM_MATH_ROUNDING -DUNALIGNED_SUPPORT_DISABLE

ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -D__FPU_PRESENT="1" -D__FPU_USED="1" -D__VFP_FP__
else
CFLAGS += -msoft-float
endif

CFLAGS += -ffreestanding

# Includes
CFLAGS += -Iinc -I../Core/cmsis -I../Core/stm32 -I../DSP_Lib/inc
CFLAGS += -Icodec2 -Ifdmdv

DSP_LIB = ../DSP_Lib/libarm_math.a

# Sources

#math helpers:
SRCS = arm_2copy_f32.c  arm_2mult_f32.c  arm_add_interleave_f32.c  arm_cmplx_ersq_f32.c  arm_cmplx_pwr_f32.c  arm_mav_f32.c  arm_srsq_f32.c

#modulator:
SRCS += fdmdv.c

#voice codebooks:
SRCS += \
codebook.c \
codebookd.c \
codebookdt.c \
codebookge.c \
codebookjnd.c \
codebookjvm.c \
codebookres.c \
codebookvq.c

#voice coding:
SRCS += interp.c \
lpc.c \
lsp.c \
nlp.c \
pack.c \
codec2.c \
dump.c \
fft.c \
glottal.c \
phase.c \
postfilter.c \
quantise.c \
sine.c

#core library:
SRCS += codec2_core.c
#SRCS += varicode.c




OBJS = $(SRCS:.c=.o)
LIBNAME = libcodec2m.a

all: $(LIBNAME)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $^ 

$(LIBNAME): $(OBJS)
	#hack for joining two static libs togather
	#cp $(DSP_LIB) $@
	#end of hack
	$(AR) -r $@ $(OBJS) 

clean:
	rm -f $(OBJS) $(LIBNAME)
	