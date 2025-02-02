#ifndef GALBA_COLORMATH_TEST
#define GALBA_COLORMATH_TEST

#include <stdbool.h>


// simple conversions tests

bool test_i32_rgb_conversion1(void);
bool test_i32_rgb_conversion2(void);
bool test_i32_rgb_conversion3(void);

bool test_i32_hex_conversion_happy1(void);
bool test_i32_hex_conversion_happy2(void);

bool test_rgb_hex_conversion_happy1(void);
bool test_rgb_hex_conversion_happy2(void);

bool test_hex_conversion_unusual1(void);
bool test_hex_conversion_unusual2(void);
bool test_hex_conversion_unusual3(void);

// color distance tests
// cie 76 and euclid distance formulas are one line and hence too simple to warrant testing. 


bool test_CIEDE2000_premade_data(void);
bool test_CIEDE2000_params_nondefault(void);

bool test_CMC_params_2_1(void);
bool test_CMC_params_1_1(void);

bool test_CIE94_base(void);


// color conversion tests

bool test_RGB_LAB_roundtrip(void);
bool test_XYZ_LAB_roundtrip(void);
bool test_LAB_XYZ_roundtrip(void);


bool test_premade_XYZ_LAB(void);
bool test_premade_LAB_XYZ(void);
bool test_premade_XYZ_RGB(void);

#endif //GALBA_COLORMATH_TEST