/*
 * This file is part of the leaf3d project.
 *
 * Copyright 2014-2015 Emanuele Bertoldi. All rights reserved.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * You should have received a copy of the modified BSD License along with this
 * program. If not, see <http://www.opensource.org/licenses/bsd-license.php>
 */

#ifndef L3D_PLATFORM_H
#define L3D_PLATFORM_H
#pragma once

/*
 * Inspired by:
 *
 * https://github.com/horde3d/Horde3D/blob/develop/Horde3D/Source/Shared/utPlatform.h
 */

// Detect platform.
#if defined( WIN32 ) || defined( _WINDOWS ) || defined( _WIN32 )
#	if !defined( L3D_PLATFORM_WIN )
#		define L3D_PLATFORM_WIN
#	endif
#elif defined( __APPLE__ ) || defined( __APPLE_CC__ )
#   if !defined( L3D_PLATFORM_MAC )
#      define L3D_PLATFORM_MAC
#   endif
#else
#	if !defined( L3D_PLATFORM_LINUX )
#		define L3D_PLATFORM_LINUX
#	endif
#endif

// Endianess.
#if defined (__GLIBC__)
#   include <endian.h>
#   if __BYTE_ORDER == __LITTLE_ENDIAN
#       define L3D_PLATFORM_LITTLE_ENDIAN
#   elif __BYTE_ORDER == __BIG_ENDIAN
#       define L3D_PLATFORM_BIG_ENDIAN
#   else
#       error Unknown __BYTE_ORDER on endian.h
#   endif
#elif defined(_BIG_ENDIAN) && !defined(_LITTLE_ENDIAN) ||\
      defined(__BIG_ENDIAN__) && !defined(__LITTLE_ENDIAN__) ||\
      defined(_STLP_BIG_ENDIAN) && !defined(_STLP_LITTLE_ENDIAN)
#       define L3D_PLATFORM_BIG_ENDIAN
#elif defined(_LITTLE_ENDIAN) && !defined(_BIG_ENDIAN) ||\
      defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__) ||\
      defined(_STLP_LITTLE_ENDIAN) && !defined(_STLP_BIG_ENDIAN)
#       define L3D_PLATFORM_LITTLE_ENDIAN
#elif defined(__sparc) || defined(__sparc__) \
   || defined(_POWER) || defined(__powerpc__) \
   || defined(__ppc__) || defined(__hpux) || defined(__hppa) \
   || defined(_MIPSEB) || defined(_POWER) \
   || defined(__s390__)
#       define L3D_PLATFORM_BIG_ENDIAN
#elif defined(__i386__) || defined(__alpha__) \
   || defined(__ia64) || defined(__ia64__) \
   || defined(_M_IX86) || defined(_M_IA64) \
   || defined(_M_ALPHA) || defined(__amd64) \
   || defined(__amd64__) || defined(_M_AMD64) \
   || defined(__x86_64) || defined(__x86_64__) \
   || defined(_M_X64) || defined(__bfin__)
#       define L3D_PLATFORM_LITTLE_ENDIAN
#else
#   error Unknown endianess.
#endif

// Build type.
#if defined (NDEBUG)
# define L3D_RELEASE
#else
# define L3D_DEBUG
#endif

// Runtime assertion.
#if defined (L3D_DEBUG)
# include <assert.h>
#	define L3D_ASSERT( exp ) assert( exp );
#else
#	define L3D_ASSERT( exp )
#endif

// API export.
#if defined (L3D_PLATFORM_WIN)
# define L3D_API extern "C" __declspec(dllexport)
#else
# define L3D_API
#endif

#endif // L3D_PLATFORM_H
