#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int _16bit(char *in){
    unsigned char c = 0;
    unsigned char start = 0;
    char input[256] = { 0 };
    char path[256] = { 0 };
    char file_name[256] = { 0 };
    unsigned char *image = NULL;
    unsigned int width = 0, height = 0;
    unsigned int size = 0, extra = 0;
    unsigned int for_check = 0;
    unsigned int i = 0, j = 0;
    FILE *I = NULL;
    FILE *O = NULL;

    strcpy(input, in);
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
    I = fopen(input, "rb");
    if (I == NULL){
        printf("FAIL!\nFailed to open %s\n", input);
        fclose(O);
        return -1;
    }
    printf("\nChecking %s...", input);
    if ((c = fgetc(I)) == 'B'){
        if ((c = fgetc(I)) != 'M'){
            printf("FAIL!\n%s is not a Windows BMP file\n", input);
            fclose(I);
            return -1;
        }
    }
    else{
        printf("FAIL!\n%s is not a Windows BMP file\n", input);
        fclose(I);
        return -1;
    }
    fread(&for_check, sizeof(unsigned int), 1, I);
    fseek(I, 0L, SEEK_END);
    if (ftell(I) != for_check){
        printf("FAIL!\n%s is a damaged Windows BMP file\nFile size mismatch: %lu bytes, expected %u bytes\n", input, ftell(I),for_check);
        fclose(I);
        return -1;
    }
    fseek(I, 0x0E, SEEK_SET);
    c = fgetc(I);
    if (c == 0x0C){
        printf("FAIL!\nBITMAPCOREHEADER structure is not supported\nRe-save the editet pictures with Paint, Gimp or Photoshop\n");
        fclose(I);
        return -1;
    }
    fseek(I, 0x1C, SEEK_SET);
    if ((c = fgetc(I)) != 0x18){
        printf("FAIL!\n%s is not a 24 bit Windows BMP file\n", input);
        fclose(I);
        return -1;
    }
    fseek(I, 0x1E, SEEK_SET);
    fread(&for_check, sizeof(unsigned int), 1, I);
    if (for_check != 0){
        printf("FAIL!\n%s is a compressed Windows BMP file\nCompressed Windows BMP files are not supported\n", input);
        fclose(I);
        return -1;
    }
    fseek(I, 0x12, SEEK_SET);
    fread(&width, sizeof(unsigned int), 1, I);
    fread(&height, sizeof(unsigned int), 1, I);
    if ((width == 0) || (width > 1080)){
        printf("FAIL!\n%s is too wide\nImage width is %u pixels\nMaximum witdth is 1080 pixels\n", input, width);
        fclose(I);
        return -1;
    }
    if ((height == 0) || (height > 1920)){
        printf("FAIL!\n%s is too high\nImage height is %u pixels\nMaximum height is 1920 pixels\n", input, height);
        fclose(I);
        return -1;
    }
    fseek(I, 0x06, SEEK_SET);
    if (fgetc(I) == 0x01){
        printf("FAIL!\n%s image is already reduced to 16 bit\n", input);
        return -1;
    }
    printf("OK!");
    fseek(I, 0L, SEEK_END);
    size = (unsigned int)ftell(I);
    fseek(I,0x0A, SEEK_SET); //| Seeks to the byte which tells the start of the pixel table.
    start = fgetc(I);        //| Reads that byte
    fseek(I, 0x12, SEEK_SET);
    fread(&width, sizeof(unsigned int), 1, I);
    if (4 - ((width * 3) % 4) == 4){
        extra = 0;
    }
    else{
        extra = 4 - ((width * 3) % 4);
    }
    fseek(I, 0L, SEEK_SET);
    image = (unsigned char*)malloc(sizeof(unsigned char)*size);
    fread(image, sizeof(char)*size, 1, I);
    fclose(I);
    i = (unsigned int)strlen(input);
