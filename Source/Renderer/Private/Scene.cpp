#include "Scene.hpp"
#include "Eigen/Dense"

#include <fstream>
#include <filesystem>

using namespace Eigen;

void Scene::SetCamera(Camera &_camera)
{
    camera = _camera;
}

void Scene::SetObjects(Scene::ObjectList &objectList)
{
    objects = objectList;
}

void Scene::SetLights(Scene::LightList &lightList)
{
    lights = lightList;
}

Camera &Scene::GetCamera()
{
    return camera;
}

Scene::ObjectList &Scene::GetObjects()
{
    return objects;
}

Scene::LightList &Scene::GetLights()
{
    return lights;
}

Scene::Scene(std::string filePath)
{
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        throw std::runtime_error("Can not read scene file");
    }
    
    std::string line;
    
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "#")
        {
            continue;
        }

        // 物体
        if (prefix == "o")
        {
            Object obj;

            Vector4d position{0.0, 0.0, 0.0, 1.0};
            Vector3d rotation;
            Vector3d scale;

            std::string objFilePath;

            iss >> objFilePath
                >> position[0] >> position[1] >> position[2]
                >> scale[0] >> scale[1] >> scale[2]
                >> rotation[0] >> rotation[1] >> rotation[2];

            obj.SetFilename((std::filesystem::path(filePath).parent_path() / objFilePath).string());
            obj.SetPosition(position);
            obj.SetRotation(rotation);
            obj.SetScale(scale);

            obj.Init();

            objects.push_back(obj);
        }

        if (prefix == "l")
        {
            Light light;

            Vector4d position{0.0, 0.0, 0.0, 1.0};
            double I;

            iss >> position[0] >> position[1] >> position[2] >> I;

            light.I = I;
            light.position = position;

            lights.push_back(light);
        }

        if (prefix == "c")
        {
            Camera camera1;

            Vector4d position{0.0, 0.0, 0.0, 1.0};
            Vector4d gazeDirection{0.0, 0.0, 0.0, 0.0};
            Vector4d upDirection{0.0, 0.0, 0.0, 0.0};

            iss >> position[0] >> position[1] >> position[2]
                >> gazeDirection[0] >> gazeDirection[1] >> gazeDirection[2]
                >> upDirection[0] >> upDirection[1] >> upDirection[2];

            camera1.position = position;
            camera1.gaezeDirection = gazeDirection;
            camera1.upDirection = upDirection;

            this->camera = camera1;
        }
    }
}