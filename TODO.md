# Todo list

### Splash screen

- [ ] Flappy bird from Right collides with `DOOM` text, death sound on impact, falls to ground

### Game menu

- [ ] `SPLASHSCREEN` only displays on game startup and cannot be navigated to
- [ ] Implement screen `MAINSCREEN`
- [ ] Background for `MAINSCREEN` should be translucent black over the game screen
- [ ] Add menu option rendering to be x centered
- [ ] `MAINSCREEN` should have options (`New game`, `High scores` (if games played), `Quit`)
- [ ] `DIFFICULTYSELECT` should have options `EASY`,`MEDIUM`,`HARD`,`NIGHTMARE`,`IMPOSSIBLE`,`DYNAMIC`, where `DYNAMIC` increases difficulty by game progression
- [ ] `DIFFICULTYSELECT` should have the option `Back` to return to the `MAINMENU`
- [ ] `RESUME` should have the options `Resume game`, `High scores`, `Back to menu`, `Quit`
- [ ] `HIGHSCORES` should have the options `Back`, which can return to the origin of the page either `RESUME` or `MAINSCREEN`
- [ ] `DIFFICULTYSELECT` should render over the top of `MAINSCREEN` background
- [ ] `RESUME` should render over the top of `MAINSCREEN` background
- [ ] `HIGHSCORES` should render over the top of `MAINSCREEN` background
- [ ] `HIGHSCORES` should display the top 10 runs with `Score`, `Run` & `Difficulty`

### Game menu navigation

- [ ] Esc should navigate backwards from the current screen
- [ ] When a user presses escape while playing the game, the `RESUME` screen should display
- [ ] Add handling for keys to navigate and select menu items (up/down/enter), enter should move forward in screens
- [ ] The first option in a menu should always be active
- [ ] When a menu item is active, change the display state, perhaps add decal on either side to show it is selected
- [ ] Screen navigation is `SPLASHSCREEN` -> `MAINSCREEN` -> `DIFFICULTYSELECT` -> Start game
- [ ] Navigation: `MAINSCREEN` -> `HIGHSCORES`
- [ ] Navigation: `MAINSCREEN` -> `DIFFICULTYSELECT`
- [ ] Navigation: `MAINSCREEN` -> `QUIT` - should quit the game
- [ ] Navigation: `HIGHSCORES` -> `MAINMENU`
- [ ] Navigation: `HIGHSCORES` -> `RESUME`
- [ ] Navigation `RESUME` -> `HIGHSCORES`
- [ ] Navigation `RESUME` -> `MAINSCREEN`
- [ ] Navigation `RESUME` -> `QUIT`
- [ ] Navigation `DIFFICULTYSELECT` -> `GAME`
- [ ] Navigation `DIFFICULTYSELECT` -> `MAINMENU`


### Graphics

- [ ] Build fire animation from CoderSpace example

### Misc
