#!/bin/bash

sudo apt update

echo                                          
echo ============= setup compiler =============
sudo apt install clang-12 -y
sudo apt install libc6-dev-i386 -y
sudo apt install gcc-multilib -y
sudo apt install g++-multilib -y

echo                                          
echo ============= setup JWasm =============
git clone https://github.com/JWasm/JWasm.git
cd JWasm || exit 1
cmake .
make
sudo cp jwasm /usr/local/bin
cd ..

echo                                          
echo ============= setup SDL2 =============
git clone https://github.com/libsdl-org/SDL
cd SDL || exit 2
mkdir build
cd build || exit 2
../configure --host=i686-pc-linux-gnu CFLAGS=-m32 CXXFLAGS=-m32 LDFLAGS=-m32
make
sudo make install
cd ..
cd ..

echo                                          
echo ============= setup SDL_mixer =============
git clone https://github.com/libsdl-org/SDL_mixer
cd SDL_mixer || exit 3
mkdir build
cd build || exit 3
../configure --host=i686-pc-linux-gnu CFLAGS=-m32 CXXFLAGS=-m32 LDFLAGS=-m32
make
sudo make install
cd ..
cd ..
