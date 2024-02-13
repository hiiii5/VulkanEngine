#pragma once
#include <memory>
#include <vulkan/vulkan_core.h>

#define GATEWARE_ENABLE_CORE // All libraries need this
#define GATEWARE_ENABLE_SYSTEM // Graphics libs require system level libraries
#define GATEWARE_ENABLE_GRAPHICS // Enables all Graphics Libraries

// Ignore some GRAPHICS libraries we aren't going to use
#define GATEWARE_DISABLE_GDIRECTX11SURFACE // we have another template for this
#define GATEWARE_DISABLE_GDIRECTX12SURFACE // we have another template for this
#define GATEWARE_DISABLE_GRASTERSURFACE // we have another template for this
#define GATEWARE_DISABLE_GOPENGLSURFACE // we have another template for this

#include "gateware/Gateware.h"

class Window
{
private:
    std::shared_ptr<GW::SYSTEM::GWindow> window_;
    std::unique_ptr<GW::CORE::GEventResponder> eventResponder_;
    std::shared_ptr<GW::GRAPHICS::GVulkanSurface> vulkanSurface_;

    bool wasWindowCreated_ = false;
    bool isRunning_ = false;

    VkClearValue clrAndDepth_[2];
    
public:
    Window(const char* Title, int X, int Y, int Width, int Height);
    ~Window();

    [[nodiscard]] std::shared_ptr<GW::GRAPHICS::GVulkanSurface> GetVulkanSurface() const;
    [[nodiscard]] std::shared_ptr<GW::SYSTEM::GWindow> GetWindow() const;
    [[nodiscard]] bool IsRunning() const;

    void Update();
    void Shutdown();

    /** Prevent copying to protect the underlying vulkan surface and event responder. */
    Window(const Window& Other) = delete;
    Window(Window&& Other) noexcept = delete;
    Window& operator=(const Window& Other) = delete;
    Window& operator=(Window&& Other) noexcept = delete;
};
