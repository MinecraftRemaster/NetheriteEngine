#ifndef NTH_IMPLEMENTATION
#pragma once
#endif

//
#ifndef NTH_IMPLEMENTATION
#include "./Core.hpp"
#endif

//
#ifdef NTH_IMPLEMENTATION
#undef NTH_IMPLEMENTATION
#include <vulkan/vulkan.hpp>
#include <vma/vk_mem_alloc.hpp>
// push headers without implementation
#define NTH_IMPLEMENTATION
#else
// push headers for headers
#endif
//

//
#ifdef NTH_IMPLEMENTATION
#include "./Vulkan.hpp"
#endif

//
namespace nth {
    #ifndef NTH_IMPLEMENTATION
    //

    #else
    // implementation

    #endif
}
