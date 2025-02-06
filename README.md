# galba_colormath
Galba colormath is a lightweight C language library for color conversions and distances. It offers conversions between RGB, XYZ and LAB colorspaces and computation of the following lab color distances: delta CIE76, delta CIE94, delta CMC and delta CIEDE2000. Aside from that, the library offers simple conversion functions allowing for easier handling of color hexstrings and RGB values. Full details on all provided functions can be found (link this) here.

The repository also contains a test suite for almost all defined functions. (Some extremely simple functions are omitted) More details on the test suite (link this) here.

### Language version and dependencies
Galba colormath is written in a C99 style and doesn't use specific compiler extensions. Hence the code should compile correctly with any C99 (or later) conformant compiler. The only dependency in the library is C standard library. Details on required libc includes and functions can be found (link this) here.

All functions provided in this library are completely strict-aliasing-safe and thread-safe. The code was developed with an assumption that it will be run on at most -O3 optimization level (gcc). If you plan on enabling fast-math or equivalent optimizations in your compiler, throughoutly test whether functions yield acceptable results. 

### Installation and usage
In order to use the provided library all that is necessary is to move contents of galba colormath ```includes``` folder into the folder where your project holds common .h files and move contents of galba colormath ```src``` folder to your project's source folder. Provided .c files look for the header directly by saying ```#include "galba_colormath.h"```. As the library is extremely simple, no build script is provided at the moment. The user can arrange the files according to their project structure as they see fit.

Once the galba colormath files have been placed in the project and are visible to the compiler, the user should be able to access all functions declared in the header by saying ```#include "galba_colormath.h"``` 
