/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#pragma once

#include "Eigen/Dense"

#include <cstdint>

using namespace Eigen;

const int DEFAULT_WIDTH = 1920;
const int DEFAULT_HEIGHT = 1080;
const double ASPECT = DEFAULT_WIDTH * 1.0 / DEFAULT_HEIGHT;

/**
 * Device 是对成像设备的一个抽象
 * 更确切地说，我们在渲染时并不关注我们的输出目标是一个图片还是一个窗口——我们只需要能够将渲染结果输出就可以了
 * 所以这里对成像设备进行了一个抽象，它用于写入渲染的结果，具体的细节由不同的子类实现
 * 在这个渲染器中，我们认为设备是二维的像素数组，每个像素的坐标由(0,0)到(width,height)，像素的中心是(x+0.5,y+0.5)，与 games101 相同
 */
class Device
{
public:
    Device() = default;
    ~Device() = default;

    virtual void Init() = 0;
    virtual void Clean() = 0;

    /**
     * 获取设备上的一个像素。
     * 尽管返回的类型是一个引用，但是仍然建议调用 `SetPixel` 函数进行修改，即不保证修改引用能够修改像素
     * 
     * @param x 像素的x坐标
     * @param y 像素的y坐标
     * @return Vec4d& 获取到的像素值
     */
    virtual Vector4d &GetPixel(uint32_t x, uint32_t y) = 0;

    /**
     * 设置一个像素的颜色
     * 
     * @param x 像素的x坐标
     * @param y 像素的y坐标
     * @param pixelValue 修改的像素值
     */
    virtual void SetPixel(uint32_t x, uint32_t y, Vector4d &pixelValue) = 0;


    /**
     * 如果 Device 自带缓冲，通过 Push 函数提交修改，具体是否调用这个函数看具体实现
     */
    virtual void Push() = 0;
};