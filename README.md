Codec2 for Cortex-M4F
==========
This is some Codec2 tests for ARM Cortex-M4F STM32F4-Discovery, THIS IS ENCODER + MODULATOR ONLY (yet).


To build this project, you need this list of tools:
------------

* [GNU toolchain from ARM Cortex-R & Cortex-M:](https://launchpad.net/gcc-arm-embedded) -- compilator and toolchains
* [Eclipse Indigo with CDT](http://www.eclipse.org/downloads/packages/eclipse-ide-cc-developers-includes-incubating-components/indigosr2) -- development tool
* [GNU ARM Eclipse plugin](http://gnuarmeclipse.livius.net/blog/) -- plug-in for simple cinfiguration of eclipse for ARM microcontrollers.
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
9. By default, firmware just pass-thru microphone samples into output, to enable Codec2+FDMDV just click to user-button (blue one).
10. CMSIS DSP library is used to accelerate encoding.
