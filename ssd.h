// Copyright (C) 2016 Parabola Research Limited
//
// Use of this source code is governed by a BSD-style license that
// can be found in the COPYING file in the root of the source tree.



/* Sum of square differences */


#ifndef INCLUDED_ssd_h
#define INCLUDED_ssd_h

#include "havoc.h"


template <typename Sample>
using havoc_ssd = uint32_t(Sample const *srcA, intptr_t stride_srcA, Sample const *srcB, intptr_t stride_srcB, int w, int h);


template <typename Sample>
struct havoc_table_ssd
{
	havoc_ssd<Sample> *ssd[5];
};


template <typename Sample>
static havoc_ssd<Sample>** havoc_get_ssd(havoc_table_ssd<Sample> *table, int log2TrafoSize)
{

	return &table->ssd[log2TrafoSize - 2];
}


template <typename Sample>
void havoc_populate_ssd(havoc_table_ssd<Sample> *table, havoc_code code);


void havoc_test_ssd(int *error_count, havoc_instruction_set mask);


#endif
