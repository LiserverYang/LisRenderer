/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#pragma once

#include "Device.hpp"

#include <cstdint>
#include <string>
#include <vector>

using namespace Eigen;

/**
 * 类似于 ImageDevice，实现写入PNG文件
 */
class PNGDevice final : public Device
{
private:
    std::string filePath;

    std::vector<std::vector<Vector4d>> imageBuffer;

public:
    PNGDevice(std::string imageFilePath);

    virtual Vector4d &GetPixel(uint32_t x, uint32_t y) override;
    virtual void SetPixel(uint32_t x, uint32_t y, Vector4d &pixelValue) override;

    virtual void Init() override;
    virtual void Clean() override;
    virtual void Push() override;
};