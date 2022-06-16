#!/bin/sh

umask 022

emcc -DCHPEG_PACKRAT -DCHPEG_EXTENSION_ALL \
    -std=c99 -O3 -Wall -Wno-unused-function -pedantic \
    -sEXPORTED_FUNCTIONS=_parse -sEXPORTED_RUNTIME_METHODS=ccall,cwrap \
    chpeg-playground.c -o chpeg-playground.js

# --pre-js chpeg-pre.js \
# -g2 -gsource-map --source-map-base='./'
