
#include "galba_colormath.h"
#include "tests.h"


#include <stdio.h>
#include <math.h>



static void print_rgb(rgb_t p){
    printf("%u %u %u\n", p.r, p.g, p.b);
}


static bool rgb_equals(rgb_t l, rgb_t r){
	return l.r == r.r && l.g == r.g && l.b == r.b;
}

static void lab_print(lab_t dupa){
	printf("LAB(%lf %lf %lf)\n", dupa.l, dupa.a, dupa.b);
}


#include <time.h>
#include <stdlib.h>
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

    // int different = 0;
    // for(int i = 0; i< 256*256*256 ;i++){
    // 	rgb_t start = RGB_from_i32(i);
    // 	rgb_t end = RGB_from_XYZ(XYZ_from_LAB(LAB_from_XYZ(XYZ_from_RGB(start))));
    // 	if(!rgb_equals(start, end)){
    // 		different += 1;
    // 	}
    // }


    test_CIEDE2000_premade_data();
    test_CIEDE2000_params_nondefault();

    test_CMC_params_2_1();
    test_CMC_params_1_1();

    test_CIE94_base();
}


//todo
//investigate their test data
//https://hajim.rochester.edu/ece/sites/gsharma/ciede2000/

//todo investigate colors.io
//https://www.programiz.com/online-compiler/2DMVlGy5M0uo4

//https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
//https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html

//https://godbolt.org/z/s8foMezKr