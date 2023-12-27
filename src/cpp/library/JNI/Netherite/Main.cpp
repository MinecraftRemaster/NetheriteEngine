#include <Netherite/Core/Core.hpp>
#include <memory>

//
namespace jni {
    
    //
    struct void_t{};
    using JNIHandle = uintptr_t;

    //
    nth::HQ GetHandle(JNIHandle jniPtr) {
        return *((nth::HQ*)jniPtr);
    }

    //
    template<class W, class C = void_t, class B = nth::BaseData>
    JNIHandle MakeHandle(std::shared_ptr<nth::Handle> base, C const& cInfo) {
        auto jniPtr = new nth::HQ{};
        *jniPtr = B::createBy<W>(base, cInfo);
        return JNIHandle(jniPtr);
    }

    //
    void DestroyHandle(JNIHandle jniPtr) {
        delete (nth::HQ*)jniPtr;
    }
}

//
extern "C" {

    //
    JNIEXPORT jlong JNICALL com_netherite_engine_JNIBase_Create(JNIEnv *, jobject, jlong pBase, jlong cInfo) {
        auto base = !pBase ? std::shared_ptr<nth::Handle>{} : GetSharedPtr(pBase);
        return MakeSharedPtr<nth::BaseData>(base, (void*)cInfo);
    }

}

//
#if !defined(main)
int main() {
    

    return 0;
}
#endif
