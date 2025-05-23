/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#include "Renderer.hpp"

#include <iostream>

void Renderer::Init(std::unique_ptr<Device> outputDevice, std::unique_ptr<Device> zBufferDevice, std::unique_ptr<Scene> scene, std::unique_ptr<Arguments> arguments)
{
    this->device = std::move(outputDevice);
    this->scene = std::move(scene);
    this->arguments = std::move(arguments);
    this->bufferdevice = std::move(zBufferDevice);

    Vector4d backGroundColor = {218.0 / 255, 213.0 / 255, 208.0 / 255, 1.0};

    for (int i = 0; i < DEFAULT_WIDTH; i ++)
    {
        for (int j = 0; j < DEFAULT_HEIGHT; j ++)
        {
            device->SetPixel(i, j, backGroundColor);
        }
    }
}

void Renderer::Exit()
{
    device->Clean();
    device.reset();

    bufferdevice->Clean();
    bufferdevice.reset();

    scene.reset();
    arguments.reset();
}