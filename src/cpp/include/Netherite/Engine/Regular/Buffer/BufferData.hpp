#ifndef NTH_IMPLEMENTATION
#pragma once
#endif

//
#ifndef NTH_IMPLEMENTATION
#include "../../../Core/Vulkan.hpp"
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
#include "./BufferData.hpp"
#endif

//
namespace nth {
    #ifndef NTH_IMPLEMENTATION
    struct BufferGetInfo {
        
    };

    // header
    class BufferData : public BaseData {
        
    };
    #else
    // implementation

    #endif
}
