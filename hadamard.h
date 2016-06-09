// Copyright (C) 2016 Parabola Research Limited
//
// Use of this source code is governed by a BSD-style license that
// can be found in the COPYING file in the root of the source tree.


/* Functions for decoding HEVC residual: inverse transform and add the result to predictor */


#ifndef INCLUDED_hadamard_h
#define INCLUDED_hadamard_h

#include "havoc.h"


template <typename Sample>
using havoc_hadamard_satd = int(Sample const *srcA, intptr_t stride_srcA, Sample const *srcB, intptr_t stride_srcB);


template <typename Sample>
struct havoc_table_hadamard_satd
{
	havoc_hadamard_satd<Sample> *satd[3];
};


template <typename Sample>
havoc_hadamard_satd<Sample>** havoc_get_hadamard_satd(havoc_table_hadamard_satd<Sample> *table, int log2TrafoSize)
{
	return &table->satd[log2TrafoSize - 1];
}


template <typename Sample>
void havoc_populate_hadamard_satd(havoc_table_hadamard_satd<Sample> *table, havoc_code code);


void havoc_test_hadamard_satd(int *error_count, havoc_instruction_set mask);


#endif
