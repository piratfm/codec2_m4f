# Build all libraries

# Check for valid float argument
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
override FLOAT_TYPE = hard
#override FLOAT_TYPE = soft
endif
endif

all:
	$(MAKE) -C StdPeriph FLOAT_TYPE=$(FLOAT_TYPE)
	$(MAKE) -C DSP_Lib FLOAT_TYPE=$(FLOAT_TYPE)
	$(MAKE) -C PDM_filter FLOAT_TYPE=$(FLOAT_TYPE)
	$(MAKE) -C Codec2_Lib FLOAT_TYPE=$(FLOAT_TYPE)

clean:
	$(MAKE) clean -C StdPeriph
	$(MAKE) clean -C DSP_Lib
	$(MAKE) clean -C PDM_filter
	$(MAKE) clean -C Codec2_Lib
