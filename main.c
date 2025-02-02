
#include "galba_colormath.h"
#include "tests.h"


#include <stdio.h>
#include <math.h>






#define TEST_COUNT 21
bool (*test_functions[TEST_COUNT])(void) = {
    test_i32_rgb_conversion1,
    test_i32_rgb_conversion2,
    test_i32_rgb_conversion3,
    test_i32_hex_conversion_happy1,
    test_i32_hex_conversion_happy2,
    test_rgb_hex_conversion_happy1,
    test_rgb_hex_conversion_happy2,
    test_hex_conversion_unusual1,
    test_hex_conversion_unusual2,
    test_hex_conversion_unusual3,

    test_CIEDE2000_premade_data,
    test_CIEDE2000_params_nondefault,
    test_CMC_params_2_1,
    test_CMC_params_1_1,
    test_CIE94_base,

    test_RGB_LAB_roundtrip,
    test_XYZ_LAB_roundtrip,
    test_LAB_XYZ_roundtrip,

    test_premade_XYZ_LAB,
    test_premade_LAB_XYZ,
    test_premade_XYZ_RGB
};


#include <time.h>
#include <stdlib.h>
int main(void){

    int tests_succeeded = 0;

    for(int i=0; i<TEST_COUNT; i++){
        tests_succeeded += test_functions[i]();

    }

    printf("All tests completed.\nSuccessful tests: %d/%d", tests_succeeded, TEST_COUNT);

}


//todo
//investigate their test data
//https://hajim.rochester.edu/ece/sites/gsharma/ciede2000/

//todo investigate colors.io
//https://www.programiz.com/online-compiler/2DMVlGy5M0uo4

//https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
//https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html

//https://godbolt.org/z/s8foMezKr



