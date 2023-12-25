#ifndef NTH_IMPLEMENTATION
#pragma once
#endif

//
#ifndef NTH_IMPLEMENTATION
#include "../../Core/Core.hpp"
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
#include "./InstanceData.hpp"
#endif

//
namespace nth {
    #ifndef NTH_IMPLEMENTATION
    struct PhysicalDeviceGetInfo {
        
    };

    //
    struct DeviceCreateInfo {
        
    };

    // header
    class InstanceData : public BaseData {

    };

    // header
    class PhysicalDeviceData : public BaseData {
        
    };

    #else
    // implementation

    #endif
}
