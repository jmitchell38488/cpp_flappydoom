# Todo list

### Splash screen

- [x] Flappy bird from Right collides with `DOOM` text, death sound on impact, falls to ground

### Game menu

- [x] `SPLASHSCREEN` only displays on game startup and cannot be navigated to
- [x] Implement screen `MAINSCREEN`
- [x] Background for `MAINSCREEN` should be translucent black over the game screen
- [x] Add menu option rendering to be x centered
- [x] `MAINSCREEN` should have options (`New game`, `High scores` (if games played), `Quit`)
- [x] `DIFFICULTYSELECT` should have options `EASY`,`MEDIUM`,`HARD`,`NIGHTMARE`,`IMPOSSIBLE`,`DYNAMIC`, where `DYNAMIC` increases difficulty by game progression
- [x] `DIFFICULTYSELECT` should have the option `Back` to return to the `MAINMENU`
- [x] `RESUME` should have the options `Resume game`, `High scores`, `Back to menu`, `Quit`
- [x] `HIGHSCORES` should have the options `Back`, which can return to the origin of the page either `RESUME` or `MAINSCREEN`
- [x] `DIFFICULTYSELECT` should render over the top of `MAINSCREEN` background
- [x] `RESUME` should render over the top of `MAINSCREEN` background
- [x] `HIGHSCORES` should render over the top of `MAINSCREEN` background
- [x] `HIGHSCORES` should display the top 10 runs with `Score`, `Run` & `Difficulty`

### Game menu navigation

- [x] ~~Esc should navigate backwards from the current screen~~ Esc highlights the back option
- [x] When a user presses escape while playing the game, the `RESUME` screen should display
- [x] Add handling for keys to navigate and select menu items (up/down/enter), enter should move forward in screens
- [x] The first option in a menu should always be active
- [x] When a menu item is active, change the display state, perhaps add decal on either side to show it is selected
- [x] Screen navigation is `SPLASHSCREEN` -> `MAINSCREEN` -> `DIFFICULTYSELECT` -> Start game
- [X] Navigation: `MAINSCREEN` -> `HIGHSCORES`
- [x] Navigation: `MAINSCREEN` -> `DIFFICULTYSELECT`
- [x] Navigation: `MAINSCREEN` -> `QUIT` - should quit the game
- [x] Navigation: `HIGHSCORES` -> `MAINMENU`
- [x] Navigation: `HIGHSCORES` -> `RESUME`
- [x] Navigation `RESUME` -> `HIGHSCORES`
- [x] Navigation `RESUME` -> `MAINSCREEN`
- [x] Navigation `RESUME` -> `QUIT`
- [x] Navigation `DIFFICULTYSELECT` -> `GAME`
- [x] Navigation `DIFFICULTYSELECT` -> `MAINMENU`


### Graphics

- [ ] Build fire animation from CoderSpace example

### Misc
