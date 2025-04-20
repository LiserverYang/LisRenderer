/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#pragma once

#include "Device.hpp"

#include <string>
#include <vector>
#include <cstdint>

using namespace Eigen;

/**
 * 对 Device 的具体实现，可以实现对图片文件的交互
 */
class ImageDevice final : public Device
{
private:
    std::string filePath;

    std::vector<std::vector<Vector4d>> imageBuffer;

public:
    ImageDevice(std::string imageFilePath);

    virtual Vector4d &GetPixel(uint32_t x, uint32_t y) override;
    virtual void SetPixel(uint32_t x, uint32_t y, Vector4d &pixelValue) override;

    virtual void Init() override;
    virtual void Clean() override;
    virtual void Push() override;
};