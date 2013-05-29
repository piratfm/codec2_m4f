Codec2 for Cortex-M4F
==========
This is some Codec2 tests for ARM Cortex-M4F STM32F4-Discovery, THIS IS ENCODER + MODULATOR ONLY (yet).
The codec2-dev branch also contains stm32 port of codec2: https://freetel.svn.sourceforge.net/svnroot/freetel/codec2-dev/stm32


To build this project, you need this list of tools:
------------

* [GNU toolchain from ARM Cortex-R & Cortex-M:](https://launchpad.net/gcc-arm-embedded) -- compilator and toolchains
* [ST-LINK utils](http://github.com/texane/stlink) -- Debugger & programmer software.

Some info about implementation:
------------

1. The existant code implements only encoder and modulator.
2. 8kHz -> 48 kHz resampler may be used for converting FDMDV signal (turned on by default).
3. Floating-Point subprocessor is used where possible.
4. The open-source realization of PDM filter is used, the cutoff frequency - is 3,6 kHz.
5. Microphone sensitivity setted to minimum for preventing noise.
6. You can connect some 5/3.3 V terminal RX pin to the board's PA2 pin for receiving various debug messages. The terminal speed is 115200 8N1.
7. There is 6x FIFO Queue between encoder and modulator, it needs because sometimes one 320-sample frame takes longer than real time, this adds delay up to ~250ms.
8. Application also used additional 64k memory (so-called core-coupled memory).
9. By default, firmware just pass-thru microphone samples into output. To enable Codec2+FDMDV - click to user-button (blue one).
10. CMSIS DSP library is used to accelerate encoding.
11. Rightnow Codec2 core is old, to correctly decode you need svn rev. 416 of the Codec2.
