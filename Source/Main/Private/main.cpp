/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#include "ImageDevice.hpp"
#include "PNGDevice.hpp"
#include "RasterizationRenderer.hpp"

#include <iostream>

int main(int argc, const char **argv)
{
    // 初始化成像设备
    Device *device{new PNGDevice{"./output.png"}};
    device->Init();

    // 初始化场景
    Scene *scene{new Scene{"./Scene/s1.scene"}};

    Renderer *renderer = new RasterizationRenderer();
    renderer->Init(device, scene);
    renderer->Render();
    renderer->Exit();

    delete renderer;
}