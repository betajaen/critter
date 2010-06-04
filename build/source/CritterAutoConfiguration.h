/** 
    
    This file is part of Critter.
    
    Copyright (c) 2009 Robin Southern, http://www.nxogre.org
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
    
*/

                                                                                       

#ifndef CRITTER_AUTOCONFIGURATION_H
#define CRITTER_AUTOCONFIGURATION_H

#include "NxOgreConfiguration.h"

#define CritterMemoryAllocatorMalloc 1
#define CritterMemoryAllocatorNedmalloc 2

#define CritterFloatingPointFloat 1
#define CritterFloatingPointDouble 2
#define CritterCompilerUnknown 0
#define CritterCompilerMSVC 1
#define CritterCompilerGNUC 2

#define CritterPlatformUnknown 0
#define CritterPlatformWindows 1
#define CritterPlatformLinux 2

#define CritterArchitecture32Bit 32
#define CritterArchitecture64Bit 64

#define CRITTER_EXPORT_OPTIONS_EXPORT 1
#define CRITTER_EXPORT_OPTIONS_IMPORT 2
#define CRITTER_EXPORT_OPTIONS_AVOID 3

#if defined (_MSC_VER)
# define CritterCompiler CritterCompilerMSVC
#elif defined ( __GNUC__ )
# define CritterCompiler CritterCompilerGNUC
#endif


#if defined (_WIN32) || defined (__WIN32)
# define CritterPlatform CritterPlatformWindows
#else
# define CritterPlatform CritterPlatformLinux
#endif

#if defined(_M_X64) || defined (_M_X64)
# define CritterArchitecture CritterArchitecture64Bit
#else
# define CritterArchitecture CritterArchitecture32Bit
#endif


// CritterArray.h generates this warning - a lot.
#pragma warning (disable : 4251)

// CritterSingleton.h and other classes that inherit generate this warning in Visual Studio.
#pragma warning (disable : 4661)

// Occurs when inheriting from a PhysX class.
#pragma warning (disable : 4275)

// Conversion from 'double' to 'float', possible loss of data
#pragma warning (disable : 4244)

// forcing value to bool 'true' or 'false' (performance warning)
#pragma warning (disable : 4800)

// 'fopen': This function or variable may be unsafe. Consider using fopen_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.
#pragma warning (disable : 4996)


#ifdef CRITTER_SDK
# define CritterForceInline __inline
# define CritterPublicClass
# define CritterPublicFunction
#else
# define CritterForceInline __inline
# define CritterPublicClass
# define CritterPublicFunction
#endif

#if defined(_DEBUG) || defined(DEBUG)
# ifndef CRITTER_DEBUG
#  define CRITTER_DEBUG
# endif
#endif


#ifdef Critter_UsesOgreTerrain
 #ifdef CRITTER_DEBUG
  # pragma comment(lib, "OgreTerrain_d.lib")
 #else
  # pragma comment(lib, "OgreTerrain.lib")
 #endif
#endif

#endif
