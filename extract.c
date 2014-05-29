#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extract.h"

int extract(char *input){
	char *lines = NULL;	
	char BMP_HEADER1[] = "BM";
	char BMP_HEADER2[] = { 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00 };
	char BMP_HEADER3[28] = { 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };	
	char file_path[256] = { 0 };
	char pos_data[256] = { 0 };
	char magic[9] = { 0 };
	size_t i = 0, j = 0, k = 0, l = 0, BMP_BIT = 0;
	size_t extra = 0, BMP_SIZE=0, count = 0, R = 0, G = 0, B = 0;
	raw_image_header RAW_IMAGE_HEADERS[12] = { 0 };
	FILE *I = NULL;
	FILE *O = NULL;

	I = fopen(input, "rb");
	if (I == NULL){
		printf("Could not open %s\n", input);
		return -1;
	}
	printf("Reading headers in %s...", input);
	fread(&magic, sizeof(char), 8, I);
	magic[8] = '\0';
	
	if (!(strcmp(magic, "IMGDATA!") == 0)){
		printf("FAIL!\n%s is not valid file\n", input);
		return -1;
	}

	fseek(I, 0x18, SEEK_SET);
	for (i = 0; i < 12; i++){
		fread(RAW_IMAGE_HEADERS[i].name, sizeof(char), 16, I);
		fread(&RAW_IMAGE_HEADERS[i].width, sizeof(size_t), 1, I);
		fread(&RAW_IMAGE_HEADERS[i].height, sizeof(size_t),1, I);
		fread(&RAW_IMAGE_HEADERS[i].x_pos, sizeof(size_t), 1, I);
		fread(&RAW_IMAGE_HEADERS[i].y_pos, sizeof(size_t), 1, I);
		fread(&RAW_IMAGE_HEADERS[i].offset, sizeof(size_t), 1, I);
		fread(&RAW_IMAGE_HEADERS[i].size, sizeof(size_t), 1, I);
	}
	printf("Done\n");
	for (i = 0; i < 12; i++){
		strcpy(file_path, ".\\images\\");
		strcat(file_path, RAW_IMAGE_HEADERS[i].name);
		strcat(file_path, ".bmp");
		O = fopen(file_path, "wb");
		if (O == NULL){
			printf("\nCould not create %s.bmp.\nMake sure you the \"images\" folder is created\n", RAW_IMAGE_HEADERS[i].name);
			return -1;
		}
		if (4 - ((RAW_IMAGE_HEADERS[i].width * 3) % 4) == 4){
			extra = 0;
		}
		else{
			extra = 4 - ((RAW_IMAGE_HEADERS[i].width * 3) % 4);
		}
		BMP_SIZE = RAW_IMAGE_HEADERS[i].width * RAW_IMAGE_HEADERS[i].height * 3 + 54 + RAW_IMAGE_HEADERS[i].height*extra;
		lines = (char*)malloc(sizeof(char)*(BMP_SIZE - 54));
		fseek(I, RAW_IMAGE_HEADERS[i].offset, SEEK_SET);
		printf("\nConverting %s...", RAW_IMAGE_HEADERS[i].name);
		while (j < RAW_IMAGE_HEADERS[i].size){
			count = fgetc(I);
			R = fgetc(I);
			G = fgetc(I);
			B = fgetc(I);
			while (count != 0){
				lines[BMP_BIT] = B;
				BMP_BIT++;
				lines[BMP_BIT] = G;
				BMP_BIT++;
				lines[BMP_BIT] = R;
				BMP_BIT++;
				if (k == RAW_IMAGE_HEADERS[i].width - 1){
					for (l = 0; l < extra; l++){
						lines[BMP_BIT]=0x00;
						BMP_BIT++;
					}
					k = 0;
				}
				else{
					k++;
				}
				count--;
			}
			j = j + 4;
		}
		printf("Done\n");
		printf("Writing %s.bmp to .\\images...", RAW_IMAGE_HEADERS[i].name);
		fwrite(BMP_HEADER1, sizeof(char), sizeof(BMP_HEADER1)-1, O);
		fwrite(&BMP_SIZE, sizeof(size_t), 1, O);
		fwrite(BMP_HEADER2, sizeof(char), sizeof(BMP_HEADER2), O);
		fwrite(&RAW_IMAGE_HEADERS[i].width, sizeof(size_t), 1, O);
		fwrite(&RAW_IMAGE_HEADERS[i].height, sizeof(size_t), 1, O);
		fwrite(BMP_HEADER3, sizeof(char), sizeof(BMP_HEADER3), O);
		for (j = RAW_IMAGE_HEADERS[i].width * 3 + extra; j != (BMP_SIZE - 54 + RAW_IMAGE_HEADERS[i].width * 3 + extra); j = j + RAW_IMAGE_HEADERS[i].width * 3 + extra){
			for (k = 0; k <= RAW_IMAGE_HEADERS[i].width * 3 + extra - 1; k++){
				fputc(lines[BMP_SIZE - 54 - j + k], O);
			}
		}
		j = 0;
		k = 0;
		BMP_BIT = 0;
		free(lines);
		fclose(O);
		printf("Done\n");
	}
	O = fopen(".\\images\\pos.txt", "wb");
	if (O == NULL){
			printf("\nCould not create pos.txt.\nMake sure you have the \"images\" folder\n");
			return -1;
		}
	printf("\nWriting .\\images\\pos.txt...");
	for (i = 0; i < 12; i++){
		strcpy(file_path, RAW_IMAGE_HEADERS[i].name);
		fwrite(file_path, sizeof(char), strlen(file_path), O);
		fputc(0x20, O);
		_itoa(RAW_IMAGE_HEADERS[i].x_pos, pos_data, 10);
		fwrite(pos_data, sizeof(char), strlen(pos_data), O);
		fputc('x', O);
		_itoa(RAW_IMAGE_HEADERS[i].y_pos, pos_data, 10);
		fwrite(pos_data, sizeof(char), strlen(pos_data), O);
		fputc(0x0D, O);
		fputc(0x0A, O);
	}
	printf("Done\n");
	fclose(O);
	fclose(I);
	return 0;
}