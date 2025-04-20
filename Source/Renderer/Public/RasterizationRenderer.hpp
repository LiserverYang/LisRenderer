/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#pragma once

#include "Renderer.hpp"

/**
 * 光栅化渲染器，经典的渲染流程
 */
class RasterizationRenderer final : public Renderer
{
public:
    virtual void Render() override;

    void Transform();
    void Shading();
};