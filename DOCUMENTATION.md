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
Since this library makes use of C-specific features such as compound literals, it is not strictly C++ conformant. It is advised to compile galba colormath files with a C compiler and link them to C++ programs with use of ```extern "C"``` directive. However, it's worth to note that g++ compiler can successfully compile galba colormath, thanks to compiler extension. Read more [here](https://gcc.gnu.org/onlinedocs/gcc/Compound-Literals.html). 
### Dependencies
The only dependency of the galba colormath is the C standard library. 
The exhaustive list of necessary libc headers and their used contents is:
- math.h
    - pow, sqrt, fabs, atan2, cos, sin, exp (functions)
- stdint.h
    - int32_t (type)

Stdint.h is included inside the header, math.h is only used by source files. 

Additionally, test suite requies the following headers and their contents: 
- stdbool.h
- math.h
    - round (function)
- stdint.h
    - int32_t, uint32_t (types)
- inttypes.h
    - PRId32 (macro)
- string.h
    - memcpy, memset (functions)
- stdio.h
    - printf, puts, putchar (functions)

### Headscratchers
1) Is the code thread-safe?
    - Yes
2) Is the code strict-aliasing-safe?
    - Yes, the code should produce correct results when compiled with optimization level of O3 on gcc. It doesn't rely on dodgy pointer/union casts in any of the provided functions.
3) Can this code be compiled with -fast math option using gcc?
    - Yes, but it was not developed with this assumption in mind, make sure it passes the tests if you try compiling with mast math enabled
## Structure definitions
### Color structs
The header defines 3 separate structures for dealing with color math and conversions. All colorspace and color distance related functions in the library operate on these types.

The color structures are the following: 
```c
typedef struct rgb_t{
	unsigned char r, g, b;
}rgb_t;

typedef struct lab_t{
	double l, a, b;
}lab_t;

typedef struct xyz_t{
	double x, y, z;
}xyz_t;
```
##### struct rgb_t 
Denotes an instance of color in sRGB color space. 
All possible value combinations of struct rgb_t members are valid. 

Example: \
```(rgb_t){.r = 0, .g = 0, .b = 0}``` is a rgb_t value for color black.\
```(rgb_t){.r = 255, .g = 255, .b = 255}``` is a rgb_t value for color white.


##### struct xyz_t
Denotes an instance of color in XYZ color space with D65 white reference point. 
Each channel is represented as percentage, so in short: range is 0.0 - 100.0 (%), not 0.0 - 1.0.

In practice, channels can sometimes slightly exceed the given range, but as long as the value is not nonsensical (for example 108.9, instead of 0.3E21), then the other functions in this header will process it and yield expected results. 

Example:\
```(xyz_t){.x = 86.5, .y = 52.311, .z = 100.001}``` is a xyz_t value for color pink.


##### struct lab_t
Denotes an instance of color in Lab color space with D65 white reference point. \
l channel is in range of 0.0 - 100.0.\
a and b channels are in range -125.0, 125.0

Similarly as in xyz_t, values slightly outside of range won't shouldn't cause significant errors. 

Example:\
```(lab_t){.l = 100.0, .a = -125.01, .b = 124.99}``` is a lab_t value for color cyan.

### Distance params
Some of the color difference functions require additional parameters. If that's the case, these parameters can be passed to the relevant function using special structure. (This is done in order to reduce the number of parameters and make it more convinient to use these functions) 
Field names in the defined structures are the same as parameter names in the formulas. Currently there are three such parameters structures.
```c
typedef struct{
	double K1, K2, KL, KC, KH;
}CIE94_params;

typedef struct{
	double KL, KC, KH;
}CIEDE2000_params;

typedef struct{
	double KL, KC;
}CMC_params;
```

The above structures define parameters for color distances ```CIE94```,  ```CIEDE2000``` and  ```CMC``` respectively.
## Define macros
The only macros defined in ```galba_colormath.h``` are pre-made parameter strucuture literals.

The purpose of these macros is to provide the user with ready to use parameter structs with desirable default values. These defaults were chosen to reflect most common usages of the distance functions.

Currently there are five pre-made parameter struct literals, defined as the following: 
```c
#define CIE94_TEXTILES ((const CIE94_params){.K1 = 0.048, .K2 = 0.014, .KL = 2, .KC = 1, .KH = 1 })
#define CIE94_GRAPHIC_ARTS ((const CIE94_params){.K1 = 0.045, .K2 = 0.015, .KL = 1, .KC = 1, .KH = 1})

#define CIEDE2000_BASE ((const CIEDE2000_params){.KL = 1, .KC = 1, .KH = 1})

#define CMC_ACCEPTABILITY ((const CMC_params){.KL = 2, .KC = 1})
#define CMC_PERCEPTIBILITY ((const CMC_params){.KL = 1, .KC = 1})
```
## Functions

### RGB encoding-related functions
The purpose of functions below is to help user conviniently convert between ```struct rgb_t```, integer-encoded RGB and hex-string-encoded RGB.
#### Integer encoding
Galba colormath allows encoding RGB data in ```int32_t``` values. The format is as follows:\
```0xXXRRGGBB```\
Where RR, GG, BB are bytes encoding a value between 0 and 255 for color channels R, G, B respectively.\
XX denotes unused bytes. Usually best set to 0, however library will accept any value for XX byte and ignore it.\
Example:
```c
int32_t color = 0x00FF0000;
// represents the same color as (rgb_t){.r = 255, .g = 0, .b = 0}
```

