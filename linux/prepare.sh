#!/bin/bash

sudo apt update
sudo apt install clang-12 -y


git clone https://github.com/JWasm/JWasm.git
cd JWasm 
cmake . 
make
sudo cp jwasm /usr/local/bin 


git clone https://github.com/libsdl-org/SDL
cd SDL
mkdir build
cd build
../configure --host=i686-pc-linux-gnu CFLAGS=-m32 CXXFLAGS=-m32 LDFLAGS=-m32
make
sudo make install


git clone https://github.com/libsdl-org/SDL_mixer
cd SDL_mixer
mkdir build
cd build
../configure --host=i686-pc-linux-gnu CFLAGS=-m32 CXXFLAGS=-m32 LDFLAGS=-m32
make
sudo make install