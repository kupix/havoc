// Copyright (C) 2016 Parabola Research Limited
//
// Use of this source code is governed by a BSD-style license that
// can be found in the COPYING file in the root of the source tree.


#ifndef INCLUDED_havoc_prediction_inter_h
#define INCLUDED_havoc_prediction_inter_h

// HEVC inter prediction

// Note that these functions may write data to the right of the destination block.


#include "havoc.h"


// HEVC uni prediction
template <typename Sample>
using HavocPredUni = void (Sample *dst, intptr_t stride_dst, Sample const *ref, intptr_t stride_ref, int nPbW, int nPbH, int xFrac, int yFrac, int bitDepth);
typedef HavocPredUni<uint8_t> havoc_pred_uni_8to8;
typedef HavocPredUni<uint16_t> havoc_pred_uni_16to16;

template <typename Sample>
struct HavocTablePredUni
{
	HavocPredUni<Sample>* p[3][2][17][2][2];
};


template <typename Sample>
static HavocPredUni<Sample>** havocGetPredUni(HavocTablePredUni<Sample> *table, int taps, int w, int h, int xFrac, int yFrac, int bitDepth)
{
	return &table->p[bitDepth - 8][taps / 4 - 1][(w + taps - 1) / taps][xFrac ? 1 : 0][yFrac ? 1 : 0];
}


template <typename Sample>
void havocPopulatePredUni(HavocTablePredUni<Sample> *table, havoc_code code);


havoc_test_function havoc_test_pred_uni;


// HEVC bi prediction

template <typename Sample>
using HavocPredBi = void(Sample *dst0, intptr_t stride_dst, const Sample *ref0, const Sample *ref1, intptr_t stride_ref, int nPbW, int nPbH, int xFrac0, int yFrac0, int xFrac1, int yFrac1, int bitDepth);

template <typename Sample>
struct HavocTablePredBi
{
	HavocPredBi<Sample> * p[3][2][9][2];
};

template <typename Sample>
static HavocPredBi<Sample>** havocGetPredBi(HavocTablePredBi<Sample> *table, int taps, int w, int h, int xFracA, int yFracA, int xFracB, int yFracB, int bitDepth)
{
	const int frac = xFracA || yFracA || xFracB || yFracB;
	return &table->p[bitDepth - 8][taps / 4 - 1][(w + 2 * taps - 1) / (2 * taps)][frac];
}

template <typename Sample>
void havocPopulatePredBi(HavocTablePredBi<Sample> *table, havoc_code code);

havoc_test_function havoc_test_pred_bi;


#endif
