/*
Copyright (C) 2003-2010, Mark Borgerding

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
    * Neither the author nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdlib.h>
#include <string.h>

#include "kiss_fft_guts.h"
/* The guts header contains all the multiplication and addition macros that are defined for
 fixed or floating point complex numbers.  It also delares the kf_ internal functions.
 */

static void kf_bfly2(kiss_fft_cpx * fout, const size_t fstride, const kiss_fft_cfg st, int m)
{
    kiss_fft_cpx * fout2;
    kiss_fft_cpx * tw1 = st->twiddles;
    kiss_fft_cpx t;
    fout2 = fout + m;
    do{
        C_FIXDIV(*fout,2); C_FIXDIV(*fout2,2);

        C_MUL (t,  *fout2 , *tw1);
        tw1 += fstride;
        C_SUB( *fout2 ,  *fout , t );
        C_ADDTO( *fout ,  t );
        ++fout2;
        ++fout;
    }while(--m);
}

static void kf_bfly4(kiss_fft_cpx * fout, const size_t fstride, const kiss_fft_cfg st, const size_t m)
{
    kiss_fft_cpx *tw1,*tw2,*tw3;
    kiss_fft_cpx scratch[6];
    size_t k=m;
    const size_t m2=2*m;
    const size_t m3=3*m;

    tw3 = tw2 = tw1 = st->twiddles;

    do {
        C_FIXDIV(*fout,4); C_FIXDIV(fout[m],4); C_FIXDIV(fout[m2],4); C_FIXDIV(fout[m3],4);

        C_MUL(scratch[0],fout[m] , *tw1 );
        C_MUL(scratch[1],fout[m2] , *tw2 );
        C_MUL(scratch[2],fout[m3] , *tw3 );

        C_SUB( scratch[5] , *fout, scratch[1] );
        C_ADDTO(*fout, scratch[1]);
        C_ADD( scratch[3] , scratch[0] , scratch[2] );
        C_SUB( scratch[4] , scratch[0] , scratch[2] );
        C_SUB( fout[m2], *fout, scratch[3] );
        tw1 += fstride;
        tw2 += fstride*2;
        tw3 += fstride*3;
        C_ADDTO( *fout , scratch[3] );

        if(st->inverse) {
            fout[m].r = scratch[5].r - scratch[4].i;
            fout[m].i = scratch[5].i + scratch[4].r;
            fout[m3].r = scratch[5].r + scratch[4].i;
            fout[m3].i = scratch[5].i - scratch[4].r;
        }else{
            fout[m].r = scratch[5].r + scratch[4].i;
            fout[m].i = scratch[5].i - scratch[4].r;
            fout[m3].r = scratch[5].r - scratch[4].i;
            fout[m3].i = scratch[5].i + scratch[4].r;
        }
        ++fout;
    }while(--k);
}

static void kf_bfly3(kiss_fft_cpx * fout, const size_t fstride, const kiss_fft_cfg st, size_t m)
{
     size_t k=m;
     const size_t m2 = 2*m;
     kiss_fft_cpx *tw1,*tw2;
     kiss_fft_cpx scratch[5];
     kiss_fft_cpx epi3;
     epi3 = st->twiddles[fstride*m];

     tw1=tw2=st->twiddles;

     do{
         C_FIXDIV(*fout,3); C_FIXDIV(fout[m],3); C_FIXDIV(fout[m2],3);

         C_MUL(scratch[1],fout[m] , *tw1);
         C_MUL(scratch[2],fout[m2] , *tw2);

         C_ADD(scratch[3],scratch[1],scratch[2]);
         C_SUB(scratch[0],scratch[1],scratch[2]);
         tw1 += fstride;
         tw2 += fstride*2;

         fout[m].r = fout->r - HALF_OF(scratch[3].r);
         fout[m].i = fout->i - HALF_OF(scratch[3].i);

         C_MULBYSCALAR( scratch[0] , epi3.i );

         C_ADDTO(*fout,scratch[3]);

         fout[m2].r = fout[m].r + scratch[0].i;
         fout[m2].i = fout[m].i - scratch[0].r;

         fout[m].r -= scratch[0].i;
         fout[m].i += scratch[0].r;

         ++fout;
     }while(--k);
}