#### Hex strings
Galba colormath also allows to work with hex strings. Conventionally formed hex color strings of forms "rrggbb" or "RRGGBB" both upper and lowercase letters are accepted and can be mixed in a single string. Usually present "#" character is not required. More details on "#" and null terminator requirements can be read in the following section.\
Example:
```c
char color[7] = "fF0000";
// represents the same color as (rgb_t){.r = 255, .g = 0, .b = 0}
```
#### Encoding conversion functions
The header declares the following 6 functions that allow to convert from each of the three encodings (int32_t, string and rgb_t struct) to any of the remaining two.
```c
rgb_t RGB_from_i32(int32_t int_repr);
int32_t i32_from_RGB(rgb_t rgb_input);

int32_t i32_from_HEX(const char hex_arr[6]);
void HEX_from_i32(char arr_buffer[6], int32_t int_repr);

rgb_t RGB_from_HEX(const char hex_arr[6]);
void HEX_from_RGB(char arr_buffer[6], rgb_t rgb_input);
```
##### RGB_from_i32
This function takes int32_t and interprets it as a RGB-encoding integer according to aforementioned rules.\
First byte is ignored and 2nd, 3rd and 4th bytes are interpreted as channels R, G, B respectively. 

##### i32_from_RGB
This function takes struct rgb_t and returns the same color encoded as int32_t.\
First byte of resulting value is always 0. 

##### i32_from_HEX
This function takes constant char pointer pointing to the first element of hex string and returns the same color in the int32_t representation. 


Char pointer must be a valid (not NULL) pointer to a memory with at least 6 accessible bytes. The provided memory doesn't need to be a null-terminated string, however it must be at least 6 bytes long, regardless of array contents. (Function doesn't consider null terminators and always reads exactly 6 bytes to produce the output)\
Attempting to use the function with NULL pointer will cause a null pointer dereference error. Similarly, providing the function with pointer with less than 6 bytes of available memory will cause a buffer over-read error.
```c
int32_t color1 = i32_from_HEX("ABC");  				   // WRONG, buffer overread
int32_t color2 = i32_from_HEX(NULL);				   // WRONG, nullptr dereference
int32_t color3 = i32_from_HEX((char[6]){'a','b','c','d','e','f'}); // OK, result = 0x00ABCDEF
```
The pointer should contain an address of first meaningful character of hexstring - in other words for string ```"#abFfcC"``` the ```hex_arr``` pointer should point to 'a' character, not to '#' character.
If provided hex string contains invalid characters (for example: '\0', '\n', 'R', etc.), then these characters will be interpreted as 0.
```c
const char hex_color[] = "#AABBCC";
int32_t color1 = i32_from_HEX(hex_color);  // WRONG, result = 0x000AABBC
int32_t color2 = i32_from_HEX(hex_color + 1); // OK, result = 0x00AABBCC
```
First byte of resulting value is always 0.\
The contents of char array are declared const and are not modified by the function. 

##### RGB_from_HEX
This function takes constant char pointer pointing to the first element of hex string and returns the same color as an rgb_t structure.\
Rules for the value of input parameter the same as in ```i32_from_HEX```. 

##### HEX_from_i32
This function writes to a char buffer.\
Data written to the buffer is a hex string representation of color provided in int32_t representation.

Char pointer must be a valid (not NULL) pointer to a writable memory with at least 6 accessible bytes.\
Attempting to use the function with NULL pointer will cause a null pointer dereference error. Similarly, providing the function with pointer with less than 6 bytes of available memory will cause a buffer over-write error. Providing a pointer to read-only memory will also likely cause an error.
```c
char buf_short[3];
char buffer[6];
HEX_from_i32(buf_short, (int32_t)0x00CCBBAA);   	// WRONG, buffer over-write
HEX_from_i32(buffer, NULL);  				// WRONG, nullptr dereference.
HEX_from_i32("CONSTANT_STRING", (int32_t)0x00CCBBAA);	// WRONG, read only memory attempted to write
HEX_from_i32(buffer, 0x00123456);  			// OK, written "123456" to memory  
```

Hex string written by the function doesn not contain ```'#'``` character, nor ```'\0'``` character.\
```c
char hex_color[7] = {0};
char hex_color2[7];
HEX_from_i32(hex_color, (int32_t)0x00CCBBAA);   // OK, written "CCBBAA" to memory.
HEX_from_i32(hex_color2, (int32_t)0x00BCBCAA);  // OK, written "BCBCAA" to memory.
puts(hex_color); 				// OK, prints "CCBBAA\n"
puts(hex_color2);                               // ERROR, non-null-terminated string passed to puts function
```
If generated string has letters between ```'a'``` and ```'f'```, then they will always be generated as uppercase.\

##### HEX_from_RGB
This function writes to a char buffer.\
Data written to the buffer is a hex string representation of color provided in struct rgb_t.

Rules for the value of output pointer parameter the same as in ```HEX_from_i32```.

##### Convinience functions
The header also defines two convinience functions aiming to provide an alternative to ```HEX_from_i32``` and ```HEX_from_RGB``` for users that expect the ```'#'``` character at the beginning and ```'\0'``` string terminator at the end. These functions also return the passed char pointer to ease logging with functions such as ```printf``` or ```puts```.

Rules for output parameter is the same as in the corresponding base variants, however the required write-able memory size is 8 bytes instead of 6.
```c
char* HEX_from_RGB_2(char string_buffer[8], rgb_t rgb_input);
char* HEX_from_i32_2(char string_buffer[8], int32_t int_repr);
```
Example:
```c
char buffer[8];
puts(HEX_from_RGB_2(buffer, (rgb_t){0,255,0})); 	 // prints "#00FF00\n"
puts(HEX_from_i32_2(buffer, (int32_t)0xfccffa));         // prints "#FCCFFA\n"
```
### Colorspace conversion functions

### Color distance functions

## Use examples

## Test suite

### TODO DECIDE ON STRUCTURE OF THIS
