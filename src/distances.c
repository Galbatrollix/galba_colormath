#include "galba_colormath.h"

#include <math.h>


double euclid_dist(rgb_t color1, rgb_t color2){
	double r_diff = (double)color1.r - color2.r;
	double g_diff = (double)color1.g - color2.g;
	double b_diff = (double)color1.b - color2.b;

	return sqrt(r_diff * r_diff + g_diff * g_diff + b_diff * b_diff);
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

//pretty sure inline doesnt do literally anything but it looks cool and smart xD
static inline double deg_from_rad(double radians){
	return radians / M_PI * 180;
}
static inline double rad_from_deg(double degrees){
	return degrees / 180 * M_PI;
}



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
		double H = atan2(color1.b, color1.a);
	#else  					 // IEEE not supported
		// note: the atan2(0,0) result depends on signedness of floating point zeros
		// however for this purpose simply returning 0 is sufficient
		double H = (color1.a == 0 && color1.b == 0) ? 0 : atan2(color1.b,  color1.a);
	#endif 					 

	double H1rad = (H >= 0) ? H : H + 2 * M_PI;

	double T;
	if(rad_from_deg(164) <= H1rad && H1rad <= rad_from_deg(345)){
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
		deltaLsq / (params.KL * params.KL * SL * SL)
		+
		deltaCsq / (params.KC * params.KC * SC * SC)
		+
		deltaHsq / (SH * SH)
	);

	return result;
}

double delta_CMC_p(lab_t color1, lab_t color2){
	return delta_CMC_full(color1, color2, CMC_PERCEPTIBILITY);
}

double delta_CMC_a(lab_t color1, lab_t color2){
	return delta_CMC_full(color1, color2, CMC_ACCEPTABILITY);
}

//stdlib pow function is twice as slow without -ffast-math and is not particularly more correct than this
static inline double pow_to_7(double input){
    return (input * input) * (input * input)  * (input * input )* input;
}

#include <stdio.h>

//http://brucelindbloom.com/index.html?Eqn_DeltaE_CIE2000.html
double delta_CIEDE2000_full(lab_t color1, lab_t color2, CIEDE2000_params params){
	double C1 = sqrt(color1.a * color1.a + color1.b * color1.b);
	double C2 = sqrt(color2.a * color2.a + color2.b * color2.b);
	double meanCx2 = C1 + C2;
	/*
	This latex string explains how original formula for G was transformed to simplify computation 

	\sqrt{\dfrac{C^7}{C^7 + 25^7}} ==  \sqrt{\dfrac{X^7}{X^7 + 50^7}} \hspace{0.5 cm} IF \hspace{0.5 cm} X == 2C
	https://latexeditor.lagrida.com/
	*/
	double meanCx2to7 = pow_to_7(meanCx2);
	double G = (1 - sqrt( meanCx2to7 / (meanCx2to7 + pow_to_7(50.0)) )) / 2;

	// p suffix stands for "prime" (')
	double a1p = color1.a * (1 + G);
	double a2p = color2.a * (1 + G);

	double C1p = sqrt(a1p * a1p + color1.b * color1.b);
	double C2p = sqrt(a2p * a2p + color2.b * color2.b);


	// Conditional compilation, read more in delta CMC function implementation
	#ifdef __STDC_IEC_559__  			// IEEE supported
		double h1_atan = atan2(color1.b, a1p);
		double h2_atan = atan2(color2.b, a2p);
	#else  					 			// IEEE not supported
		double h1_atan = (a1p == 0 && color1.b == 0) ? 0 : atan2(color1.b, a1p);
		double h2_atan = (a2p == 0 && color2.b == 0) ? 0 : atan2(color2.b, a2p);
	#endif 		


	double h1p = h1_atan >= 0 ? h1_atan : h1_atan + 2 * M_PI;
	double h2p = h2_atan >= 0 ? h2_atan : h2_atan + 2 * M_PI;


	double h1ph2pabs = fabs(h1p - h2p);

	double meanHp;
	if(h1ph2pabs > M_PI){
		meanHp = (h1p + h2p + 2 * M_PI) / 2;
	}else{
		meanHp = (h1p + h2p) / 2;
	}


	double T = 1 + ( 
		- 0.17 * cos(meanHp - rad_from_deg(30))
		+ 0.24 * cos(2 * meanHp)
		+ 0.32 * cos(3 * meanHp + rad_from_deg(6))
		- 0.20 * cos(4 * meanHp - rad_from_deg(63))
	);


	double deltahp;
	if(h1ph2pabs <= M_PI){
		deltahp = h2p - h1p;
	}else if(h2p <= h1p){
		deltahp = h2p - h1p + 2 * M_PI;
	}else{
		deltahp = h2p - h1p - 2 * M_PI;
	}


	// the same transformation was applied to Rc as to G
	// meanCpx2 means "mean c prime times two"
	double meanCpx2 = C1p + C2p;
	double meanCpx2to7 = pow_to_7(meanCpx2);

	double Rc = 2 * sqrt( meanCpx2to7 / (meanCpx2to7 + pow_to_7(50.0)) );

	double meanLpx2 = color1.l + color2.l;

	// (x/2 - 50)^2 == 1/4 * (x- 100)^2 ------> therefore ------> (meanL' - 50)^2 == 1/4 * (2 * meanL - 100)^2
	double Sl = 1 + (
		0.00375 * (meanLpx2 - 100) * (meanLpx2 - 100) / sqrt(20 + 0.25 * (meanLpx2 - 100) * (meanLpx2 - 100))
	);


	double Sc = 1 + meanCpx2 * 0.0225;
	double Sh = 1 + meanCpx2 * 0.0075 * T; 

	double deltaHp = 2 * sqrt(C1p * C2p) * sin(deltahp / 2);
	double deltaCp = C2p - C1p;

	double deltaLp = color2.l - color1.l;


	double deltaTheta = rad_from_deg(30) * exp(
		-(deg_from_rad(meanHp)  - 275) * (deg_from_rad(meanHp)  - 275) / (25*25)
	);

	double Rt = -Rc * sin(2 * deltaTheta);

	double result = sqrt(
		deltaLp * deltaLp / (params.KL * params.KL * Sl * Sl)
		+ 
		deltaCp * deltaCp / (params.KC * params.KC * Sc * Sc)
		+
		deltaHp * deltaHp / (params.KH * params.KH * Sh * Sh)
		+ 
		Rt * deltaCp * deltaHp / (params.KC * Sc * params.KH * Sh)
	);

	return result;
}

double delta_CIEDE2000(lab_t color1, lab_t color2){
	return delta_CIEDE2000_full(color1, color2, CIEDE2000_BASE);
}