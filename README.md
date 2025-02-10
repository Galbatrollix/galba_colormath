# galba_colormath
Galba colormath is a lightweight C language library for color conversions and distances. It offers conversions between RGB, XYZ and LAB colorspaces and computation of the following lab color distances: delta CIE76, delta CIE94, delta CMC and delta CIEDE2000. Aside from that, the library offers simple conversion functions allowing for easier handling of color hexstrings and RGB values. Full details on all provided functions can be found [here](https://github.com/Galbatrollix/galba_colormath/blob/master/DOCUMENTATION.md#functions).

### Language version and dependencies
Galba colormath is written in a C99 style and doesn't use specific compiler extensions. Hence the code should compile correctly with any C99 (or later) conformant compiler. The only dependency in the library is C standard library. Details on required libc includes and functions can be found [here](https://github.com/Galbatrollix/galba_colormath/blob/master/DOCUMENTATION.md#dependencies).

All functions provided in this library are completely strict-aliasing-safe and thread-safe. The code was developed with an assumption that it will be run on at most -O3 optimization level (gcc). If you plan on enabling fast-math or equivalent optimizations in your compiler, throughoutly test whether functions yield acceptable results. 

### Installation and usage
In order to use the provided library all that is necessary is to move contents of galba colormath ```includes``` folder into the folder where your project holds common .h files and move contents of galba colormath ```src``` folder to your project's source folder. Provided .c files look for the header directly by saying ```#include "galba_colormath.h"```. As the library is extremely simple, no build script is provided at the moment. The user can arrange the files according to their project structure as they see fit.

Once the galba colormath files have been placed in the project and are visible to the compiler, the user should be able to access all functions declared in the header by saying ```#include "galba_colormath.h"``` 

### Running tests 
The repository also contains a test suite for almost all defined functions. (Some extremely simple functions are omitted) More details on the test suite [here](https://github.com/Galbatrollix/galba_colormath/blob/master/DOCUMENTATION.md#test-suite). 

All test-related files in the repository are located in ```tests``` folder. Using them follows the same logic as using the library itself. However, repository comes with a ```main.c``` file at the root level that runs the complete test suite. 

There is also a convinience ```makefile``` at root directory which allows to quickly run the ```main.c``` file on either mingw GCC or linux GCC directly from the repository project structure. To compile the project on linux, just type make in the project's root. (make sure gcc and make is installed). If using mingw, uncomment the relevant sections of makefile first and then type make in cmd in project's root. 
