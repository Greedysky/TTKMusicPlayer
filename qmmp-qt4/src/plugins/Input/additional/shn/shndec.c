/*
    SHN (Shorten) plugin for DeaDBeeF Player
    Copyright (C) 2009-2014 Alexey Yakovenko <waker@users.sourceforge.net>

    Based on xmms-shn, http://www.etree.org/shnutils/xmms-shn/
    Copyright (C) 2000-2007  Jason Jordan <shnutils@freeshell.org>

    This software is provided 'as-is', without any express or implied
    warranty.  In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/

// based on xmms-shn, http://www.etree.org/shnutils/xmms-shn/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "shndec.h"
#include "bitshift.h"
#include "stdio_file.h"

shn_config shn_cfg;

int ddb_getc(FILE *fp) 
{
    uint8_t c;
    if (stdio_read(&c, 1, 1, fp) != 1) {
        return EOF;
    }
    return (int)c;
}

int init_decode_state(shn_file *this_shn)
{
	if (this_shn->decode_state)
	{
		if (this_shn->decode_state->getbuf)
		{
			free(this_shn->decode_state->getbuf);
			this_shn->decode_state->getbuf = NULL;
		}

		if (this_shn->decode_state->writebuf)
		{
			free(this_shn->decode_state->writebuf);
			this_shn->decode_state->writebuf = NULL;
		}

		if (this_shn->decode_state->writefub)
		{
			free(this_shn->decode_state->writefub);
			this_shn->decode_state->writefub = NULL;
		}

		free(this_shn->decode_state);
		this_shn->decode_state = NULL;
	}

	if (!(this_shn->decode_state = malloc(sizeof(shn_decode_state))))
	{
		shn_debug("Could not allocate memory for decode state data structure");
		return 0;
	}

	this_shn->decode_state->getbuf = NULL;
	this_shn->decode_state->getbufp = NULL;
	this_shn->decode_state->nbitget = 0;
	this_shn->decode_state->nbyteget = 0;
	this_shn->decode_state->gbuffer = 0;
	this_shn->decode_state->writebuf = NULL;
    this_shn->decode_state->writefub = NULL;
    this_shn->decode_state->nwritebuf = 0;

    this_shn->vars.bytes_in_buf = 0;

    return 1;
}

int shn_init_decoder(shn_info_t *info)
 {
    int version = FORMAT_VERSION;
    info->ftype = TYPE_EOF;
    info->magic = MAGIC;
    info->blocksize = DEFAULT_BLOCK_SIZE;
    info->nchan = DEFAULT_NCHAN;
    info->nskip = DEFAULT_NSKIP;
    info->maxnlpc = DEFAULT_MAXNLPC;
    info->nmean = UNDEFINED_UINT;

    info->shnfile->vars.bytes_in_buf = 0;
    if (!init_decode_state(info->shnfile)) {
        return -1;
    }
    info->shnfile->vars.going = 1;

    info->blk_size = 512 * (info->shnfile->wave_header.bits_per_sample / 8) * info->shnfile->wave_header.channels;

    /* read magic number */
#ifdef STRICT_FORMAT_COMPATABILITY
    if(FORMAT_VERSION < 2)
    {
        for(i = 0; i < strlen(magic); i++)
            if(getc_exit(this_shn->vars.fd) != magic[i]) {
                shn_error_fatal(this_shn,"Bad magic number");
                goto exit_thread;
            }

        /* get version number */
        version = getc_exit(this_shn->vars.fd);
    }
    else
#endif /* STRICT_FORMAT_COMPATABILITY */
    {
        int nscan = 0;

        version = MAX_VERSION + 1;
        while(version > MAX_VERSION)
        {
            int byte = ddb_getc(info->shnfile->vars.fd);
            if(byte == EOF) {
                shn_error_fatal(info->shnfile,"No magic number");
                return -1;
            }
            if(info->magic[nscan] != '\0' && byte == info->magic[nscan]) {
                nscan++;
            }
            else {
                if(info->magic[nscan] == '\0' && byte <= MAX_VERSION)
                    version = byte;
                else
                {
                    if(byte == info->magic[0])
                        nscan = 1;
                    else
                    {
                        nscan = 0;
                    }
                    version = MAX_VERSION + 1;
                }
            }
        }
    }

    /* check version number */
    if(version > MAX_SUPPORTED_VERSION) {
        shn_error_fatal(info->shnfile,"Can't decode version %d", version);
        return -1;
    }

    /* set up the default nmean, ignoring the command line state */
    info->nmean = (version < 2) ? DEFAULT_V0NMEAN : DEFAULT_V2NMEAN;

    /* initialise the variable length file read for the compressed stream */
    var_get_init(info->shnfile);
    if (info->shnfile->vars.fatal_error) {
        return -1;
    }

    /* initialise the fixed length file write for the uncompressed stream */
    fwrite_type_init(info->shnfile);

    /* get the internal file type */
    info->internal_ftype = UINT_GET(TYPESIZE, info->shnfile);

    /* has the user requested a change in file type? */
    if(info->internal_ftype != info->ftype) {
        if(info->ftype == TYPE_EOF) {
            info->ftype = info->internal_ftype;    /*  no problems here */
        }
        else {            /* check that the requested conversion is valid */
            if(info->internal_ftype == TYPE_AU1 || info->internal_ftype == TYPE_AU2 ||
                    info->internal_ftype == TYPE_AU3 || info->ftype == TYPE_AU1 || info->ftype == TYPE_AU2 || info->ftype == TYPE_AU3) {
                shn_error_fatal(info->shnfile,"Not able to perform requested output format conversion");
                return -1;
            }
        }
    }

    info->nchan = UINT_GET(CHANSIZE, info->shnfile);

    /* get blocksize if version > 0 */
    if(version > 0)
    {
        int byte;
        info->blocksize = UINT_GET((int) (log((double) DEFAULT_BLOCK_SIZE) / M_LN2),info->shnfile);
        info->maxnlpc = UINT_GET(LPCQSIZE, info->shnfile);
        info->nmean = UINT_GET(0, info->shnfile);
        info->nskip = UINT_GET(NSKIPSIZE, info->shnfile);
        int i;
        for(i = 0; i < info->nskip; i++)
        {
            byte = uvar_get(XBYTESIZE,info->shnfile);
        }
    }
    else
        info->blocksize = DEFAULT_BLOCK_SIZE;

    info->nwrap = MAX(NWRAP, info->maxnlpc);

    /* grab some space for the input buffer */
    info->buffer  = long2d((uint32_t) info->nchan, (uint32_t) (info->blocksize + info->nwrap),info->shnfile);
    if (info->shnfile->vars.fatal_error) {
        return -1;
    }
    info->offset  = long2d((uint32_t) info->nchan, (uint32_t) MAX(1, info->nmean),info->shnfile);
    if (info->shnfile->vars.fatal_error) {
        if (info->buffer) {
            free(info->buffer);
            info->buffer = NULL;
        }
        return -1;
    }

    for(info->chan = 0; info->chan < info->nchan; info->chan++)
    {
        int i;
        for(i = 0; i < info->nwrap; i++)
            info->buffer[info->chan][i] = 0;
        info->buffer[info->chan] += info->nwrap;
    }

    if(info->maxnlpc > 0) {
        info->qlpc = (int*) pmalloc((uint32_t) (info->maxnlpc * sizeof(*info->qlpc)),info->shnfile);
        if (info->shnfile->vars.fatal_error) {
            if (info->buffer) {
                free(info->buffer);
                info->buffer = NULL;
            }
            if (info->offset) {
                free(info->offset);
                info->offset = NULL;
            }
            return -1;
        }
    }

    if(version > 1)
        info->lpcqoffset = V2LPCQOFFSET;

    init_offset (info->offset, info->nchan, MAX(1, info->nmean), info->internal_ftype);

    /* get commands from file and execute them */
    info->chan = 0;
    info->version = version;

    return 0;
}

