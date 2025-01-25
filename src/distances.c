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



#ifndef M_PI
#define M_PI 3.1415926535897932
#endif

// pretty sure inline doesnt do literally anything but it looks cool and smart xD
static inline double deg_from_rad(double radians){
	return radians / M_PI * 180;
}
static inline double rad_from_deg(double degrees){
	return degrees / 180 * M_PI;
}


#define 

double delta_CMC_full(lab_t color1, lab_t color2, CMC_params params){
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


	// Conditional compilation based on the fact that atan2(0, 0) may return an unexpected value if:  
	// environment doesn't fully support IEEE floating point standard that defines atan(0, 0) behavior

	#ifdef __STDC_IEC_559__  // IEEE supported
		double H = atan2(color1.b / color1.a);
	#else  					 // IEEE not supported
		// note: the atan2(0,0) result depends on signedness of floating point zeros
		// however for this purpose simply returning 0 is sufficient
		double H = (color1.a == 0 && color1.b == 0) ? 0 : atan2(color1.b / color1.a);
	#endif 					 

	double H1rad = (H >= 0) ? H : H + 2 * M_PI;

	double T;
	if(164 <= deg_from_rad(H1rad) && deg_from_rad(H1rad) <= 345){
		T = 0.56 + fabs(0.2 * cos(H1rad + rad_from_deg(168)));
	}else{
		T = 0.36 + fabs(0.4 * cos(H1rad + rad_from_deg(35)));
	}


	double F  = sqrt((C1 * C1 * C1 * C1) / (1900 + C1 * C1 * C1 * C1));
	double SC = (0.0638 * C1) / (1 + 0.0131 * C1) + 0.638;
	double SH = SC * (F * T + 1 - F);
	double SL;
	if(color1.l < 16){
		SL = 0.511;
	}else{
		SL = (0.040975 * color1.l) / (1 + 0.01765 * color1.l);
	}

	double result = sqrt(
		deltaLsq / (params.lightness * params.lightness * SL * SL)
		+
		deltaCsq / (params.chroma * params.chroma * SC * SC)
		+
		deltaHsq / (SH * SH)
	);

	return result;
}

double delta_CMC_p(lab_t color1, lab_t color2){
	return delta_CMC_full(color1, color2, CMC_PERCEPTIBILITY);
}

double delta_CMC_a(lab_t color1, lab_t color2){
	return delta_CMC_full(color1, color2, CMC_ACCEPTIBILITY);
}
