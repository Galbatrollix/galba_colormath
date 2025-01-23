
#include <stdio.h>
#include "src/galba_colormath.h"

static void print_rgb(rgb_t p){
    printf("%u %u %u\n", p.r, p.g, p.b);
}


int main(){
	char buffer[20];
	HEX_from_RGB_2(buffer,(rgb_t){15,133,255});
	puts(buffer);
	char string[] = "#ff1134";

	print_rgb(RGB_from_HEX(string+1));

}