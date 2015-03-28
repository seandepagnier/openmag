#include <stdio.h>

FILE *fopen(const char *filename, const char *mode);
int fclose(FILE *fp);
int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
void rewind(FILE *stream);
void uartfs_init(unsigned char baudrate);
