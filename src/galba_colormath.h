#ifndef GALBA_COLORMATH_H
#define GALBA_COLORMATH_H

#include <stdint.h>

typedef struct RGB{
	unsigned char r, g, b;
}rgb_t;

typedef struct LAB{
	double l, a, b;
}lab_t;

typedef struct XYZ{
	double x, y, z;
}xyz_t;


xyz_t XYZ_from_RGB(rgb_t rgb_input);
rgb_t RGB_from_XYZ(xyz_t xyz_input);


xyz_t XYZ_from_LAB(lab_t lab_input);
lab_t LAB_from_XYZ(xyz_t xyz_input);


rgb_t RGB_from_LAB(lab_t lab_input);
lab_t LAB_from_RGB(rgb_t rgb_input);


rgb_t RGB_from_i32(int32_t int_repr);
int32_t i32_from_RGB(rgb_t rgb_input);


rgb_t RGB_from_HEX(const char hex_arr[6]);
void HEX_from_RGB(char arr_buffer[6], rgb_t rgb_input);

// convinience variant of the above. inserts '#' at the beggining and '\0' null terminator at the end
void HEX_from_RGB_2(char string_buffer[8], rgb_t rgb_input);




#endif // GALBA_COLORMATH_H
