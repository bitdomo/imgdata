#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extract.h"

int extract(char *in, char *out){
	char *lines = NULL; // Stores the BMP pixels with the extra bytes for an image, but the lines not in BMP order
//////////// BMP headers
	char BMP_HEADER1[] = "BM";
	char BMP_HEADER2[] = { 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00 };
	char BMP_HEADER3[28] = { 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
/////////////////////////
	char file_path[256] = { 0 };
	char pos_data[256] = { 0 };
	char input[256] = { 0 };
	char output[256] = { 0 };
	char magic[9] = { 0 };	// For checking the imgdata file
	unsigned int i = 0, j = 0, k = 0, l = 0;
	unsigned int BMP_BYTE = 0;
	unsigned int extra = 0, BMP_SIZE=0, count = 0, R = 0, G = 0, B = 0;
	unsigned int entries = 0;
	raw_image_header *RAW_IMAGE_HEADERS = NULL;
	FILE *I;
	FILE *O;

	strcpy(input, in);
	strcpy(output, out);

#if defined(_WIN32) || defined(WIN32)
    if (strchr(input, '/') != NULL){
        i = 0;
        while (input[i] != 0) {
            if (input[i] == '/'){
                input[i] = '\\';
            }
            i++;
        }
    }
#else
    if (strchr(input, '\\') != NULL){
        i = 0;
        while (input[i] != 0) {
            if (input[i] == '\\'){
                input[i] = '/';
            }
            i++;
        }
    }
#endif
#if defined(_WIN32) || defined(WIN32)
    if (strchr(output, '/') != NULL){
        i = 0;
        while (output[i] != 0) {
            if (output[i] == '/'){
                output[i] = '\\';
            }
            i++;
        }
    }
#else
    if (strchr(output, '\\') != NULL){
        i = 0;
        while (output[i] != 0) {
            if (output[i] == '\\'){
                output[i] = '/';
            }
            i++;
        }
    }
#endif
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
	fseek(I, 0x0C, SEEK_SET);
	fread(&entries, sizeof(unsigned int), 1, I);
	fseek(I, 0x18, SEEK_SET);
	RAW_IMAGE_HEADERS = (raw_image_header*)malloc(sizeof(raw_image_header)*entries);
	for (i = 0; i < entries; i++){	// Reads the image headers from imgdata
		fread(RAW_IMAGE_HEADERS[i].name, sizeof(char), 16, I);
		fread(&RAW_IMAGE_HEADERS[i].width, sizeof(unsigned int), 1, I);
		fread(&RAW_IMAGE_HEADERS[i].height, sizeof(unsigned int),1, I);
		fread(&RAW_IMAGE_HEADERS[i].x_pos, sizeof(unsigned int), 1, I);
		fread(&RAW_IMAGE_HEADERS[i].y_pos, sizeof(unsigned int), 1, I);
		fread(&RAW_IMAGE_HEADERS[i].offset, sizeof(unsigned int), 1, I);
		fread(&RAW_IMAGE_HEADERS[i].size, sizeof(unsigned int), 1, I);
	}
	printf("Done\n");
	for (i = 0; i < entries; i++){	// Converts the RLE formated images to BMP pixels
        strcpy(file_path, output);
#if defined(_WIN32) || defined(WIN32)
        if (file_path[strlen(file_path)] != '\\'){
            strcat(file_path, "\\");
        }
#else
        if (file_path[strlen(file_path)] != '/'){
            strcat(file_path, "/");
        }
#endif
		strcat(file_path, RAW_IMAGE_HEADERS[i].name);
		strcat(file_path, ".bmp");
		O = fopen(file_path, "wb");
		if (O == NULL){
			printf("\nCould not create %s.bmp.\nMake sure the \"%s\" path is valid\n", RAW_IMAGE_HEADERS[i].name, output);
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
				lines[BMP_BYTE] = B;
				BMP_BYTE++;
				lines[BMP_BYTE] = G;
				BMP_BYTE++;
				lines[BMP_BYTE] = R;
				BMP_BYTE++;
				if (k == RAW_IMAGE_HEADERS[i].width - 1){
					for (l = 0; l < extra; l++){
						lines[BMP_BYTE]=0x00;
						BMP_BYTE++;
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
		printf("Writing %s.bmp...", RAW_IMAGE_HEADERS[i].name);
/////////////////// Writes the BMP file header /////////////////////
		fwrite(BMP_HEADER1, sizeof(char), sizeof(BMP_HEADER1)-1, O);
		fwrite(&BMP_SIZE, sizeof(unsigned int), 1, O);
		fwrite(BMP_HEADER2, sizeof(char), sizeof(BMP_HEADER2), O);
		fwrite(&RAW_IMAGE_HEADERS[i].width, sizeof(unsigned int), 1, O);
		fwrite(&RAW_IMAGE_HEADERS[i].height, sizeof(unsigned int), 1, O);
		fwrite(BMP_HEADER3, sizeof(char), sizeof(BMP_HEADER3), O);
/////////////////////////////////////////////////////////////////////
		for (j = RAW_IMAGE_HEADERS[i].width * 3 + extra; j != (BMP_SIZE - 54 + RAW_IMAGE_HEADERS[i].width * 3 + extra); j = j + RAW_IMAGE_HEADERS[i].width * 3 + extra){
			for (k = 0; k <= RAW_IMAGE_HEADERS[i].width * 3 + extra - 1; k++){
				fputc(lines[BMP_SIZE - 54 - j + k], O);	// Writes out the BMP pixels in BMP line order
			}
		}
		j = 0;
		k = 0;
		BMP_BYTE = 0;
		free(lines);
		fclose(O);
		printf("Done\n");
	}
    strcpy(file_path, output);
#if defined(_WIN32) || defined(WIN32)
    if (file_path[strlen(file_path)] != '\\'){
        strcat(file_path, "\\");
    }
#else
    if (file_path[strlen(file_path)] != '/'){
        strcat(file_path,  "/");
    }
#endif
    strcat(file_path, "pos.txt");
	O = fopen(file_path, "wb");
	if (O == NULL){
        printf("\nCould not create pos.txt.\nMake sure the \"%s\" path is valid\n", output);
        return -1;
    }
	printf("\nWriting pos.txt...");
	strcpy(file_path, "entries=");
	fwrite(file_path, sizeof(char), strlen(file_path), O);
#if defined(_WIN32) || defined(WIN32)
	_itoa(entries, pos_data, 10);
#else
	snprintf(pos_data, 5, "%u", entries);
#endif
	fwrite(pos_data, sizeof(char), strlen(pos_data), O);
	fputc(0x0D, O);
	fputc(0x0A, O);
	for (i = 0; i < entries; i++){	// Writes out the pos.txt to the given path
		strcpy(file_path, RAW_IMAGE_HEADERS[i].name);
		fwrite(file_path, sizeof(char), strlen(file_path), O);
		fputc(0x20, O);
#if defined(_WIN32) || defined(WIN32)
		_itoa(RAW_IMAGE_HEADERS[i].x_pos, pos_data, 10);
#else
		snprintf(pos_data,5,"%u",RAW_IMAGE_HEADERS[i].x_pos);
#endif
		fwrite(pos_data, sizeof(char), strlen(pos_data), O);
		fputc('x', O);
#if defined(_WIN32) || defined(WIN32)
		_itoa(RAW_IMAGE_HEADERS[i].y_pos, pos_data, 10);
#else
		snprintf(pos_data,5,"%u",RAW_IMAGE_HEADERS[i].y_pos);
#endif
		fwrite(pos_data, sizeof(char), strlen(pos_data), O);
		fputc(0x0D, O);
		fputc(0x0A, O);
	}
	printf("Done\n");
	free(RAW_IMAGE_HEADERS);
	fclose(O);
	fclose(I);
	return 0;
}
