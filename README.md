# cpp_flappydoom

> [!NOTE]  
> This is pre-alpha stage game. There may be performance issues when your frame rate drops too low.

I have created my own version of FlappyDOOM in C++, inspired from the YouTube video [Doom styles Flappy Bird in Python][flappy-doom-py].

This project uses the [olcPixelGameEngine][pge] from [One Lone Coder (@javidx9)][yt].

This game has been made with the OLC 

See: [Linux DOOM][linux-doom].

### How to build
This project uses CMake for cross-platform support. Load the project into your favourite IDE and build!

From the CLI:

```bash
# compile it
cmake --build <build_path> --config Debug --target ALL_BUILD -j 14

# run it
build/bin/main<.ext>
```

The game has been tested on OSX & Windows 10. Make sure you bundle the assets along with the executable when compressing.

### How to play
The game is composed of runs. A run is an attempt to navigate the gates. There are 5 difficulty levels, that affect the score the player receives for passing through a gate, and the speed the game plays at.

* `SPACEBAR` to start a new run and jump Flappy DOOM
* `ESCAPE` to reset the current run
* `ENTER` to pause the current run
* `Q` to quit the game

Check `definitions.h` for enabling debug mode and full screen.

[flappy-doom-py]: https://www.youtube.com/watch?v=HISF5Ddx7y4
[linux-doom]: https://github.com/id-Software/DOOM
[pge]: https://github.com/OneLoneCoder/olcPixelGameEngine/
[yt]: https://www.youtube.com/@javidx9