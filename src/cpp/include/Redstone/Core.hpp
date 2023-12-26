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
#include <Vulkan/spirv/unified1/spirv.hpp>
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
    struct OpCodeT { uint16_t OpCode = 0, WCount = 1; };

    //
    inline decltype(auto) PushOp(std::vector<uint32_t>& code, OpCodeT const& OpCode, std::vector<uint32_t> const& ops = {}) {
        code.push_back(reinterpret_cast<uint32_t const&>(OpCode));
        code.insert(code.end(), ops.begin(), ops.end());
        return code;
    }

    //
    template<class... O>
    inline decltype(auto) PushOpSeq(std::vector<uint32_t>& code, OpCodeT const& OpCode, O const& ...Seq) {
        using T = std::common_type_t<O...>;
        std::initializer_list<T> li{std::forward<O const&>(Seq)...};
        std::vector<T> Q{li};
        code.push_back(reinterpret_cast<uint32_t const&>(OpCode));
        code.insert(code.end(), Q.begin(), Q.end());
        return code;
    }

    //
    inline uint16_t StringWordCount(std::string const& name = "") {
        return (name.size() + 4 - 1) / 4;
    }

    //
    inline decltype(auto) PushCode(std::vector<uint32_t>& code, std::vector<uint32_t> const& ops = {}) {
        code.insert(code.end(), ops.begin(), ops.end());
        return code;
    }

    //
    inline decltype(auto) PushString(std::vector<uint32_t>& code, std::string const& name = "") {
        std::vector<uint32_t> packed( StringWordCount(name), 0 );
        memcpy(packed.data(), name.c_str(), name.size());
        code.insert(code.end(), packed.begin(), packed.end());
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
        //
        std::vector<uint32_t> code = { 
            0x07230203,
            spv::Version,
            0x00080001,
            count.get(), 0
        };

        //
        PushOpSeq(code, OpCodeT{spv::OpCapability, 2}, spv::CapabilityShader);
        PushOpSeq(code, OpCodeT{spv::OpMemoryModel, 3}, spv::AddressingModelLogical, spv::MemoryModelVulkan);

        // import GLSL.450
        const std::string GLSL_std_450 = "GLSL.std.450";
        PushOpSeq(code, OpCodeT{spv::OpExtInstImport, StringWordCount(GLSL_std_450)}, 1);
        PushString(code, GLSL_std_450);

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
