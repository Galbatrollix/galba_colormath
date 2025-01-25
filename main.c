
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

xyz_t lab2xyz( lab_t value ) {
	xyz_t result;

	result.y = ( value.l + 16 ) / 116.0;
	result.x = value.a / 500.0 + result.y;
	result.z = result.y - value.b / 200.0;

	if( pow( result.x, 3 ) > 0.008856 )
		result.x = pow( result.x, 3 );
	else
		result.x = ( result.x - 16 / 116 ) / 7.787;

	if( pow( result.y, 3 ) > 0.008856 )
		result.y = pow( result.y, 3 );
	else
		result.y = ( result.y - 16 / 116 ) / 7.787;

	if( pow( result.z, 3 ) > 0.008856 )
		result.z = pow( result.z, 3 );
	else
		result.z = ( result.z - 16 / 116 ) / 7.787;

	result.x *=  95.047;
	result.y *= 100.000;
	result.z *= 108.883;

	return result;
}

rgb_t xyz2rgb( xyz_t value ) {
	rgb_t result;
	double r, g, b;

	value.x /= 100;
	value.y /= 100;
	value.z /= 100;

	r = value.x *  3.2406 + value.y * -1.5372 + value.z * -0.4986;
	g = value.x * -0.9689 + value.y *  1.8758 + value.z *  0.0415;
	b = value.x *  0.0557 + value.y * -0.2040 + value.z *  1.0570;

	if( r > 0.0031308 )
		r = 1.055 * pow( r, 1 / 2.4 ) - 0.055;
	else
		r *= 12.92;

	if( g > 0.0031308 )
		g = 1.055 * pow( g, 1 / 2.4 ) - 0.055;
	else
		g *= 12.92;

	if( b > 0.0031308 )
		b = 1.055 * pow( b, 1 / 2.4 ) - 0.055;
	else
		b *= 12.92;

	result.r = r * 255 + 0.5;
	result.g = g * 255 + 0.5;
	result.b = b * 255 + 0.5;

	return result;
}

static bool rgb_equals(rgb_t l, rgb_t r){
	return l.r == r.r && l.g == r.g && l.b == r.b;
}

static void lab_print(lab_t dupa){
	printf("LAB(%lf %lf %lf)\n", dupa.l, dupa.a, dupa.b);
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

    rgb_t input = (rgb_t){5,222,0};	

    lab_t dupa = LAB_from_XYZ(XYZ_from_RGB(input));

    printf("LAB %lf %lf %lf\n", dupa.l, dupa.a, dupa.b);
    // dupa = xyz2lab(XYZ_from_RGB(input));
    // printf("%lf %lf %lf\n", dupa.l, dupa.a, dupa.b);

    xyz_t outputxyz = XYZ_from_LAB(dupa);
    rgb_t output = RGB_from_XYZ(XYZ_from_LAB(dupa));

    printf("XYZ %lf %lf %lf\n", outputxyz.x, outputxyz.y, outputxyz.z);
    printf("RGB %u %u %u\n", output.r, output.g, output.b);

    // int different = 0;
    // for(int i = 0; i< 256*256*256 ;i++){
    // 	rgb_t start = RGB_from_i32(i);
    // 	rgb_t end = RGB_from_XYZ(XYZ_from_LAB(LAB_from_XYZ(XYZ_from_RGB(start))));
    // 	if(!rgb_equals(start, end)){
    // 		different += 1;
    // 	}
    // }

    // printf("Different pairs detected: %d", different);

	rgb_t input1 = (rgb_t){0,0,0};
	rgb_t input2 = (rgb_t){255,0,255};
	lab_t input1lab = LAB_from_RGB(input1);
	lab_t input2lab = LAB_from_RGB(input2);


	lab_print(input1lab);
	lab_print(input2lab);
    double CIE94_dist = delta_CIE94(input1lab,input2lab);
    double CIE94_dist2 = delta_CIE94_full(input1lab,input2lab, CIE94_TEXTILES);
    printf("dist1: %lf dist2: %lf\n", CIE94_dist, CIE94_dist2);

}