void shn_init_config() 
{
	shn_cfg.error_output_method = ERROR_OUTPUT_DEVNULL;
	shn_cfg.verbose = 0;
	shn_cfg.swap_bytes = 0;
}

void shn_free_decoder(shn_info_t *info) 
{
    if (info->shnfile) {
        if (info->shnfile->decode_state) {
            if(info->shnfile->decode_state->writebuf != NULL) {
                free(info->shnfile->decode_state->writebuf);
                info->shnfile->decode_state->writebuf = NULL;
            }
            if(info->shnfile->decode_state->writefub != NULL) {
                free(info->shnfile->decode_state->writefub);
                info->shnfile->decode_state->writefub = NULL;
            }
        }
    }
}

void swap_bytes(shn_file *this_shn,int bytes)
{
	int i;
    uint8_t tmp;

	for (i=0;i<bytes;i=i+2) {
		tmp = this_shn->vars.buffer[i+1];
		this_shn->vars.buffer[i+1] = this_shn->vars.buffer[i];
		this_shn->vars.buffer[i] = tmp;
	}
}

int shn_decode(shn_info_t *info)
{
    int i;
    int version = info->version;
    while(1)
    {
        info->cmd = uvar_get(FNSIZE,info->shnfile);
        if (info->shnfile->vars.fatal_error) {
            return -1;
        }

        switch(info->cmd)
        {
        case FN_ZERO:
        case FN_DIFF0:
        case FN_DIFF1:
        case FN_DIFF2:
        case FN_DIFF3:
        case FN_QLPC:
            {
                int32_t coffset, *cbuffer = info->buffer[info->chan];
                int resn = 0, nlpc, j;

                if(info->cmd != FN_ZERO)
                {
                    resn = uvar_get(ENERGYSIZE,info->shnfile);
                    if (info->shnfile->vars.fatal_error) {
                        return -1;
                    }
                    /* this is a hack as version 0 differed in definition of var_get */
                    if(info->version == 0)
                        resn--;
                }

                /* find mean offset : N.B. this code duplicated */
                if(info->nmean == 0)
                    coffset = info->offset[info->chan][0];
                else
                {
                    int32_t sum = (info->version < 2) ? 0 : info->nmean / 2;
                    for(i = 0; i < info->nmean; i++)
                        sum += info->offset[info->chan][i];
                    if(info->version < 2)
                        coffset = sum / info->nmean;
                    else
                        coffset = ROUNDEDSHIFTDOWN(sum / info->nmean, info->bitshift);
                }

                switch(info->cmd)
                {
                case FN_ZERO:
                    for(i = 0; i < info->blocksize; i++)
                        cbuffer[i] = 0;
                    break;
                case FN_DIFF0:
                    for(i = 0; i < info->blocksize; i++) {
                        cbuffer[i] = var_get(resn,info->shnfile) + coffset;
                        if (info->shnfile->vars.fatal_error) {
                            return -1;
                        }
                    }
                    break;
                case FN_DIFF1:
                    for(i = 0; i < info->blocksize; i++) {
                        cbuffer[i] = var_get(resn,info->shnfile) + cbuffer[i - 1];
                        if (info->shnfile->vars.fatal_error) {
                            return -1;
                        }
                    }
                    break;
                case FN_DIFF2:
                    for(i = 0; i < info->blocksize; i++) {
                        cbuffer[i] = var_get(resn,info->shnfile) + (2 * cbuffer[i - 1] -	cbuffer[i - 2]);
                        if (info->shnfile->vars.fatal_error) {
                            return -1;
                        }
                    }
                    break;
                case FN_DIFF3:
                    for(i = 0; i < info->blocksize; i++) {
                        cbuffer[i] = var_get(resn,info->shnfile) + 3 * (cbuffer[i - 1] -  cbuffer[i - 2]) + cbuffer[i - 3];
                        if (info->shnfile->vars.fatal_error) {
                            return -1;
                        }
                    }
                    break;
                case FN_QLPC:
                    nlpc = uvar_get(LPCQSIZE,info->shnfile);
                    if (info->shnfile->vars.fatal_error) {
                        return -1;
                    }

                    for(i = 0; i < nlpc; i++) {
                        info->qlpc[i] = var_get(LPCQUANT,info->shnfile);
                        if (info->shnfile->vars.fatal_error) {
                            return -1;
                        }
                    }
                    for(i = 0; i < nlpc; i++)
                        cbuffer[i - nlpc] -= coffset;
                    for(i = 0; i < info->blocksize; i++)
                    {
                        int32_t sum = info->lpcqoffset;

                        for(j = 0; j < nlpc; j++)
                            sum += info->qlpc[j] * cbuffer[i - j - 1];
                        cbuffer[i] = var_get(resn,info->shnfile) + (sum >> LPCQUANT);
                        if (info->shnfile->vars.fatal_error) {
                            return -1;
                        }
                    }
                    if(coffset != 0)
                        for(i = 0; i < info->blocksize; i++)
                            cbuffer[i] += coffset;
                    break;
                }

                /* store mean value if appropriate : N.B. Duplicated code */
                if(info->nmean > 0)
                {
                    int32_t sum = (info->version < 2) ? 0 : info->blocksize / 2;

                    for(i = 0; i < info->blocksize; i++)
                        sum += cbuffer[i];

                    for(i = 1; i < info->nmean; i++)
                        info->offset[info->chan][i - 1] = info->offset[info->chan][i];
                    if(info->version < 2)
                        info->offset[info->chan][info->nmean - 1] = sum / info->blocksize;
                    else
                        info->offset[info->chan][info->nmean - 1] = (sum / info->blocksize) << info->bitshift;
                }

                /* do the wrap */
                for(i = -info->nwrap; i < 0; i++)
                    cbuffer[i] = cbuffer[i + info->blocksize];

                fix_bitshift(cbuffer, info->blocksize, info->bitshift, info->internal_ftype);

                if(info->chan == info->nchan - 1)
                {
                    if (!info->shnfile->vars.going || info->shnfile->vars.fatal_error) {
                        return -1;
                    }

                    fwrite_type(info->buffer, info->ftype, info->nchan, info->blocksize, info->shnfile);
                    info->chan = (info->chan + 1) % info->nchan;
                    // now we have buffer of size info->shnfile->vars.bytes_in_buf
                    if (shn_cfg.swap_bytes) {
                        swap_bytes(info->shnfile, info->shnfile->vars.bytes_in_buf);
                    }
                    return info->shnfile->vars.bytes_in_buf;


// !!!!!!!!!!!!!!!!!!FIXME
 //                   write_and_wait(info->shnfile,blk_size);

#if 0 // seeking
                    if (info->shnfile->vars.seek_to != -1)
                    {
                        shn_seek_entry *seek_info;
                        int j;

                        shn_debug("Seeking to %d:%02d",info->shnfile->vars.seek_to/60,info->shnfile->vars.seek_to%60);

                        seek_info = shn_seek_entry_search(info->shnfile->seek_table,info->shnfile->vars.seek_to * (ulong)info->shnfile->wave_header.samples_per_sec,0,
                                (ulong)(info->shnfile->vars.seek_table_entries - 1),info->shnfile->vars.seek_resolution);

                        /* loop through number of channels in this file */
                        for (i=0;i<info->nchan;i++) {
                            /* load the three sample buffer values for this channel */
                            for (j=0;j<3;j++)
                                info->buffer[i][j-3] = shn_uchar_to_slong_le(seek_info->data+32+12*i-4*j);

                            /* load the variable number of offset history values for this channel */
                            for (j=0;j<MAX(1,info->nmean);j++)
                                info->offset[i][j]  = shn_uchar_to_slong_le(seek_info->data+48+16*i+4*j);
                        }

                        info->bitshift = shn_uchar_to_ushort_le(seek_info->data+22);

                        info->seekto_offset = shn_uchar_to_ulong_le(seek_info->data+8) + info->shnfile->vars.seek_offset;

                        stdio_seek(info->shnfile->vars.fd,(slong)seekto_offset,SEEK_SET);
                        stdio_read((uchar*) info->shnfile->decode_state->getbuf, 1, BUFSIZ, info->shnfile->vars.fd);

                        info->shnfile->decode_state->getbufp = info->shnfile->decode_state->getbuf + shn_uchar_to_ushort_le(seek_info->data+14);
                        info->shnfile->decode_state->nbitget = shn_uchar_to_ushort_le(seek_info->data+16);
                        info->shnfile->decode_state->nbyteget = shn_uchar_to_ushort_le(seek_info->data+12);
                        info->shnfile->decode_state->gbuffer = shn_uchar_to_ulong_le(seek_info->data+18);

                        info->shnfile->vars.bytes_in_buf = 0;

                        shn_ip.output->flush(info->shnfile->vars.seek_to * 1000);
                        info->shnfile->vars.seek_to = -1;
                    }
#endif

                }
                info->chan = (info->chan + 1) % info->nchan;
                break;
            }

            break;

        case FN_QUIT:
            /* empty out last of buffer */
            info->shnfile->vars.eof = 1;
            if (shn_cfg.swap_bytes) {
                swap_bytes(info->shnfile, info->shnfile->vars.bytes_in_buf);
            }
            return info->shnfile->vars.bytes_in_buf;
#if 0
            write_and_wait(info->shnfile,info->shnfile->vars.bytes_in_buf);
            info->shnfile->vars.eof = 1;

            while (1)
            {
                if (!info->shnfile->vars.going)
                    goto finish;
                if (info->shnfile->vars.seek_to != -1)
                {
                    var_get_quit(info->shnfile);
                    fwrite_type_quit(info->shnfile);

                    if (buffer) free((void *) buffer);
                    if (offset) free((void *) offset);
                    if(maxnlpc > 0 && qlpc)
                        free((void *) qlpc);

                    fseek(info->shnfile->vars.fd,0,SEEK_SET);
                    goto restart;
                }
                else
                    xmms_usleep(10000);
            }

            goto cleanup;
#endif
            break;

        case FN_BLOCKSIZE:
            info->blocksize = UINT_GET((int) (log((double) info->blocksize) / M_LN2), info->shnfile);
            if (info->shnfile->vars.fatal_error) {
                return -1;
            }
            break;
        case FN_BITSHIFT:
            info->bitshift = uvar_get(BITSHIFTSIZE,info->shnfile);
            if (info->shnfile->vars.fatal_error) {
                return -1;
            }
            break;
        case FN_VERBATIM:
            info->cklen = uvar_get(VERBATIM_CKSIZE_SIZE,info->shnfile);
            if (info->shnfile->vars.fatal_error) {
                return -1;
            }

            while (info->cklen--) {
                info->tmp = (uint8_t)uvar_get(VERBATIM_BYTE_SIZE,info->shnfile);
                if (info->shnfile->vars.fatal_error) {
                    return -1;
                }
            }

            break;

        default:
            shn_error_fatal(info->shnfile,"Sanity check fails trying to decode function: %d",info->cmd);
            return -1;
        }
    }
    return 0;
}

