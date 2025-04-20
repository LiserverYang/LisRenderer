#include "PNGDevice.hpp"
#include <algorithm>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace Eigen;

PNGDevice::PNGDevice(std::string imageFilePath) : filePath(imageFilePath) {}

Vector4d &PNGDevice::GetPixel(uint32_t x, uint32_t y)
{
    return imageBuffer[y][x];
}

void PNGDevice::SetPixel(uint32_t x, uint32_t y, Vector4d &pixelValue)
{
    imageBuffer[y][x] = pixelValue;
}

void PNGDevice::Init()
{
    imageBuffer.resize(DEFAULT_HEIGHT);
    for (auto &row : imageBuffer)
    {
        row.resize(DEFAULT_WIDTH, Vector4d(0.0, 0.0, 0.0, 0.0));
    }
}

void PNGDevice::Clean()
{
    imageBuffer.clear();
}

void PNGDevice::Push()
{
    const int width = DEFAULT_WIDTH;
    const int height = DEFAULT_HEIGHT;
    std::vector<unsigned char> data(width * height * 4);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            const Vector4d &pixel = imageBuffer[y][x];
            double r = std::clamp(pixel[0], 0.0, 1.0);
            double g = std::clamp(pixel[1], 0.0, 1.0);
            double b = std::clamp(pixel[2], 0.0, 1.0);
            double a = std::clamp(pixel[3], 0.0, 1.0);

            unsigned char r_byte = static_cast<unsigned char>(r * 255.0 + 0.5);
            unsigned char g_byte = static_cast<unsigned char>(g * 255.0 + 0.5);
            unsigned char b_byte = static_cast<unsigned char>(b * 255.0 + 0.5);
            unsigned char a_byte = static_cast<unsigned char>(a * 255.0 + 0.5);

            int data_row = height - 1 - y;
            size_t index = (data_row * width + x) * 4;
            data[index] = r_byte;
            data[index + 1] = g_byte;
            data[index + 2] = b_byte;
            data[index + 3] = a_byte;
        }
    }

    stbi_write_png(filePath.c_str(), width, height, 4, data.data(), width * 4);
}