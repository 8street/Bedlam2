# Linux compile

You can download binary file from CI artefacts. Dependencies for run: SDL2 i686, SDL_mixer i686.

### Dependencies for compilation

You need:

1. Clang compiler

```bash
sudo apt update
sudo apt install clang-12
```

2. Some cross platform libs

```bash
sudo apt install libc6-dev-i386
sudo apt install gcc-multilib
sudo apt install g++-multilib
```

3. Install assembly compiler JWasm that understands masm syntax

```bash
git clone https://github.com/JWasm/JWasm.git 
cd JWasm 
cmake . 
make
sudo cp jwasm /usr/local/bin 
```

4. Install x86 lib SDL2 not lower than version SDL 2.0.12. You may compile it yourself

```bash
git clone https://github.com/libsdl-org/SDL
cd SDL
mkdir build
cd build
../configure --host=i686-pc-linux-gnu CFLAGS=-m32 CXXFLAGS=-m32 LDFLAGS=-m32
make
sudo make install
```

5. Install x86 lib SDL2_mixer. You may compile it yourself

```bash
git clone https://github.com/libsdl-org/SDL_mixer
cd SDL_mixer
mkdir build
cd build
../configure --host=i686-pc-linux-gnu CFLAGS=-m32 CXXFLAGS=-m32 LDFLAGS=-m32
make
sudo make install
```

### Compile

To compile run build script from Bedlam2/linux folder 

```bash
git clone https://github.com/8street/Bedlam2
cd Bedlam2/linux
chmod +x compile.sh
./compile.sh
```
