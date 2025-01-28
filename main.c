
#include <stdio.h>
#include "tests.h"
#include "galba_colormath.h"

static void print_rgb(rgb_t p){
    printf("%u %u %u\n", p.r, p.g, p.b);
}

#include <math.h>


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

    // printf("Different pairs detected: %d", different);

	rgb_t input1 = (rgb_t){0,33,15};
	rgb_t input2 = (rgb_t){11,51,22};

	input1 = (rgb_t){0,255,15};
	input2 = (rgb_t){255,51,125};	

    input1 = (rgb_t){255,255,254};
    input2 = (rgb_t){255,255,255};   

	lab_t input1lab = LAB_from_RGB(input1);    
	lab_t input2lab = LAB_from_RGB(input2);

    // input1lab = (lab_t){
    //     99.9751541, -0.1694255, 0.465877863506425
    // };
    // input2lab = (lab_t){
    //     100, 0.005260499, -0.0104081845252
    // };

	lab_print(input1lab);
	lab_print(input2lab);
    double CIE94_dist = delta_CIE94_g(input1lab,input2lab);
    double CIE94_dist2 = delta_CIE94_t(input1lab,input2lab);

    printf("dist1: %lf dist2: %lf\n", CIE94_dist, CIE94_dist2);

    double CMC_dist1 = delta_CMC_p(input1lab, input2lab);
    double CMC_dist2 = delta_CMC_a(input1lab, input2lab);
    printf("Perceptibility: %lf Acceptability: %lf\n", CMC_dist1, CMC_dist2);


    double CIEDE2000dist = delta_CIEDE2000(input1lab, input2lab);

 	printf("Ciede: %.20lf\n", CIEDE2000dist);

    test_CIEDE2000_premade_data();
}


//todo
//investigate their test data
//https://hajim.rochester.edu/ece/sites/gsharma/ciede2000/

//todo investigate colors.io
//https://www.programiz.com/online-compiler/2DMVlGy5M0uo4

//https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
//https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html

//https://godbolt.org/z/s8foMezKr