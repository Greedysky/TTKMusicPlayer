#ifndef RIFF_H
#define RIFF_H

struct riff;

struct riff_chunk
{
	unsigned type;
    long offset;
	unsigned size;
    struct riff * nested;
};

struct riff
{
	unsigned type;
	unsigned chunk_count;
	struct riff_chunk * chunks;
};

struct riff * riff_parse( DUMBFILE * f, long offset, long size, unsigned proper );
void riff_free( struct riff * );

#endif