static void kf_bfly5(kiss_fft_cpx * fout, const size_t fstride, const kiss_fft_cfg st, int m)
{
    kiss_fft_cpx *fout0,*fout1,*fout2,*fout3,*fout4;
    int u;
    kiss_fft_cpx scratch[13];
    kiss_fft_cpx * twiddles = st->twiddles;
    kiss_fft_cpx *tw;
    kiss_fft_cpx ya,yb;
    ya = twiddles[fstride*m];
    yb = twiddles[fstride*2*m];

    fout0=fout;
    fout1=fout0+m;
    fout2=fout0+2*m;
    fout3=fout0+3*m;
    fout4=fout0+4*m;

    tw=st->twiddles;
    for( u=0; u<m; ++u ) {
        C_FIXDIV( *fout0,5); C_FIXDIV( *fout1,5); C_FIXDIV( *fout2,5); C_FIXDIV( *fout3,5); C_FIXDIV( *fout4,5);
        scratch[0] = *fout0;

        C_MUL(scratch[1] ,*fout1, tw[u*fstride]);
        C_MUL(scratch[2] ,*fout2, tw[2*u*fstride]);
        C_MUL(scratch[3] ,*fout3, tw[3*u*fstride]);
        C_MUL(scratch[4] ,*fout4, tw[4*u*fstride]);

        C_ADD( scratch[7],scratch[1],scratch[4]);
        C_SUB( scratch[10],scratch[1],scratch[4]);
        C_ADD( scratch[8],scratch[2],scratch[3]);
        C_SUB( scratch[9],scratch[2],scratch[3]);

        fout0->r += scratch[7].r + scratch[8].r;
        fout0->i += scratch[7].i + scratch[8].i;

        scratch[5].r = scratch[0].r + S_MUL(scratch[7].r,ya.r) + S_MUL(scratch[8].r,yb.r);
        scratch[5].i = scratch[0].i + S_MUL(scratch[7].i,ya.r) + S_MUL(scratch[8].i,yb.r);

        scratch[6].r = S_MUL(scratch[10].i,ya.i) + S_MUL(scratch[9].i,yb.i);
        scratch[6].i = -S_MUL(scratch[10].r,ya.i) - S_MUL(scratch[9].r,yb.i);

        C_SUB(*fout1,scratch[5],scratch[6]);
        C_ADD(*fout4,scratch[5],scratch[6]);

        scratch[11].r = scratch[0].r + S_MUL(scratch[7].r,yb.r) + S_MUL(scratch[8].r,ya.r);
        scratch[11].i = scratch[0].i + S_MUL(scratch[7].i,yb.r) + S_MUL(scratch[8].i,ya.r);
        scratch[12].r = - S_MUL(scratch[10].i,yb.i) + S_MUL(scratch[9].i,ya.i);
        scratch[12].i = S_MUL(scratch[10].r,yb.i) - S_MUL(scratch[9].r,ya.i);

        C_ADD(*fout2,scratch[11],scratch[12]);
        C_SUB(*fout3,scratch[11],scratch[12]);

        ++fout0;++fout1;++fout2;++fout3;++fout4;
    }
}

/* perform the butterfly for one stage of a mixed radix FFT */
static void kf_bfly_generic(kiss_fft_cpx * fout, const size_t fstride, const kiss_fft_cfg st, int m, int p)
{
    int u,k,q1,q;
    kiss_fft_cpx * twiddles = st->twiddles;
    kiss_fft_cpx t;
    int Norig = st->nfft;

    kiss_fft_cpx * scratch = (kiss_fft_cpx*)KISS_FFT_TMP_ALLOC(sizeof(kiss_fft_cpx)*p);

    for( u=0; u<m; ++u ) {
        k=u;
        for( q1=0; q1<p; ++q1 ) {
            scratch[q1] = fout[ k  ];
            C_FIXDIV(scratch[q1],p);
            k += m;
        }

        k=u;
        for( q1=0; q1<p; ++q1 ) {
            int twidx=0;
            fout[ k ] = scratch[0];
            for(q=1;q<p;++q ) {
                twidx += fstride * k;
                if(twidx>=Norig) twidx-=Norig;
                C_MUL(t,scratch[q] , twiddles[twidx] );
                C_ADDTO( fout[ k ] ,t);
            }
            k += m;
        }
    }
    KISS_FFT_TMP_FREE(scratch);
}

