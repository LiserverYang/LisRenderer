/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#pragma once

#include "Eigen/Dense"

/**
 * Camera 是图形学意义上的摄像机
 */
class Camera
{
public:
    Vector4d position;
    Vector4d gaezeDirection;
    Vector4d upDirection;

    Camera() = default;
    ~Camera() = default;
};