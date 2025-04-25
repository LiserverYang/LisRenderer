/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#include "RasterizationRenderer.hpp"
#include "Arguments.hpp"
#include "DeviceFactory.hpp"
#include "Drawer.hpp"

#include <iostream>
#include <unordered_map>

const double PI = 3.1415926;

struct TriangleInformation
{
    int triangleIndex;
    int objectIndex;
    bool saved = 0;

    TriangleInformation() = default;
    TriangleInformation(int _tindex, int _oindex) : triangleIndex(_tindex), objectIndex(_oindex), saved(1) {}
};

std::array<std::array<TriangleInformation, DEFAULT_WIDTH>, DEFAULT_HEIGHT> refMap;
std::unique_ptr<Device> zBufferDevice;

void RasterizationRenderer::Render()
{
    Transform();
    Shading();

    // for (auto &object : scene->GetObjects())
    // {
    //     for (auto &trangle : object.GetTrianglesList())
    //     {
    //         Drawer::DrawLine((trangle[0].position.head<2>().array().round()).cast<int>(), (trangle[1].position.head<2>().array().round()).cast<int>(), {1.0, 1.0, 1.0, 1.0}, device.get());
    //         Drawer::DrawLine((trangle[0].position.head<2>().array().round()).cast<int>(), (trangle[2].position.head<2>().array().round()).cast<int>(), {1.0, 1.0, 1.0, 1.0}, device.get());
    //         Drawer::DrawLine((trangle[1].position.head<2>().array().round()).cast<int>(), (trangle[2].position.head<2>().array().round()).cast<int>(), {1.0, 1.0, 1.0, 1.0}, device.get());
    //     }
    // }

    device->Push();
}

void RasterizationRenderer::Transform()
{
    // 投影矩阵参数
    double fovRad = fovY * PI / 180.0;
    double tanHalfFov = tan(fovRad / 2.0);
    double aspect = ASPECT;

    // 观测变换
    const auto &camera = scene->GetCamera();
    Vector3d gaze = camera.gaezeDirection.head<3>().normalized();
    Vector3d up = camera.upDirection.head<3>().normalized();
    Vector3d eye = camera.position.head<3>();

    // 计算视图矩阵
    Vector3d right = up.cross(gaze).normalized();
    Vector3d realUp = gaze.cross(right);

    Matrix4d view = Matrix4d::Identity();
    view << right[0], right[1], right[2], 0,
        realUp[0], realUp[1], realUp[2], 0,
        -gaze[0], -gaze[1], -gaze[2], 0,
        -right.dot(eye), -realUp.dot(eye), gaze.dot(eye), 1;

    // 透视投影矩阵
    Matrix4d projection;
    projection << 1.0 / (aspect * tanHalfFov), 0, 0, 0,
        0, 1.0 / tanHalfFov, 0, 0,
        0, 0, -(zFar + zNear) / (zFar - zNear), -2 * zFar * zNear / (zFar - zNear),
        0, 0, -1, 0;

    // 视口矩阵（Y轴翻转）
    Matrix4d viewport;
    viewport << (DEFAULT_WIDTH - 1) / 2.0, 0, 0, (DEFAULT_WIDTH - 1) / 2.0,
        0, -(DEFAULT_HEIGHT - 1) / 2.0, 0, (DEFAULT_HEIGHT - 1) / 2.0,
        0, 0, 1, 0,
        0, 0, 0, 1;

    auto &lights = scene->GetLights();
    auto &objects = scene->GetObjects();

    for (auto &object : objects)
    {
        // 获取模型变换参数
        const Vector3d &scale = object.GetScale();
        const Vector3d &rotation = object.GetRotation();
        const Vector4d &position = object.GetPosition();

        // 构建模型矩阵（按 SRT 顺序）
        Matrix4d modelMatrix = Matrix4d::Identity();

        // 缩放
        modelMatrix(0, 0) = scale[0];
        modelMatrix(1, 1) = scale[1];
        modelMatrix(2, 2) = scale[2];

        // 旋转（Z-Y-X顺序）
        Matrix4d rotX = Matrix4d::Identity();
        rotX(1, 1) = cos(rotation[0]);
        rotX(1, 2) = -sin(rotation[0]);
        rotX(2, 1) = sin(rotation[0]);
        rotX(2, 2) = cos(rotation[0]);

        Matrix4d rotY = Matrix4d::Identity();
        rotY(0, 0) = cos(rotation[1]);
        rotY(0, 2) = sin(rotation[1]);
        rotY(2, 0) = -sin(rotation[1]);
        rotY(2, 2) = cos(rotation[1]);

        Matrix4d rotZ = Matrix4d::Identity();
        rotZ(0, 0) = cos(rotation[2]);
        rotZ(0, 1) = -sin(rotation[2]);
        rotZ(1, 0) = sin(rotation[2]);
        rotZ(1, 1) = cos(rotation[2]);

        modelMatrix = rotZ * rotY * rotX * modelMatrix;

        // 平移
        modelMatrix(0, 3) = position[0];
        modelMatrix(1, 3) = position[1];
        modelMatrix(2, 3) = position[2];

        // 构建法线矩阵（模型矩阵的逆转置）
        Matrix4d normalMatrix = modelMatrix.inverse().transpose();

        // 整体变换矩阵
        Matrix4d mvp = viewport * projection * view * modelMatrix;

        // 变换所有顶点
        for (auto &triangle : object.GetTrianglesList())
        {
            for (auto &vertex : triangle)
            {
                // 位置变换
                Vector4d pos = mvp * vertex.position;
                pos /= pos.w(); // 透视除法

                vertex.viewPoisition = (view * modelMatrix * vertex.position).head<3>();

                // 更新顶点数据
                vertex.position = pos;
                vertex.normal = (normalMatrix * vertex.normal).normalized();
            }
        }
    }

    for (auto &light : lights)
    {
        light.position = view * light.position;
    }
}

