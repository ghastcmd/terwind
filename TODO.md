# TODO for next patch

- [x] hot reloading for render obj
> How to do it?

Search for videos on hot reloading and try to implement it
on the 'render.c' obj.

> Made a dynamic library loading.
> Now just need to compile the source code with button press.

- [x] Make hot reloading to the entire codebase;
- [ ] (*) Define debug onscreen;
  - [ ] Needs to define sub window;
  - [ ] Scrolling sub window;
- [x] (**) Dependency makefile;
- [ ] Mess with assembly;
- [ ] Make another line raster.


## Future plans to mess with in code

- [ ] Native tool profiling
- [ ] Socket communication
- [ ] Video codificator (print economy)

- [ ] Simple interpreter and syntax error message


## Defining debug window inside the application

The window is with set width and height.
You can scroll sideways per line.
You can scroll to see other lines, but this only
when the total line height is overflown.