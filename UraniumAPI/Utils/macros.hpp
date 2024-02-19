#pragma once
#include "../ProjectConfig.hpp"

#define FORMAT_VERSION_1_20_51 Uranium::Version(1, 20, 51)
#define FORMAT_VERSION_1_20_50 Uranium::Version(1, 20, 50)
#define FORMAT_VERSION_1_20_30 Uranium::Version(1, 20, 30)

#define CURRENT_URANIUM_VERSION Uranium::Version(0,0,1)


#define LATEST_FORMAT_VERSION FORMAT_VERSION_1_20_51
#ifdef _MSC_VER // Check if MSVC compiler is being used
#define COMPILER_MSVC
#elif defined(__GNUC__) || defined(__GNUG__) // Check if GCC or compatible compiler is being used
#define COMPILER_GCC
#endif

#ifdef DEBUG
#define DEBUGCODE(x) x
#else
#define DEBUGCODE(x)
#endif

#ifdef COMPILER_MSVC
#define COMPILER_DEPENDENT_CODE(MSVC, GCC) MSVC
#elif defined(COMPILER_GCC)
#define COMPILER_DEPENDENT_CODE(MSVC, GCC) GCC
#endif
