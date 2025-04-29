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

    /**
     * 从.scene文件读取场景，此文件所有描述单独一行，# 开头的行表示该行为注释
     * 其它的行描述如下：
     * 
     * o 开头表示定义场景中物体，其格式为：
     * o 物体文件路径（相对路径以scene文件为计算标准，obj格式） 物体位置 物体缩放 物体旋转
     * 
     * l 开头表示场景中的灯光，其格式为：
     * l 灯光位置 灯光强度
     * 
     * c 开头表示场景中的摄像机，其格式为：
     * c 摄像机位置 摄像机朝向 摄像机头顶朝向
     * 
     * 参考 Scene/s1.scene 的编写方式
     * 
     * @param filePath 要读取的scene文件路径
     */
    Scene(std::string filePath);

    void SetCamera(Camera &_camera);
    Camera &GetCamera();

    void SetObjects(ObjectList &objectList);
    ObjectList &GetObjects();
    
    void SetLights(LightList &lightList);
    LightList &GetLights();
};