#include "galba_colormath.h"

#include <math.h>


/*

	Int encoding related functions:

*/

// Least significant byte of int32 representation is interpretted as blue channel.
// Second least significant byte of int32 repr is interpretted as green channel.
// Third least significant byte of int32 repr is interpretted as red channel.
// Most significant byte is ignored and has no effect on the interpretation as RGB
rgb_t RGB_from_i32(int32_t int_repr){
	return (rgb_t){
		.r = (unsigned char) (int_repr >> 16) & 0xFF, 
		.g = (unsigned char) (int_repr >> 8 ) & 0xFF,
		.b = (unsigned char) (int_repr >> 0 ) & 0xFF
	};
}

int32_t i32_from_RGB(rgb_t rgb_input){
 	return (int32_t)rgb_input.r * 256 * 256 + (int32_t)rgb_input.g * 256 + rgb_input.b; 
}

/*

	Hex string encoding related internal functions:

*/

static unsigned char hex_letter_to_right_halfbyte(unsigned char letter){
	if(letter <= '9' && letter >= '0'){
		return letter - '0';
	}
	if(letter <= 'F' && letter >= 'A'){
		return letter - 'A' + 10;
	}
	if(letter <= 'f' && letter >= 'a'){
		return letter - 'a' + 10;
	}
	return 0;

}

static char right_halfbyte_to_hex_letter(unsigned char halfbyte){
	static const char lookup[17] = "0123456789ABCDEF";
	// unsafe, be sure this function is called with halfbyte no larger than 15.
	return lookup[halfbyte]; 
}


int32_t i32_from_HEX(const char hex_arr[6]){
	int32_t int_repr = 0;

	for(int i=0; i<6; i++){
		unsigned char halfbyte = hex_letter_to_right_halfbyte(hex_arr[i]);

		int_repr <<= 4;
		int_repr += halfbyte;
	}
	return int_repr;
}
void HEX_from_i32(char arr_buffer[6], int32_t int_repr){
	for(int i=0; i<6; i++){
	    unsigned char halfbyte = (int_repr >> (4 * i)) & 0x0F;
		arr_buffer[5 - i] = right_halfbyte_to_hex_letter(halfbyte);
	}
}



rgb_t RGB_from_HEX(const char hex_arr[6]){
	int32_t int_repr = i32_from_HEX(hex_arr);
	return RGB_from_i32(int_repr);
}



void HEX_from_RGB(char arr_buffer[6], rgb_t rgb_input){
	int32_t int_repr = i32_from_RGB(rgb_input);
	HEX_from_i32(arr_buffer, int_repr);
}


void HEX_from_RGB_2(char string_buffer[8], rgb_t rgb_input){
	string_buffer[0] = '#';
	string_buffer[7] = '\0';
	HEX_from_RGB(string_buffer + 1, rgb_input);
}

void HEX_from_i32_2(char string_buffer[8], int32_t int_repr){
	string_buffer[0] = '#';
	string_buffer[7] = '\0';
	HEX_from_i32(string_buffer + 1, int_repr);
}



#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

/*

	LAB XYZ RGB conversion functions

*/

// RGB to XYZ and other way around conversions logic from here
// https://www.image-engineering.de/library/technotes/958-how-to-convert-between-srgb-and-ciexyz
xyz_t XYZ_from_RGB(rgb_t rgb_input){

	// step 1: calculating RGB' 
	double rgb_prime[3] = {rgb_input.r, rgb_input.g, rgb_input.b};
	for(int i=0; i<3; i++){
	    // linearize data to range <0.0, 1.0>
	    rgb_prime[i] /= 255; 
	    
		if(rgb_prime[i] <= 0.04045){
			rgb_prime[i] /= 12.92;
		}else{
			double power_base = (rgb_prime[i] + 0.055) / 1.055;
			rgb_prime[i] = pow(power_base, 2.4);
		}
		rgb_prime[i] *= 100;
	}

	// step 2: apply transformation matrix on RGB'
    double R_p = rgb_prime[0];
	double G_p = rgb_prime[1];
	double B_p = rgb_prime[2];

	xyz_t result;
	result.x = R_p * 0.4124564 + G_p * 0.3575761 + B_p * 0.1804375;
	result.y = R_p * 0.2126729 + G_p * 0.7151522 + B_p * 0.0721750;
	result.z = R_p * 0.0193339 + G_p * 0.1191920 + B_p * 0.9503041;

	return result;

}



