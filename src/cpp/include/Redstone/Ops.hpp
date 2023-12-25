// THIS HEADER SHOULD TO BE PRE-COMPILED!
#ifndef NTH_IMPLEMENTATION
#pragma once
#endif

//
#include "./Core.hpp"

//
namespace rds {
    #ifndef NTH_IMPLEMENTATION
    // header

    //
    inline decltype(auto) OpLabel(std::vector<uint32_t>& code, uint32_t const& id) {
        return PushOpSeq(code, spv::OpLabel, id);
    }

    //
    inline decltype(auto) OpBranch(std::vector<uint32_t>& code, uint32_t const& id) {
        return PushOpSeq(code, spv::OpBranch, id);
    }

    #else
    // implementation

    #endif
}
