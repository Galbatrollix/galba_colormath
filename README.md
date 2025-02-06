# galba_colormath
Galba colormath is a lightweight C language library for color conversions and distances. It offers conversions between RGB, XYZ and LAB colorspaces and computation of the following lab color distances: delta CIE76, delta CIE94, delta CMC and delta CIEDE2000. Aside from that, the library offers simple conversion functions allowing for easier handling of color hexstrings and RGB values. Full details on all provided functions can be found (link this) here.

The repository also contains a test suite for almost all defined functions. (Some extremely simple functions are omitted) More details on the test suite (link this) here.

### Language version and dependencies
Galba colormath is written in a C99 style and doesn't use specific compiler extensions. Hence the code should compile correctly with any C99 (or later) conformant compiler. The only dependency in the library is C standard library. Details on required libc includes and functions can be found (link this) here.

