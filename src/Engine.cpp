#include "Engine.h"

#include "Logger.h"
#include "Renderer.h"
#include "Window.h"
#include "Object.h"

bool Engine::Init(const char* Title, int X, int Y, int Width, int Height)
{
    logger_ = std::make_unique<Logger>();
    window_ = std::make_unique<Window>(Title, X, Y, Width, Height);
    
    // Start the graphics renderer, with 2 frames of buffer.
    if (!Renderer::GRenderer().Init(window_->GetVulkanSurface(), Width, Height, 2))
    {
        VK_LOG(LogCategory::Error, "Failed to initialize the renderer.")
        isRunning_ = false;
        return false;
    }

    return true;
}

void Engine::Start()
{
    isRunning_ = true;
    
    // Kick off the rendering and update loops.
    Update();
}

void Engine::Update()
{
    while (isRunning_)
    {
        // Update the window and all objects with the last render data.
        window_->Update();

        // If the window is not running then do not calculate another render step.
        if (!window_->IsRunning())
        {
            isRunning_ = false;
            break;
        }

        // Renderer::GRenderer().Render();
        // Renderer::GRenderer().Update();
        
        // Get the frame time.
        const float deltaTime = Renderer::GRenderer().GetDeltaTime();

        // Update all objects.
        for (const auto& object : objects_)
        {
            object->Update(deltaTime);
        }
    }
}

Logger* Engine::GetLogger() const
{
    return logger_.get();
}

template <typename T = Object, typename... Args>
std::shared_ptr<T> Engine::AddObject(Args&&... ConstructorArgs)
{
    auto obj = std::make_shared<T>(std::forward<Args>(ConstructorArgs)...);
    objects_.push_back(obj);
    return obj;
}
