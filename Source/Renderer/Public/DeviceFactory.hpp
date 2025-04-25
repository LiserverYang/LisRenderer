/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#pragma once

#include "Device.hpp"
#include "ImageDevice.hpp"
#include "PNGDevice.hpp"

#include <memory>

/**
 * 存放所有渲染器用到的参数
 */
class DeviceFactory
{
public:
    static std::unique_ptr<Device> CreateDeviceFromString(std::string deviceType, std::string filePath)
    {
        if (deviceType == "ImageDevice" || deviceType == "TgaDevice")
        {
            return std::make_unique<ImageDevice>(filePath);
        }
        else if (deviceType == "PNGDevice")
        {
            return std::make_unique<PNGDevice>(filePath);
        }
        else
        {
            return nullptr;
        }
    }
};