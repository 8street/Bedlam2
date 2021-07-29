# Linux compile

I have compiled the game, but I have not tested it to work.

### Dependencies

You need:

1. Clang compiler

'''
sudo apt update
sudo apt upgrade
sudo apt install clang-12
'''

2. Install assembly compiler JWasm that understands masm syntax

'''
git clone https://github.com/JWasm/JWasm.git 
cd JWasm 
cmake . 
make
cp jwasm /usr/local/bin 
'''

3. Instal x86 lib SDL2 not lower than version SDL 2.0.12. You may compile it yourself

'''
git clone https://github.com/libsdl-org/SDL
cd SDL
mkdir build
cd build
../configure --host=i686-pc-linux-gnu CFLAGS=-m32 CXXFLAGS=-m32 LDFLAGS=-m32
make
sudo make install
'''

4. Instal x86 lib SDL2_mixer. You may compile it yourself

'''
git clone https://github.com/libsdl-org/SDL_mixer
cd SDL_mixer
mkdir build
cd build
../configure --host=i686-pc-linux-gnu CFLAGS=-m32 CXXFLAGS=-m32 LDFLAGS=-m32
make
sudo make install
'''

5. Run build script from this_repo/linux folder 

'''
git clone https://github.com/8street/Bedlam2
cd Bedlam2/linux
./compile.sh
'''

### Issues

There are several problems here to run Bedlam2 game on linux. 

* All file paths in Windows transcription. It use "\\" separator
* All file path use install drive letter, eg. "C"
* Bedlam2.asm use platform dependent direct.h header for mkdir, getcwd etc.
* Something else

Please, if you have fixed any issue, do not hesitate to contribute.