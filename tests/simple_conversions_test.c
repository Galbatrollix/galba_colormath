#include "tests.h"

#include "galba_colormath.h"

#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include <string.h>


/*
	Helper functions for testing
*/
static bool rgb_equals(rgb_t l, rgb_t r){
	return l.r == r.r && l.g == r.g && l.b == r.b;
}

static void print_rgb(rgb_t p){
    printf("rgb(%u %u %u)", p.r, p.g, p.b);
}
// DISREGARDS NULL TERMINATOR BY DESIGN
static void to_uppercase_n(char * arr, size_t size){
	for(size_t i = 0; i<size; i++){
		if(arr[i] >= 'a' && arr[i] <= 'z'){
			arr[i] = arr[i] + 'A' - 'a';
		}
	}
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
		number += 1;

		// inner uint32_t cast is necessary otherwise shifting 255 by 24 in a signed 32 integer will cause UB
		int32_t most_significant_byte_mask = (int32_t)((uint32_t)0xFF << (8 * 3));
		rgb_t rgb_last_3_bytes = RGB_from_i32(number & ~most_significant_byte_mask);
		rgb_t rgb_normal = RGB_from_i32(number);
		if(! rgb_equals(rgb_normal, rgb_last_3_bytes)){	
			printf("Test test_i32_rgb_conversion1 FAILED at: %" PRId32"\n", number);
			return false;
		}
	}while(number != INT32_MAX);



	for(number = INT32_MIN; number<0; number++){
		int32_t most_significant_byte_mask = (int32_t)((uint32_t)0xFF << (8 * 3));
		rgb_t rgb_last_3_bytes = RGB_from_i32(number & ~most_significant_byte_mask);
		rgb_t rgb_normal = RGB_from_i32(number);
		if(! rgb_equals(rgb_normal, rgb_last_3_bytes)){	
			printf("Test test_i32_rgb_conversion1 FAILED at: %" PRId32 "\n", number);
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


#define HEX_CASES_LEN 6
const rgb_t HEXRGB_CASES[HEX_CASES_LEN] = {
	{0x00, 0x1f, 0x12},
	{0x13, 0x31, 0x51},
	{0xFF, 0xFF, 0xFF},
	{0xF1, 0xF1, 0xF1},
	{0x10, 0x00, 0x00},
	{0xab, 0xcd, 0xfa}
};
const char HEX_PATTERNS[HEX_CASES_LEN][10] = {"#001f12", "#133151", "#FFFFFF", "#F1F1F1", "#100000", "#abcdfa"};
const int32_t HEXINT_CASES[HEX_CASES_LEN] = {0x001f12, 0x133151, 0xFFFFFF, 0xF1F1F1, 0x100000, 0xabcdfa};
const int32_t HEXINT_BEYOND_CASES[HEX_CASES_LEN] = { 
		HEXINT_CASES[0] | ((int32_t)0xff << 8*3),
		HEXINT_CASES[1] | ((int32_t)0xfa << 8*3),
		HEXINT_CASES[2] | ((int32_t)0xfc << 8*3),
		HEXINT_CASES[3] | ((int32_t)0xa1 << 8*3),
		HEXINT_CASES[4] | ((int32_t)0x0c << 8*3),
		HEXINT_CASES[5] | ((int32_t)0x11 << 8*3),
};

const char HEX_NONULL[HEX_CASES_LEN][6] = {
		{'0', '0', '1', 'f', '1', '2'},
		{'1', '3', '3', '1', '5', '1'},
		{'f', 'f', 'f', 'f', 'f', 'f'},
		{'f', '1', 'f', '1', 'f', '1'},
		{'1', '0', '0', '0', '0', '0'},
		{'a', 'b', 'c', 'd', 'f', 'a'},
};

const char HEX_WRONGCHARS[HEX_CASES_LEN][10] = {"#XH1f12", "#133151", "#FFFFFF", "#F1F1F1", "#1\0\0000", "#abcdfa"};

// some happy cases of integers in range of rgb and their coresponding hex patterns
bool test_i32_hex_conversion_happy1(void){
	for(int i=0; i<HEX_CASES_LEN; i++){
		char as_upper[10];
		memcpy(as_upper, HEX_PATTERNS[i], 10);
		to_uppercase_n(as_upper, 10);

		char buffer[10];
		memcpy(buffer, HEX_PATTERNS[i], 10);

		int32_t expected = HEXINT_CASES[i];
		int32_t from_pattern = i32_from_HEX(buffer + 1);
		int32_t from_uppercase = i32_from_HEX(as_upper + 1);
		if(expected != from_pattern || from_pattern != from_uppercase){
			printf("Test test_i32_hex_conversion_happy1 FAILED at: %" PRId32"\n", expected);
			return false;
		}

	}
	puts("SUCCESS");
	return true;
}

bool test_i32_hex_conversion_happy2(void){
	for(int i=0; i<HEX_CASES_LEN; i++){
		char as_upper[10];
		memcpy(as_upper, HEX_PATTERNS[i], 10);
		to_uppercase_n(as_upper, 10);

		char buffer[10];
		memset(buffer, 'V', 10);
		HEX_from_i32(buffer, HEXINT_CASES[i]);
		if(buffer[6] != 'V'){
			printf("Test test_i32_hex_conversion_happy2 FAILED at: %" PRId32"\n"
					"Function HEX_from_i32 written past 6th byte\n",HEXINT_CASES[i]);
			return false;
		}

		char str_buffer[10];
		memset(str_buffer, 'V', 10);
		HEX_from_i32_2(str_buffer, HEXINT_CASES[i]);
		if(str_buffer[0] != '#'){
			printf("Test test_i32_hex_conversion_happy2 FAILED at: %" PRId32"\n"
					"Function HEX_from_i32_2 didnt write '#' \n", HEXINT_CASES[i]);
			return false;
		}
		if(str_buffer[7] != '\0'){
			printf("Test test_i32_hex_conversion_happy2 FAILED at: %" PRId32"\n"
					"Function HEX_from_i32_2 didnt write null terminator \n", HEXINT_CASES[i]);
			return false;
		}

		int cmp_result = memcmp(buffer, str_buffer + 1, 6);
		if(cmp_result != 0){
			printf("Test test_i32_hex_conversion_happy2 FAILED at: %" PRId32"\n"
					"Functions HEX_from_i32_2 and HEX_from_i32 wrote different hex\n", HEXINT_CASES[i]);
			return false;
		}

		cmp_result = memcmp(as_upper, str_buffer, 7);
		if(cmp_result != 0){
			printf("Test test_i32_hex_conversion_happy2 FAILED\n"
					"Functions HEX_from_i32_2 and HEX_from_i32 didnt write in uppercase\n");
			return false;
		}

	}
	puts("SUCCESS");
	return true;
}

bool test_rgb_hex_conversion_happy1(void){
	for(int i=0; i<HEX_CASES_LEN; i++){
		char as_upper[10];
		memcpy(as_upper, HEX_PATTERNS[i], 10);
		to_uppercase_n(as_upper, 10);

		char buffer[10];
		memcpy(buffer, HEX_PATTERNS[i], 10);

		rgb_t expected = HEXRGB_CASES[i];
		rgb_t from_pattern = RGB_from_HEX(buffer + 1);
		rgb_t from_uppercase = RGB_from_HEX(as_upper + 1);
		if(!rgb_equals(expected, from_pattern) || !rgb_equals(from_pattern, from_uppercase)){
			printf("Test test_i32_hex_conversion_happy1 FAILED at: ");
			print_rgb(HEXRGB_CASES[i]);
			putchar('\n');
			return false;
		}

	}
	puts("SUCCESS");
	return true;

}
bool test_rgb_hex_conversion_happy2(void){
	for(int i=0; i<HEX_CASES_LEN; i++){
		char as_upper[10];
		memcpy(as_upper, HEX_PATTERNS[i], 10);
		to_uppercase_n(as_upper, 10);

		char buffer[10];
		memset(buffer, 'V', 10);
		HEX_from_RGB(buffer, HEXRGB_CASES[i]);
		if(buffer[6] != 'V'){
			printf("Test test_rgb_hex_conversion_happy2 FAILED at: ");
			print_rgb(HEXRGB_CASES[i]);
			printf("\nFunction HEX_from_RGB written past 6th byte\n");
			return false;

		}

		char str_buffer[10];
		memset(str_buffer, 'V', 10);
		HEX_from_RGB_2(str_buffer, HEXRGB_CASES[i]);
		if(str_buffer[0] != '#'){
			printf("Test test_rgb_hex_conversion_happy2 FAILED at: ");
			print_rgb(HEXRGB_CASES[i]);
			printf("\nFunction HEX_from_RGB_2 didnt write '#'\n");
			return false;
		}
		if(str_buffer[7] != '\0'){
			printf("Test test_rgb_hex_conversion_happy2 FAILED at: ");
			print_rgb(HEXRGB_CASES[i]);
			printf("\nFunction HEX_from_RGB_2 didnt write null terminator\n");
			return false;
		}

		int cmp_result = memcmp(buffer, str_buffer + 1, 6);
		if(cmp_result != 0){
			printf("Test test_rgb_hex_conversion_happy2 FAILED at: ");
			print_rgb(HEXRGB_CASES[i]);
			printf("\nFunctions HEX_from_RGB_2 and HEX_from_RGB wrote different hex\n");
			return false;
		}

		cmp_result = memcmp(as_upper, str_buffer, 7);
		if(cmp_result != 0){
			printf("Test test_rgb_hex_conversion_happy2 FAILED at: ");
			print_rgb(HEXRGB_CASES[i]);
			printf("\nFunctions HEX_from_RGB_2 and HEX_from_RGB didnt write in uppercase\n");
			return false;
		}

	}
	puts("SUCCESS");
	return true;
}


bool test_hex_conversion_unusual1(void){
	for(int i=0; i<HEX_CASES_LEN; i++){
		int32_t from_nonull = i32_from_HEX(HEX_NONULL[i]);
		int32_t from_wrongchars = i32_from_HEX(HEX_WRONGCHARS[i] + 1);
		int32_t expected = HEXINT_CASES[i];
		if(expected != from_nonull || expected != from_wrongchars){
			printf("Test test_hex_conversion_unusual1 FAILED at: %" PRId32"\n", expected);
			return false;
		}

	}
	puts("SUCCESS");
	return true;
}
bool test_hex_conversion_unusual2(void){
	for(int i=0; i<HEX_CASES_LEN; i++){
		char buffer[10];
		HEX_from_i32(buffer, HEXINT_BEYOND_CASES[i]);
		char str_buffer[10];
		HEX_from_i32_2(str_buffer, HEXINT_BEYOND_CASES[i]);

		int32_t converted_back2 = i32_from_HEX(str_buffer + 1);
		int32_t converted_back = i32_from_HEX(buffer);
		int32_t expected = HEXINT_CASES[i];
		if(converted_back != HEXINT_CASES[i] || converted_back2 != HEXINT_CASES[i]){
			printf("Test test_hex_conversion_unusual2 FAILED at: %" PRId32"\n", expected);
			return false;
		}
	}
	puts("SUCCESS");
	return true;
}

bool test_hex_conversion_unusual3(void){
	for(int i=0; i<HEX_CASES_LEN; i++){
		rgb_t from_nonull = RGB_from_HEX(HEX_NONULL[i]);
		rgb_t from_wrongchars = RGB_from_HEX(HEX_WRONGCHARS[i] + 1);
		rgb_t expected = HEXRGB_CASES[i];
		if(!rgb_equals(from_nonull, expected) || !rgb_equals(from_wrongchars, expected)){

			printf("Test test_hex_conversion_unusual3 FAILED at: ");
			print_rgb(expected);
			puts("");
			return false;
		}

	}
	puts("SUCCESS");
	return true;
}
