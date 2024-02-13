#pragma once

#include <filesystem>
#include <fstream>

class Filesystem
{
public:
    static std::string GetCurrentDirectory()
    {
        return std::filesystem::current_path().string();
    }

    static std::string GetParentDirectory(const std::string& Path)
    {
        return std::filesystem::path(Path).parent_path().string();
    }

    static std::string GetAbsolutePath(const std::string& Path)
    {
        return std::filesystem::absolute(Path).string();
    }

    static std::string GetShadersPath()
    {
        const std::string currentDirectory = GetCurrentDirectory();
        const std::string absolutePath = GetAbsolutePath(currentDirectory);
        return GetAbsolutePath(absolutePath + "\\shaders");
    }

    static std::string ReadFile(const std::string& Path)
    {
        if (!std::filesystem::exists(Path))
        {
            return "";
        }

        if (!std::filesystem::is_regular_file(Path))
        {
            return "";
        }

        // Read the file using a stream iterator.
        std::ifstream fileStream(Path.c_str());
        std::string fileContents((std::istreambuf_iterator(fileStream)), std::istreambuf_iterator<char>());
        fileStream.close();

        return fileContents;
    }
};
