
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

double Lab_color_difference_CIE94(lab_t color1, lab_t color2){
	 double inL1; double ina1; double  inb1; double inL2; double ina2; double  inb2;
	inL1 = color1.l; inL2 = color2.l;
	ina1 = color1.a; ina2 = color2.a;
	inb1 = color1.b; inb2 = color2.b;

	// case Application.GraphicArts:
		double Kl = 1.0;
		double K1 = 0.045;
		double K2 = 0.015;
	// 	break;
	// case Application.Textiles:
	// 	Kl = 2.0;
	// 	K1 = .048;
	// 	K2 = .014;
	// break;

	double deltaL = inL1 - inL2;
	double deltaA = ina1 - ina2;
	double deltaB = inb1 - inb2;

	double c1 = sqrt(pow(ina1, 2) + pow(inb1, 2));
	double c2 = sqrt(pow(ina2, 2) + pow(inb2, 2));
	double deltaC = c1 - c2;

	double deltaH = pow(deltaA,2) + pow(deltaB,2) - pow(deltaC,2);
	deltaH = deltaH < 0 ? 0 : sqrt(deltaH);

	const double sl = 1.f;
	const double kc = 1.f;
	const double kh = 1.f;

	double sc = 1.f + K1*c1;
	double sh = 1.f + K2*c1;

	double i = pow(deltaL/(Kl*sl), 2) +
	                pow(deltaC/(kc*sc), 2) +
	                pow(deltaH/(kh*sh), 2);

	double finalResult = i < 0 ? 0 : sqrt(i);
	return (finalResult);
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

	rgb_t input1 = (rgb_t){31,122,100};
	rgb_t input2 = (rgb_t){233,15,245};
	lab_t input1lab = LAB_from_RGB(input1);
	lab_t input2lab = LAB_from_RGB(input2);


	lab_print(input1lab);
	lab_print(input2lab);
    double CIE94_dist = delta_CIE94_g(input1lab,input2lab);
    double CIE94_dist2 = delta_CIE94_t(input1lab,input2lab);
    double CIE94_dist_CLRKTH = Lab_color_difference_CIE94(input1lab, input2lab);
    printf("dist1: %lf dist2: %lf\nclrtk: %lf\n", CIE94_dist, CIE94_dist2, CIE94_dist_CLRKTH);

}