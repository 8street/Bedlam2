
# Bedlam 2: Absolute Bedlam

[![Lines of Code](https://tokei.rs/b1/github/8street/Bedlam2?category=code)](https://github.com/XAMPPRocky/tokei)
[![CodeFactor](https://www.codefactor.io/repository/github/8street/bedlam2/badge)](https://www.codefactor.io/repository/github/8street/bedlam2)

Source code for Bedlam 2: Absolute Bedlam executable file. To preserve the cultural heritage. Working on modern OS.
Some game mechanics and balance have been overhauled because game was never officially released and was unfinished.

This repository does not contain any original game resources.

---

![Bedlam 2 Preview](https://www.old-games.ru/forum/attachments/bedlam2-png.241732/)

---

# Project State

The sources can be compiled and fully playable in single player mode. All known crashes has been fixed.

# List of changes

* Port to SDL (working on Windows and Linux)
* Add various screen resolutions (game selects a resolution close up to your monitor resolution. From 480p to 1440p)
* Fix overkill robot balance (now similar to Bedlam 1)
* Skip missing levels (some level files seems were never created)
* Fix some bugs

# Contributing

Contributors, welcome. Create a fork of this repository, then create a branch. Modify the source code in your branch. Finally, create a pull request to merge with my main branch.
Patches and improvements are welcome.

# Compiling

### Windows

1. Open solution vs2019/Bedlam2.sln in Visual Studio 2019
2. Compile x86 platform
3. Get original DOS game Bedlam2: Absolute Bedlam
4. Put compiled .exe, SDL2.dll, SDL2_mixer.dll files in your original Bedlam2 folder. Or put original Bedlam2 files in VS output directory
5. Run

You can download executable file from [releases](https://github.com/8street/Bedlam2/releases). You may need to install "Microsoft Visual C++ 2015 Redistributable x86" to run game.

### Linux

Please read [linux/readme.md](https://github.com/8street/Bedlam2/blob/master/linux/readme.md).

You can download binary file from [releases](https://github.com/8street/Bedlam2/releases). Put compiled binary file in your Bedlam2 folder with game files.
You need to install SDL i686 v2.0.12 or above and SDL_mixer i686 to run game. How to do this, see the readme above.

To work save files run "chmod 755 %folder%" for MIRAGE and MIRAGE/AB_BED folders in Bedlam2 directory. This folders will created after first run bedlam2 game.
