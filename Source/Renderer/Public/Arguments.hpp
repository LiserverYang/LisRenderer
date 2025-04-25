/**
 * Copyrigt 2025, LiserverYang. All rights reserved.
 */

#pragma once

#include <string>

/**
 * 存放所有渲染器用到的参数
 */
class Arguments
{
public:
    std::string sceneFilePath;
    std::string deviceType;
    std::string outputFilePath;
    std::string zBufferFilePath;
};