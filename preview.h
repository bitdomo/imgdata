#ifndef PREVIEW_H_INCLUDED
#define PREVIEW_H_INCLUDED

int write_background(char *out, char *option);
int write_image(char *in, char* out, char *option, raw_image_header RAW_IMAGE_HEADER);
int preview(char *option, char *in, char *out);

#endif // PREVIEW_H_INCLUDED
