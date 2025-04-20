/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#pragma once

#include "Device.hpp"

namespace Drawer
{
    void DrawLine(Vector2i start, Vector2i end, Vector4d color, Device *device);
    void DrawTriangle(Vector2i p1, Vector2i p2, Vector2i p3, Vector4d color, Device *device);
};