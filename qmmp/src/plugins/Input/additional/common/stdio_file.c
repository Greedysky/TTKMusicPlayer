#include "stdio_file.h"

#ifndef __linux__
#define off64_t off_t
#define lseek64 lseek
#define O_LARGEFILE 0
#endif

#define BUFSIZE 1024

FILE *stdio_open(const char *fname)
{
    if(!memcmp(fname, "file://", 7))
    {
        fname += 7;
    }

    FILE *file = fopen(fname, "rb");
    if(!file)
    {
        return NULL;
    }
    return file;
}

void stdio_close(FILE *stream)
{
    fclose(stream);
}

size_t stdio_read(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    return fread(ptr, size, nmemb, stream);
}

int stdio_seek(FILE *stream, int64_t offset, int whence)
{
    return fseek(stream, offset, whence);
}

int64_t stdio_tell(FILE *stream)
{
    return ftell(stream);
}

void stdio_rewind(FILE *stream)
{
    rewind(stream);
}

int64_t stdio_length(FILE *stream)
{
    size_t offs = ftell(stream);
    fseek(stream, 0, SEEK_END);
    size_t l = ftell(stream);
    fseek(stream, offs, SEEK_SET);
    return l;
}

int stdio_get_leading_size(FILE *stream)
{
    uint8_t header[10];
    int pos = ftell(stream);
    if(fread(header, 1, 10, stream) != 10)
    {
        fseek(stream, pos, SEEK_SET);
        return -1; // too short
    }

    fseek(stream, pos, SEEK_SET);
    if(strncmp(header, "ID3", 3))
    {
        return -1; // no tag
    }

    uint8_t flags = header[5];
    if(flags & 15)
    {
        return -1; // unsupported
    }

    int footerpresent = (flags & (1<<4)) ? 1 : 0;
    // check for bad size
    if((header[9] & 0x80) || (header[8] & 0x80) || (header[7] & 0x80) || (header[6] & 0x80))
    {
        return -1; // bad header
    }

    uint32_t size = (header[9] << 0) | (header[8] << 7) | (header[7] << 14) | (header[6] << 21);
    return size + 10 + 10 * footerpresent;
}
