#include "galba_colormath.h"

#include <math.h>


double euclidean(rgb_t color1, rgb_t color2){
	double r_diff = (double)color1.r - color2.r;
	double g_diff = (double)color1.g - color2.g;
	double b_diff = (double)color1.b - color2.b;
	
	return sqrt(r_diff * r_diff + g_diff * g_diff + b_diff * b_diff);
}
int32_t euclidean_noroot(rgb_t color1, rgb_t color2){
	int32_t r_diff = (int32_t)color1.r - color2.r;
	int32_t g_diff = (int32_t)color1.g - color2.g;
	int32_t b_diff = (int32_t)color1.b - color2.b;

	return (int32_t)(r_diff * r_diff + g_diff * g_diff + b_diff * b_diff);
}

double delta_CIE76(lab_t color1, lab_t color2);
double delta_CIE94(lab_t color1, lab_t color2);
double delta_CIEDE2000(lab_t color1, lab_t color2);
double delta_CMC(lab_t color1, lab_t color2);
double delta_CMC_extended(lab_t color1, lab_t color2, double lightness, double chroma);