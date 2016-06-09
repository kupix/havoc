// Copyright (C) 2016 Parabola Research Limited
//
// Use of this source code is governed by a BSD-style license that
// can be found in the COPYING file in the root of the source tree.


#ifndef INCLUDED_havoc_h
#define INCLUDED_havoc_h

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>


/* macro used to ringfence code derived from the f265 project */
#ifndef USE_F265_DERIVED
#define USE_F265_DERIVED 1
#endif

/* macro used to ringfence code derived from the WebM project */
#ifndef USE_WEBM_DERIVED
#define USE_WEBM_DERIVED 1
#endif

/* macro used to ringfence code derived from the HM codec */
#ifndef USE_HM_DERIVED
#define USE_HM_DERIVED 1
#endif



/* compiler-specific inline timestamp function and alignment macro */
#ifdef _MSC_VER

#include <intrin.h>

#if _WIN64
#define HAVOC_X64
#endif

typedef int64_t havoc_timestamp;


static havoc_timestamp havoc_get_timestamp()
{
	return __rdtsc();
}

#define HAVOC_ALIGN(n, T, v) \
	__declspec(align(n)) T v

#endif

/* compiler-specific inline timestamp function and alignment macro */
#ifdef __GNUC__

#if __x86_64__
#define HAVOC_X64
#endif

typedef uint64_t havoc_timestamp;

#if defined(__i386__)

static havoc_timestamp havoc_get_timestamp(void)
{
	uint64_t timestamp;
	__asm__ volatile (".byte 0x0f, 0x31" : "=A" (timestamp));
	return timestamp;
}

#elif defined(__x86_64__)

static havoc_timestamp havoc_get_timestamp(void)
{
	unsigned h, l;
	__asm__ __volatile__ ("rdtsc" : "=a"(l), "=d"(h));
	return (havoc_timestamp)l | ((havoc_timestamp)h << 32);
}

#endif

#define HAVOC_ALIGN(n, T, v) \
	T v __attribute__((aligned(n)))

#endif


#ifdef __cplusplus
extern "C" {
#endif


#define HAVOC_INSTRUCTION_SET_XMACRO \
	X(0, C_REF, "C - reference; may be slow") \
	X(1, C_OPT, "C - optimised") \
	X(2, SSE2, "SSE2") \
	X(3, SSE3, "SSE3") \
	X(4, SSSE3, "Supplementary SSE3") \
	X(5, SSE41, "SSE4.1") \
	X(6, SSE42, "SSE4.2") \
	X(7, AVX, "AVX") \
	X(8, AVX2, "AVX2")


/* bitmask type for describing instruction sets */
typedef enum 
{
	HAVOC_NONE = 0,
#define X(value, name, description) HAVOC_ ## name = 1 << value,
	HAVOC_INSTRUCTION_SET_XMACRO
#undef X
} havoc_instruction_set;


/* queries processor via cpuid instruction and returns a bitmask representing supported instruction sets */
havoc_instruction_set havoc_instruction_set_support();


void havoc_print_instruction_set_support(FILE *f, havoc_instruction_set mask);


typedef struct
{
	void *implementation;
} havoc_code;


/* create a new buffer for JIT assembler emitted object code */
havoc_code havoc_new_code(havoc_instruction_set mask, int size);


/* must be called to free resources used by a code buffer created by havoc_new_code() */
void havoc_delete_code(havoc_code);


/* library self-test entry point */
int havoc_main(int argc, const char *argv[]);


#define HAVOC_RECT(width, height) (((width) << 8) | (height))


typedef void havoc_test_function(int *error_count, havoc_instruction_set mask);


#ifdef __cplusplus
}
#endif

#endif
