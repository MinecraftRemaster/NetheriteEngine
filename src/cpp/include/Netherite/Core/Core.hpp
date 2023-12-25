#ifndef NTH_IMPLEMENTATION
#pragma once
#endif

//
#ifdef NTH_IMPLEMENTATION
#undef NTH_IMPLEMENTATION
// push headers without implementation
#define NTH_IMPLEMENTATION
#else
#include <cstring>
#include <cstdint>
#include <unordered_map>
#include <memory>
#include <atomic>
// push headers for headers
#endif
//

//
#ifdef NTH_IMPLEMENTATION
//#include "./Handle.hpp"
#endif

//
#ifndef NTH_IMPLEMENTATION
namespace nth {
    //
    class Handle;
    class RegistryMemberBase;
}

template<> struct std::hash<nth::Handle> {
    inline std::size_t operator()(const nth::Handle& s) const noexcept;
};
#endif

//
namespace nth {
    #ifndef NTH_IMPLEMENTATION
    // header

    //
    using reference_t = uintptr_t;

    //
    class Handle {
        public: uintptr_t addressOrId = 0ull;
        public: uint32_t typeOf = 0;

        //
        public: Handle(uintptr_t const& addressOrId, uint32_t const& typeOf) {
            this->addressOrId = addressOrId;
            this->typeOf = typeOf;
        }

        //
        public: Handle(Handle const& handle) = default;
        public: Handle() = default;

        //
        /*public: Handle(Handle const& handle = Handle(0ull, 0u)) {
            this->addressOrId = handle.addressOrId;
            this->typeOf = handle.typeOf;
        }*/

        //
        public: Handle& operator=(Handle const& handle) {
            this->addressOrId = handle.addressOrId;
            this->typeOf = handle.typeOf;
            return *this;
        }

        //
        public: bool operator==(Handle const& handle) const {
            return (handle.addressOrId == this->addressOrId && handle.typeOf == this->typeOf);
        }

        //
        public: operator size_t() {
            return std::hash<Handle>{}(*this);
        }

        //
        public: operator bool() const {
            return this->addressOrId && this->typeOf;
        }

        //
        public: template<class T> T& as() { return reinterpret_cast<T&>(this->addressOrId); }
        public: template<class T> T const& as() const { return reinterpret_cast<T const&>(this->addressOrId); }
    };

    //
    class BaseDataObj : public std::enable_shared_from_this<BaseDataObj> {
        public: BaseDataObj() {};
    };

    //
    template<class T = BaseDataObj>
    class RegistryMember;

    //
#if defined(__clang__)
    //
    template<class T>
    using xn_shared_ptr = std::shared_ptr<T>;

    //
    template<class T>
    using xn_weak_ptr = std::weak_ptr<T>;

    //
    #define xn_lock lock
#else
    //
    template<class T>
    using xn_shared_ptr = std::atomic<std::shared_ptr<T>>;

    //
    template<class T>
    using xn_weak_ptr = std::atomic<std::weak_ptr<T>>;

    //
    #define xn_lock load().lock
#endif

    //
    class RegistryMemberBase : public std::enable_shared_from_this<RegistryMemberBase> {
        protected: 
        xn_weak_ptr<Handle> handle = {};
        xn_shared_ptr<Handle> base = {};
        xn_shared_ptr<BaseDataObj> data = {};

        //
        public: RegistryMemberBase(std::shared_ptr<Handle> base = {}, std::shared_ptr<Handle> handle = {}): base(base), handle(handle) {
            
        }

        //
        std::weak_ptr<RegistryMemberBase> inline weak() {
            return shared_from_this();
        }

        //
        public: template<class T = RegistryMemberBase>
        std::shared_ptr<RegistryMember<T>> inline as() const {
            return std::dynamic_pointer_cast<T>(shared_from_this());
        }

        //
        public: template<class T = BaseDataObj>
        inline std::shared_ptr<T> getData() const {
            return std::reinterpret_pointer_cast<T>(this->data);
        }

        //
        public: inline static std::shared_ptr<Handle> create() {
            return std::make_shared<Handle>();
        }

        //
        public: template<class H = uintptr_t>
        inline static xn_shared_ptr<H> create() {
            return std::make_shared<H>((H)0ull);
        }

        //
        public: inline std::shared_ptr<Handle> getHandle() const {
            return this->handle.xn_lock();
        }

        //
        public: inline std::shared_ptr<Handle> getBase() const {
            return this->base;
        }
    };

    //
    template<class T>
    class RegistryMember: public RegistryMemberBase {
        
        //
        public: inline std::shared_ptr<T> getData() const {
            return std::reinterpret_pointer_cast<T>(this->data);
        }

        
    };

    // for buffer, image, etc.
    template<class K = uintptr_t, class T = RegistryMemberBase>
    using weak_map_t = std::unordered_map<std::weak_ptr<K>, T, std::owner_less<std::weak_ptr<K>>>;

    // for device, commandbuffer, instance, queue, etc.
    template<class T = RegistryMemberBase> 
    using registry_t = std::unordered_map<Handle, T>;

    // for buffer, image, etc.
    inline static weak_map_t<> WeakMap = {};

    // for device, commandbuffer, instance, queue, etc.
    inline static registry_t<> Registry = {};

    //
    #else
    // implementation

    #endif
}

//
#ifndef NTH_IMPLEMENTATION
inline std::size_t std::hash<nth::Handle>::operator()(const nth::Handle& s) const noexcept {
    auto h1 = std::hash<uintptr_t>{}(s.addressOrId), h2 = std::hash<uintptr_t>{}(s.typeOf);
    return h1 ^ (h2 << 1); // or use boost::hash_combine
};
#endif
