#include "tests.h"

#include "galba_colormath.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#define CIEDE2000_TEST_COUNT 39

// Test values taken from:
//https://hajim.rochester.edu/ece/sites/gsharma/ciede2000/

const lab_t CIEDE2000_TEST_INPUTS[CIEDE2000_TEST_COUNT][2] = {
	// Test values taken from:
	//https://hajim.rochester.edu/ece/sites/gsharma/ciede2000/
	{{50.0000, 2.6772, -79.7751}, {50.0000, 0.0000, -82.7485}},
	{{50.0000, 3.1571, -77.2803}, {50.0000, 0.0000, -82.7485}},
	{{50.0000, 2.8361, -74.0200}, {50.0000, 0.0000, -82.7485}},
	{{50.0000, -1.3802, -84.2814}, {50.0000, 0.0000, -82.7485}},
	{{50.0000, -1.1848, -84.8006}, {50.0000, 0.0000, -82.7485}},
	{{50.0000, -0.9009, -85.5211}, {50.0000, 0.0000, -82.7485}},
	{{50.0000, 0.0000, 0.0000}, {50.0000, -1.0000, 2.0000}},
	{{50.0000, -1.0000, 2.0000}, {50.0000, 0.0000, 0.0000}},
	{{50.0000, 2.4900, -0.0010}, {50.0000, -2.4900, 0.0009}},
	{{50.0000, 2.4900, -0.0010}, { 50.0000, -2.4900, 0.0010}},
	{{50.0000, 2.4900, -0.0010}, { 50.0000, -2.4900, 0.0011}},
	{{50.0000, 2.4900, -0.0010}, { 50.0000, -2.4900, 0.0012}},
	{{50.0000, -0.0010, 2.4900}, { 50.0000, 0.0009, -2.4900}},
	{{50.0000, -0.0010, 2.4900}, { 50.0000, 0.0010, -2.4900}},
	{{50.0000, -0.0010, 2.4900}, { 50.0000, 0.0011, -2.4900}},
	{{50.0000, 2.5000, 0.0000}, { 50.0000, 0.0000, -2.5000}},
	{{50.0000, 2.5000, 0.0000}, { 73.0000, 25.0000, -18.0000}},
	{{50.0000, 2.5000, 0.0000}, { 61.0000, -5.0000, 29.0000}},
	{{50.0000, 2.5000, 0.0000}, { 56.0000, -27.0000, -3.0000}},
	{{50.0000, 2.5000, 0.0000}, { 58.0000, 24.0000, 15.0000}},
	{{50.0000, 2.5000, 0.0000}, { 50.0000, 3.1736, 0.5854}},
	{{50.0000, 2.5000, 0.0000}, { 50.0000, 3.2972, 0.0000}},
	{{50.0000, 2.5000, 0.0000}, { 50.0000, 1.8634, 0.5757}},
	{{50.0000, 2.5000, 0.0000}, { 50.0000, 3.2592, 0.3350}},
	{{60.2574, -34.0099, 36.2677}, { 60.4626, -34.1751, 39.4387}},
	{{63.0109, -31.0961, -5.8663}, { 62.8187, -29.7946, -4.0864}},
	{{61.2901, 3.7196, -5.3901}, { 61.4292, 2.2480, -4.9620}},
	{{35.0831, -44.1164, 3.7933}, { 35.0232, -40.0716, 1.5901}},
	{{22.7233, 20.0904, -46.6940}, { 23.0331, 14.9730, -42.5619}},
	{{36.4612, 47.8580, 18.3852}, { 36.2715, 50.5065, 21.2231}},
	{{90.8027, -2.0831, 1.4410}, { 91.1528, -1.6435, 0.0447}},
	{{90.9257, -0.5406, -0.9208}, { 88.6381, -0.8985, -0.7239}},
	{{6.7747, -0.2908, -2.4247}, { 5.8714, -0.0985, -2.2286}},
	{{2.0776, 0.0795, -1.1350}, { 0.9033, -0.0636, -0.5514}},

	//  hand picked and cross referenced with websites, including color.js implementation
	{{99.9751541, -0.1694255, 0.465877863506425},{ 100, 0.005260499, -0.0104081845252}},

	//{{0,0,0},{0,0,0}},
	{{99.9751541, -0.1694255, 0.465877863506425},{ 100, 0.005260499, -0.0104081845252}},
	{{99.9751541, -0.1694255, 0.465877863506425},{ 100, 0.005260499, -0.0104081845252}},
	{{99.9751541, -0.1694255, 0.465877863506425},{ 100, 0.005260499, -0.0104081845252}},
	{{99.9751541, -0.1694255, 0.465877863506425},{ 100, 0.005260499, -0.0104081845252}},
};

const double CIEDE2000_TEST_EXPECTED[CIEDE2000_TEST_COUNT] = {
	2.0425, 2.8615, 3.4412, 1.0000, 1.0000, 1.0000, 2.3669, 2.3669, 
	7.1792, 7.1792, 7.2195, 7.2195, 4.8045, 4.8045, 4.7461, 4.3065, 
	27.1492, 22.8977, 31.9030, 19.4535, 1.0000, 1.0000, 1.0000, 1.0000, 
	1.2644, 1.2630, 1.8731, 1.8645, 2.0373, 1.4146, 1.4441, 1.5381, 0.6377, 0.9082,

	0.537679,0.537679,0.537679,0.537679,0.537679,
};


static double round_to_n_digits(double x, unsigned int digits) {
    double fac = pow(10, digits);
    return round(x* fac)/ fac;
}
static void print_lab(lab_t p){
    printf("lab(%lf %lf %lf)", p.l, p.a, p.b);
}

bool test_CIEDE2000_premade_data(void){

	for(int i=0;i<CIEDE2000_TEST_COUNT;i++){
		double expected = CIEDE2000_TEST_EXPECTED[i];
		double calculated = delta_CIEDE2000(CIEDE2000_TEST_INPUTS[i][0], CIEDE2000_TEST_INPUTS[i][1]);
		double calculated_swap = delta_CIEDE2000(CIEDE2000_TEST_INPUTS[i][1], CIEDE2000_TEST_INPUTS[i][0]);

		expected = round_to_n_digits(expected, 4);
		calculated = round_to_n_digits(calculated, 4);
		calculated_swap = round_to_n_digits(calculated_swap, 4);

		// yes, comparing floats for equality IS DEFINED and PERFECTLY fine. 
		// In this case numbers are rounded in the same way so they are eihter identical or the function returns incorrectly
		bool correct =  expected == calculated && calculated == calculated_swap;
		if(! correct){
			printf("Test test_CIEDE2000_premade_data FAILED at:\n");
			print_lab(CIEDE2000_TEST_INPUTS[i][0]);
			printf(", ");
			print_lab(CIEDE2000_TEST_INPUTS[i][1]);
			printf("\n");

			printf("Expected: %.4lf, calculated: %.4lf, calculated with arguments swapped: %.4lf\n", 
														expected, calculated, calculated_swap);
			return false;
		}

	}

	return true;


}

