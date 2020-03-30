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
 * dumbfile.c - Hookable, strictly sequential         / / \  \
 *              file input functions.                | <  /   \_
 *                                                   |  \/ /\   /
 * By entheh.                                         \_  /  > /
 *                                                      | \ / /
 *                                                      |  ' /
 *                                                       \__/
 */

#include <stdlib.h>

#include "dumb.h"



static const DUMBFILE_SYSTEM *the_dfs = NULL;



void register_dumbfile_system(const DUMBFILE_SYSTEM *dfs)
{
	ASSERT(dfs);
	ASSERT(dfs->open);
	ASSERT(dfs->getc);
	ASSERT(dfs->close);
    ASSERT(dfs->seek);
    ASSERT(dfs->get_size);
	the_dfs = dfs;
}



#include "internal/dumbfile.h"



DUMBFILE *dumbfile_open(const char *filename)
{
	DUMBFILE *f;

	ASSERT(the_dfs);

	f = (DUMBFILE *) malloc(sizeof(*f));

	if (!f)
		return NULL;

	f->dfs = the_dfs;

	f->file = (*the_dfs->open)(filename);

	if (!f->file) {
		free(f);
		return NULL;
	}

	f->pos = 0;

	return f;
}



DUMBFILE *dumbfile_open_ex(void *file, const DUMBFILE_SYSTEM *dfs)
{
	DUMBFILE *f;

	ASSERT(dfs);
	ASSERT(dfs->getc);
	ASSERT(file);

	f = (DUMBFILE *) malloc(sizeof(*f));

	if (!f) {
		if (dfs->close)
			(*dfs->close)(file);
		return NULL;
	}

	f->dfs = dfs;
	f->file = file;

	f->pos = 0;

	return f;
}



long dumbfile_pos(DUMBFILE *f)
{
	ASSERT(f);

	return f->pos;
}



int dumbfile_skip(DUMBFILE *f, long n)
{
	int rv;

	ASSERT(f);
	ASSERT(n >= 0);

	if (f->pos < 0)
		return -1;

	f->pos += n;

	if (f->dfs->skip) {
		rv = (*f->dfs->skip)(f->file, n);
		if (rv) {
			f->pos = -1;
			return rv;
		}
	} else {
		while (n) {
			rv = (*f->dfs->getc)(f->file);
			if (rv < 0) {
				f->pos = -1;
				return rv;
			}
			n--;
		}
	}

	return 0;
}



int dumbfile_getc(DUMBFILE *f)
{
	int rv;

	ASSERT(f);

	if (f->pos < 0)
		return -1;

	rv = (*f->dfs->getc)(f->file);

	if (rv < 0) {
		f->pos = -1;
		return rv;
	}

	f->pos++;

	return rv;
}



int dumbfile_igetw(DUMBFILE *f)
{
	int l, h;

	ASSERT(f);

	if (f->pos < 0)
		return -1;

	l = (*f->dfs->getc)(f->file);
	if (l < 0) {
		f->pos = -1;
		return l;
	}

	h = (*f->dfs->getc)(f->file);
	if (h < 0) {
		f->pos = -1;
		return h;
	}

	f->pos += 2;

	return l | (h << 8);
}



int dumbfile_mgetw(DUMBFILE *f)
{
	int l, h;

	ASSERT(f);

	if (f->pos < 0)
		return -1;

	h = (*f->dfs->getc)(f->file);
	if (h < 0) {
		f->pos = -1;
		return h;
	}

	l = (*f->dfs->getc)(f->file);
	if (l < 0) {
		f->pos = -1;
		return l;
	}

	f->pos += 2;

	return l | (h << 8);
}



long dumbfile_igetl(DUMBFILE *f)
{
	unsigned long rv, b;

	ASSERT(f);

	if (f->pos < 0)
		return -1;

	rv = (*f->dfs->getc)(f->file);
	if ((signed long)rv < 0) {
		f->pos = -1;
		return rv;
	}

	b = (*f->dfs->getc)(f->file);
	if ((signed long)b < 0) {
		f->pos = -1;
		return b;
	}
	rv |= b << 8;

	b = (*f->dfs->getc)(f->file);
	if ((signed long)b < 0) {
		f->pos = -1;
		return b;
	}
	rv |= b << 16;

	b = (*f->dfs->getc)(f->file);
	if ((signed long)b < 0) {
		f->pos = -1;
		return b;
	}
	rv |= b << 24;

	f->pos += 4;

	return rv;
}



long dumbfile_mgetl(DUMBFILE *f)
{
	unsigned long rv, b;

	ASSERT(f);

	if (f->pos < 0)
		return -1;

	rv = (*f->dfs->getc)(f->file);
	if ((signed long)rv < 0) {
		f->pos = -1;
		return rv;
	}
	rv <<= 24;

	b = (*f->dfs->getc)(f->file);
	if ((signed long)b < 0) {
		f->pos = -1;
		return b;
	}
	rv |= b << 16;

	b = (*f->dfs->getc)(f->file);
	if ((signed long)b < 0) {
		f->pos = -1;
		return b;
	}
	rv |= b << 8;

	b = (*f->dfs->getc)(f->file);
	if ((signed long)b < 0) {
		f->pos = -1;
		return b;
	}
	rv |= b;

	f->pos += 4;

	return rv;
}



unsigned long dumbfile_cgetul(DUMBFILE *f)
{
	unsigned long rv = 0;
	int v;

	do {
		v = dumbfile_getc(f);

		if (v < 0)
			return v;

		rv <<= 7;
		rv |= v & 0x7F;
	} while (v & 0x80);

	return rv;
}



signed long dumbfile_cgetsl(DUMBFILE *f)
{
	unsigned long rv = dumbfile_cgetul(f);

	if (f->pos < 0)
		return rv;

	return (rv >> 1) | (rv << 31);
}



long dumbfile_getnc(char *ptr, long n, DUMBFILE *f)
{
	long rv;

	ASSERT(f);
	ASSERT(n >= 0);

	if (f->pos < 0)
		return -1;

	if (f->dfs->getnc) {
		rv = (*f->dfs->getnc)(ptr, n, f->file);
		if (rv < n) {
			f->pos = -1;
			return MAX(rv, 0);
		}
	} else {
		for (rv = 0; rv < n; rv++) {
			int c = (*f->dfs->getc)(f->file);
			if (c < 0) {
				f->pos = -1;
				return rv;
			}
			*ptr++ = c;
		}
	}

	f->pos += rv;

	return rv;
}



int dumbfile_seek(DUMBFILE *f, long n, int origin)
{
    switch ( origin )
    {
    case DFS_SEEK_CUR: n += f->pos; break;
    case DFS_SEEK_END: n += (*f->dfs->get_size)(f->file); break;
    }
    f->pos = n;
    return (*f->dfs->seek)(f->file, n);
}



long dumbfile_get_size(DUMBFILE *f)
{
    return (*f->dfs->get_size)(f->file);
}



int dumbfile_error(DUMBFILE *f)
{
	ASSERT(f);

	return f->pos < 0;
}



int dumbfile_close(DUMBFILE *f)
{
	int rv;

	ASSERT(f);

	rv = f->pos < 0;

	if (f->dfs->close)
		(*f->dfs->close)(f->file);

	free(f);

	return rv;
}