inline int sgn(double a)
{
    if (a > 0)
    {
        return 1;
    }
    else if (a < 0)
    {
        return -1;
    }
    return 0;
}

void RasterizationRenderer::Shading()
{
    zBufferDevice = DeviceFactory::CreateDeviceFromString(arguments->deviceType, arguments->zBufferFilePath);
    zBufferDevice->Init();

    // 深度缓冲，获得所有像素所在的三角形

    Vector4d defaultColor{0xFFFFF, 0xFFFFF, 0xFFFFF, 1.0};

    for (int i = 0; i < DEFAULT_WIDTH; i++)
    {
        for (int j = 0; j < DEFAULT_HEIGHT; j++)
        {
            zBufferDevice->SetPixel(i, j, defaultColor);
        }
    }

    const auto &objects = scene->GetObjects();

    for (auto objIndex = 0; objIndex < objects.size(); objIndex++)
    {
        const auto &triangleList = objects[objIndex].GetTrianglesList();

        for (auto i = 0; i < triangleList.size(); i++)
        {
            const auto triangle = triangleList[i];

            // AABB Box
            int xMin = std::min(std::min(triangle[0].position[0], triangle[1].position[0]), triangle[2].position[0]);
            int xMax = std::max(std::max(triangle[0].position[0], triangle[1].position[0]), triangle[2].position[0]);

            int yMin = std::min(std::min(triangle[0].position[1], triangle[1].position[1]), triangle[2].position[1]);
            int yMax = std::max(std::max(triangle[0].position[1], triangle[1].position[1]), triangle[2].position[1]);

            // 遍历所有像素
            for (int x = xMin; x <= xMax; x++)
            {
                for (int y = yMin; y <= yMax; y++)
                {
                    // 检测是否在三角形内
                    const Vector3d P{x * 1.0, y * 1.0, 0};
                    const Vector3d AP = -P + triangle[0].position.head<3>();
                    const Vector3d BP = -P + triangle[1].position.head<3>();
                    const Vector3d CP = -P + triangle[2].position.head<3>();
                    const Vector3d AB = triangle[0].position.head<3>() - triangle[1].position.head<3>();
                    const Vector3d BC = triangle[1].position.head<3>() - triangle[2].position.head<3>();
                    const Vector3d CA = triangle[2].position.head<3>() - triangle[0].position.head<3>();

                    if (sgn(AB.cross(AP)[2]) == sgn(BC.cross(BP)[2]) && sgn(BC.cross(BP)[2]) == sgn(CA.cross(CP)[2]))
                    {
                        // 在三角形内，首先通过重心坐标求像素深度

                        const double xA = triangle[0].position[0];
                        const double xB = triangle[1].position[0];
                        const double xC = triangle[2].position[0];

                        const double yA = triangle[0].position[1];
                        const double yB = triangle[1].position[1];
                        const double yC = triangle[2].position[1];

                        const double beta = ((y - yA) * (xC - xA) - (x - xA) * (yC - yA)) / ((yB - yA) * (xC - xA) - (xB - xA) * (yC - yA));
                        const double gamma = ((y - yA) * (xB - xA) - (x - xA) * (yB - yA)) / ((yC - yA) * (xB - xA) - (xC - xA) * (yB - yA));
                        const double alpha = 1 - beta - gamma;

                        const double z = triangle[0].position[2] * alpha + triangle[1].position[2] * beta + triangle[2].position[2] * gamma;

                        if (z <= zBufferDevice->GetPixel(x, y)[0])
                        {
                            // 更新
                            refMap[y][x] = TriangleInformation{i, objIndex};

                            Vector4d newColor{z, z, z, 1.0};
                            zBufferDevice->SetPixel(x, y, newColor);
                        }
                    }
                }
            }
        }
    }

    // 现在可以针对每个像素进行着色
    // 遍历所有的像素
    for (int x = 0; x < DEFAULT_WIDTH; x++)
    {
        for (int y = 0; y < DEFAULT_HEIGHT; y++)
        {
            if (!refMap[y][x].saved)
            {
                continue;
            }

            const TriangleInformation information = refMap[y][x];
            const auto &objects = scene->GetObjects();
            const Object::Triangle &triangle = objects[information.objectIndex].GetTrianglesList()[information.triangleIndex];

            Vector3d color{0.0, 0.0, 0.0};

            // 针对每一个光源进行着色
            for (auto &light : scene->GetLights())
            {
                const Vector3d APosition = triangle[0].viewPoisition;
                const Vector3d BPosition = triangle[1].viewPoisition;
                const Vector3d CPosition = triangle[2].viewPoisition;

                // 根据 Blinn-Phong模型着色
                const double xA = triangle[0].position[0];
                const double xB = triangle[1].position[0];
                const double xC = triangle[2].position[0];

                const double yA = triangle[0].position[1];
                const double yB = triangle[1].position[1];
                const double yC = triangle[2].position[1];

                const double beta = ((y - yA) * (xC - xA) - (x - xA) * (yC - yA)) / ((yB - yA) * (xC - xA) - (xB - xA) * (yC - yA));
                const double gamma = ((y - yA) * (xB - xA) - (x - xA) * (yB - yA)) / ((yC - yA) * (xB - xA) - (xC - xA) * (yB - yA));
                const double alpha = 1 - beta - gamma;

                // 通过插值计算出法线方向和着色点
                const Vector3d normal{(triangle[0].normal * alpha + triangle[1].normal * beta + triangle[2].normal * gamma).head<3>().normalized()};
                const Vector3d position{(APosition * alpha + BPosition * beta + CPosition * gamma).head<3>()};
                const Vector3d lightDirection{(-position + light.position.head<3>()).normalized()};
                const Vector3d eyeDirection{(-position + scene->GetCamera().position.head<3>()).normalized()};
                const Vector3d half = (lightDirection + eyeDirection).normalized();

                const double distance = (-light.position.head<3>() + position).norm();

                // 漫反射
                color += objects[information.objectIndex].GetMaterial().diffuse * (light.I / (distance * distance)) * std::max(lightDirection.dot(normal), 0.0);
                // 高光
                color += objects[information.objectIndex].GetMaterial().specular * (light.I / (distance * distance)) * std::pow(normal.dot(half), objects[information.objectIndex].GetMaterial().shininess);
            }

            // 环境光照
            color += objects[information.objectIndex].GetMaterial().ambient * 0.1;

            Vector4d pixelColor{color[0], color[1], color[2], 1.0};
            device->SetPixel(x, y, pixelColor);

            Vector4d recColor{1.0, 0.0, 0.0, 1.0};
        }
    }

    zBufferDevice->Push();
    zBufferDevice->Clean();
    zBufferDevice.reset();
}