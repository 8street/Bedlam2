#!/bin/bash

cd o
#rm -rf *
#echo run g++-8
#g++-8 -c ../../CPP_Sources/*.cpp -std=c++17 `sdl2-config --cflags --libs` -lSDL_mixer
cd ..
echo run ld
g++-8 o/*.o -o bedlam2 -lstdc++fs `sdl2-config --cflags --libs` -lSDL2_mixer 
#-L../../SDL2/lib/x86