int shn_seek_sample(shn_info_t *info, int sample) 
{
    sample += info->startsample;

    info->shnfile->vars.seek_to = sample / info->shnfile->wave_header.samples_per_sec;

    if (info->shnfile->vars.seek_table_entries == NO_SEEK_TABLE) {
        // seek by skipping samples from the start
        if (sample > info->currentsample) {
            info->samples_to_skip = sample - info->currentsample;
        }
        else {
            // restart
            shn_free_decoder (info);
            stdio_rewind (info->shnfile->vars.fd);
            if (shn_init_decoder (info) < 0) {
                return -1;
            }
            info->samples_to_skip = sample;
        }
        info->currentsample = info->shnfile->vars.seek_to * info->shnfile->wave_header.samples_per_sec;
        info->readpos = info->shnfile->vars.seek_to;
        return 0;
    }

    uint32_t seekto_offset;
    int i, j;
    shn_seek_entry *seek_info;

    seek_info = shn_seek_entry_search(info->shnfile->seek_table,info->shnfile->vars.seek_to * (uint32_t)info->shnfile->wave_header.samples_per_sec,0,
            (uint32_t)(info->shnfile->vars.seek_table_entries - 1),info->shnfile->vars.seek_resolution);

    /* loop through number of channels in this file */
    for (i=0;i<info->nchan;i++) {
        /* load the three sample buffer values for this channel */
        for (j=0;j<3;j++)
            info->buffer[i][j-3] = shn_uchar_to_slong_le(seek_info->data+32+12*i-4*j);

        /* load the variable number of offset history values for this channel */
        for (j=0;j<MAX(1,info->nmean);j++)
            info->offset[i][j]  = shn_uchar_to_slong_le(seek_info->data+48+16*i+4*j);
    }

    info->bitshift = shn_uchar_to_ushort_le(seek_info->data+22);

    seekto_offset = shn_uchar_to_ulong_le(seek_info->data+8) + info->shnfile->vars.seek_offset;

    stdio_seek(info->shnfile->vars.fd,(int32_t)seekto_offset,SEEK_SET);
    stdio_read((uint8_t*) info->shnfile->decode_state->getbuf, 1, BUFSIZ, info->shnfile->vars.fd);

    info->shnfile->decode_state->getbufp = info->shnfile->decode_state->getbuf + shn_uchar_to_ushort_le(seek_info->data+14);
    info->shnfile->decode_state->nbitget = shn_uchar_to_ushort_le(seek_info->data+16);
    info->shnfile->decode_state->nbyteget = shn_uchar_to_ushort_le(seek_info->data+12);
    info->shnfile->decode_state->gbuffer = shn_uchar_to_ulong_le(seek_info->data+18);

    info->shnfile->vars.bytes_in_buf = 0;

    info->currentsample = info->shnfile->vars.seek_to * info->shnfile->wave_header.samples_per_sec;
    info->readpos = info->shnfile->vars.seek_to;
    return 0;
}

