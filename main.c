
#include <stdio.h>
#include "tests.h"
#include "galba_colormath.h"

static void print_rgb(rgb_t p){
    printf("%u %u %u\n", p.r, p.g, p.b);
}

#include <math.h>
lab_t xyz2lab( xyz_t value ) {
	lab_t result;

	value.x /=  95.047;
	value.y /=   100.0;
	value.z /= 108.883;

	if( value.x > 0.008856 ) 
		value.x = pow( value.x, 1.0/3 );
	else
		value.x = 7.787 * value.x + 16.0 / 116;

	if( value.y > 0.008856 ) 
		value.y = pow( value.y, 1.0/3 );
	else
		value.y = 7.787 * value.y + 16.0 / 116;

	if( value.z > 0.008856 ) 
		value.z = pow( value.z, 1.0/3 );
	else
		value.z = 7.787 * value.z + 16.0 / 116;

	result.l = ( 116 * value.y ) - 16;
	result.a = 500 * ( value.x - value.y );
	result.b = 200 * ( value.y - value.z );

	return result;
}



int main(void){
	// test_i32_rgb_conversion1();
	// test_i32_rgb_conversion2();
	// test_i32_rgb_conversion3();
	test_i32_hex_conversion_happy1();
	test_i32_hex_conversion_happy2();
	test_rgb_hex_conversion_happy1();
    test_rgb_hex_conversion_happy2();
    test_hex_conversion_unusual1();
    test_hex_conversion_unusual2();
    test_hex_conversion_unusual3();

    rgb_t input = (rgb_t){0,7,3};

    lab_t dupa = LAB_from_XYZ(XYZ_from_RGB(input));

    printf("%lf %lf %lf\n", dupa.l, dupa.a, dupa.b);
    dupa = xyz2lab(XYZ_from_RGB(input));
    printf("%lf %lf %lf\n", dupa.l, dupa.a, dupa.b);

}