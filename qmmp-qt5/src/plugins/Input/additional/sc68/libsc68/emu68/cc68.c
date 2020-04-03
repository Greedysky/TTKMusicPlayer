/*
 * @file    emu68/cc68.c
 * @brief   Code condition function table
 * @author  http://sourceforge.net/users/benjihan
 *
 * Copyright (c) 1998-2015 Benjamin Gerard
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#error "deprecated: should not be compiled"

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "struct68.h"
#include "srdef68.h"
#include "cc68.h"

/* ,--------------------------.
 * | Condition code functions |
 * `--------------------------' */

static int is_cc0(const int sr) { return inl_is_cc0(sr); } /* 0000 TR */
static int is_cc1(const int sr) { return inl_is_cc1(sr); } /* 0001 FA */
static int is_cc2(const int sr) { return inl_is_cc2(sr); } /* 0010 HI */
static int is_cc3(const int sr) { return inl_is_cc3(sr); } /* 0011 LS */
static int is_cc4(const int sr) { return inl_is_cc4(sr); } /* 0100 CC */
static int is_cc5(const int sr) { return inl_is_cc5(sr); } /* 0101 CS */
static int is_cc6(const int sr) { return inl_is_cc6(sr); } /* 0110 NE */
static int is_cc7(const int sr) { return inl_is_cc7(sr); } /* 0111 EQ */
static int is_cc8(const int sr) { return inl_is_cc8(sr); } /* 1000 VC */
static int is_cc9(const int sr) { return inl_is_cc9(sr); } /* 1001 VS */
static int is_ccA(const int sr) { return inl_is_ccA(sr); } /* 1010 PL */
static int is_ccB(const int sr) { return inl_is_ccB(sr); } /* 1011 MI */
static int is_ccC(const int sr) { return inl_is_ccC(sr); } /* 1100 GE */
static int is_ccD(const int sr) { return inl_is_ccD(sr); } /* 1101 LT */
static int is_ccE(const int sr) { return inl_is_ccE(sr); } /* 1110 GT */
static int is_ccF(const int sr) { return inl_is_ccF(sr); } /* 1111 LE */

int (* const is_cc68[16])(const int) = {
  is_cc0, is_cc1, is_cc2, is_cc3, is_cc4, is_cc5, is_cc6, is_cc7,
  is_cc8, is_cc9, is_ccA, is_ccB, is_ccC, is_ccD, is_ccE, is_ccF
};
