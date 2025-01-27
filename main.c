
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


double pow_to_7(double input){
    return (input * input) * (input * input)  * (input * input) * input;
}

double pow_to_7a(double input){
    return input * input * input * input  * input * input * input;
}

double pow_to_7_2(double input){
    return pow(input, 7.0);
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

	lab_t input1lab = LAB_from_RGB(input1);
	lab_t input2lab = LAB_from_RGB(input2);


	lab_print(input1lab);
	lab_print(input2lab);
    double CIE94_dist = delta_CIE94_g(input1lab,input2lab);
    double CIE94_dist2 = delta_CIE94_t(input1lab,input2lab);

    printf("dist1: %lf dist2: %lf\n", CIE94_dist, CIE94_dist2);

    double CMC_dist1 = delta_CMC_p(input1lab, input2lab);
    double CMC_dist2 = delta_CMC_a(input1lab, input2lab);
    printf("Perceptibility: %lf Acceptability: %lf\n", CMC_dist1, CMC_dist2);


    double CIEDE2000dist = delta_CIEDE2000(input1lab, input2lab);
    double CIEDE2000dist2 = Lab_color_difference_CIEDE2000(input1lab, input2lab);
    double CIEDE2000dist3 = ciede2000(input1lab, input2lab);

 	printf("Ciede: %lf clrkth:%lf other:%lf\n", CIEDE2000dist, CIEDE2000dist2, CIEDE2000dist3);

    // todo check if the formula in return value would be better with (a/(c*d)) ^2
 	// todo theta stuff


	// time_t start,end;
	// double diff;
	// double result;

	// time (&start);
	// result = 0;
	// for(int i=0; i<1000000000;i++){
	// 	double val = (double)rand()/ 1000000;

	// 	result += pow_to_7(val);
	// }
 	// time (&end);
 	// printf("%.30lf\n", result);
  	// diff = difftime (end, start);
  	// printf("diff: %lf\n", diff);

  	// 	time (&start);
	// result = 0;
	// for(int i=0; i<1000000000;i++){
	// 	double val = (double)rand()/ 1000000;

	// 	result += pow_to_7a(val);
	// }
 	// time (&end);
	//  	printf("%.30lf\n", result);
  	// diff = difftime (end, start);
  	// printf("diff: %lf\n", diff);

	// time (&start);
	// result = 0;
	// for(int i=0; i<1000000000;i++){
	// 	double val = (double)rand()/ 1000000;

	// 	result += pow_to_7_2(val);
	// }
 	// time (&end);
 	// printf("%.30lf\n", result);
  	// diff = difftime (end, start);
  	// printf("diff: %lf\n", diff);
  	// return 0;

}