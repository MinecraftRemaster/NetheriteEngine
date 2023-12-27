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
#include <memory>
#include <map>
#include <functional>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <span>
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
    class WeakMemberBase;
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
    enum class HType : uint32_t {
        Unknown = 0,
        Instance = 1,
        PhysicalDevice = 2,
        Device = 3,

        Buffer = 8,
        Image = 9
    };

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
    class Handle {
        public: uintptr_t addressOrId = 0ull;
        public: uint32_t typeOf = 0; // i.e. aka HType

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
    using HQ = xn_shared_ptr<Handle>;

    //
    /*class Holder {
        protected: HQ handle = {};

        //
        public: Holder(HQ handle = {}) : handle(handle) {};

        //
        public: operator HQ&() { return handle; };
        public: operator HQ const&() const { return handle; };
        public: Holder& operator=(HQ const& h) { handle = h; return *this; };
    };*/

    //
    class BaseData : public std::enable_shared_from_this<BaseData> {
        public: BaseData() = default;

        //
        public: virtual xn_shared_ptr<Handle> create(std::shared_ptr<Handle> base, void const* cInfoPtr = nullptr);
        public: virtual void get(std::shared_ptr<Handle> handle, void* output = nullptr, void const* gInfoPtr = nullptr);

        //
        public: template<class O, class T> inline static decltype(auto) create(std::shared_ptr<Handle> base, T const& info) { return std::make_shared<O>()->create(base, &info); };
        public: template<class O, class T> static inline decltype(auto) get(std::shared_ptr<Handle> handle, T const& info);

        // routine when create device or instance
        public: template<class T, class F = char const*>
        inline static decltype(auto) filterBy(std::vector<F>& what, std::function<bool(F const&, T const&)> filter, std::vector<T> const& available = {}) {
            for (const auto& ext : available) {
                const auto e = std::find_if(what.begin(), what.end(), [ext, filter](F const& a){
                    return filter(a, ext); //strcmp(a, ext.layerName) == 0;
                });
                if (e == what.end()) { what.erase(e); };
            }
            return what;
        }

        // routine when create device or instance
        public: template<class T, class F>
        inline static decltype(auto) transform(std::vector<F>& what, std::function<T(F const&)> transform) {
            std::vector<T> to = {}; to.reserve(what.size());
            std::transform(what.begin(), what.end(), std::back_inserter(to), transform);
            return to;
        }
    };

    //
    template<class T = BaseData>
    class HolderOf : public HQ //public Holder 
    {
        public: template<uint32_t M, class C>
        inline HQ create(C const& cInfo) {
            decltype(auto) handle = shared_from_this();
            return T::create<M>(handle, cInfo);
        }

        //
        public: template<class C>
        inline decltype(auto) get(C const& getInfo) {
            decltype(auto) handle = shared_from_this();
            return T::get(handle, getInfo);
        }
    };


    //
    template<class T = BaseData> class RegistryMember;
    template<class T = WeakMemberBase> class WeakMember;

    //
    class RegistryMemberBase : public std::enable_shared_from_this<RegistryMemberBase> {
        protected: 

        //
        xn_weak_ptr<Handle> handle = {};
        xn_shared_ptr<Handle> base = {};
        xn_shared_ptr<BaseData> data = {};

        //
        //std::unordered_map<Handle, xn_weak_ptr<RegistryMemberBase>> objMap = {};
        std::unordered_map<Handle, xn_weak_ptr<Handle>> objMap = {};

        //
        public: RegistryMemberBase(std::shared_ptr<Handle> base = {}, std::shared_ptr<Handle> handle = {}, std::shared_ptr<BaseData> data = {}): base(base), handle(handle), data(data) {
            
        }

        //
        public: std::weak_ptr<RegistryMemberBase> inline weak() {
            return shared_from_this();
        }

        //
        public: template<class T = BaseData>
        xn_shared_ptr<RegistryMember<T>> inline specify() const {
            return std::dynamic_pointer_cast<T>(shared_from_this());
        }

        //
        public: template<class T = BaseData>
        inline xn_shared_ptr<T> getData() const {
            return std::reinterpret_pointer_cast<T>(this->data);
        }

        //
        public: inline static xn_shared_ptr<Handle> create() {
            return std::make_shared<Handle>();
        }

        //
        public: template<class H = uintptr_t>
        inline static xn_shared_ptr<H> create() {
            return std::make_shared<H>((H)0ull);
        }

        //
        public: inline xn_shared_ptr<Handle> getHandle() const {
            return this->handle.xn_lock();
        }

        //
        public: inline xn_shared_ptr<Handle> getBase() const {
            return this->base;
        }
    };

    //
    template<class T>
    class RegistryMember: public RegistryMemberBase {
        
        //
        public: inline xn_shared_ptr<T> getData() const {
            return std::reinterpret_pointer_cast<T>(this->data);
        }

    };

    //
    template<class T>
    class view {
        protected: T const* _ptr = nullptr;
        protected: uint32_t _size = 0;

        //
        public: view(std::vector<T> const& vect = {}) : _ptr(vect.data()), _size(vect.size()) {};
        public: view(T const* _ptr, size_t const& _size = 1) : _ptr(_ptr), _size(_size) {};
        public: view(T const& _ptr, size_t const& _size = 1) : _ptr(&_ptr), _size(_size) {};

        //
        public: uint32_t const& size() const { return _size; }
        public: T const* data() const { return _ptr; }

        //
        public: operator std::vector<T>() const {  return std::vector<T>(_ptr, _ptr + _size); }
        //public: operator std::span<T>() const {  return std::span<T>(_ptr, _ptr + _size); }
        public: operator T const*() const { return _ptr; }
    };

    // for buffer, image, etc.
    template<class K = Handle, class T = RegistryMemberBase>
    using weak_map_t = std::map<std::weak_ptr<K>, xn_shared_ptr<T>, std::owner_less<std::weak_ptr<K>>>;
    using handle_map = std::map<uintptr_t, xn_weak_ptr<Handle>>;

    //
    inline static weak_map_t<> Registry = {};
    inline static handle_map HMap = {};

    //
    inline static HQ getHandle(HQ handle) {
        return Registry.at(handle)->getBase();
    }

    //
    template<class O, class T>
    inline decltype(auto) BaseData::get(std::shared_ptr<Handle> handle, T const& gInfoPtr) {
        auto output = O{};
        Registry.at(handle)->getData()->get(handle, &output, &gInfoPtr);
        return output;
    };

    //
    #else
    // implementation
    HQ BaseData::create(std::shared_ptr<Handle> base, void const* cInfoPtr) {
        auto baseOf = Registry.at(base);//->specify<DeviceData>();
        auto value = 0ull;
        auto hq = std::make_shared<Handle>((uintptr_t)value, (uint32_t)HType::Unknown);
        auto by = std::make_shared<RegistryMember<BaseData>>(base, hq, shared_from_this());
        Registry.emplace(std::weak_ptr<Handle>(hq), by);
        return hq;
    }

    #endif
}

//
#ifndef NTH_IMPLEMENTATION
inline std::size_t std::hash<nth::Handle>::operator()(const nth::Handle& s) const noexcept {
    auto h1 = std::hash<uintptr_t>{}(s.addressOrId), h2 = std::hash<uintptr_t>{}(s.typeOf);
    return h1 ^ (h2 << 1); // or use boost::hash_combine
};
#endif
