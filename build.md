### Dependencies:
- gcc (linux) / mingw-w64 (windows)
- cmake

Project will *just compile* because all dependencies are included or precompiled (glfw)

### Building:
**On linux**:
Install gcc cmake and make `cmake -S -B && make`

**On windows**:
I recommend just using mingw-w64 bundled with Clion. Otherwise using mingw-w64 is painfull



### Other notes:
Sometimes (mainly in game_loop.c) code might look chaotic. 
This is because it is debug code and is not final code.
It is meant to be easy to write so I can figure out what will I need to build a good abstraction.