#!/bin/bash

cd o
rm -rf *

echo run g++-8 compiler
g++-8 -m32 -c ../../CPP_Sources/*.cpp -std=c++17 `pkg-config --cflags --libs SDL2_mixer sdl2` -I/usr/include/x86_64-linux-gnu

echo run jwasm compiler
jwasm -elf ../../ASM_Sources/bedlam2.asm

cd ..

echo run ld linker
g++-8 -m32 o/*.o -o bedlam2 -lstdc++fs `pkg-config --cflags --libs SDL2_mixer sdl2`