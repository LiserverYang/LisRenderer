#include "Raytracing/RaytracingRenderer.hpp"

#include "Eigen/Dense"

using namespace Eigen;

void RaytracingRenderer::Render()
{
    for (int i = 0; i < DEFAULT_HEIGHT; i ++)
    {
        for (int j = 0; j < DEFAULT_WIDTH; j ++)
        {
            Vector4d color{0.0, j * 1.0 / DEFAULT_WIDTH , i * 1.0 / DEFAULT_HEIGHT, 1.0};
            device->SetPixel(j, i, color);
        }
    }

    device->Push();
}