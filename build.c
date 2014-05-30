#ifdef __unix__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extract.h"
#include "build.h"

#elif defined(_WIN32) || defined(WIN32)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extract.h"
#include "build.h"

#define OS_Windows
#endif
int build_imgdata(){
	char c = 0, R = 0, G = 0, B = 0;
	char x_pos[5] = { 0 };
	char y_pos[5] = { 0 };
	char *names[] = { "boot", "charger", "unlocked", "start", "bootloader", "recovery", "poweroff", "fastboot_op", "oem_unlock", "unlock_yes", "unlock_no", "downloadmode" };
	char IMGDATA_HEADER[] = { 'I', 'M', 'G', 'D', 'A', 'T', 'A', '!', 0x01, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	char file_path[256] = { 0 };
	BMP_PIXEL pixels[12] = { NULL };
	unsigned int i = 0, j = 0, k = 0;
	unsigned int extra = 0;
	unsigned int for_check = 0;
	raw_image_header RAW_IMAGE_HEADERS[12] = { 0 };
	raw_images *RAW_IMAGES[12] = {NULL};
	raw_images *cur = NULL;
	FILE *I = NULL;
	FILE *O = NULL;
		
	/*for (i = 0; i < 12; i++){
		RAW_IMAGES[i] = (raw_images*)malloc(sizeof(raw_images));
		RAW_IMAGES[i]->B = 0;
		RAW_IMAGES[i]->G = 0;
		RAW_IMAGES[i]->R = 0;
		RAW_IMAGES[i]->count = 0;
		RAW_IMAGES[i]->next = NULL;
	}
		*/
	printf("Reading pos.txt...");
#ifdef OS_Windows
	I = fopen(".\\images\\pos.txt", "rb");
	if (I == NULL){
		printf("FAIL!\nCould not open .\\images\\pos.txt\n");
		return -1;
	}
#else
	I = fopen("./images/pos.txt", "rb");
	if (I == NULL){
		printf("FAIL!\nCould not open ./images/pos.txt\n");
		return -1;
	}
#endif
	for (i = 0; i < 12; i++){
		c = fgetc(I);
		while (c != 0x20){
			RAW_IMAGE_HEADERS[i].name[j] = c;
			j++;
			c = fgetc(I);
			if (j>15){
				RAW_IMAGE_HEADERS[i].name[15] = '\0';
#ifdef OS_Windows
				printf("FAIL!\nUnknown name \"%s\" at line %u in .\\images\\pos.txt\n", RAW_IMAGE_HEADERS[i].name, i + 1);
#else
				printf("FAIL!\nUnknown name \"%s\" at line %u in ./images/pos.txt\n", RAW_IMAGE_HEADERS[i].name, i + 1);
#endif
				fclose(I);
				return -1;
			}
		}
		RAW_IMAGE_HEADERS[i].name[j] = '\0';
		if (!(strcmp(names[i], RAW_IMAGE_HEADERS[i].name) == 0)){
#ifdef OS_Windows
			printf("FAIL!\nUnknown name \"%s\" at line %u in .\\images\\pos.txt", RAW_IMAGE_HEADERS[i].name, i + 1);
#else
			printf("FAIL!\nUnknown name \"%s\" at line %u in ./images/pos.txt", RAW_IMAGE_HEADERS[i].name, i + 1);
#endif
			fclose(I);
			return -1;
		}
		j = 0;
		k = 0;
		c = fgetc(I);
		while (c != 'x'){
			x_pos[j] = c;
			j++;
			c = fgetc(I);
			if (j > 4){
#ifdef OS_Windows
				printf("FAIL!\nCheck \"%s\" in .\\images\\pos.txt\nX position must be maximum 4 digits long\nCheck for the \"x\" too", RAW_IMAGE_HEADERS[i].name);
#else
				printf("FAIL!\nCheck \"%s\" in ./images/pos.txt\nX position must be maximum 4 digits long\nCheck for the \"x\" too", RAW_IMAGE_HEADERS[i].name);
#endif
				fclose(I);
				return -1;
			}
		}
		x_pos[j] = '\0';
		RAW_IMAGE_HEADERS[i].x_pos = atoi(x_pos);
		if (RAW_IMAGE_HEADERS[i].x_pos > 1079){
			printf("FAIL\nX position %u is exceeded the maximum 1079 for %s", RAW_IMAGE_HEADERS[i].x_pos,RAW_IMAGE_HEADERS[i].name);
			fclose(I);
			return -1;
		}
		j = 0;
		k = 0;
		c = fgetc(I);
		while (c != 0x0D && c != 0x0A && c != -1){
			y_pos[j] = c;
			j++;
			c = fgetc(I);
			if (j > 4){
#ifdef OS_Windows
				printf("FAIL!\nCheck \"%s\" .\\images\\pos.txt\nY position must be maximum 4 digits long\nCheck for new line too", RAW_IMAGE_HEADERS[i].name);
#else
				printf("FAIL!\nCheck \"%s\" ./images/pos.txt\nY position must be maximum 4 digits long\nCheck for new line too", RAW_IMAGE_HEADERS[i].name);
#endif
				fclose(I);
				return -1;
			}
		}
		y_pos[j] = '\0';
		RAW_IMAGE_HEADERS[i].y_pos = atoi(y_pos);
		if (RAW_IMAGE_HEADERS[i].y_pos > 1919){
			printf("FAIL\nY position %u is exceeded the maximum 1919 for %s", RAW_IMAGE_HEADERS[i].y_pos, RAW_IMAGE_HEADERS[i].name);
			fclose(I);
			return -1;
		}
		j = 0;
		if (c == 0x0D){
			fgetc(I);
		}
	}
	printf("Done\n");
	fclose(I);
	for (i = 0; i < 12; i++){
#ifdef OS_Windows
		strcpy(file_path, ".\\images\\");
#else
		strcpy(file_path, "./images/");
#endif
		strcat(file_path, RAW_IMAGE_HEADERS[i].name);
		strcat(file_path, ".bmp");
		I = fopen(file_path, "rb");
		if (I == NULL){
			printf("\nCould not open %s\n", file_path);
			return -1;
		}

		printf("\nChecking %s...", file_path);
		if ((c = fgetc(I)) == 'B'){
			if ((c = fgetc(I)) != 'M'){
				printf("FAIL!\n%s is not a Windows BMP file\n", file_path);
				fclose(I);
				return -1;
			}
		}
		else{
			printf("FAIL!\n%s is not a Windows BMP file\n", file_path);
			fclose(I);
			return -1;
		}
		fread(&for_check, sizeof(unsigned int), 1, I);
		fseek(I, 0L, SEEK_END);
		if (ftell(I) != for_check){
			printf("FAIL!\n%s is a damaged Windows BMP file\nFile size mismatch: %lu bytes, expected %u bytes\n", file_path,ftell(I),for_check);
			fclose(I);
			return -1;
		}
		fseek(I, 0x1C, SEEK_SET);
		if ((c = fgetc(I)) != 0x18){
			printf("FAIL!\n%s is not a 24 bit Windows BMP file\n", file_path);
			fclose(I);
			return -1;
		}
		fseek(I, 0x1E, SEEK_SET);
		fread(&for_check, sizeof(unsigned int), 1, I);
		if (for_check != 0){
			printf("FAIL!\n%s is a compressed Windows BMP file\nCompressed Windows BMP files are not supported\n", file_path);
			fclose(I);
			return -1;
		}
		fseek(I, 0x12, SEEK_SET);
		fread(&RAW_IMAGE_HEADERS[i].width, sizeof(unsigned int), 1, I);
		fread(&RAW_IMAGE_HEADERS[i].height, sizeof(unsigned int), 1, I);
		if ((RAW_IMAGE_HEADERS[i].width == 0) || (RAW_IMAGE_HEADERS[i].width > 1080)){
			printf("FAIL!\n%s is too wide\nImage width is %u pixels\nMaximum witdth is 1080 pixels\n", file_path, RAW_IMAGE_HEADERS[i].width);
			fclose(I);
			return -1;
		}
		if ((RAW_IMAGE_HEADERS[i].height == 0) || (RAW_IMAGE_HEADERS[i].height > 1920)){
			printf("FAIL!\n%s is too high\nImage height is %u pixels\nMaximum height is 1920 pixels\n", file_path, RAW_IMAGE_HEADERS[i].height);
			fclose(I);
			return -1;
		}
		if (RAW_IMAGE_HEADERS[i].width + RAW_IMAGE_HEADERS[i].x_pos > 1080){
			printf("FAIL!\n%s is off the screen by %u pixel(s) horizontaly\nCheck X position and the image dimensions\n", file_path, RAW_IMAGE_HEADERS[i].width + RAW_IMAGE_HEADERS[i].x_pos - 1080);
			if (RAW_IMAGE_HEADERS[i].height + RAW_IMAGE_HEADERS[i].y_pos > 1920){
				printf("%s is off the screen by %u pixel(s) verticaly\nCheck y position and the image dimensions\n", file_path, RAW_IMAGE_HEADERS[i].height + RAW_IMAGE_HEADERS[i].y_pos - 1920);
			}
			fclose(I);
			return -1;
		}
		if(RAW_IMAGE_HEADERS[i].height + RAW_IMAGE_HEADERS[i].y_pos > 1920){
			printf("FAIL!\n%s is off the screen by %u pixel(s) verticaly\nCheck y position and the image dimensions\n", file_path, RAW_IMAGE_HEADERS[i].height + RAW_IMAGE_HEADERS[i].y_pos - 1920);
			if (RAW_IMAGE_HEADERS[i].width + RAW_IMAGE_HEADERS[i].x_pos > 1080){
				printf("%s is off the screen by %u pixel(s) horizontaly\nCheck X position and the image dimensions\n", file_path, RAW_IMAGE_HEADERS[i].width + RAW_IMAGE_HEADERS[i].x_pos - 1080);
			}
			fclose(I);
			return -1;
		}
		printf("OK!");
		printf("\nReading %s...", file_path);
		fseek(I, 0x36, SEEK_SET);
		if (4 - ((RAW_IMAGE_HEADERS[i].width * 3) % 4) == 4){
			extra = 0;
		}
		else{
			extra = 4 - ((RAW_IMAGE_HEADERS[i].width * 3) % 4);
		}
		pixels[i].data = (char*)malloc(sizeof(char)*(3 * RAW_IMAGE_HEADERS[i].width*RAW_IMAGE_HEADERS[i].height));
		for (j = 0; j < (3*RAW_IMAGE_HEADERS[i].width*RAW_IMAGE_HEADERS[i].height); j++){
			pixels[i].data[j] = fgetc(I);
			k++;
			if (k == 3 * RAW_IMAGE_HEADERS[i].width){
				fseek(I, extra, SEEK_CUR);
				k = 0;
			}
		}
		fclose(I);
		printf("Done\n");
	}
	for (i = 0; i < 12; i++){

#ifdef OS_Windows
		printf("\nConverting .\\images\\%s.bmp...", RAW_IMAGE_HEADERS[i].name);
#else
		printf("\nConverting ./images/%s.bmp...", RAW_IMAGE_HEADERS[i].name);
#endif
		
		if (i == 0){
			RAW_IMAGE_HEADERS[i].offset = 0x400;
		}
		else{
			if (RAW_IMAGE_HEADERS[i-1].size % 512 == 0){
				RAW_IMAGE_HEADERS[i].offset = RAW_IMAGE_HEADERS[i - 1].offset + RAW_IMAGE_HEADERS[i - 1].size;
			}
			else{
				RAW_IMAGE_HEADERS[i].offset = RAW_IMAGE_HEADERS[i - 1].offset + RAW_IMAGE_HEADERS[i - 1].size + (512 - RAW_IMAGE_HEADERS[i-1].size % 512);
			}
		}
		RAW_IMAGE_HEADERS[i].size = 0;
		cur = RAW_IMAGES[i];
		for (j = 3 * RAW_IMAGE_HEADERS[i].width*RAW_IMAGE_HEADERS[i].height - 3 * RAW_IMAGE_HEADERS[i].width; j != 0; j = j - 3 * RAW_IMAGE_HEADERS[i].width){
			k = 0;
			while (k != 3 * RAW_IMAGE_HEADERS[i].width){
				if (k != 3 * RAW_IMAGE_HEADERS[i].width){
					if (cur == NULL){
						cur = (raw_images*)malloc(sizeof(raw_images));
						cur->count = 1;
						cur->B = pixels[i].data[j + k];
						k++;
						cur->G = pixels[i].data[j + k];
						k++;
						cur->R = pixels[i].data[j + k];
						k++;
						cur->next = NULL;
						RAW_IMAGES[i] = cur;
						RAW_IMAGE_HEADERS[i].size = RAW_IMAGE_HEADERS[i].size + 4;
					}
				}
				if (k != 3 * RAW_IMAGE_HEADERS[i].width){
					B = pixels[i].data[j + k];
					k++;
					G = pixels[i].data[j + k];
					k++;
					R = pixels[i].data[j + k];
					k++;
					if (R == cur->R && G == cur->G && B == cur->B && cur->count != (char)0xFF){
						cur->count++;
					}
					else{
						cur->next = (raw_images*)malloc(sizeof(raw_images));
						cur = cur->next;
						cur->R = R;
						cur->G = G;
						cur->B = B;
						cur->count = 1;
						cur->next = NULL;
						RAW_IMAGE_HEADERS[i].size = RAW_IMAGE_HEADERS[i].size + 4;
					}
				}
			}
		}
		k = 0;
		while (k != 3 * RAW_IMAGE_HEADERS[i].width){
			if (k != 3 * RAW_IMAGE_HEADERS[i].width){
				if (cur == NULL){
					cur = (raw_images*)malloc(sizeof(raw_images));
					cur->count = 1;
					cur->B = pixels[i].data[j + k];
					k++;
					cur->G = pixels[i].data[j + k];
					k++;
					cur->R = pixels[i].data[j + k];
					k++;
					cur->next = NULL;
					RAW_IMAGES[i] = cur;
					RAW_IMAGE_HEADERS[i].size = RAW_IMAGE_HEADERS[i].size + 4;
				}
			}
			if (k != 3 * RAW_IMAGE_HEADERS[i].width){
				B = pixels[i].data[j + k];
				k++;
				G = pixels[i].data[j + k];
				k++;
				R = pixels[i].data[j + k];
				k++;
				if (R == cur->R && G == cur->G && B == cur->B && cur->count != (char)0xFF){
					cur->count++;
				}
				else{
					cur->next = (raw_images*)malloc(sizeof(raw_images));
					cur = cur->next;
					cur->R = R;
					cur->G = G;
					cur->B = B;
					cur->count = 1;
					cur->next = NULL;
					RAW_IMAGE_HEADERS[i].size = RAW_IMAGE_HEADERS[i].size + 4;
				}
			}
		}
		printf("Done\n");
	}
	O = fopen("new-imgdata.img", "wb");
	if (O == NULL){
		printf("\nCould not create new-imgdata.img\n");
		return -1;
	}
	printf("\nWriting new-imgdata.img...");
	fwrite(&IMGDATA_HEADER, sizeof(char), sizeof(IMGDATA_HEADER), O);
	fwrite(RAW_IMAGE_HEADERS, sizeof(raw_image_header)* 12, 1, O);
	for (i = 0; i < 520; i++){
		fputc(0x00, O);
	}
	for (i = 0; i < 12; i++){
		cur = RAW_IMAGES[i];
		while (cur != NULL){
			fputc(cur->count, O);
			fputc(cur->R, O);
			fputc(cur->G, O);
			fputc(cur->B, O);
			cur = cur->next;
		}
		if (RAW_IMAGE_HEADERS[i].size % 512 != 0){
			for (j = 0; j < (512 - RAW_IMAGE_HEADERS[i].size % 512); j++){
				fputc(0x00, O);
			}
		}
	}
	fclose(O);
	printf("Done\n");
	for (i = 0; i < 12; i++){;
		while (RAW_IMAGES[i] != NULL){
			cur = RAW_IMAGES[i];
			RAW_IMAGES[i] = RAW_IMAGES[i]->next;
			free(cur);
		}
		free(pixels[i].data);
	}

	/*cur = RAW_IMAGES[7];
	O = fopen(RAW_IMAGE_HEADERS[7].name, "wb");
	while (cur != NULL){
		fputc(cur->count, O);
		fputc(cur->R, O);
		fputc(cur->G, O);
		fputc(cur->B, O);
		cur = cur->next;
	}
	fclose(O);
	*/








	/*for (i = 0; i < 12; i++){
		O = fopen(RAW_IMAGE_HEADERS[i].name, "wb");
		for (j = 0; j < 3 * RAW_IMAGE_HEADERS[i].width*RAW_IMAGE_HEADERS[i].height; j++){
			fputc(pixels[i].data[j], O);
		}
		fclose(O);
	}*/
	return 0;
}