#define CAPMAXSCHAR(x)  ((x > 127) ? 127 : x)
#define CAPMAXUCHAR(x)  ((x > 255) ? 255 : x)
#define CAPMAXSHORT(x)  ((x > 32767) ? 32767 : x)
#define CAPMAXUSHORT(x) ((x > 65535) ? 65535 : x)

static int sizeof_sample[TYPE_EOF];

void init_sizeof_sample() {
    sizeof_sample[TYPE_AU1]   = sizeof(uint8_t);
    sizeof_sample[TYPE_S8]    = sizeof(int8_t);
    sizeof_sample[TYPE_U8]    = sizeof(uint8_t);
    sizeof_sample[TYPE_S16HL] = sizeof(uint16_t);
    sizeof_sample[TYPE_U16HL] = sizeof(uint16_t);
    sizeof_sample[TYPE_S16LH] = sizeof(uint16_t);
    sizeof_sample[TYPE_U16LH] = sizeof(uint16_t);
    sizeof_sample[TYPE_ULAW]  = sizeof(uint8_t);
    sizeof_sample[TYPE_AU2]   = sizeof(uint8_t);
    sizeof_sample[TYPE_AU3]   = sizeof(uint8_t);
    sizeof_sample[TYPE_ALAW]  = sizeof(uint8_t);
}


/***************/
/* fixed write */
/***************/

void fwrite_type_init(shn_file *this_shn)
{
    init_sizeof_sample();
    this_shn->decode_state->writebuf  = (int8_t*) NULL;
    this_shn->decode_state->writefub  = (int8_t*) NULL;
    this_shn->decode_state->nwritebuf = 0;
}

void fwrite_type_quit(shn_file *this_shn) 
{
    if(this_shn->decode_state->writebuf != NULL) {
        free(this_shn->decode_state->writebuf);
        this_shn->decode_state->writebuf = NULL;
    }
    if(this_shn->decode_state->writefub != NULL) {
        free(this_shn->decode_state->writefub);
        this_shn->decode_state->writefub = NULL;
    }
}

