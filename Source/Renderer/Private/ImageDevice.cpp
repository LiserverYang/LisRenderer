/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#include "ImageDevice.hpp"

#include <fstream>
#include <algorithm>
#include <iostream>

#pragma pack(push, 1)
struct TGAHeader
{
    uint8_t idLength = 0;
    uint8_t colorMapType = 0;
    uint8_t imageType = 2;
    uint16_t colorMapOrigin = 0;
    uint16_t colorMapLength = 0;
    uint8_t colorMapDepth = 0;
    uint16_t xOrigin = 0;
    uint16_t yOrigin = 0;
    uint16_t width;
    uint16_t height;
    uint8_t bitsPerPixel = 32;
    uint8_t imageDescriptor = 0b10000000;
};
#pragma pack(pop)

ImageDevice::ImageDevice(std::string imageFilePath)
    : filePath(std::move(imageFilePath)) {}

Vector4d &ImageDevice::GetPixel(uint32_t x, uint32_t y)
{
    return imageBuffer[y][x];
}

void ImageDevice::SetPixel(uint32_t x, uint32_t y, Vector4d &pixelValue)
{
    imageBuffer[y][x] = pixelValue;
}

void ImageDevice::Init()
{
    imageBuffer.resize(DEFAULT_HEIGHT);
    
    for (auto &row : imageBuffer)
    {
        row.resize(DEFAULT_WIDTH, {0xFF, 0xFF, 0xFF, 0xFF});
    }
}

void ImageDevice::Clean()
{
    imageBuffer.clear();
}

void ImageDevice::Push()
{
    std::ofstream file(filePath, std::ios::binary);
    if (!file)
        return;

    TGAHeader header;
    header.width = static_cast<uint16_t>(imageBuffer[0].size());
    header.height = static_cast<uint16_t>(imageBuffer.size());

    file.write(reinterpret_cast<char *>(&header), sizeof(header));

    for (const auto &row : imageBuffer)
    {
        for (const auto &pixel : row)
        {
            auto clamp = [](double v)
            {
                return static_cast<uint8_t>(std::clamp(v, 0.0, 1.0) * 255 + 0.5);
            };

            uint8_t bgra[4] = {
                clamp(pixel[2]), // B
                clamp(pixel[1]), // G
                clamp(pixel[0]), // R
                clamp(pixel[3])  // A
            };
            file.write(reinterpret_cast<char *>(bgra), sizeof(bgra));
        }
    }

    uint8_t end[2] = {0x2E, 0x00};
    file.write(reinterpret_cast<char *>(&end), sizeof(end));
}