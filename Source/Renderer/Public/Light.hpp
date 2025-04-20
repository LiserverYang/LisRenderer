/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#pragma once

#include "Eigen/Dense"

using namespace Eigen;

/**
 * 在光栅化时会用到的点光源，简化版本，用于着色
 */
class Light
{
public:
    Vector4d position;
    double I;

    Light() = default;
    ~Light() = default;
};