/* convert from signed ints to a given type and write */
void fwrite_type(int32_t **data,int ftype,int nchan,int nitem,shn_file *this_shn)
{
    int hiloint = 1, hilo = !(*((char*) &hiloint));
    int i, nwrite = 0, datasize = sizeof_sample[ftype], chan;
    int32_t *data0 = data[0];
    int bufAvailable = OUT_BUFFER_SIZE - this_shn->vars.bytes_in_buf;

    if(this_shn->decode_state->nwritebuf < nchan * nitem * datasize) {
        this_shn->decode_state->nwritebuf = nchan * nitem * datasize;
        if(this_shn->decode_state->writebuf != NULL) free(this_shn->decode_state->writebuf);
        if(this_shn->decode_state->writefub != NULL) free(this_shn->decode_state->writefub);
        this_shn->decode_state->writebuf = (int8_t*) pmalloc((uint32_t) this_shn->decode_state->nwritebuf,this_shn);
        if (!this_shn->decode_state->writebuf)
            return;
        this_shn->decode_state->writefub = (int8_t*) pmalloc((uint32_t) this_shn->decode_state->nwritebuf,this_shn);
        if (!this_shn->decode_state->writefub)
            return;
    }

    switch(ftype) {
    case TYPE_AU1: /* leave the conversion to fix_bitshift() */
    case TYPE_AU2: {
                       uint8_t *writebufp = (uint8_t*) this_shn->decode_state->writebuf;
                       if(nchan == 1)
                           for(i = 0; i < nitem; i++)
                               *writebufp++ = data0[i];
                       else
                           for(i = 0; i < nitem; i++)
                               for(chan = 0; chan < nchan; chan++)
                                   *writebufp++ = data[chan][i];
                       break;
                   }
    case TYPE_U8: {
                      uint8_t *writebufp = (uint8_t*) this_shn->decode_state->writebuf;
                      if(nchan == 1)
                          for(i = 0; i < nitem; i++)
                              *writebufp++ = CAPMAXUCHAR(data0[i]);
                      else
                          for(i = 0; i < nitem; i++)
                              for(chan = 0; chan < nchan; chan++)
                                  *writebufp++ =  CAPMAXUCHAR(data[chan][i]);
                      break;
                  }
    case TYPE_S8: {
                      int8_t *writebufp = (int8_t*) this_shn->decode_state->writebuf;
                      if(nchan == 1)
                          for(i = 0; i < nitem; i++)
                              *writebufp++ = CAPMAXSCHAR(data0[i]);
                      else
                          for(i = 0; i < nitem; i++)
                              for(chan = 0; chan < nchan; chan++)
                                  *writebufp++ = CAPMAXSCHAR(data[chan][i]);
                      break;
                  }
    case TYPE_S16HL:
    case TYPE_S16LH: {
                         short *writebufp = (short*) this_shn->decode_state->writebuf;
                         if(nchan == 1)
                             for(i = 0; i < nitem; i++)
                                 *writebufp++ = CAPMAXSHORT(data0[i]);
                         else
                             for(i = 0; i < nitem; i++)
                                 for(chan = 0; chan < nchan; chan++)
                                     *writebufp++ = CAPMAXSHORT(data[chan][i]);
                         break;
                     }
    case TYPE_U16HL:
    case TYPE_U16LH: {
                         uint16_t *writebufp = (uint16_t*) this_shn->decode_state->writebuf;
                         if(nchan == 1)
                             for(i = 0; i < nitem; i++)
                                 *writebufp++ = CAPMAXUSHORT(data0[i]);
                         else
                             for(i = 0; i < nitem; i++)
                                 for(chan = 0; chan < nchan; chan++)
                                     *writebufp++ = CAPMAXUSHORT(data[chan][i]);
                         break;
                     }
    case TYPE_ULAW: {
                        uint8_t *writebufp = (uint8_t*) this_shn->decode_state->writebuf;
                        if(nchan == 1)
                            for(i = 0; i < nitem; i++)
                                *writebufp++ = Slinear2ulaw(CAPMAXSHORT((data0[i] << 3)));
                        else
                            for(i = 0; i < nitem; i++)
                                for(chan = 0; chan < nchan; chan++)
                                    *writebufp++ = Slinear2ulaw(CAPMAXSHORT((data[chan][i] << 3)));
                        break;
                    }
    case TYPE_AU3: {
                       uint8_t *writebufp = (uint8_t*) this_shn->decode_state->writebuf;
                       if(nchan == 1)
                           for(i = 0; i < nitem; i++)
                               if(data0[i] < 0)
                                   *writebufp++ = (127 - data0[i]) ^ 0xd5;
                               else
                                   *writebufp++ = (data0[i] + 128) ^ 0x55;
                       else
                           for(i = 0; i < nitem; i++)
                               for(chan = 0; chan < nchan; chan++)
                                   if(data[chan][i] < 0)
                                       *writebufp++ = (127 - data[chan][i]) ^ 0xd5;
                                   else
                                       *writebufp++ = (data[chan][i] + 128) ^ 0x55;
                       break;
                   }
    case TYPE_ALAW: {
                        uint8_t *writebufp = (uint8_t*) this_shn->decode_state->writebuf;
                        if(nchan == 1)
                            for(i = 0; i < nitem; i++)
                                *writebufp++ = Slinear2alaw(CAPMAXSHORT((data0[i] << 3)));
                        else
                            for(i = 0; i < nitem; i++)
                                for(chan = 0; chan < nchan; chan++)
                                    *writebufp++ = Slinear2alaw(CAPMAXSHORT((data[chan][i] << 3)));
                        break;
                    }
    }

    switch(ftype) {
    case TYPE_AU1:
    case TYPE_S8:
    case TYPE_U8:
    case TYPE_ULAW:
    case TYPE_AU2:
    case TYPE_AU3:
    case TYPE_ALAW:
        if (datasize*nchan*nitem <= bufAvailable) {
            memcpy((void *)&this_shn->vars.buffer[this_shn->vars.bytes_in_buf],(const void *)this_shn->decode_state->writebuf,datasize*nchan*nitem);
            this_shn->vars.bytes_in_buf += datasize*nchan*nitem;
            nwrite = nitem;
        }
        else
            shn_debug("Buffer overrun in fwrite_type() [case 1]: %d bytes to read, but only %d bytes are available",datasize*nchan*nitem,bufAvailable);
        break;
    case TYPE_S16HL:
    case TYPE_U16HL:
        if(hilo)
        {
            if (datasize*nchan*nitem <= bufAvailable) {
                memcpy((void *)&this_shn->vars.buffer[this_shn->vars.bytes_in_buf],(const void *)this_shn->decode_state->writebuf,datasize*nchan*nitem);
                this_shn->vars.bytes_in_buf += datasize*nchan*nitem;
                nwrite = nitem;
            }
            else
                shn_debug("Buffer overrun in fwrite_type() [case 2]: %d bytes to read, but only %d bytes are available",datasize*nchan*nitem,bufAvailable);
        }
        else
        {
            swab(this_shn->decode_state->writebuf, this_shn->decode_state->writefub, datasize * nchan * nitem);
            if (datasize*nchan*nitem <= bufAvailable) {
                memcpy((void *)&this_shn->vars.buffer[this_shn->vars.bytes_in_buf],(const void *)this_shn->decode_state->writefub,datasize*nchan*nitem);
                this_shn->vars.bytes_in_buf += datasize*nchan*nitem;
                nwrite = nitem;
            }
            else
                shn_debug("Buffer overrun in fwrite_type() [case 3]: %d bytes to read, but only %d bytes are available",datasize*nchan*nitem,bufAvailable);
        }
        break;
    case TYPE_S16LH:
    case TYPE_U16LH:
        if(hilo)
        {
            swab(this_shn->decode_state->writebuf, this_shn->decode_state->writefub, datasize * nchan * nitem);
            if (datasize*nchan*nitem <= bufAvailable) {
                memcpy((void *)&this_shn->vars.buffer[this_shn->vars.bytes_in_buf],(const void *)this_shn->decode_state->writefub,datasize*nchan*nitem);
                this_shn->vars.bytes_in_buf += datasize*nchan*nitem;
                nwrite = nitem;
            }
            else
                shn_debug("Buffer overrun in fwrite_type() [case 4]: %d bytes to read, but only %d bytes are available",datasize*nchan*nitem,bufAvailable);
        }
        else
        {
            if (datasize*nchan*nitem <= bufAvailable) {
                memcpy((void *)&this_shn->vars.buffer[this_shn->vars.bytes_in_buf],(const void *)this_shn->decode_state->writebuf,datasize*nchan*nitem);
                this_shn->vars.bytes_in_buf += datasize*nchan*nitem;
                nwrite = nitem;
            }
            else
                shn_debug("Buffer overrun in fwrite_type() [case 5]: %d bytes to read, but only %d bytes are available",datasize*nchan*nitem,bufAvailable);
        }
        break;
    }

    if(nwrite != nitem)
        shn_error_fatal(this_shn,"Failed to write decompressed stream -\npossible corrupt or truncated file");
}

