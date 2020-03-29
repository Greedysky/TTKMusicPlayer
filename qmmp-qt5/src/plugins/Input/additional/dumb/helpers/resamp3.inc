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
 * resamp3.inc - Resampling helper template.          / / \  \
 *                                                   | <  /   \_
 * By Bob and entheh.                                |  \/ /\   /
 *                                                    \_  /  > /
 * In order to find a good trade-off between            | \ / /
 * speed and accuracy in this code, some tests          |  ' /
 * were carried out regarding the behaviour of           \__/
 * long long ints with gcc. The following code
 * was tested:
 *
 * int a, b, c;
 * c = ((long long)a * b) >> 16;
 *
 * DJGPP GCC Version 3.0.3 generated the following assembly language code for
 * the multiplication and scaling, leaving the 32-bit result in EAX.
 *
 * movl  -8(%ebp), %eax    ; read one int into EAX
 * imull -4(%ebp)          ; multiply by the other; result goes in EDX:EAX
 * shrdl $16, %edx, %eax   ; shift EAX right 16, shifting bits in from EDX
 *
 * Note that a 32*32->64 multiplication is performed, allowing for high
 * accuracy. On the Pentium 2 and above, shrdl takes two cycles (generally),
 * so it is a minor concern when four multiplications are being performed
 * (the cubic resampler). On the Pentium MMX and earlier, it takes four or
 * more cycles, so this method is unsuitable for use in the low-quality
 * resamplers.
 *
 * Since "long long" is a gcc-specific extension, we use LONG_LONG instead,
 * defined in dumb.h. We may investigate later what code MSVC generates, but
 * if it seems too slow then we suggest you use a good compiler.
 *
 * FIXME: these comments are somewhat out of date now.
 */



