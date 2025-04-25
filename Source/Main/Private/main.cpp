/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#include "DeviceFactory.hpp"
#include "RasterizationRenderer.hpp"
#include "Raytracing/RaytracingRenderer.hpp"

#include "Arguments.hpp"

#include <iostream>
#include <string>

#ifndef COMPILING
#define COMPILE_INFO 1, 2, 3, 4
#define GITHUB_VERSION "xxx"
#endif

#define _STR(a, b, c, d) #a ", " #b ", " #c ", " #d
#define STR(x) _STR(x)

#define _STRGV(a) #a
#define STRGV(x) _STRGV(x)

const std::string VERSION = (std::string) "0.0.1 " + STRGV(GITHUB_VERSION);

int main(int argc, const char **argv)
{
    if (strcmp(argv[1], "--help") == 0)
    {
        std::cout << "Renderer [SceneFilePath] [DeviceType = TgaDevice/ImageDevice/PNGDevice] [OutputFilePath] [ZBufferFilePath]";

        return 0;
    }
    else if (strcmp(argv[1], "--version") == 0)
    {
        std::cout << VERSION + " (" + std::string(STR(COMPILE_INFO)) + ")\n" + (std::string) "Copyrigt 2025, LiserverYang. All rights reserved. MIT License.";

        return 0;
    }

    auto arguments = std::make_unique<Arguments>();

    arguments->sceneFilePath = argv[1];
    arguments->deviceType = argv[2];
    arguments->outputFilePath = argv[3];
    arguments->zBufferFilePath = argv[4];

    // 初始化成像设备
    std::unique_ptr<Device> device = DeviceFactory::CreateDeviceFromString(arguments->deviceType, arguments->outputFilePath);
    device->Init();

    // 初始化场景
    std::unique_ptr<Scene> scene = std::make_unique<Scene>(arguments->sceneFilePath);

    Renderer *renderer = new RasterizationRenderer();
    renderer->Init(std::move(device), std::move(scene), std::move(arguments));
    renderer->Render();
    renderer->Exit();

    delete renderer;
}