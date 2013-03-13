#!/bin/bash

arecord -c 1 -t raw -f S16_LE -r 48000 - | \
sox -r 48k -e signed -b 16 -c 1 -t raw - -r 8k -t raw - | \
~/codec2-dev-r416/src/fdmdv_demod - - | \
~/codec2-dev-r416/src/c2dec 1400 - - | \
aplay -c 1 -t raw -f S16_LE -r 8000 -
