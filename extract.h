#ifndef EXTRACT_H_INCLUDED
#define EXTRACT_H_INCLUDED

typedef struct RAW_IMAGE_HEADER{
	char name[16];
	size_t width;
	size_t height;
	size_t x_pos;
	size_t y_pos;
	size_t offset;
	size_t size;
}RAW_IMAGE_HEADER;

void extract(char *input);

#endif // EXTRACT_H_INCLUDED