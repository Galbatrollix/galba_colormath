# Galba Colormath Documentation
## Overwiew
Galba Colormath library contains a single header - ```galba_colormath.h```. This header defines structures and functions for three main purposes: 
- Decoding and encoding RGB in various formats [section link](#rgb-encoding-related-functions)
- Conversion between sRGB, XYZ and Lab color spaces [section link](#colorspace-conversion-functions)
- Calculating color distances [section link](#color-distance-functions)
  
The header offers functions for each of these tasks. This file provides description of each function, macro and structure declared or defined in the header.
### Language Requirements
#### C version
Galba color math is written in style conformant to C99 standard specification and as of writing this (February 2025) doesn't use any deprecated features according to the latest C standard. This means that the provided code should compile without any issues on any C99 conformant or later C compiler. 
#### C++ compatibility
Since this library makes use of C-specific features such as compound literals, it is not strictly C++ conformant. It is advised to compile galba colormath files with a C compiler and link them to C++ programs with use of ```extern "C" ``` directive. However, it's worth to note that g++ compiler can successfully compile galba colormath, thanks to compiler extension. Read more [here](https://gcc.gnu.org/onlinedocs/gcc/Compound-Literals.html). 
### Dependencies

### Installation

## Structure definitions

## Define macros

## Functions

### RGB encoding-related functions

### Colorspace conversion functions

### Color distance functions

## Test suite

### TODO DECIDE ON STRUCTURE OF THIS
