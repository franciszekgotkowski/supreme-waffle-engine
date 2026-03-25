#pragma once

#if defined(PLATFORM_WINDOWS)
    #include <malloc.h>
    // #define alloca _alloca
#elif defined(PLATFORM_LINUX)
    #include <alloca.h>
#endif

