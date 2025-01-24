#include "tests.h"

#include "galba_colormath.h"

#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

static bool rgb_equals(rgb_t l, rgb_t r){
	return l.r == r.r && l.g == r.g && l.b == r.b;
}

static void print_rgb(rgb_t p){
    printf("rgb(%u %u %u)", p.r, p.g, p.b);
}


// tests if all RGB values map correctly to int32_t and back and forth
bool test_i32_rgb_conversion1(void){
	int32_t counter = 0;

	for(int R = 0; R<256 ; R++){
		for (int G = 0; G<256; G++){
			for(int B = 0; B <256; B++){
				rgb_t rgb = {R, G, B};
				rgb_t from_counter = RGB_from_i32(counter);
				int32_t from_rgb = i32_from_RGB(rgb);

				if(counter != from_rgb || !rgb_equals(rgb, from_counter)){

					printf("Test test_i32_rgb_conversion1 FAILED at: ");
					print_rgb(rgb);
					puts("");

					return false;
				}
				counter += 1;
			}
		}
	}

	puts("SUCCESS");
	return true;

}

// tests if conversion behaves correctly for integers outside range <0, 256^3)
bool test_i32_rgb_conversion2(void){
	int32_t number = (int32_t)256 * 256 * 256 - 1;

	do{
		int32_t most_significant_byte_mask = (int32_t)0xFF << (8 * 3);
		rgb_t rgb_last_3_bytes = RGB_from_i32(number & ~most_significant_byte_mask);
		rgb_t rgb_normal = RGB_from_i32(number);
		if(! rgb_equals(rgb_normal, rgb_last_3_bytes)){	
			printf("Test test_i32_rgb_conversion1 FAILED at: %" PRId32"\n", number);
			return false;
		}
		number += 1;
	}while(number != INT32_MAX);



	for(number = INT32_MIN; number<0; number++){
		int32_t most_significant_byte_mask = (int32_t)0xFF << (8 * 3);
		rgb_t rgb_last_3_bytes = RGB_from_i32(number & ~most_significant_byte_mask);
		rgb_t rgb_normal = RGB_from_i32(number);
		if(! rgb_equals(rgb_normal, rgb_last_3_bytes)){	
			printf("Test test_i32_rgb_conversion1 FAILED at: %" PRId32"\n", number);
			return false;
		}
	}

	puts("SUCCESS");
	return true;

}

// tests some select cases in and outside of range
bool test_i32_rgb_conversion3(void){
	int32_t numbers[6] = {10,  (int32_t)0xff11ff, (int32_t)256*256*256 , INT32_MAX, INT32_MIN, 0};
	rgb_t expected[6] = {
		{0,0,10},
		{255,17,255},
		{0,0,0},
		{255,255,255},
		{0,0,0},
		{0,0,0}
	};

	for(int i = 0; i < 6 ; i++){
		if(! rgb_equals(expected[i], RGB_from_i32(numbers[i]))){
			printf("Test test_i32_rgb_conversion1 FAILED at: %" PRId32"\n", numbers[i]);
			return false;
		}
	}
	puts("SUCCESS");
	return true;
}