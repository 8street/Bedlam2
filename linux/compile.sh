#!/bin/bash

mkdir o
cd o
rm -rf *

echo ============= run clang++-12 compiler =============
clang++-12 -m32 -c ../../CPP_Sources/*.cpp -std=c++17 `pkg-config --cflags SDL2_mixer sdl2`

echo ============= run jwasm compiler =============
jwasm -elf -c -zcw ../../ASM_Sources/bedlam2.asm

cd ..

echo ============= run ld linker =============
clang++-12 -m32 o/*.o -o bedlam2 -lstdc++fs `pkg-config --libs SDL2_mixer sdl2`

chmod 755 bedlam2 
