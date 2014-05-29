#ifndef EXTRACT_H_INCLUDED
#define EXTRACT_H_INCLUDED

typedef struct raw_image_header{
	char name[16];
	size_t width;
	size_t height;
	size_t x_pos;
	size_t y_pos;
	size_t offset;
	size_t size;
}raw_image_header;

int extract(char *input);

#endif // EXTRACT_H_INCLUDED