/*************/
/* bitshifts */
/*************/
void fix_bitshift(int32_t *buffer, int nitem, int bitshift, int ftype)
{
  int i;

  if(ftype == TYPE_AU1)
    for(i = 0; i < nitem; i++)
      buffer[i] = ulaw_outward[bitshift][buffer[i] + 128];
  else if(ftype == TYPE_AU2)
    for(i = 0; i < nitem; i++) {
      if(buffer[i] >= 0)
	buffer[i] = ulaw_outward[bitshift][buffer[i] + 128];
      else if(buffer[i] == -1)
	buffer[i] =  NEGATIVE_ULAW_ZERO;
      else
	buffer[i] = ulaw_outward[bitshift][buffer[i] + 129];
    }
  else
    if(bitshift != 0)
      for(i = 0; i < nitem; i++)
	buffer[i] <<= bitshift;
}

int get_wave_header(shn_file *this_shn)
{
    int32_t  **buffer = NULL, **offset = NULL;
    int32_t  lpcqoffset = 0;
    int   version = FORMAT_VERSION, bitshift = 0;
    int   ftype = TYPE_EOF;
    char  *magic = MAGIC;
    int   blocksize = DEFAULT_BLOCK_SIZE, nchan = DEFAULT_NCHAN;
    int   i, chan, nwrap, nskip = DEFAULT_NSKIP;
    int   *qlpc = NULL, maxnlpc = DEFAULT_MAXNLPC, nmean = UNDEFINED_UINT;
    int   cmd;
    int   internal_ftype;
    int   cklen;
    int   retval = 0;

    if (!init_decode_state(this_shn))
        return 0;

    /***********************/
    /* EXTRACT starts here */
    /***********************/

    /* read magic number */
#ifdef STRICT_FORMAT_COMPATABILITY
    if(FORMAT_VERSION < 2)
    {
        for(i = 0; i < strlen(magic); i++) {
            if(getc_exit(this_shn->vars.fd) != magic[i])
                return 0;
            this_shn->vars.bytes_read++;
        }

        /* get version number */
        version = getc_exit(this_shn->vars.fd);
        this_shn->vars.bytes_read++;
    }
    else
#endif /* STRICT_FORMAT_COMPATABILITY */
    {
        int nscan = 0;

        version = MAX_VERSION + 1;
        while(version > MAX_VERSION)
        {
            int byte = ddb_getc(this_shn->vars.fd);
            this_shn->vars.bytes_read++;
            if(byte == EOF)
                return 0;
            if(magic[nscan] != '\0' && byte == magic[nscan])
                nscan++;
            else
                if(magic[nscan] == '\0' && byte <= MAX_VERSION)
                    version = byte;
                else
                {
                    if(byte == magic[0])
                        nscan = 1;
                    else
                    {
                        nscan = 0;
                    }
                    version = MAX_VERSION + 1;
                }
        }
    }

    /* check version number */
    if(version > MAX_SUPPORTED_VERSION)
        return 0;

    /* set up the default nmean, ignoring the command line state */
    nmean = (version < 2) ? DEFAULT_V0NMEAN : DEFAULT_V2NMEAN;

    /* initialise the variable length file read for the compressed stream */
    var_get_init(this_shn);
    if (this_shn->vars.fatal_error)
        return 0;

    /* initialise the fixed length file write for the uncompressed stream */
    fwrite_type_init(this_shn);

    /* get the internal file type */
    internal_ftype = UINT_GET(TYPESIZE, this_shn);

    /* has the user requested a change in file type? */
    if(internal_ftype != ftype) {
        if(ftype == TYPE_EOF) {
            ftype = internal_ftype;    /*  no problems here */
        }
        else {           /* check that the requested conversion is valid */
            if(internal_ftype == TYPE_AU1 || internal_ftype == TYPE_AU2 ||
                    internal_ftype == TYPE_AU3 || ftype == TYPE_AU1 ||ftype == TYPE_AU2 || ftype == TYPE_AU3)
            {
                retval = 0;
                goto got_enough_data;
            }
        }
    }

    nchan = UINT_GET(CHANSIZE, this_shn);
    this_shn->vars.actual_nchan = nchan;

    /* get blocksize if version > 0 */
    if(version > 0)
    {
        int byte;
        blocksize = UINT_GET((int) (log((double) DEFAULT_BLOCK_SIZE) / M_LN2),this_shn);
        maxnlpc = UINT_GET(LPCQSIZE, this_shn);
        this_shn->vars.actual_maxnlpc = maxnlpc;
        nmean = UINT_GET(0, this_shn);
        this_shn->vars.actual_nmean = nmean;
        nskip = UINT_GET(NSKIPSIZE, this_shn);
        for(i = 0; i < nskip; i++)
        {
            byte = uvar_get(XBYTESIZE,this_shn);
        }
    }
    else
        blocksize = DEFAULT_BLOCK_SIZE;

    nwrap = MAX(NWRAP, maxnlpc);

    /* grab some space for the input buffer */
    buffer  = long2d((uint32_t) nchan, (uint32_t) (blocksize + nwrap),this_shn);
    if (this_shn->vars.fatal_error)
        return 0;
    offset  = long2d((uint32_t) nchan, (uint32_t) MAX(1, nmean),this_shn);
    if (this_shn->vars.fatal_error) {
        if (buffer) {
            free(buffer);
            buffer = NULL;
        }
        return 0;
    }

    for(chan = 0; chan < nchan; chan++)
    {
        for(i = 0; i < nwrap; i++)
            buffer[chan][i] = 0;
        buffer[chan] += nwrap;
    }

    if(maxnlpc > 0) {
        qlpc = (int*) pmalloc((uint32_t) (maxnlpc * sizeof(*qlpc)),this_shn);
        if (this_shn->vars.fatal_error) {
            if (buffer) {
                free(buffer);
                buffer = NULL;
            }
            if (offset) {
                free(offset);
                buffer = NULL;
            }
            return 0;
        }
    }

    if(version > 1)
        lpcqoffset = V2LPCQOFFSET;

    init_offset(offset, nchan, MAX(1, nmean), internal_ftype);

    /* get commands from file and execute them */
    chan = 0;
    while(1)
    {
        this_shn->vars.reading_function_code = 1;
        cmd = uvar_get(FNSIZE,this_shn);
        this_shn->vars.reading_function_code = 0;

        switch(cmd)
        {
        case FN_ZERO:
        case FN_DIFF0:
        case FN_DIFF1:
        case FN_DIFF2:
        case FN_DIFF3:
        case FN_QLPC:
            {
                int32_t coffset, *cbuffer = buffer[chan];
                int resn = 0, nlpc, j;

                if(cmd != FN_ZERO)
                {
                    resn = uvar_get(ENERGYSIZE,this_shn);
                    if (this_shn->vars.fatal_error) {
                        retval = 0;
                        goto got_enough_data;
                    }
                    /* this is a hack as version 0 differed in definition of var_get */
                    if(version == 0)
                        resn--;
                }

                /* find mean offset : N.B. this code duplicated */
                if(nmean == 0)
                    coffset = offset[chan][0];
                else
                {
                    int32_t sum = (version < 2) ? 0 : nmean / 2;
                    for(i = 0; i < nmean; i++)
                        sum += offset[chan][i];
                    if(version < 2)
                        coffset = sum / nmean;
                    else
                        coffset = ROUNDEDSHIFTDOWN(sum / nmean, bitshift);
                }

                switch(cmd)
                {
                case FN_ZERO:
                    for(i = 0; i < blocksize; i++)
                        cbuffer[i] = 0;
                    break;
                case FN_DIFF0:
                    for(i = 0; i < blocksize; i++) {
                        cbuffer[i] = var_get(resn,this_shn) + coffset;
                        if (this_shn->vars.fatal_error) {
                            retval = 0;
                            goto got_enough_data;
                        }
                    }
                    break;
                case FN_DIFF1:
                    for(i = 0; i < blocksize; i++) {
                        cbuffer[i] = var_get(resn,this_shn) + cbuffer[i - 1];
                        if (this_shn->vars.fatal_error) {
                            retval = 0;
                            goto got_enough_data;
                        }
                    }
                    break;
                case FN_DIFF2:
                    for(i = 0; i < blocksize; i++) {
                        cbuffer[i] = var_get(resn,this_shn) + (2 * cbuffer[i - 1] -	cbuffer[i - 2]);
                        if (this_shn->vars.fatal_error) {
                            retval = 0;
                            goto got_enough_data;
                        }
                    }
                    break;
                case FN_DIFF3:
                    for(i = 0; i < blocksize; i++) {
                        cbuffer[i] = var_get(resn,this_shn) + 3 * (cbuffer[i - 1] -  cbuffer[i - 2]) + cbuffer[i - 3];
                        if (this_shn->vars.fatal_error) {
                            retval = 0;
                            goto got_enough_data;
                        }
                    }
                    break;
                case FN_QLPC:
                    nlpc = uvar_get(LPCQSIZE,this_shn);
                    if (this_shn->vars.fatal_error) {
                        retval = 0;
                        goto got_enough_data;
                    }

                    for(i = 0; i < nlpc; i++) {
                        qlpc[i] = var_get(LPCQUANT,this_shn);
                        if (this_shn->vars.fatal_error) {
                            retval = 0;
                            goto got_enough_data;
                        }
                    }
                    for(i = 0; i < nlpc; i++)
                        cbuffer[i - nlpc] -= coffset;
                    for(i = 0; i < blocksize; i++)
                    {
                        int32_t sum = lpcqoffset;

                        for(j = 0; j < nlpc; j++)
                            sum += qlpc[j] * cbuffer[i - j - 1];
                        cbuffer[i] = var_get(resn,this_shn) + (sum >> LPCQUANT);
                        if (this_shn->vars.fatal_error) {
                            retval = 0;
                            goto got_enough_data;
                        }
                    }
                    if(coffset != 0)
                        for(i = 0; i < blocksize; i++)
                            cbuffer[i] += coffset;
                    break;
                }

                /* store mean value if appropriate : N.B. Duplicated code */
                if(nmean > 0)
                {
                    int32_t sum = (version < 2) ? 0 : blocksize / 2;

                    for(i = 0; i < blocksize; i++)
                        sum += cbuffer[i];

                    for(i = 1; i < nmean; i++)
                        offset[chan][i - 1] = offset[chan][i];
                    if(version < 2)
                        offset[chan][nmean - 1] = sum / blocksize;
                    else
                        offset[chan][nmean - 1] = (sum / blocksize) << bitshift;
                }

                if (0 == chan) {
                    this_shn->vars.initial_file_position = this_shn->vars.last_file_position_no_really;
                    goto got_enough_data;
                }

                /* do the wrap */
                for(i = -nwrap; i < 0; i++)
                    cbuffer[i] = cbuffer[i + blocksize];

                fix_bitshift(cbuffer, blocksize, bitshift, internal_ftype);

                if(chan == nchan - 1)
                {
                    fwrite_type(buffer, ftype, nchan, blocksize, this_shn);
                    this_shn->vars.bytes_in_buf = 0;
                }

                chan = (chan + 1) % nchan;
                break;
            }
            break;

        case FN_BLOCKSIZE:
            UINT_GET((int) (log((double) blocksize) / M_LN2), this_shn);
            break;

        case FN_VERBATIM:
            cklen = uvar_get(VERBATIM_CKSIZE_SIZE,this_shn);

            while (cklen--) {
                if (this_shn->vars.bytes_in_header >= OUT_BUFFER_SIZE) {
                    goto got_enough_data;
                }
                this_shn->vars.bytes_in_buf = 0;
                this_shn->vars.header[this_shn->vars.bytes_in_header++] = (char)uvar_get(VERBATIM_BYTE_SIZE,this_shn);
            }
            retval = 1;
            break;

        case FN_BITSHIFT:
            bitshift = uvar_get(BITSHIFTSIZE,this_shn);
            this_shn->vars.actual_bitshift = bitshift;
            break;

        default:
            goto got_enough_data;
        }
    }

got_enough_data:

    /* wind up */
    var_get_quit(this_shn);
    fwrite_type_quit(this_shn);

    if (buffer) free((void *) buffer);
    if (offset) free((void *) offset);
    if(maxnlpc > 0 && qlpc)
        free((void *) qlpc);

    this_shn->vars.bytes_in_buf = 0;

    return retval;
}

