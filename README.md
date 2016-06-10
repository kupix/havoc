# HAVOC - Handcoded Assembly optimisations for VideO Codecs

## Introduction

Havoc is a collection of permissively-licensed optimised DSP functions for video coding. Instead of using intrinsics or a build-time assembler, Havoc makes use of just-in-time (JIT) assembler [Xbyak](https://github.com/herumi/xbyak), . This means that the executable code is assembled at runtime, just before it runs. JIT assembly has the following advantages:

* No special tool (assembler) is needed to build the project. That means one less dependency to find, version and install.
* Developers can write assembly code. Xbyak offers a syntactically sweet domain-specific language having a syntax very similar to assembly code.
* Preprocesing can be done with idiomatic C++ constructs such as functions and loops - there is no need for complex assembler-specific macros. Parameters are regular C++ variables and regular debugging can be used to debug the assembly.
* All sorts of interesting optimisations and adaptations are possible. For example, video-stream-specific parameters could be embedded in the code and code could be tuned for a specific machine's measured characteristics.

There are a few drawbacks to the current JIT implementation for which workarounds are possible:

* Debug symbols are unavailable - debuggers cannot know or show the name of the JIT-assembled function that is running. This also affects profilers - only the code address is known, not the function name.
* Assembly takes a small but non-zero amount of time during program initialisation.


## Continuous integration

The Linux build of this project is continuously integrated by Travis-CI. Because Travis-CI is hosted on Amazon EC2's older C3 instances, it does not support the AVX2 instruction set.  To get around this, an emulator (Intel SDE) is used to emulate a more recent processor. This allows all optimised functions to be tested, slowly.  The project CI has the following limitations:

* Only 64-bit Linux is tested currently
* Performance results are inaccurate (and completely wrong for AVX2 functions as these are emulated)


## Credits

Some functions are derived from the [f265](http://vantrix.com/f-265/) project, others from Google's [WebM](http://www.webmproject.org/) project. Further functions were written from scratch. See COPYING and the source code for further details.