rgb_t RGB_from_XYZ(xyz_t xyz_input){
	xyz_input.x /= 100;
	xyz_input.y /= 100;
	xyz_input.z /= 100;

	double R_p = xyz_input.x *  3.2404542 + xyz_input.y * -1.5371385 + xyz_input.z * -0.4985314;
	double G_p = xyz_input.x * -0.9692660 + xyz_input.y *  1.8760108 + xyz_input.z *  0.0415560;
	double B_p = xyz_input.x *  0.0556434 + xyz_input.y * -0.2040259 + xyz_input.z *  1.0572252;

	double rgb_prime[3] = {R_p, G_p, B_p};
	for(int i=0; i<3; i++){
		if(rgb_prime[i] <= 0.0031308){
			rgb_prime[i] *= 12.92;
		}else{
			rgb_prime[i] = 1.055 * pow(rgb_prime[i], 1/2.4) - 0.055;
		}

		rgb_prime[i] *= 255;
	}

	rgb_t result = (rgb_t){
		.r = (unsigned char) MAX(MIN(rgb_prime[0]+ 0.5, 255.0), 0.0),
		.g = (unsigned char) MAX(MIN(rgb_prime[1]+ 0.5, 255.0), 0.0),
		.b = (unsigned char) MAX(MIN(rgb_prime[2]+ 0.5, 255.0), 0.0)
	};
	return result;


}


// http://www.brucelindbloom.com/index.html?LContinuity.html
// http://brucelindbloom.com/index.html?Eqn_RGB_XYZ_Matrix.html

#define LAB_XYZ_e (216.0 / 24389)
#define LAB_XYZ_k (24389 / 27.0)
#define LAB_XYZ_k_by_116 (24389/(27.0 * 116))



//http://www.brucelindbloom.com/index.html?Eqn_Lab_to_XYZ.html
xyz_t XYZ_from_LAB(lab_t lab_input){
	double Fy = (lab_input.l + 16) / 116;
	double Fz = Fy - (lab_input.b / 200);
	double Fx = Fy + (lab_input.a / 500);

	// its weird but the source's formula says different computation is made for Yr
	// probably some big IQ math im too uneducated to comprehend
	double Xr, Yr, Zr;
	if(lab_input.l > LAB_XYZ_k * LAB_XYZ_e){
		double temp = (lab_input.l + 16) / 116;
		Yr = temp*temp*temp;
	}else{
		Yr = lab_input.l / LAB_XYZ_k;
	}

	if( Fx*Fx*Fx > LAB_XYZ_e){
		Xr = Fx*Fx*Fx;
	}else{
		Xr = (116 * Fx - 16) / LAB_XYZ_k;
	}

	if( Fz*Fz*Fz > LAB_XYZ_e){
		Zr = Fz*Fz*Fz;
	}else{
		Zr = (116 * Fz - 16) / LAB_XYZ_k;
	}

	// D65 white reference point
	return (xyz_t){
		.x = Xr * 95.047,
		.y = Yr * 100.0,
		.z = Zr * 108.883,
	};
}

// http://www.brucelindbloom.com/index.html?Eqn_XYZ_to_Lab.html
lab_t LAB_from_XYZ(xyz_t xyz_input){
	// D65 white reference point
	double ref_xyz[3] = {
		xyz_input.x / 95.047,
		xyz_input.y / 100.0,
	 	xyz_input.z / 108.883
	};
	double fxyz[3];
	for(int i=0; i<3; i++){
		if(ref_xyz[i] > LAB_XYZ_e){
			fxyz[i] = pow(ref_xyz[i], 1/3.0);
		}else{
			fxyz[i] = LAB_XYZ_k_by_116 * ref_xyz[i] + (16.0 / 116);
		}
	}

	return (lab_t){
		.l = 116 * fxyz[1] - 16,			// L = 116 * Fy - 16
		.a = 500 * (fxyz[0] - fxyz[1]),		// a = 500 * (Fx - Fy)
		.b = 200 * (fxyz[1] - fxyz[2])		// b = 200 * (Fy - Fz)
	};
}


rgb_t RGB_from_LAB(lab_t lab_input){
	return RGB_from_XYZ(XYZ_from_LAB(lab_input));
}
lab_t LAB_from_RGB(rgb_t rgb_input){
	return LAB_from_XYZ(XYZ_from_RGB(rgb_input));
}