void shn_unload(shn_file *this_shn)
{
	if (this_shn)
	{
		if (this_shn->vars.fd)
		{
			stdio_close(this_shn->vars.fd);
			this_shn->vars.fd = NULL;
		}

		if (this_shn->decode_state)
		{
			if (this_shn->decode_state->getbuf)
			{
				free(this_shn->decode_state->getbuf);
				this_shn->decode_state->getbuf = NULL;
			}

			if (this_shn->decode_state->writebuf)
			{
				free(this_shn->decode_state->writebuf);
				this_shn->decode_state->writebuf = NULL;
			}

			if (this_shn->decode_state->writefub)
			{
				free(this_shn->decode_state->writefub);
				this_shn->decode_state->writefub = NULL;
			}

			free(this_shn->decode_state);
			this_shn->decode_state = NULL;
		}

		if (this_shn->seek_table)
		{
			free(this_shn->seek_table);
			this_shn->seek_table = NULL;
		}

		free(this_shn);
		this_shn = NULL;
	}
}

shn_file *load_shn(const char *filename)
{
	shn_file *tmp_file;
    shn_seek_entry *first_seek_table;

	shn_debug("Loading file: '%s'",filename);

	if (!(tmp_file = malloc(sizeof(shn_file))))
	{
		shn_debug("Could not allocate memory for SHN data structure");
		return NULL;
	}

	memset(tmp_file, 0, sizeof(shn_file));

	tmp_file->vars.fd = NULL;
	tmp_file->vars.seek_to = -1;
	tmp_file->vars.eof = 0;
	tmp_file->vars.going = 0;
	tmp_file->vars.seek_table_entries = NO_SEEK_TABLE;
	tmp_file->vars.bytes_in_buf = 0;
	tmp_file->vars.bytes_in_header = 0;
	tmp_file->vars.reading_function_code = 0;
	tmp_file->vars.initial_file_position = 0;
	tmp_file->vars.last_file_position = 0;
	tmp_file->vars.last_file_position_no_really = 0;
	tmp_file->vars.bytes_read = 0;
	tmp_file->vars.actual_bitshift = 0;
	tmp_file->vars.actual_maxnlpc = 0;
	tmp_file->vars.actual_nmean = 0;
	tmp_file->vars.actual_nchan = 0;
	tmp_file->vars.seek_offset = 0;

	tmp_file->decode_state = NULL;

	tmp_file->wave_header.filename = filename;
	tmp_file->wave_header.wave_format = 0;
	tmp_file->wave_header.channels = 0;
	tmp_file->wave_header.block_align = 0;
	tmp_file->wave_header.bits_per_sample = 0;
	tmp_file->wave_header.samples_per_sec = 0;
	tmp_file->wave_header.avg_bytes_per_sec = 0;
	tmp_file->wave_header.rate = 0;
	tmp_file->wave_header.header_size = 0;
	tmp_file->wave_header.data_size = 0;
	tmp_file->wave_header.file_has_id3v2_tag = 0;
	tmp_file->wave_header.id3v2_tag_size = 0;

	tmp_file->seek_header.version = NO_SEEK_TABLE;
	tmp_file->seek_header.shnFileSize = 0;

	tmp_file->seek_trailer.seekTableSize = 0;

	tmp_file->seek_table = NULL;

    tmp_file->vars.fd = stdio_open (filename);
    if (!tmp_file->vars.fd) {
		shn_debug("Could not open file: '%s'",filename);
		shn_unload(tmp_file);
		return NULL;
    }

    tmp_file->wave_header.id3v2_tag_size = stdio_get_leading_size (tmp_file->vars.fd);
    if (tmp_file->wave_header.id3v2_tag_size > 0) {
        tmp_file->wave_header.file_has_id3v2_tag = 2;
        if (0 != stdio_seek(tmp_file->vars.fd,(long)tmp_file->wave_header.id3v2_tag_size,SEEK_SET)) {
            shn_debug("Error while discarding ID3v2 tag in file '%s'.",filename);
            stdio_rewind (tmp_file->vars.fd);
        }
    }

	if (0 == get_wave_header(tmp_file))
	{
		shn_debug("Unable to read WAVE header from file '%s'",filename);
		shn_unload(tmp_file);
		return NULL;
	}

	if (tmp_file->wave_header.file_has_id3v2_tag)
	{
		stdio_seek(tmp_file->vars.fd,tmp_file->wave_header.id3v2_tag_size,SEEK_SET);
		tmp_file->vars.bytes_read += tmp_file->wave_header.id3v2_tag_size;
		tmp_file->vars.seek_offset = tmp_file->wave_header.id3v2_tag_size;
	}
    else
	{
		stdio_seek(tmp_file->vars.fd,0,SEEK_SET);
	}

	if (0 == shn_verify_header(tmp_file))
	{
		shn_debug("Invalid WAVE header in file: '%s'",filename);
		shn_unload(tmp_file);
		return NULL;
	}

	if (tmp_file->decode_state)
	{
		free(tmp_file->decode_state);
		tmp_file->decode_state = NULL;
	}

	shn_load_seek_table(tmp_file,filename);

	if (NO_SEEK_TABLE != tmp_file->vars.seek_table_entries)
	{
		/* verify seek tables */

		first_seek_table = (shn_seek_entry *)tmp_file->seek_table;

		if (tmp_file->vars.actual_bitshift != shn_uchar_to_ushort_le(first_seek_table->data+22))
		{
			/* initial bitshift value in the file does not match the first bitshift value of the first seektable entry - seeking is broken */
			shn_debug("Broken seek table detected (invalid bitshift) - seeking disabled for this file.");
			tmp_file->vars.seek_table_entries = NO_SEEK_TABLE;
		}
		else if (tmp_file->vars.actual_nchan > 2)
		{
			/* nchan is greater than the number of such entries stored in a seek table entry - seeking won't work */
			shn_debug("Broken seek table detected (nchan %d not in range [1 .. 2]) - seeking disabled for this file.",tmp_file->vars.actual_nchan);
			tmp_file->vars.seek_table_entries = NO_SEEK_TABLE;
		}
		else if (tmp_file->vars.actual_maxnlpc > 3)
		{
			/* maxnlpc is greater than the number of such entries stored in a seek table entry - seeking won't work */
			shn_debug("Broken seek table detected (maxnlpc %d not in range [0 .. 3]) - seeking disabled for this file.",tmp_file->vars.actual_maxnlpc);
			tmp_file->vars.seek_table_entries = NO_SEEK_TABLE;
		}
		else if (tmp_file->vars.actual_nmean > 4)
		{
			/* nmean is greater than the number of such entries stored in a seek table entry - seeking won't work */
			shn_debug("Broken seek table detected (nmean %d not in range [0 .. 4]) - seeking disabled for this file.",tmp_file->vars.actual_nmean);
			tmp_file->vars.seek_table_entries = NO_SEEK_TABLE;
		}
		else
		{
			/* seek table appears to be valid - now adjust byte offsets in seek table to match the file */
			tmp_file->vars.seek_offset += tmp_file->vars.initial_file_position - shn_uchar_to_ulong_le(first_seek_table->data+8);

			if (0 != tmp_file->vars.seek_offset)
			{
				shn_debug("Adjusting seek table offsets by %ld bytes due to mismatch between seek table values and input file - seeking might not work correctly.",
					tmp_file->vars.seek_offset);
			}
		}
	}

	shn_debug("Successfully loaded file: '%s'",filename);

	return tmp_file;
}
