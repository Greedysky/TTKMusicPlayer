/*  _______         ____    __         ___    ___
 * \    _  \       \    /  \  /       \   \  /   /       '   '  '
 *  |  | \  \       |  |    ||         |   \/   |         .      .
 *  |  |  |  |      |  |    ||         ||\  /|  |
 *  |  |  |  |      |  |    ||         || \/ |  |         '  '  '
 *  |  |  |  |      |  |    ||         ||    |  |         .      .
 *  |  |_/  /        \  \__//          ||    |  |
 * /_______/ynamic    \____/niversal  /__\  /____\usic   /|  .  . ibliotheque
 *                                                      /  \
 *                                                     / .  \
 * stdfile.c - stdio file input module.               / / \  \
 *                                                   | <  /   \_
 * By entheh.                                        |  \/ /\   /
 *                                                    \_  /  > /
 *                                                      | \ / /
 *                                                      |  ' /
 *                                                       \__/
 */

#include <stdio.h>

#include "dumb.h"



typedef struct dumb_stdfile
{
    FILE * file;
    long size;
} dumb_stdfile;



static void *dumb_stdfile_open(const char *filename)
{
    dumb_stdfile * file = ( dumb_stdfile * ) malloc( sizeof(dumb_stdfile) );
    if ( !file ) return 0;
    file->file = fopen(filename, "rb");
    fseek(file->file, 0, SEEK_END);
    file->size = ftell(file->file);
    fseek(file->file, 0, SEEK_SET);
    return file;
}



static int dumb_stdfile_skip(void *f, long n)
{
    dumb_stdfile * file = ( dumb_stdfile * ) f;
    return fseek(file->file, n, SEEK_CUR);
}



static int dumb_stdfile_getc(void *f)
{
    dumb_stdfile * file = ( dumb_stdfile * ) f;
    return fgetc(file->file);
}



static long dumb_stdfile_getnc(char *ptr, long n, void *f)
{
    dumb_stdfile * file = ( dumb_stdfile * ) f;
    return fread(ptr, 1, n, file->file);
}



static void dumb_stdfile_close(void *f)
{
    dumb_stdfile * file = ( dumb_stdfile * ) f;
    fclose(file->file);
    free(f);
}



static void dumb_stdfile_noclose(void *f)
{
    free(f);
}



static int dumb_stdfile_seek(void *f, long n)
{
    dumb_stdfile * file = ( dumb_stdfile * ) f;
    return fseek(file->file, n, SEEK_SET);
}



static long dumb_stdfile_get_size(void *f)
{
    dumb_stdfile * file = ( dumb_stdfile * ) f;
    return file->size;
}



static const DUMBFILE_SYSTEM stdfile_dfs = {
	&dumb_stdfile_open,
	&dumb_stdfile_skip,
	&dumb_stdfile_getc,
	&dumb_stdfile_getnc,
    &dumb_stdfile_close,
    &dumb_stdfile_seek,
    &dumb_stdfile_get_size
};



void dumb_register_stdfiles(void)
{
	register_dumbfile_system(&stdfile_dfs);
}



static const DUMBFILE_SYSTEM stdfile_dfs_leave_open = {
	NULL,
	&dumb_stdfile_skip,
	&dumb_stdfile_getc,
	&dumb_stdfile_getnc,
    &dumb_stdfile_noclose,
    &dumb_stdfile_seek,
    &dumb_stdfile_get_size
};



DUMBFILE *dumbfile_open_stdfile(FILE *p)
{
    dumb_stdfile * file = ( dumb_stdfile * ) malloc( sizeof(dumb_stdfile) );
	DUMBFILE *d;
    if ( !file ) return 0;
    file->file = p;
    fseek(p, 0, SEEK_END);
    file->size = ftell(p);
    fseek(p, 0, SEEK_SET);
    d = dumbfile_open_ex(file, &stdfile_dfs_leave_open);

	return d;
}
