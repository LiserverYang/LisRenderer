/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#pragma once

#include "Eigen/Dense"

#include <array>
#include <string>
#include <vector>
#include <map>

using namespace Eigen;

/**
 * 我们使用 Object 描述一个三维物体，这有些类似于游戏引擎的 gameobject
 * 但是由于不实现逻辑，所以这个 Object 只描述视觉属性，不描述行为逻辑（component）
 *
 * 尽管理论上 Object 仍然需要与 Device 类似，进行不同格式文件的抽象，但是这个项目进行省略，默认读取 .obj 文件
 */
class Object
{
public:
    struct Vertex
    {
        Vector4d position; // 齐次坐标
        Vector4d normal;   // 法线
        Vector2d texCoord; // 纹理坐标
        Vector3d viewPoisition;

        Vertex() = default;
        Vertex(const Vector4d &pos, const Vector4d &n, const Vector2d &uv)
            : position(pos), normal(n), texCoord(uv) {}
    };

    struct Material
    {
        Vector3d ambient = Vector3d{0.2, 0.2, 0.2};  // 环境光
        Vector3d diffuse = Vector3d{0.8, 0.8, 0.8};  // 漫反射
        Vector3d specular = Vector3d{0.0, 0.0, 0.0}; // 高光
        double shininess = 0.0;                // 高光指数
        std::string diffuseTexture;            // 漫反射贴图路径

        std::string name;

        Material() = default;
    };

    using Triangle = std::array<Vertex, 3>;
    using TrianglesList = std::vector<Triangle>;

private:
    Vector4d position = Vector4d{0.0, 0.0, 0.0, 1.0};
    Vector3d rotation = Vector3d{0.0, 0.0, 0.0}; // 彭德里斯旋转（角度）
    Vector3d scale = Vector3d{1.0, 1.0, 1.0};

    TrianglesList triangles;
    Material material;
    std::string filename;

    void LoadMaterialLibrary(const std::string &mtlPath, std::map<std::string, Material> &materials);

public:
    Object() = default;

    void SetPosition(const Vector4d &newPosition);
    const Vector4d &GetPosition() const;

    void SetRotation(const Vector3d &newRotation);
    const Vector3d &GetRotation() const;

    void SetScale(const Vector3d &newScale);
    const Vector3d &GetScale() const;

    void SetTriangles(const TrianglesList &trianglesList);
    const TrianglesList &GetTrianglesList() const;

    TrianglesList &GetTrianglesList();
    void SetMaterial(const Material &newMaterial);

    const Material &GetMaterial() const;
    Material &GetMaterial();

    void SetFilename(const std::string &name);
    const std::string &GetFilename() const;

    void Init();
    void Clean();
};