static void kf_work(kiss_fft_cpx * fout, const kiss_fft_cpx * f, const size_t fstride, int in_stride, int * factors, const kiss_fft_cfg st)
{
    kiss_fft_cpx * fout_beg=fout;
    const int p=*factors++; /* the radix  */
    const int m=*factors++; /* stage's fft length/p */
    const kiss_fft_cpx * fout_end = fout + p*m;

#ifdef _OPENMP
    // use openmp extensions at the 
    // top-level (not recursive)
    if(fstride==1 && p<=5)
    {
        int k;

        // execute the p different work units in different threads
#       pragma omp parallel for
        for(k=0;k<p;++k)
            kf_work( fout +k*m, f+ fstride*in_stride*k,fstride*p,in_stride,factors,st);
        // all threads have joined by this point

        switch(p) {
            case 2: kf_bfly2(fout,fstride,st,m); break;
            case 3: kf_bfly3(fout,fstride,st,m); break;
            case 4: kf_bfly4(fout,fstride,st,m); break;
            case 5: kf_bfly5(fout,fstride,st,m); break;
            default: kf_bfly_generic(fout,fstride,st,m,p); break;
        }
        return;
    }
#endif

    if(m==1) {
        do{
            *fout = *f;
            f += fstride*in_stride;
        }while(++fout != fout_end );
    }else{
        do{
            // recursive call:
            // DFT of size m*p performed by doing
            // p instances of smaller DFTs of size m, 
            // each one takes a decimated version of the input
            kf_work( fout , f, fstride*p, in_stride, factors,st);
            f += fstride*in_stride;
        }while( (fout += m) != fout_end );
    }

    fout=fout_beg;

    // recombine the p smaller DFTs 
    switch(p) {
        case 2: kf_bfly2(fout,fstride,st,m); break;
        case 3: kf_bfly3(fout,fstride,st,m); break;
        case 4: kf_bfly4(fout,fstride,st,m); break;
        case 5: kf_bfly5(fout,fstride,st,m); break;
        default: kf_bfly_generic(fout,fstride,st,m,p); break;
    }
}

/*  facbuf is populated by p1,m1,p2,m2, ...
    where 
    p[i] * m[i] = m[i-1]
    m0 = n                  */
static void kf_factor(int n,int * facbuf)
{
    int p=4;
    double floor_sqrt;
    floor_sqrt = floor( sqrt((double)n) );

    /*factor out powers of 4, powers of 2, then any remaining primes */
    do {
        while(n % p) {
            switch(p) {
                case 4: p = 2; break;
                case 2: p = 3; break;
                default: p += 2; break;
            }
            if(p > floor_sqrt)
                p = n;          /* no more factors, skip to end */
        }
        n /= p;
        *facbuf++ = p;
        *facbuf++ = n;
    } while(n > 1);
}

/*
 *
 * User-callable function to allocate all necessary storage space for the fft.
 *
 * The return value is a contiguous block of memory, allocated with malloc.  As such,
 * It can be freed with free(), rather than a kiss_fft-specific function.
 * */
kiss_fft_cfg kiss_fft_alloc(int nfft,int inverse_fft,void * mem,size_t * lenmem)
{
    kiss_fft_cfg st=NULL;
    size_t memneeded = sizeof(struct kiss_fft_state)
        + sizeof(kiss_fft_cpx)*(nfft-1); /* twiddle factors*/

    if(lenmem==NULL ) {
        st = ( kiss_fft_cfg)KISS_FFT_MALLOC( memneeded );
    }else{
        if(mem != NULL && *lenmem >= memneeded)
            st = (kiss_fft_cfg)mem;
        *lenmem = memneeded;
    }
    if(st) {
        int i;
        st->nfft=nfft;
        st->inverse = inverse_fft;

        for(i=0;i<nfft;++i) {
            const double pi=3.141592653589793238462643383279502884197169399375105820974944;
            double phase = -2*pi*i / nfft;
            if(st->inverse)
                phase *= -1;
            kf_cexp(st->twiddles+i, phase );
        }

        kf_factor(nfft,st->factors);
    }
    return st;
}


void kiss_fft_stride(kiss_fft_cfg st,const kiss_fft_cpx *fin,kiss_fft_cpx *fout, int in_stride)
{
    if(fin == fout) {
        //NOTE: this is not really an in-place FFT algorithm.
        //It just performs an out-of-place FFT into a temp buffer
        kiss_fft_cpx * tmpbuf = (kiss_fft_cpx*)KISS_FFT_TMP_ALLOC( sizeof(kiss_fft_cpx)*st->nfft);
        kf_work(tmpbuf,fin,1,in_stride, st->factors,st);
        memcpy(fout,tmpbuf,sizeof(kiss_fft_cpx)*st->nfft);
        KISS_FFT_TMP_FREE(tmpbuf);
    }else{
        kf_work( fout, fin, 1,in_stride, st->factors,st );
    }
}

void kiss_fft(kiss_fft_cfg cfg,const kiss_fft_cpx *fin,kiss_fft_cpx *fout)
{
    kiss_fft_stride(cfg,fin,fout,1);
}


void kiss_fft_cleanup(void)
{
    // nothing needed any more
}

int kiss_fft_next_fast_size(int n)
{
    while(1) {
        int m=n;
        while( (m%2) == 0 ) m/=2;
        while( (m%3) == 0 ) m/=3;
        while( (m%5) == 0 ) m/=5;
        if(m<=1)
            break; /* n is completely factorable by twos, threes, and fives */
        n++;
    }
    return n;
}
