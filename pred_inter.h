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
using HevcasmPredUni = void (Sample *dst, intptr_t stride_dst, Sample const *ref, intptr_t stride_ref, int nPbW, int nPbH, int xFrac, int yFrac, int bitDepth);
typedef HevcasmPredUni<uint8_t> havoc_pred_uni_8to8;
typedef HevcasmPredUni<uint16_t> havoc_pred_uni_16to16;

template <typename Sample>
struct HevcasmTablePredUni
{
	HevcasmPredUni<Sample>* p[3][2][17][2][2];
};


template <typename Sample>
static HevcasmPredUni<Sample>** havocGetPredUni(HevcasmTablePredUni<Sample> *table, int taps, int w, int h, int xFrac, int yFrac, int bitDepth)
{
	return &table->p[bitDepth - 8][taps / 4 - 1][(w + taps - 1) / taps][xFrac ? 1 : 0][yFrac ? 1 : 0];
}


template <typename Sample>
void havocPopulatePredUni(HevcasmTablePredUni<Sample> *table, havoc_code code);


havoc_test_function havoc_test_pred_uni;


// HEVC bi prediction

template <typename Sample>
using HevcasmPredBi = void(Sample *dst0, intptr_t stride_dst, const Sample *ref0, const Sample *ref1, intptr_t stride_ref, int nPbW, int nPbH, int xFrac0, int yFrac0, int xFrac1, int yFrac1, int bitDepth);

template <typename Sample>
struct HevcasmTablePredBi
{
	HevcasmPredBi<Sample> * p[3][2][9][2];
};

template <typename Sample>
static HevcasmPredBi<Sample>** havocGetPredBi(HevcasmTablePredBi<Sample> *table, int taps, int w, int h, int xFracA, int yFracA, int xFracB, int yFracB, int bitDepth)
{
	const int frac = xFracA || yFracA || xFracB || yFracB;
	return &table->p[bitDepth - 8][taps / 4 - 1][(w + 2 * taps - 1) / (2 * taps)][frac];
}

template <typename Sample>
void havocPopulatePredBi(HevcasmTablePredBi<Sample> *table, havoc_code code);

havoc_test_function havoc_test_pred_bi;

// review: rename, look for generic solution
template <typename Sample> struct Lookup;

template <> struct Lookup<uint8_t>
{
	typedef HevcasmTablePredUni<uint8_t> TablePredUni;
	constexpr static decltype(&havocGetPredUni<uint8_t>) getPredUni() { return &havocGetPredUni<uint8_t>; }
	typedef HevcasmTablePredBi<uint8_t> TablePredBi;
	constexpr static decltype(&havocGetPredBi<uint8_t>) getPredBi() { return &havocGetPredBi<uint8_t>; }
};

template <> struct Lookup<uint16_t>
{
	typedef HevcasmTablePredUni<uint16_t> TablePredUni;
	constexpr static decltype(&havocGetPredUni<uint16_t>) getPredUni() { return &havocGetPredUni<uint16_t>; }
	typedef HevcasmTablePredBi<uint16_t> TablePredBi;
	constexpr static decltype(&havocGetPredBi<uint16_t>) getPredBi() { return &havocGetPredBi<uint16_t>; }
};

#endif
