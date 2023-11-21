# cpp_flappydoom

> [!NOTE]  
> This is pre-alpha stage game. There _are_ bugs. There will be performance issues and the game will slow down if you enable vsync or suffer a low framerate.

I have created my own version of FlappyDOOM in C++, inspired from the YouTube video [Doom styles Flappy Bird in Python][flappy-doom-py].

This project uses the [olcPixelGameEngine][pge] from [One Lone Coder (@javidx9)][yt].

This game has been made with the OLC 

See: [Linux DOOM][linux-doom].

Included libraries:

* [Mini Audio][ln-ma]
* [STB Image][ln-stb]
* [olcPGEX Font][ln-pgex-font]
* [olcPGEX MiniAudio][ln-pgex-ma]
* [olcPixelGameEngine][ln-pge]

### How to build
This project uses CMake for cross-platform support. Load the project into your favourite IDE and build!

From the CLI:

```bash
# compile it
cmake --build <build_path> --config Debug --target ALL_BUILD -j 14

# run it
build/bin/main<.ext>
```

The game has been tested on Linux, OSX & Windows 10. Make sure you bundle the assets along with the executable when compressing.

This game is _not_ optimized to run with [OLC Pixel Game Engine Mobile][olc-pgex-mobile]

### How to play
The game is composed of runs. A run is an attempt to navigate the gates. There are 5 difficulty levels, that affect the score the player receives for passing through a gate, and the speed the game plays at.

* `SPACEBAR` to start a new run and jump Flappy DOOM
* `ESCAPE` to reset the current run
* `ENTER` to pause the current run
* `Q` to quit the game

Check `definitions.h` for enabling debug mode and full screen. Debug mode can be enabled by changing the bool `ENABLE_DEBUG_MODE` to `true` in `Settings.h`

Additional controls available during gameplay:

* `D` to enter wireframe mode
* `P` to clamp the difficulty mode
* `O` to increase difficulty
* `I` to decrease difficulty
* `Y` to increase game speed step by difficulty 0.25f
* `T` to decrease game speed step by difficulty 0.25f
* `G` to increase pipe gap distance by 5.0f, capped at `BIRD_SH`
* `H` to decrease pipe gap distance by 5.0f, capped at `PIPE_GAP * 2`


[flappy-doom-py]: https://www.youtube.com/watch?v=HISF5Ddx7y4
[linux-doom]: https://github.com/id-Software/DOOM
[pge]: https://github.com/OneLoneCoder/olcPixelGameEngine/
[yt]: https://www.youtube.com/@javidx9
[olc-pgex-mobile]: https://github.com/Johnnyg63/OLCPGEMobileAndroidStudio
[ln-ma]: https://miniaud.io/
[ln-stb]: https://github.com/nothings/stb/blob/master/stb_image.h
[ln-pgex-font]: https://github.com/gorbit99/OLC-Font
[ln-pgex-ma]: https://github.com/Moros1138/olcPGEX_MiniAudio
[ln-pge]: https://github.com/OneLoneCoder/olcPixelGameEngine