#if defined(_WIN32) || defined(WIN32)
    while(input[i] != '\\'){
#else
    while(input[i] != '/'){
#endif
        i--;
        if (i == 0){
            break;
        }
    }
    if (i != 0){
        for (j = 0; j <= i; j++){
            path[j] = input[j];
        }
        i++;
    }
    strcpy(file_name, "o-");
    j=2;
    while(input[i] != '\0'){
        file_name[j] = input[i];
        i++;
        j++;
    }
    strcat(path, file_name);
    O = fopen(path, "wb");
    if (O == NULL){
        printf("FAIL!\nCould not open %s\n", path);
    }
    fwrite(image, sizeof(char)*size, 1, O);
    fclose(O);
    i = start;
    while(i <= size){
        if ((i-start) == (width * 3)){
            i = i + extra;
        }
        if(((i-start) % 3 == 0) || ((i-start) % 3 == 2)){
            if(image[i] <= 4) image[i] = 0x00;                       // 5
            if(image[i] >= 5 && image[i] <= 12) image[i] = 0x08;    // 8
            if(image[i] >= 13 && image[i] <= 20) image[i] = 0x10;   // 8
            if(image[i] >= 21 && image[i] <= 28) image[i] = 0x19;   // 8
            if(image[i] >= 29 && image[i] <= 37) image[i] = 0x21;   // 9
            if(image[i] >= 38 && image[i] <= 45) image[i] = 0x29;    // 8
            if(image[i] >= 46 && image[i] <= 53) image[i] = 0x31;    // 8
            if(image[i] >= 54 && image[i] <= 61) image[i] = 0x3A;    // 8
            if(image[i] >= 62 && image[i] <= 69) image[i] = 0x42;    // 8
            if(image[i] >= 70 && image[i] <= 78) image[i] = 0x4A;    // 9
            if(image[i] >= 79 && image[i] <= 86) image[i] = 0x52;    // 8
            if(image[i] >= 87 && image[i] <= 94) image[i] = 0x5A;    // 8
            if(image[i] >= 95 && image[i] <= 102) image[i] = 0x63;    // 8
            if(image[i] >= 103 && image[i] <= 111) image[i] = 0x6B;    // 9
            if(image[i] >= 112 && image[i] <= 119) image[i] = 0x73;    // 8
            if(image[i] >= 120 && image[i] <= 127) image[i] = 0x7B;    // 8
            if(image[i] >= 128 && image[i] <= 135) image[i] = 0x84;    // 8
            if(image[i] >= 136 && image[i] <= 143) image[i] = 0x8C;    // 8
            if(image[i] >= 144 && image[i] <= 152) image[i] = 0x94;    // 9
            if(image[i] >= 153 && image[i] <= 160) image[i] = 0x9C;    // 8
            if(image[i] >= 161 && image[i] <= 168) image[i] = 0xA5;    // 8
            if(image[i] >= 169 && image[i] <= 176) image[i] = 0xAD;    // 8
            if(image[i] >= 177 && image[i] <= 185) image[i] = 0xB5;    // 9
            if(image[i] >= 186 && image[i] <= 193) image[i] = 0xBD;    // 8
            if(image[i] >= 194 && image[i] <= 201) image[i] = 0xC5;    // 8
            if(image[i] >= 202 && image[i] <= 209) image[i] = 0xCE;    // 8
            if(image[i] >= 210 && image[i] <= 217) image[i] = 0xD6;    // 8
            if(image[i] >= 218 && image[i] <= 226) image[i] = 0xDE;    // 9
            if(image[i] >= 227 && image[i] <= 234) image[i] = 0xE6;    // 8
            if(image[i] >= 235 && image[i] <= 242) image[i] = 0xEF;    // 8
            if(image[i] >= 243 && image[i] <= 250) image[i] = 0xF7;    // 8
            if(image[i] >= 251 && image[i] <= 255) image[i] = 0xFF;    // 5
            i++;
        }
        else{
            if(image[i] <= 2) image[i] = 0x00;                          // 3
            if(image[i] >=3 && image[i] <= 6) image[i] = 0x04;         // 4
            if(image[i] >=7 && image[i] <= 10) image[i] = 0x08;          // 4
            if(image[i] >=11 && image[i] <= 14) image[i] = 0x0C;          // 4
            if(image[i] >=15 && image[i] <= 18) image[i] = 0x10;         // 4
            if(image[i] >=19 && image[i] <= 22) image[i] = 0x14;          // 4
            if(image[i] >=23 && image[i] <= 26) image[i] = 0x18;          // 4
            if(image[i] >=27 && image[i] <= 30) image[i] = 0x1C;          // 4
            if(image[i] >=31 && image[i] <= 34) image[i] = 0x20;          // 4
            if(image[i] >=35 && image[i] <= 38) image[i] = 0x24;          // 4
            if(image[i] >=39 && image[i] <= 42) image[i] = 0x28;          // 4
            if(image[i] >=43 && image[i] <= 46) image[i] = 0x2D;          // 4
            if(image[i] >=47 && image[i] <= 50) image[i] = 0x31;          // 4
            if(image[i] >=51 && image[i] <= 54) image[i] = 0x35;          // 4
            if(image[i] >=55 && image[i] <= 58) image[i] = 0x39;          // 4
            if(image[i] >=59 && image[i] <= 62) image[i] = 0x3D;          // 4
            if(image[i] >=63 && image[i] <= 66) image[i] = 0x41;          // 4
            if(image[i] >=67 && image[i] <= 70) image[i] = 0x45;          // 4
            if(image[i] >=71 && image[i] <= 74) image[i] = 0x49;          // 4
            if(image[i] >=75 && image[i] <= 78) image[i] = 0x4D;          // 4
            if(image[i] >=79 && image[i] <= 82) image[i] = 0x51;          // 4
            if(image[i] >=83 && image[i] <= 87) image[i] = 0x55;          // 5
            if(image[i] >=88 && image[i] <= 91) image[i] = 0x59;          // 4
            if(image[i] >=92 && image[i] <= 95) image[i] = 0x5D;          // 4
            if(image[i] >=96 && image[i] <= 99) image[i] = 0x61;          // 4
            if(image[i] >=100 && image[i] <= 103) image[i] = 0x65;          // 4
            if(image[i] >=104 && image[i] <= 107) image[i] = 0x69;          // 4
            if(image[i] >=108 && image[i] <= 111) image[i] = 0x6D;          // 4
            if(image[i] >=112 && image[i] <= 115) image[i] = 0x71;          // 4
            if(image[i] >=116 && image[i] <= 119) image[i] = 0x75;          // 4
            if(image[i] >=120 && image[i] <= 123) image[i] = 0x79;          // 4
            if(image[i] >=124 && image[i] <= 127) image[i] = 0x7D;          // 4
            if(image[i] >=128 && image[i] <= 131) image[i] = 0x82;          // 4
            if(image[i] >=132 && image[i] <= 135) image[i] = 0x86;          // 4
            if(image[i] >=136 && image[i] <= 139) image[i] = 0x8A;          // 4
            if(image[i] >=140 && image[i] <= 143) image[i] = 0x8E;          // 4
            if(image[i] >=144 && image[i] <= 147) image[i] = 0x92;          // 4
            if(image[i] >=148 && image[i] <= 151) image[i] = 0x96;          // 4
            if(image[i] >=152 && image[i] <= 155) image[i] = 0x9A;          // 4
            if(image[i] >=156 && image[i] <= 159) image[i] = 0x9E;          // 4
            if(image[i] >=160 && image[i] <= 163) image[i] = 0xA2;          // 4
            if(image[i] >=164 && image[i] <= 167) image[i] = 0xA6;          // 4
            if(image[i] >=168 && image[i] <= 172) image[i] = 0xAA;          // 5
            if(image[i] >=173 && image[i] <= 176) image[i] = 0xAE;          // 4
            if(image[i] >=177 && image[i] <= 180) image[i] = 0xB2;          // 4
            if(image[i] >=181 && image[i] <= 184) image[i] = 0xB6;          // 4
            if(image[i] >=183 && image[i] <= 188) image[i] = 0xBA;          // 4
            if(image[i] >=189 && image[i] <= 192) image[i] = 0xBE;          // 4
            if(image[i] >=193 && image[i] <= 196) image[i] = 0xC2;          // 4
            if(image[i] >=197 && image[i] <= 200) image[i] = 0xC6;          // 4
            if(image[i] >=201 && image[i] <= 204) image[i] = 0xCA;          // 4
            if(image[i] >=205 && image[i] <= 208) image[i] = 0xCE;          // 4
            if(image[i] >=209 && image[i] <= 212) image[i] = 0xD2;          // 4
            if(image[i] >=213 && image[i] <= 216) image[i] = 0xD7;          // 4
            if(image[i] >=217 && image[i] <= 220) image[i] = 0xDB;          // 4
            if(image[i] >=221 && image[i] <= 224) image[i] = 0xDF;          // 4
            if(image[i] >=225 && image[i] <= 228) image[i] = 0xE3;          // 4
            if(image[i] >=229 && image[i] <= 232) image[i] = 0xE7;          // 4
            if(image[i] >=233 && image[i] <= 236) image[i] = 0xEB;          // 4
            if(image[i] >=237 && image[i] <= 240) image[i] = 0xEF;          // 4
            if(image[i] >=241 && image[i] <= 244) image[i] = 0xF3;          // 4
            if(image[i] >=245 && image[i] <= 248) image[i] = 0xF7;          // 4
            if(image[i] >=249 && image[i] <= 252) image[i] = 0xFB;          // 4
            if(image[i] >=253 && image[i] <= 255) image[i] = 0xFF;          // 3
            i++;
        }
    }
    O = fopen(input, "wb");
    if (O == NULL){
        printf("FAIL!\nFailed to open %s\n", input);
        fclose(O);
        return -1;
    }
    fwrite(image, sizeof(char)*size, 1, O);
    fseek(O, 0x6, SEEK_SET);
    fputc(0x01, O);
    fclose(O);
    free(image);
    return 0;
}
