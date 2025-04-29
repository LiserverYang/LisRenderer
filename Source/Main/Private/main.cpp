/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 * 程序的主入口，负责处理参数、初始化一些对象，然后将它们提交给渲染器
 */

#include "DeviceFactory.hpp"
#include "RasterizationRenderer.hpp"
#include "Raytracing/RaytracingRenderer.hpp"
#include "Arguments.hpp"
#include <iostream>
#include <string>

// 这个文件由编译系统自动生成。
// 未编译源码时提示找不到文件是正常的，编译一次即可
// 这个文件储存程序的版本信息和版权信息
#include "Genericed.hpp"

int main(int argc, const char **argv)
{
    // 一些特殊参数判断
    if (strcmp(argv[1], "--help") == 0)
    {
        std::cout << "Renderer [SceneFilePath] [DeviceType = TgaDevice/ImageDevice/PNGDevice] [OutputFilePath] [ZBufferFilePath]";

        return 0;
    }
    else if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "--copyright") == 0)
    {
        std::cout << VERSION + " (" + COMPILE_INFO + ")\n" + COPYRIGHT;

        return 0;
    }

    // 打包参数
    auto arguments = std::make_unique<Arguments>();

    arguments->sceneFilePath = argv[1];
    arguments->deviceType = argv[2];
    arguments->outputFilePath = argv[3];
    arguments->zBufferFilePath = argv[4];

    // 初始化成像设备
    auto device = DeviceFactory::CreateDeviceFromString(arguments->deviceType, arguments->outputFilePath);
    device->Init();

    auto zBufferdevice = DeviceFactory::CreateDeviceFromString(arguments->deviceType, arguments->zBufferFilePath);
    zBufferdevice->Init();

    // 初始化场景
    auto scene = std::make_unique<Scene>(arguments->sceneFilePath);

    // 开始渲染
    auto renderer = std::make_unique<RasterizationRenderer>();
    renderer->Init(std::move(device), std::move(zBufferdevice), std::move(scene), std::move(arguments));
    renderer->Render();
    renderer->Exit();
}