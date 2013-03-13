BINPATH=~/sat/bin

AS=$(BINPATH)/arm-none-eabi-as
AR=$(BINPATH)/arm-none-eabi-ar
CC=$(BINPATH)/arm-none-eabi-gcc

###################################################

# Check for valid float argument
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
override FLOAT_TYPE = hard
#override FLOAT_TYPE = soft
endif
endif

###################################################

CFLAGS  = -g -O2 -Wall
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

###################################################

# Sources
SRCS_AS = pdm_filter.S
SRCS_C = pdm_fir.c



OBJS_AS = $(SRCS_AS:.S=.o)
OBJS_C = $(SRCS_C:.c=.o)
LIBNAME = libpdm_filter.a

all: $(LIBNAME)

$(OBJS_AS) : $(SRCS_AS)
	$(AS) -o $@ $<

$(OBJS_C) : $(SRCS_C)
	$(CC) $(CFLAGS) -c -o $@ $^

$(LIBNAME): $(OBJS_AS) $(OBJS_C)
	$(AR) -r $@ $^

clean:
	rm -f $(OBJS_C) $(OBJS_AS) $(LIBNAME)
