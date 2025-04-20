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
    Scene *scene{new Scene{}};

    // 按照默认规定摆放摄像机
    Camera camera;
    camera.position = Vector4d{0.0, 0.0, 0.0, 1.0};
    camera.upDirection = Vector4d{0.0, 1.0, 0.0, 0.0};
    camera.gaezeDirection = Vector4d{0.0, 0.0, -1.0, 0.0};

    scene->SetCamera(camera);
    
    // 场景内的物体
    Object cube1;
    cube1.SetFilename("./cube.obj");
    cube1.Init();

    Vector4d cube1Position{2.0, 0.0, -4.5, 1.0};
    cube1.SetPosition(cube1Position);

    scene->GetObjects().push_back(cube1);

    Object ball;
    ball.SetFilename("./ball.obj");
    ball.Init();

    Vector4d ballPosition{0.0, 0.0, -2, 1.0};
    ball.SetPosition(ballPosition);

    scene->GetObjects().push_back(ball);

    Light light;
    light.position = {0.0, 0.0, 0.0, 1.0};
    // light.I = 0.35;
    light.I = 2;

    scene->GetLights().push_back(light);

    Renderer *renderer = new RasterizationRenderer();
    renderer->Init(device, scene);
    renderer->Render();
    renderer->Exit();

    delete renderer;
}