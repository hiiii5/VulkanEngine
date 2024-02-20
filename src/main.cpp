#include "Engine.h"

int main()
{
    if (!Engine::GEngine().Init("Vulkan Renderer", 100, 100, 800, 600))
    {
        return -1;
    }
    Engine::GEngine().Start();
    return 0;
}
