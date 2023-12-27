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
#include "./DeviceData.hpp"
#endif

//
namespace nth {
    #ifndef NTH_IMPLEMENTATION

    //
    struct QueueCreateInfo {
        view<float> queuePriority = {};
        uint32_t familyIndex = 0u;
    };

    //
    struct DeviceCreateInfo {
        uint32_t physicalDeviceId = 0;
        view<QueueCreateInfo> queueInfos = {};
    };

    // header
    class DeviceData : public BaseData {
        HQ virtual create(std::shared_ptr<Handle> base, void const* cInfoPtr) override;
    };
    #else
    // implementation
    HQ BaseData::create(std::shared_ptr<Handle> base, void const* cInfoPtr) {
        auto baseOf = Registry.at(base)->specify<InstanceData>();
        auto const& cInfo = *reinterpret_cast<DeviceCreateInfo const*>(cInfoPtr);

        //
        auto instance = vk::Instance(base->addressOrId);
        auto devices = instance.enumeratePhysicalDevices();
        auto physicalDevice = devices[cInfo.physicalDeviceId];

        //
        auto extensions = std::vector<char const*>{
            
        };

        //
        auto layers = std::vector<char const*>{
            
        };

        //
        BaseData::filterBy(extensions, [](auto const& a, auto const& prop){
            return (strcmp(a, prop.extensionName) == 0);
        }, physicalDevice.enumerateDeviceExtensionProperties(););

        //
        BaseData::filterBy(layers, [](auto const& a, auto const& prop){
            return (strcmp(a, prop.layerName) == 0);
        }, physicalDevice.enumerateDeviceLayerProperties());

        //
        auto queueInfos = BaseData::transform(createInfo.queueInfos, [] (auto const& queueInfo) {
            vk::DeviceQueueCreateInfo createInfo = {};
            createInfo.setQueuePriorities(queueInfo.queuePriority);
            createInfo.setQueueFamilyIndex(queueInfo.familyIndex);
            return createInfo;
        });

        //
        vk::DeviceCreateInfo deviceInfo = {};
        deviceInfo.setPEnabledExtensionNames(extensions);
        deviceInfo.setPEnabledLayerNames(layers);
        deviceInfo.setQueueCreateInfos(queueInfos);

        //
        auto value = physicalDevice.createDevice(deviceInfo);

        //
        auto hq = std::make_shared<Handle>((uintptr_t)value, (uint32_t)HType::Device);
        auto by = std::make_shared<RegistryMember<DeviceData>>(base, shared_from_this());
        Registry.emplace(std::weak_ptr<Handle>(hq), by);
        HMap.emplace((uintptr_t)value, hq);
        return (this->handle = hq);
    }
    #endif
}