long dumb_resample(DUMB_RESAMPLER *resampler, sample_t *dst, long dst_size, VOLUME_PARAMETERS, float delta)
{
	int dt, inv_dt;
	int VOLUME_VARIABLES;
	long done;
	long todo;
	LONG_LONG todo64;
	int quality;
	int check;

	if (!resampler || resampler->dir == 0) return 0;
	ASSERT(resampler->dir == -1 || resampler->dir == 1);

	done = 0;
	dt = (int)(delta * 65536.0 + 0.5);
        if (dt == 0 || dt == (int)-0x80000000) return 0;
        inv_dt = (int)(1.0 / delta * 65536.0 + 0.5);
	SET_VOLUME_VARIABLES;

	if (VOLUMES_ARE_ZERO) dst = NULL;

	_dumb_init_cubic();

	quality = resampler->quality;

	while (done < dst_size) {
                if (process_pickup(resampler)) {
			RETURN_VOLUME_VARIABLES;
			return done;
		}

		if ((resampler->dir ^ dt) < 0)
			dt = -dt;

		if (resampler->dir < 0)
			todo64 = ((((LONG_LONG)(resampler->pos - resampler->start) << 16) + resampler->subpos - dt) / -dt);
		else
			todo64 = ((((LONG_LONG)(resampler->end - resampler->pos) << 16) - resampler->subpos - 1 + dt) / dt);

		if (todo64 < 0)
			todo = 0;
		else if (todo64 > dst_size - done)
			todo = dst_size - done;
		else
			todo = (long) todo64;

		done += todo;

		{
			SRCTYPE *src = resampler->src;
			long pos = resampler->pos;
			int subpos = resampler->subpos;
			long diff = pos;
			long overshot;
			if (resampler->dir < 0) {
				if (!dst) {
					/* Silence or simulation */
					LONG_LONG new_subpos = subpos + (LONG_LONG)dt * todo;
					pos += (long)(new_subpos >> 16);
					subpos = (long)new_subpos & 65535;
                                } else {
                                    /* FIR resampling, backwards */
                                    SRCTYPE *x;
                                    if ( resampler->fir_resampler_ratio != delta ) {
                                        resampler_set_rate( resampler->fir_resampler[0], delta );
                                        resampler_set_rate( resampler->fir_resampler[1], delta );
                                        resampler->fir_resampler_ratio = delta;
                                    }
                                    x = &src[pos*SRC_CHANNELS];
                                    while ( todo ) {
                                            while ( ( resampler_get_free_count( resampler->fir_resampler[0] ) ||
                                            (!resampler_get_sample_count( resampler->fir_resampler[0] )
#if SRC_CHANNELS == 2
                                            && !resampler_get_sample_count( resampler->fir_resampler[1] )
#endif
                                            ) ) && pos >= resampler->start )
                                            {
                                                    POKE_FIR(0);
                                                    pos--;
                                                    x -= SRC_CHANNELS;
                                            }
                                            if ( !resampler_get_sample_count( resampler->fir_resampler[0] ) ) break;
                                            MIX_FIR;
                                            ADVANCE_FIR;
                                            --todo;
                                    }
                                    done -= todo;
                                }
				diff = diff - pos;
				overshot = resampler->start - pos - 1;
				if (diff >= 3) {
					COPYSRC2(resampler->X, 0, overshot < 3, src, pos+3);
					COPYSRC2(resampler->X, 1, overshot < 2, src, pos+2);
					COPYSRC2(resampler->X, 2, overshot < 1, src, pos+1);
				} else if (diff >= 2) {
					COPYSRC(resampler->X, 0, resampler->X, 2);
					COPYSRC2(resampler->X, 1, overshot < 2, src, pos+2);
					COPYSRC2(resampler->X, 2, overshot < 1, src, pos+1);
				} else if (diff >= 1) {
					COPYSRC(resampler->X, 0, resampler->X, 1);
					COPYSRC(resampler->X, 1, resampler->X, 2);
					COPYSRC2(resampler->X, 2, overshot < 1, src, pos+1);
				}
			} else {
				if (!dst) {
					/* Silence or simulation */
					LONG_LONG new_subpos = subpos + (LONG_LONG)dt * todo;
					pos += (long)(new_subpos >> 16);
					subpos = (long)new_subpos & 65535;
                                } else {
                                    /* FIR resampling, forwards */
                                    SRCTYPE *x;
                                    if ( resampler->fir_resampler_ratio != delta ) {
                                        resampler_set_rate( resampler->fir_resampler[0], delta );
                                        resampler_set_rate( resampler->fir_resampler[1], delta );
                                        resampler->fir_resampler_ratio = delta;
                                    }
                                    x = &src[pos*SRC_CHANNELS];
                                    while ( todo ) {
                                            while ( ( resampler_get_free_count( resampler->fir_resampler[0] ) ||
                                            (!resampler_get_sample_count( resampler->fir_resampler[0] )
#if SRC_CHANNELS == 2
                                            && !resampler_get_sample_count( resampler->fir_resampler[1] )
#endif
                                            ) ) && pos < resampler->end )
                                            {
                                                    POKE_FIR(0);
                                                    pos++;
                                                    x += SRC_CHANNELS;
                                            }
                                            if ( !resampler_get_sample_count( resampler->fir_resampler[0] ) ) break;
                                            MIX_FIR;
                                            ADVANCE_FIR;
                                            --todo;
                                    }
                                    done -= todo;
                                }
				diff = pos - diff;
				overshot = pos - resampler->end;
				if (diff >= 3) {
					COPYSRC2(resampler->X, 0, overshot < 3, src, pos-3);
					COPYSRC2(resampler->X, 1, overshot < 2, src, pos-2);
					COPYSRC2(resampler->X, 2, overshot < 1, src, pos-1);
				} else if (diff >= 2) {
					COPYSRC(resampler->X, 0, resampler->X, 2);
					COPYSRC2(resampler->X, 1, overshot < 2, src, pos-2);
					COPYSRC2(resampler->X, 2, overshot < 1, src, pos-1);
				} else if (diff >= 1) {
					COPYSRC(resampler->X, 0, resampler->X, 1);
					COPYSRC(resampler->X, 1, resampler->X, 2);
					COPYSRC2(resampler->X, 2, overshot < 1, src, pos-1);
				}
			}
			resampler->pos = pos;
			resampler->subpos = subpos;
		}
	}

	RETURN_VOLUME_VARIABLES;
	return done;
}



void dumb_resample_get_current_sample(DUMB_RESAMPLER *resampler, VOLUME_PARAMETERS, sample_t *dst)
{
	int VOLUME_VARIABLES;
	SRCTYPE *src;
	long pos;
	int subpos;
	int quality;
	SRCTYPE *x;

	if (!resampler || resampler->dir == 0) { MIX_ZEROS(=); return; }
	ASSERT(resampler->dir == -1 || resampler->dir == 1);

	if (process_pickup(resampler)) { MIX_ZEROS(=); return; }

	SET_VOLUME_VARIABLES;

	if (VOLUMES_ARE_ZERO) { MIX_ZEROS(=); return; }

	_dumb_init_cubic();

	quality = resampler->quality;

	src = resampler->src;
	pos = resampler->pos;
	subpos = resampler->subpos;
	x = resampler->X;

	if (resampler->dir < 0) {
		HEAVYASSERT(pos >= resampler->start);
                        /* FIR resampling, backwards */
                        PEEK_FIR;
	} else {
		HEAVYASSERT(pos < resampler->end);
                        /* FIR resampling, forwards */
                        PEEK_FIR;
	}
}



#undef MIX_ZEROS
#undef MIX_FIR
#undef PEEK_FIR
#undef VOLUMES_ARE_ZERO
#undef SET_VOLUME_VARIABLES
#undef RETURN_VOLUME_VARIABLES
#undef VOLUME_VARIABLES
#undef VOLUME_PARAMETERS
#undef SUFFIX3
