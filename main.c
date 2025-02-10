
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

static void print_xyz(xyz_t p){
    printf("xyz(%lf %lf %lf)", p.x, p.y, p.z);
}
static void print_rgb(rgb_t p){
    printf("rgb(%u %u %u)", p.r, p.g, p.b);
}
static void print_lab(lab_t p){
    printf("lab(%lf %lf %lf)", p.l, p.a, p.b);
}

int main(void){

    int tests_succeeded = 0;

    // for(int i=0; i<TEST_COUNT; i++){
    //     tests_succeeded += test_functions[i]();

    // }

    printf("All tests completed.\nSuccessful tests: %d/%d\n", tests_succeeded, TEST_COUNT);

    print_xyz(XYZ_from_RGB((rgb_t){255,255,255}));
    print_rgb(RGB_from_XYZ((xyz_t){95, 123, 109}));

}


//https://godbolt.org/z/s8foMezKr



