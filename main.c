#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extract.h"

int main(int argc, char **argv){

	char IMGDATA_HEADER[] = { 'I','M','G','D','A','T','A','!' , 0x01, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	char debug[] = "bootloader.imgdata.img";
	extract(debug);
	if (argc == 1){
		printf("\n%s extract <imgdata file>\n%s build_imgdata\n", argv[0], argv[0]);
		return 0;
	}

	if (argc > 3){
		printf("\n%s extract <imgdata file>\n%s build_imgdata\n", argv[0], argv[0]);
		if (((strcmp(argv[1], "extract") == 0) || (strcmp(argv[1], "build_imgdata") == 0)) == 0){
			printf("\nInvalid argument: %s\n", argv[1]);
		}
		printf("\nToo much arguments\n");
		return 0;
	}
	if (((strcmp(argv[1], "extract") == 0) || (strcmp(argv[1], "build_imgdata") == 0)) == 0){
		printf("\n%s extract <imgdata file>\n%s build_imgdata\n\nInvalid argument: %s \n", argv[0], argv[0], argv[1]);
		return 0;
	}
	if ((strcmp(argv[1], "extract") == 0) && argc == 3){
		extract(argv[2]);
	}
	else{
		if (!(strcmp(argv[1], "build_imgdata") == 0)){
			printf("\n%s extract <imgdata file>\n%s build_imgdata\n\nToo few arguments\n", argv[0], argv[0]);
		}
	}
	if ((strcmp(argv[1], "build_imgdata") == 0) && argc == 2){
		//build_imgdata();
	}
	else{
		if (!(strcmp(argv[1], "extract") == 0)){
			printf("\n%s extract <imgdata file>\n%s build_imgdata\n\nToo much arguments\n", argv[0], argv[0]);
		}
	}
	return 0;
}