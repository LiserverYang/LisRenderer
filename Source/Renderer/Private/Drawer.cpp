/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#include "Drawer.hpp"

void Drawer::DrawLine(Vector2i start, Vector2i end, Vector4d color, Device *device)
{
    int x0 = start[0], y0 = start[1];
    int x1 = end[0], y1 = end[1];

    // 计算实际增量和步进方向
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    // 判断是否陡峭线段（斜率绝对值 > 1）
    bool steep = dy > dx;
    if (steep)
    {
        std::swap(dx, dy); // 交换dx/dy角色
        std::swap(x0, y0); // 同步交换坐标
        std::swap(x1, y1);
        std::swap(sx, sy); // 同步交换步进方向
    }

    // 初始化误差项
    int err = 2 * dy - dx;
    int currentX = x0;
    int currentY = y0;

    // 主绘制循环
    for (int i = 0; i <= dx; ++i)
    {
        // 根据陡峭标志交换坐标输出
        if (steep)
        {
            device->SetPixel(currentY, currentX, color);
        }
        else
        {
            device->SetPixel(currentX, currentY, color);
        }

        // 更新误差项
        if (err >= 0)
        {
            currentY += sy;
            err -= 2 * dx;
        }

        // 总是沿主方向步进
        currentX += sx;
        err += 2 * dy;
    }
}

void Drawer::DrawTriangle(Vector2i p1, Vector2i p2, Vector2i p3, Vector4d color, Device *device)
{
    DrawLine(p1, p2, color, device);
    DrawLine(p1, p3, color, device);
    DrawLine(p2, p3, color, device);
}
