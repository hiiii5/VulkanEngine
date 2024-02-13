#pragma once

#include <string>

#include "gateware/Gateware.h"
#include "Engine.h"
#include "Filesystem.h"

class Shader
{
private:
    std::string vertexShader_;
    std::string fragmentShader_;
    unsigned long long shaderId_;

public:
    Shader() = default;
    Shader(std::string VertexShader, std::string FragmentShader, const unsigned long long ShaderId) :
        vertexShader_(std::move(VertexShader)),
        fragmentShader_(std::move(FragmentShader)),
        shaderId_(ShaderId)
    {
    }

    static std::string ReadFile(const std::string& FilePath)
    {
        // std::string output;
        // unsigned int stringLength = 0;
        // GW::SYSTEM::GFile file;
        // file.Create();
        // file.GetFileSize(FilePath.c_str(), stringLength);
        // if (stringLength && +file.OpenBinaryRead(FilePath.c_str()))
        // {
        //     output.resize(stringLength);
        //     file.Read(output.data(), stringLength);
        // }
        // else
        // {
        //     VK_LOG(LogCategory::Error, "Failed to read file: " + FilePath)
        // }

        std::string output = Filesystem::ReadFile(FilePath);
        if (output.empty())
        {
            VK_LOG(LogCategory::Error, "Failed to read file: " + FilePath);
        }

        return output;
    }
};
