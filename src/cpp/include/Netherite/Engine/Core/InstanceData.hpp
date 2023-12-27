#ifndef NTH_IMPLEMENTATION
#pragma once
#endif

//
#ifndef NTH_IMPLEMENTATION
#include "../../Core/Vulkan.hpp"
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
    //
    struct InstanceCreateInfo {
        
    };

    // header
    class InstanceData : public BaseData {
        HQ virtual create(std::shared_ptr<Handle> base, void const* cInfoPtr) override;
    };

    #else
    // implementation
    HQ BaseData::create(std::shared_ptr<Handle> base, void const* cInfoPtr) {
        auto baseOf = {};//Registry.at(base)->specify<BaseData>();
        auto const& cInfo = *reinterpret_cast<InstanceCreateInfo const*>(cInfoPtr);

        //
        vk::ApplicationInfo appInfo = {};
        appInfo.apiVersion = VK_API_VERSION_1_3;
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pApplicationName = "Netherite";
        appInfo.pEngineName = "Netherite";

        //
        auto extensions = std::vector<char const*>{
            
        };

        //
        auto layers = std::vector<char const*>{
            
        };

        //
        BaseData::filterBy(extensions, [](auto const& a, auto const& prop){
            return (strcmp(a, prop.extensionName) == 0);
        }, vk::enumerateInstanceExtensionProperties());

        //
        BaseData::filterBy(layers, [](auto const& a, auto const& prop){
            return (strcmp(a, prop.layerName) == 0);
        }, vk::enumerateInstanceLayerProperties());

        //
        vk::InstanceCreateInfo instanceInfo = {};
        instanceInfo.setPEnabledExtensionNames(extensions);
        instanceInfo.setPEnabledLayerNames(layers);
        instanceInfo.pApplicationInfo = &appInfo;

        //
        auto value = vk::createInstance(instanceInfo);

        //
        auto hq = std::make_shared<Handle>((uintptr_t)value, (uint32_t)HType::Instance);
        auto by = std::make_shared<RegistryMember<InstanceData>>(base, hq, shared_from_this());
        Registry.emplace(std::weak_ptr<Handle>(hq), by);
        HMap.emplace((uintptr_t)value, hq);

        //
        return hq;
    }
    #endif
}
