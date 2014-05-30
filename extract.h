#ifndef EXTRACT_H_INCLUDED
#define EXTRACT_H_INCLUDED

typedef struct raw_image_header{
	char name[16];
	unsigned int width;
	unsigned int height;
	unsigned int x_pos;
	unsigned int y_pos;
	unsigned int offset;
	unsigned int size;
}raw_image_header;

int extract(char *input);

#endif // EXTRACT_H_INCLUDED
