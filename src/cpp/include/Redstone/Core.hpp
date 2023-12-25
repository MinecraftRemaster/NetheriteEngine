// THIS HEADER SHOULD TO BE PRE-COMPILED!
#ifndef NTH_IMPLEMENTATION
#pragma once
#endif

//
//#ifndef NTH_IMPLEMENTATION
//#include <Core.hpp>
//#endif

//
#ifdef NTH_IMPLEMENTATION
#undef NTH_IMPLEMENTATION
// push headers without implementation
#define NTH_IMPLEMENTATION
#else
#include <cstring>
#include <cstdint>
#include <memory>
#include <map>
#include <unordered_map>
#include <atomic>

//
#include <spirv/unified1/spirv.hpp>
// push headers for headers
#endif
//

//
#ifdef NTH_IMPLEMENTATION
#include "./Core.hpp"
#endif

//
namespace rds {
    #ifndef NTH_IMPLEMENTATION
    // header

    //
    //using Encounter = uint32_t>;

    //
    class uint32_enc {
        protected: std::atomic<uint32_t> counter = 0;

        //
        public: uint32_enc(uint32_t starter = 0) : counter(starter) {}

        //
        public: uint32_t get() const {
            return counter;
        }

        //
        public: operator uint32_t() {
            return counter++;
        }

        //
        public: operator uint32_t() const {
            return counter;
        }
    };

    //
    inline decltype(auto) PushOp(std::vector<uint32_t>& code, uint32_t const& OpCode, std::vector<uint32_t> const& ops = {}) {
        code.push_back(OpCode);
        code.insert(code.end(), ops.begin(), ops.end());
        return code;
    }

    //
    template<class... O>
    inline decltype(auto) PushOpSeq(std::vector<uint32_t>& code, uint32_t const& OpCode, O const& ...Seq) {
        using T = std::common_type_t<O...>;
        std::initializer_list<T> li{std::forward<O>(Seq)...};
        std::vector<T> Q{li};
        code.push_back(OpCode);
        code.insert(code.end(), Q.begin(), Q.end());
        return code;
    }



    //
    inline decltype(auto) PushCode(std::vector<uint32_t>& code, std::vector<uint32_t> const& ops = {}) {
        code.insert(code.end(), ops.begin(), ops.end());
        return code;
    }

    //
    template<class... O>
    inline decltype(auto) PushCodeSeq(std::vector<uint32_t>& code, O const& ...Seq) {
        using T = std::common_type_t<O...>;
        std::initializer_list<T> li{std::forward<O>(Seq)...};
        std::vector<T> Q{li};
        code.insert(code.end(), Q.begin(), Q.end());
        return code;
    }

    //
    inline decltype(auto) ResolveSPIRV(std::vector<uint32_t> const& raw, uint32_enc const& count = {}) {
        // repack an string
        std::string import = "GLSL.std.450";
        std::vector<uint32_t> packed( (import.size() + 4 - 1) / 4, 0 );
        memcpy(packed.data(), import.c_str(), import.size());

        //
        std::vector<uint32_t> code = { 
            0x07230203,
            spv::Version,
            0x00080001,
            count.get(), 0
        };

        //
        PushOpSeq(code, spv::OpCapability, spv::CapabilityShader);
        PushOpSeq(code, spv::OpMemoryModel, spv::AddressingModelLogical, spv::MemoryModelVulkan);

        // import GLSL.450
        PushOpSeq(code, spv::OpExtInstImport, 1);
        PushCode(packed);

        //
        // TODO! entry-point support
        // TODO! execution-mode

        // paste an rest code
        PushCode(code, raw);

        //
        return code;
    }

    #else
    // implementation

    #endif
}
