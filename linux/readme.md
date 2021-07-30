# Linux compile

I have compiled the game, but I have not tested it to work.

### Dependencies

You need:

1. Clang compiler

```bash
sudo apt update
sudo apt install clang-12
```

2. Install assembly compiler JWasm that understands masm syntax

```bash
git clone https://github.com/JWasm/JWasm.git 
cd JWasm 
cmake . 
make
sudo cp jwasm /usr/local/bin 
```

3. Install x86 lib SDL2 not lower than version SDL 2.0.12. You may compile it yourself

```bash
git clone https://github.com/libsdl-org/SDL
cd SDL
mkdir build
cd build
../configure --host=i686-pc-linux-gnu CFLAGS=-m32 CXXFLAGS=-m32 LDFLAGS=-m32
make
sudo make install
```

4. Install x86 lib SDL2_mixer. You may compile it yourself

```bash
git clone https://github.com/libsdl-org/SDL_mixer
cd SDL_mixer
mkdir build
cd build
../configure --host=i686-pc-linux-gnu CFLAGS=-m32 CXXFLAGS=-m32 LDFLAGS=-m32
make
sudo make install
```

# Compile

To compile run build script from Bedlam2/linux folder 

```bash
git clone https://github.com/8street/Bedlam2
cd Bedlam2/linux
./compile.sh
```

### Issues

There are several problems here to run Bedlam2 game on linux. 

* All file paths in Windows transcription. It use "\\" separator
* All file path use install drive letter, eg. "C"
* Bedlam2.asm use platform dependent direct.h header for mkdir, getcwd etc.
* Something else

Please, if you have fixed any issue, do not hesitate to contribute.