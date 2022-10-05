//@HEADER
// ************************************************************************
//
//                        Kokkos v. 3.0
//       Copyright (2020) National Technology & Engineering
//               Solutions of Sandia, LLC (NTESS).
//
// Under the terms of Contract DE-NA0003525 with NTESS,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY NTESS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL NTESS OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact Siva Rajamanickam (srajama@sandia.gov)
//
// ************************************************************************
//@HEADER

// File contains common macro definitions which are both generated by cmake in
// KokkosKernels_config.h and written in this header file.

#ifndef KOKKOSKERNELS_MACROS_HPP_
#define KOKKOSKERNELS_MACROS_HPP_

/****** BEGIN macros populated by CMake ******/
#include <KokkosKernels_config.h>
/****** END macros populated by CMake ******/

/****** BEGIN other helper macros ******/
#ifndef KOKKOSKERNELS_DEBUG_LEVEL
#define KOKKOSKERNELS_DEBUG_LEVEL 1
#endif

// If KOKKOSKERNELS_ENABLE_OMP_SIMD is defined, it's legal to place
// "#pragma omp simd" before a for loop. It's never defined if a GPU-type device
// is enabled, since in that case, Kokkos::ThreadVectorRange should be used
// instead for SIMD parallel loops.

#if !defined(KOKKOS_ENABLE_CUDA) && !defined(KOKKOS_ENABLE_HIP) && \
    defined(KOKKOS_ENABLE_OPENMP)
// For clang OpenMP support, see
// https://clang.llvm.org/docs/OpenMPSupport.html#id1
#if defined(KOKKOS_COMPILER_GNU) || defined(KOKKOS_COMPILER_CLANG)
// GCC 4.8.5 and older do not support #pragma omp simd
// Do not enable when using GCC 7.2.0 or 7.3.0 + C++17 due to a bug in gcc
#if (KOKKOS_COMPILER_GNU > 485) &&                                   \
    !(KOKKOS_COMPILER_GNU == 720 && defined(KOKKOS_ENABLE_CXX17)) && \
    !(KOKKOS_COMPILER_GNU == 730 && defined(KOKKOS_ENABLE_CXX17))
#define KOKKOSKERNELS_ENABLE_OMP_SIMD
#endif
// TODO: Check for a clang version that supports #pragma omp simd
#else
// All other Kokkos-supported compilers support it.
#define KOKKOSKERNELS_ENABLE_OMP_SIMD
#endif
#endif

// Macro to place before an ordinary loop to force vectorization, based
// on the pragmas that are supported by the compiler. "Force" means to
// override the compiler's heuristics and always vectorize.
// This respects the fact that "omp simd" is incompatible with
// "vector always" and "ivdep" in the Intel OneAPI toolchain.
#ifdef KOKKOSKERNELS_ENABLE_OMP_SIMD
#define KOKKOSKERNELS_FORCE_SIMD _Pragma("omp simd")
#else
#if defined(KOKKOS_ENABLE_PRAGMA_IVDEP) && defined(KOKKOS_ENABLE_PRAGMA_VECTOR)
#define KOKKOSKERNELS_FORCE_SIMD _Pragma("ivdep") _Pragma("vector always")
#elif defined(KOKKOS_ENABLE_PRAGMA_IVDEP)
#define KOKKOSKERNELS_FORCE_SIMD _Pragma("ivdep")
#elif defined(KOKKOS_ENABLE_PRAGMA_VECTOR)
#define KOKKOSKERNELS_FORCE_SIMD _Pragma("vector always")
#else
// No macros available to suggest vectorization
#define KOKKOSKERNELS_FORCE_SIMD
#endif
#endif

// Macro that tells GCC not to worry if a variable isn't being used.
// Generalized attributes were not implemented in GCC until 4.8:
//
// https://gcc.gnu.org/gcc-4.7/cxx0x_status.html
// https://gcc.gnu.org/gcc-4.8/cxx0x_status.html
//
// Thus, we can't use [[unused]]; we have to use the older GCC syntax
// for variable attributes.  Be careful also of compilers that define
// the __GNUC__ macro but might not necessarily actually be GCC
// compliant.
#if defined(__GNUC__) && !defined(KOKKOSKERNELS_UNUSED_ATTRIBUTE)
#define KOKKOSKERNELS_UNUSED_ATTRIBUTE __attribute__((unused))
#else
#define KOKKOSKERNELS_UNUSED_ATTRIBUTE
#endif  // __GNUC__
/******* END other helper macros *******/

#endif  // KOKKOSKERNELS_MACROS_HPP_
