/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#pragma once

#include "Camera.hpp"
#include "Object.hpp"
#include "Light.hpp"

/**
 * Scene 存储了整个场景，包括所有的物体和摄像机
 */
class Scene
{
public:
    using ObjectList = std::vector<Object>;
    using LightList = std::vector<Light>;

private:
    Camera camera;
    ObjectList objects;
    LightList lights;

public:
    Scene() = default;

    void SetCamera(Camera &_camera)
    {
        camera = _camera;
    }

    void SetObjects(ObjectList &objectList)
    {
        objects = objectList;
    }

    void SetLights(LightList &lightList)
    {
        lights = lightList;
    }

    Camera &GetCamera()
    {
        return camera;
    }

    ObjectList &GetObjects()
    {
        return objects;
    }

    LightList &GetLights()
    {
        return lights;
    }
};