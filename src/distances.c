#include "galba_colormath.h"

#include <math.h>


double euclid_dist(rgb_t color1, rgb_t color2){
	double r_diff = (double)color1.r - color2.r;
	double g_diff = (double)color1.g - color2.g;
	double b_diff = (double)color1.b - color2.b;

	return sqrt(r_diff * r_diff + g_diff * g_diff + b_diff * b_diff);
}
double euclid_dist_noroot(rgb_t color1, rgb_t color2){
	double r_diff = (double)color1.r - color2.r;
	double g_diff = (double)color1.g - color2.g;
	double b_diff = (double)color1.b - color2.b;

	return r_diff * r_diff + g_diff * g_diff + b_diff * b_diff;
}

