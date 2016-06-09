// Copyright (C) 2016 Parabola Research Limited
//
// Use of this source code is governed by a BSD-style license that
// can be found in the COPYING file in the root of the source tree.


#ifndef INCLUDED_havoc_test_h
#define INCLUDED_havoc_test_h

#include "havoc.h"

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif


static const char *havoc_instruction_set_as_text(havoc_instruction_set set)
{
#define X(value, name, description) if (set == (1 << value)) return #name;
	HAVOC_INSTRUCTION_SET_XMACRO
#undef X
		return 0;
}

typedef void havoc_bound_invoke(void *bound, int n);

typedef int havoc_bound_mismatch(void *boundRef, void *boundTest);

typedef int havoc_bound_get(void *bound, havoc_code code);


int havoc_count_average_cycles(
	void *boundRef, void *boundTest,
	havoc_bound_invoke *f,
	havoc_bound_mismatch *m,
	double *first_result,
	havoc_instruction_set set,
	int iterations);


int havoc_test(
	void *ref,
	void *test,
	havoc_bound_get *get,
	havoc_bound_invoke *invoke,
	havoc_bound_mismatch *mismatch,
	havoc_instruction_set mask,
	int iterations);


#ifdef __cplusplus
}
#endif


#endif
