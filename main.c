#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extract.h"
#include "build.h"
/*
#include <time.h>
#include <Windows.h>
void random_bmp(){
	char R = 0, G = 0, B = 0,c = 0;
	char BMP_HEADER1[] = "BM";
	char BMP_HEADER2[] = { 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00 };
	char BMP_HEADER3[28] = { 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	char file_path[256] = { 0 };
	char *names[] = { "boot", "charger", "unlocked", "start", "bootloader", "recovery", "poweroff", "fastboot_op", "oem_unlock", "unlock_yes", "unlock_no", "downloadmode" };
	size_t extra = 0, BMP_SIZE = 0;
	FILE *O= NULL;
	size_t i = 0, j = 0, k = 0, l = 0, m = 0, width = 0, height = 0;
	srand(time(NULL));
	for (i = 0; i < 12; i++){
		strcpy(file_path, ".\\images\\");
		strcat(file_path, names[i]);
		strcat(file_path, ".bmp");
		O = fopen(file_path, "wb");
		width = rand() % 1080 + 1;
		height = rand() % 1920 + 1;
		if (4 - ((width * 3) % 4) == 4){
			extra = 0;
		}
		else{
			extra = 4 - ((width * 3) % 4);
		}
		BMP_SIZE = width * height * 3 + 54 + height * extra;
		fwrite(BMP_HEADER1, sizeof(char), sizeof(BMP_HEADER1)-1, O);
		fwrite(&BMP_SIZE, sizeof(size_t), 1, O);
		fwrite(BMP_HEADER2, sizeof(char), sizeof(BMP_HEADER2), O);
		fwrite(&width, sizeof(size_t), 1, O);
		fwrite(&height, sizeof(size_t), 1, O);
		fwrite(BMP_HEADER3, sizeof(char), sizeof(BMP_HEADER3), O);
		j = 0;
		while (j < BMP_SIZE - 54){
			c = rand() % 256;
			for (k = (rand() % 2073600 + 1); k != 0; k--){
				if (j == BMP_SIZE - 54){
					break;
				}
				fputc(c, O);
				fputc(c, O);
				fputc(c, O);
				if (extra != 0 && (m+3)==width*3){
					for (l = 0; l != (extra); l++){
						fputc(0x00, O);
					}
					j = j + 3 + extra;
					m = 0;
				}
				else{
					j = j + 3;
					if (extra != 0){
						m = m + 3;
					}
				}
			}
		}
		fclose(O);
	}
}
*/
int main(int argc, char **argv){
	size_t i = 0, j = 0;
	
	if (argc == 1){
		printf("\n%s extract <imgdata file>\n%s build-imgdata\n", argv[0], argv[0]);
		return 0;
	}

	if (argc > 3){
		printf("\n%s extract <imgdata file>\n%s build-imgdata\n", argv[0], argv[0]);
		if (((strcmp(argv[1], "extract") == 0) || (strcmp(argv[1], "build-imgdata") == 0)) == 0){
			printf("\nInvalid argument: %s\n", argv[1]);
		}
		printf("\nToo much arguments\n");
		return 0;
	}
	/*	if ((strcmp(argv[1], "debug") == 0) && argc == 3){
		j = atoi(argv[2]);
		for (i =0; i < j; i++){
			printf("\n                    ----------------------------------------\n                                       %Iu\n                    ----------------------------------------\n\n", i+1);
			random_bmp();
			if (build_imgdata() == -1){
				printf("BUILD ERROR");
				break;
			}
			if (extract("new-imgdata.img") == -1){
				printf("EXTRACT ERROR");
				break;
			}
		}
		return 0;
	}
	*/
	if (((strcmp(argv[1], "extract") == 0) || (strcmp(argv[1], "build-imgdata") == 0)) == 0){
		printf("\n%s extract <imgdata file>\n%s build-imgdata\n\nInvalid argument: %s \n", argv[0], argv[0], argv[1]);
		return 0;
	}
	if ((strcmp(argv[1], "extract") == 0) && argc == 3){
		extract(argv[2]);
	}
	else{
		if (!(strcmp(argv[1], "build-imgdata") == 0)){
			printf("\n%s extract <imgdata file>\n%s build-imgdata\n\nToo few arguments\n", argv[0], argv[0]);
		}
	}
	if ((strcmp(argv[1], "build-imgdata") == 0) && argc == 2){
		build_imgdata();
	}
	else{
		if (!(strcmp(argv[1], "extract") == 0)){
			printf("\n%s extract <imgdata file>\n%s build-imgdata\n\nToo much arguments\n", argv[0], argv[0]);
		}
	}
	return 0;
}