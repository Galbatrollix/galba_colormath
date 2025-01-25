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


double delta_CIE76(lab_t color1, lab_t color2){
	double l_diff = color1.l - color2.l;
	double a_diff = color1.a - color2.a;
	double b_diff = color1.b - color2.b;

	return sqrt(l_diff * l_diff + a_diff * a_diff + b_diff * b_diff);
}

// http://www.brucelindbloom.com/index.html?Eqn_DeltaE_CIE94.html
double delta_CIE94_full(lab_t color1, lab_t color2, CIE94_params params){
	double l_diff = color1.l - color2.l;
	double a_diff = color1.a - color2.a;
	double b_diff = color1.b - color2.b;

	// sq suffix stands for "squared"
	double deltaLsq = l_diff * l_diff;
	double deltaAsq = a_diff * a_diff;
	double deltaBsq = b_diff * b_diff;

	double C1 = sqrt(color1.a * color1.a + color1.b * color1.b);
	double C2 = sqrt(color2.a * color2.a + color2.b * color2.b);

	double deltaCsq = (C1 - C2) * (C1 - C2);
	double deltaHsq = deltaAsq + deltaBsq - deltaCsq;

	double SL = 1;
	double SC = 1 + params.K1 * C1;
	double SH = 1 + params.K2 * C1;

	double result = sqrt(
		deltaLsq / (params.KL * params.KL * SL * SL)
		+             
		deltaCsq / (params.KC * params.KC * SC * SC) 
		+
		deltaHsq / (params.KH * params.KH * SH * SH)
	);

	return result;

}
double delta_CIE94_g(lab_t color1, lab_t color2){
	return delta_CIE94_full(color1, color2, CIE94_GRAPHIC_ARTS);
}

double delta_CIE94_t(lab_t color1, lab_t color2){
	return delta_CIE94_full(color1, color2, CIE94_TEXTILES);
}