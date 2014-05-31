#ifndef BUILD_H_INCLUDED
#define BUILD_H_INCLUDED


typedef struct raw_images{
	char count;
	char R;
	char G;
	char B;
	struct raw_images *next;
}raw_images;
typedef struct BMP_PIXEL{
	char *data;
}BMP_PIXEL;

int build(char *in, char *out);

#endif // BUILD_H_INCLUDED
