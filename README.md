
# Bedlam 2: Absolute Bedlam

[![Lines of Code](https://tokei.rs/b1/github/8street/Bedlam2?category=code)](https://github.com/XAMPPRocky/tokei)

Source code for Bedlam 2: Absolute Bedlam executable file. To preserve the cultural heritage. Working on modern OS.
Some game mechanics have been overhauled since game was never officially released and was unfinished.

This repository does not contain any original game resources.

---

![Bedlam 2 Preview](https://www.old-games.ru/forum/attachments/bedlam2-png.241732/)

---

# Project State

The sources can be compiled and fully playable in single player mode. All known crashes has been fixed.

# List of changes

* Port to SDL (working on Windows. Linux has some issues)
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
3. Put compiled .exe, SDL2.dll, SDL2_mixer.dll files in your Bedlam2 folder. Or put Bedlam2 files in VS output directory
4. Run

You can download executable file from [releases](https://github.com/8street/Bedlam2/releases). You may need to install "Microsoft Visual C++ 2015 Redistributable x86" to run game.

### Linux

Please read [linux/readme.md](https://github.com/8street/Bedlam2/blob/master/linux/readme.md).