/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#include "Renderer.hpp"

#include <iostream>

void Renderer::Init(Device *outputDevice, Scene *scene)
{
    device.reset(outputDevice);
    this->scene.reset(scene);

    Vector4d color = {0.0, 0.0, 0.0, 1.0};

    for (int i = 0; i < DEFAULT_WIDTH; i ++)
    {
        for (int j = 0; j < DEFAULT_HEIGHT; j ++)
        {
            device->SetPixel(i, j, color);
        }
    }
}

void Renderer::Exit()
{
    device->Clean();
    device.reset();

    scene.reset();
}