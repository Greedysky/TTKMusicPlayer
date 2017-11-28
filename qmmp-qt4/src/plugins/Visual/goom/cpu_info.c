/***************************************************************************
 *   Copyright (C) 2016 by Ilya Kotov                                      *
 *   forkotov02@ya.ru                                                      *
 *                                                                         *
 *   Based on FFmpeg code                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include <stdint.h>
#include "cpu_info.h"


#ifdef CPU_X86

#if ARCH_X86_64
#    define REG_a "rax"
#    define REG_b "rbx"
#    define REG_c "rcx"
#    define REG_d "rdx"
#    define REG_D "rdi"
#    define REG_S "rsi"
#    define PTR_SIZE "8"
#    define REG_SP "rsp"
#    define REG_BP "rbp"
#    define REGBP   rbp
#    define REGa    rax
#    define REGb    rbx
#    define REGc    rcx
#    define REGd    rdx
#    define REGSP   rsp
typedef int64_t x86_reg;
#elif ARCH_X86_32
#    define REG_a "eax"
#    define REG_b "ebx"
#    define REG_c "ecx"
#    define REG_d "edx"
#    define REG_D "edi"
#    define REG_S "esi"
#    define PTR_SIZE "4"
#    define REG_SP "esp"
#    define REG_BP "ebp"
#    define REGBP   ebp
#    define REGa    eax
#    define REGb    ebx
#    define REGc    ecx
#    define REGd    edx
#    define REGSP   esp
typedef int32_t x86_reg;
#else
#warning unknown arch, SIMD optimizations will be disabled
#ifndef ARCH_UNKNOWN
#define ARCH_UNKNOWN 1
#endif
typedef int x86_reg;
#endif

typedef struct { uint64_t a, b; } xmm_reg;
#define DECLARE_ALIGNED(n,t,v)      t v __attribute__ ((aligned (n)))
#define DECLARE_ALIGNED_16(t, v) DECLARE_ALIGNED(16, t, v)

/* ebx saving is necessary for PIC. gcc seems unable to see it alone */
#define cpuid(index,eax,ebx,ecx,edx)\
    __asm__ volatile\
        ("mov %%"REG_b", %%"REG_S"\n\t"\
         "cpuid\n\t"\
         "xchg %%"REG_b", %%"REG_S\
         : "=a" (eax), "=S" (ebx),\
           "=c" (ecx), "=d" (edx)\
         : "0" (index));


int mm_support(void)
{
    int rval = 0;
    int eax, ebx, ecx, edx;
    uint32_t max_std_level, max_ext_level, std_caps=0, ext_caps=0;

#if ARCH_X86_32
    x86_reg a, c;
    __asm__ volatile (
        /* See if CPUID instruction is supported ... */
        /* ... Get copies of EFLAGS into eax and ecx */
        "pushfl\n\t"
        "pop %0\n\t"
        "mov %0, %1\n\t"

        /* ... Toggle the ID bit in one copy and store */
        /*     to the EFLAGS reg */
        "xor $0x200000, %0\n\t"
        "push %0\n\t"
        "popfl\n\t"

        /* ... Get the (hopefully modified) EFLAGS */
        "pushfl\n\t"
        "pop %0\n\t"
        : "=a" (a), "=c" (c)
        :
        : "cc"
        );

    if (a == c) {
        //trace ("ffap: cpuid is not supported\n");
        return 0; /* CPUID not supported */
    }
#endif

    cpuid(0, max_std_level, ebx, ecx, edx);

    if(max_std_level >= 1){
        cpuid(1, eax, ebx, ecx, std_caps);
        if (std_caps & (1<<23))
            rval |= FF_MM_MMX;
        if (std_caps & (1<<25))
            rval |= FF_MM_MMX2 | FF_MM_SSE;
        if (std_caps & (1<<26))
            rval |= FF_MM_SSE2;
        if (ecx & 1)
            rval |= FF_MM_SSE3;
        if (ecx & 0x00000200 )
            rval |= FF_MM_SSSE3;
        if (ecx & 0x00080000 )
            rval |= FF_MM_SSE4;
        if (ecx & 0x00100000 )
            rval |= FF_MM_SSE42;
        ;
    }

    cpuid(0x80000000, max_ext_level, ebx, ecx, edx);

    if(max_ext_level >= 0x80000001){
        cpuid(0x80000001, eax, ebx, ecx, ext_caps);
        if (ext_caps & (1<<31))
            rval |= FF_MM_3DNOW;
        if (ext_caps & (1<<30))
            rval |= FF_MM_3DNOWEXT;
        if (ext_caps & (1<<23))
            rval |= FF_MM_MMX;
        if (ext_caps & (1<<22))
            rval |= FF_MM_MMX2;
    }

    return rval;
}

#endif

