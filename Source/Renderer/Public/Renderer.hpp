/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#pragma once

#include "Device.hpp"
#include "Scene.hpp"
#include "Arguments.hpp"

#include <memory>

const double fovY = 90;
const double zNear = 1;
const double zFar = 20;

/**
 * 定义了渲染器的所有操作
 */
class Renderer
{
protected:
    std::unique_ptr<Device> device;
    std::unique_ptr<Scene> scene;
    std::unique_ptr<Arguments> arguments;

public:
    Renderer() = default;
    ~Renderer() = default;

    /**
     * 相当于构造函数
     *
     * @param outputDevice 成像设备，一般是窗口或图片文件
     */
    void Init(std::unique_ptr<Device> outputDevice, std::unique_ptr<Scene> sence, std::unique_ptr<Arguments> arguments);

    /**
     * 使渲染器进行渲染
     * 具体的渲染操作取决于渲染器的实现
     */
    virtual void Render() = 0;

    /**
     * 进行清理，只要是释放内存
     */
    void Exit();
};