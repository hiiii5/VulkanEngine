#include "Engine.h"

int main()
{
    Engine::GEngine().Init("Vulkan Renderer", 100, 100, 800, 600);
    Engine::GEngine().Start();
    return 0;
}
