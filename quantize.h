// Copyright (C) 2016 Parabola Research Limited
//
// Use of this source code is governed by a BSD-style license that
// can be found in the COPYING file in the root of the source tree.


/* Quantization and reconstruction functions */


#ifndef INCLUDED_quantize_h
#define INCLUDED_quantize_h

#include "havoc.h"

#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif


// HEVC inverse quantization ("scaling")

typedef void havoc_quantize_inverse(int16_t *dst, const int16_t *src, int scale, int shift, int n);

typedef struct
{
	havoc_quantize_inverse *p[2];
}
havoc_table_quantize_inverse;

static havoc_quantize_inverse** havoc_get_quantize_inverse(havoc_table_quantize_inverse *table, int scale, int shift)
{
	return &table->p[!!(scale & ((1 << shift) - 1))];
}

void havoc_populate_quantize_inverse(havoc_table_quantize_inverse *table, havoc_code code);

void havoc_test_quantize_inverse(int *error_count, havoc_instruction_set mask);



// HEVC simple quantization (similar to that in HM)

typedef int havoc_quantize(int16_t *dst, const int16_t *src, int scale, int shift, int offset, int n);

typedef struct
{
	havoc_quantize *p;
}
havoc_table_quantize;

static havoc_quantize** havoc_get_quantize(havoc_table_quantize *table)
{
	return &table->p;
}

void havoc_populate_quantize(havoc_table_quantize *table, havoc_code code);

void havoc_test_quantize(int *error_count, havoc_instruction_set mask);



// Reconstruction function: adds a CU's predicted and residual values

typedef void havoc_quantize_reconstruct(uint8_t *rec, intptr_t stride_rec, const uint8_t *pred, intptr_t stride_pred, const int16_t *res, int n);

typedef struct
{
	havoc_quantize_reconstruct *p[4];
}
havoc_table_quantize_reconstruct;

static havoc_quantize_reconstruct** havoc_get_quantize_reconstruct(havoc_table_quantize_reconstruct *table, int log2TrafoSize)
{
	return &table->p[log2TrafoSize - 2];
}

void havoc_populate_quantize_reconstruct(havoc_table_quantize_reconstruct *table, havoc_code code);

void havoc_test_quantize_reconstruct(int *error_count, havoc_instruction_set mask);


#ifdef __cplusplus
}
#endif

#endif
