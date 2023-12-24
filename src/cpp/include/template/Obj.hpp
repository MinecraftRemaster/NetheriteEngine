#pragma once

//
#ifndef NTH_IMPLEMENTATION
#include <Core.hpp>
#endif

//
#ifdef NTH_IMPLEMENTATION
#undef NTH_IMPLEMENTATION
// push headers without implementation
#define NTH_IMPLEMENTATION
#else
// push headers for headers
#endif
//

//
#ifdef NTH_IMPLEMENTATION
#include "./SELF.hpp"
#endif

//
namespace nth {
    #ifndef NTH_IMPLEMENTATION
    // header

    #else
    // implementation

    #endif
}
