#ifndef GALBA_COLORMATH_H
#define GALBA_COLORMATH_H

#include <stdint.h>

typedef struct rgb_t{
	unsigned char r, g, b;
}rgb_t;

typedef struct lab_t{
	double l, a, b;
}lab_t;

typedef struct xyz_t{
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

int32_t i32_from_HEX(const char hex_arr[6]);
void HEX_from_i32(char arr_buffer[6], int32_t int_repr);

rgb_t RGB_from_HEX(const char hex_arr[6]);
void HEX_from_RGB(char arr_buffer[6], rgb_t rgb_input);

//Convinience variants of the above
//Additionally insert '#' at the beggining and '\0' null terminator at the end and return string_buffer
char*  HEX_from_RGB_2(char string_buffer[8], rgb_t rgb_input);
char*  HEX_from_i32_2(char string_buffer[8], int32_t int_repr);



double euclid_dist(rgb_t color1, rgb_t color2);

double delta_CIE76(lab_t color1, lab_t color2);

typedef struct{
	double K1, K2, KL, KC, KH;
}CIE94_params;

#define CIE94_TEXTILES ((const CIE94_params){.K1 = 0.048, .K2 = 0.014, .KL = 2, .KC = 1, .KH = 1 })
#define CIE94_GRAPHIC_ARTS ((const CIE94_params){.K1 = 0.045, .K2 = 0.015, .KL = 1, .KC = 1, .KH = 1})

// equivalent to CIE94_full with parameter CIE94_GRAPHIC_ARTS
double delta_CIE94_g(lab_t color1, lab_t color2);
// equivalent to CIE94_full with parameter CIE94_TEXTILES
double delta_CIE94_t(lab_t color1, lab_t color2);
double delta_CIE94_full(lab_t color1, lab_t color2, CIE94_params params);


typedef struct{
	double KL, KC, KH;
}CIEDE2000_params;

#define CIEDE2000_BASE ((const CIEDE2000_params){.KL = 1, .KC = 1, .KH = 1})

// equivalent to CIE2000_full with parameter CIEDE2000_BASE
double delta_CIEDE2000(lab_t color1, lab_t color2);
double delta_CIEDE2000_full(lab_t color1, lab_t color2, CIEDE2000_params params);

typedef struct{
	double KL, KC;
}CMC_params;

#define CMC_ACCEPTABILITY ((const CMC_params){.KL = 2, .KC = 1})
#define CMC_PERCEPTIBILITY ((const CMC_params){.KL = 1, .KC = 1})
 
// equivalent to CMC_full with parameter CMC_PERCEPTIBILITY
double delta_CMC_p(lab_t color1, lab_t color2);
// equivalent to CMC_full with parameter CMC_ACCEPTIBILITY
double delta_CMC_a(lab_t color1, lab_t color2);
double delta_CMC_full(lab_t color1, lab_t color2, CMC_params params);



#endif // GALBA_COLORMATH_H
