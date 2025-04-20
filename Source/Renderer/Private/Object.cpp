/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#include "Object.hpp"

#include <algorithm>
#include <array>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

void Object::SetPosition(const Vector4d &newPosition)
{
    position = newPosition;
}

const Vector4d &Object::GetPosition() const
{
    return position;
}

void Object::SetRotation(const Vector3d &newRotation)
{
    rotation = newRotation;
}

const Vector3d &Object::GetRotation() const
{
    return rotation;
}

void Object::SetScale(const Vector3d &newScale)
{
    scale = newScale;
}

const Vector3d &Object::GetScale() const
{
    return scale;
}

void Object::SetTriangles(const TrianglesList &trianglesList)
{
    triangles = trianglesList;
}

const Object::TrianglesList &Object::GetTrianglesList() const
{
    return triangles;
}

Object::TrianglesList &Object::GetTrianglesList()
{
    return triangles;
}

void Object::SetMaterial(const Material &newMaterial)
{
    material = newMaterial;
}

const Object::Material &Object::GetMaterial() const
{
    return material;
}

Object::Material &Object::GetMaterial()
{
    return material;
}

void Object::SetFilename(const std::string &name)
{
    filename = name;
}

const std::string &Object::GetFilename() const
{
    return filename;
}

void Object::LoadMaterialLibrary(const std::string &mtlPath, std::map<std::string, Material> &materials)
{
    std::ifstream file(mtlPath);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open MTL file: " + mtlPath);
    }

    Material currentMat;
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "newmtl")
        {
            // 保存之前的材质
            if (!currentMat.name.empty())
            {
                materials[currentMat.name] = currentMat;
            }
            currentMat = Material();
            iss >> currentMat.name;
        }
        else if (prefix == "Ka")
        { // 环境光
            iss >> currentMat.ambient[0] >> currentMat.ambient[1] >> currentMat.ambient[2];
        }
        else if (prefix == "Kd")
        { // 漫反射
            iss >> currentMat.diffuse[0] >> currentMat.diffuse[1] >> currentMat.diffuse[2];
        }
        else if (prefix == "Ks")
        { // 高光
            iss >> currentMat.specular[0] >> currentMat.specular[1] >> currentMat.specular[2];
        }
        else if (prefix == "Ns")
        { // 高光指数
            iss >> currentMat.shininess;
        }
        else if (prefix == "map_Kd")
        { // 漫反射贴图
            iss >> currentMat.diffuseTexture;
        }
    }
    // 保存最后一个材质
    if (!currentMat.name.empty())
    {
        materials[currentMat.name] = currentMat;
    }
}

void Object::Init()
{
    if (filename.empty())
    {
        throw std::runtime_error("Object::Init: Filename not set");
    }

    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Object::Init: Failed to open file: " + filename);
    }

    std::vector<Vector4d> positions;
    std::vector<Vector4d> normals;
    std::vector<Vector2d> texCoords;
    std::map<std::string, Material> materials; // 材质库
    std::string currentMaterial;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v")
        { // 顶点位置
            double x, y, z, w = 1.0;
            iss >> x >> y >> z;
            if (iss >> w)
            {
            }
            positions.push_back(Vector4d{x, y, z, w});
        }
        else if (prefix == "vt")
        { // 纹理坐标
            double u, v, w = 0.0;
            iss >> u >> v;
            if (iss >> w)
            {
            }
            texCoords.push_back(Vector2d{u, v});
        }
        else if (prefix == "vn")
        { // 法线
            double x, y, z;
            iss >> x >> y >> z;
            normals.push_back(Vector4d{x, y, z, 0});
        }
        else if (prefix == "f")
        { // 面
            std::array<Vertex, 3> triangle;
            for (int i = 0; i < 3; ++i)
            {
                std::string vertexDesc;
                iss >> vertexDesc;

                // 分割v/vt/vn
                std::replace(vertexDesc.begin(), vertexDesc.end(), '/', ' ');
                std::istringstream vss(vertexDesc);
                int vIdx = -1, vtIdx = -1, vnIdx = -1;
                vss >> vIdx >> vtIdx >> vnIdx;

                // 处理索引（从1开始）
                Vector4d pos = (vIdx > 0) ? positions[vIdx - 1] : Vector4d();
                Vector2d uv = (vtIdx > 0) ? texCoords[vtIdx - 1] : Vector2d();
                Vector4d normal = (vnIdx > 0) ? normals[vnIdx - 1] : Vector4d();

                triangle[i] = Vertex(pos, normal, uv);
            }
            triangles.push_back(triangle);
        }
        else if (prefix == "mtllib")
        { // 材质库
            std::string mtlFile;
            iss >> mtlFile;
            LoadMaterialLibrary(mtlFile, materials);
        }
        else if (prefix == "usemtl")
        { // 使用材质
            iss >> currentMaterial;
            if (materials.find(currentMaterial) != materials.end())
            {
                material = materials[currentMaterial];
            }
        }
    }
}

void Object::Clean()
{
    triangles.clear();
    material = Material();
    filename.clear();
}