#include "Window.h"

#include "Renderer.h"

Window::Window(const char* Title, const int X, const int Y, const int Width, const int Height)
{
    window_ = std::make_shared<GWindow>();
    eventResponder_ = std::make_unique<GEventResponder>();
    vulkanSurface_ = std::make_shared<GVulkanSurface>();

    if (window_->Create(X, Y, Width, Height, GWindowStyle::WINDOWEDBORDERED) == GReturn::SUCCESS)
    {
        window_->SetWindowName(Title);
        clrAndDepth_[0].color = { {0.4f, 0.4f, 0.4f, 1} };
        clrAndDepth_[1].depthStencil = { 1.0f, 0u };
        window_->Register(*eventResponder_);

#ifndef NDEBUG
        const char* debugLayers[] = {
            "VK_LAYER_KHRONOS_validation", // standard validation layer
            "VK_LAYER_LUNARG_standard_validation", // add if not on MacOS
            "VK_LAYER_RENDERDOC_Capture" // add this if you have installed RenderDoc
        }; 
        if (+vulkanSurface_->Create(*window_, DEPTH_BUFFER_SUPPORT, std::size(debugLayers), debugLayers, 0, nullptr, 0, nullptr, false))
#else
        if (+vulkanSurface_->Create(*window_, DEPTH_BUFFER_SUPPORT))
#endif
        {
            wasWindowCreated_ = true;
        }

        isRunning_ = true;
    }
}

Window::~Window() = default;

std::shared_ptr<GVulkanSurface> Window::GetVulkanSurface() const
{ return vulkanSurface_; }

std::shared_ptr<GWindow> Window::GetWindow() const
{ return window_; }

bool Window::IsRunning() const
{ return isRunning_; }

void Window::Update()
{
    if (!wasWindowCreated_)
    {
        return;
    }

    if (!isRunning_)
    {
        return;
    }
    
    if (+window_->ProcessWindowEvents())
    {
        if (+vulkanSurface_->StartFrame(2, clrAndDepth_))
        {
            // Render the scene.
            Renderer::GRenderer().Render();
            Renderer::GRenderer().Update();
            vulkanSurface_->EndFrame(true);
        }
    }
}

void Window::Shutdown()
{
    isRunning_ = false;
}
