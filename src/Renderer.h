#pragma once
#include <chrono>
#include <memory>
#include <vector>

// Simple basecode showing how to create a window and attatch a vulkansurface
#define GATEWARE_ENABLE_CORE // All libraries need this
#define GATEWARE_ENABLE_SYSTEM // Graphics libs require system level libraries
#define GATEWARE_ENABLE_GRAPHICS // Enables all Graphics Libraries
#define GATEWARE_ENABLE_MATH
#define GATEWARE_ENABLE_INPUT
// Ignore some GRAPHICS libraries we aren't going to use
#define GATEWARE_DISABLE_GDIRECTX11SURFACE // we have another template for this
#define GATEWARE_DISABLE_GDIRECTX12SURFACE // we have another template for this
#define GATEWARE_DISABLE_GRASTERSURFACE // we have another template for this
#define GATEWARE_DISABLE_GOPENGLSURFACE // we have another template for this

#include "gateware/Gateware.h"
#include "Renderable.h"

class Shader;
using namespace GW;
using namespace CORE;
using namespace SYSTEM;
using namespace GRAPHICS;

class Renderer final
{
public:
    static Renderer& GRenderer()
    {
        static Renderer inst;
        return inst;
    }
    Renderer(const Renderer& Other) = delete;
    Renderer(Renderer&& Other) noexcept = delete;
    Renderer& operator=(const Renderer& Other) = delete;
    Renderer& operator=(Renderer&& Other) noexcept = delete;
    
private:
    Renderer(){} // Private constructor for singleton
    ~Renderer() = default; // Private destructor for singleton
    
private:
    /** The render surface for the window, using Vulkan. */
    std::shared_ptr<GVulkanSurface> vulkanSurface_;
    /** Will receive shutdown events from the window. */
    GEventReceiver shutdown_;

   // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   // Memory Management
   // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    /** This is a handle to the memory on the GPU. */
    std::vector<VkBuffer> storageHandle_;
    /** This is the actual data on the GPU. */
    std::vector<VkDeviceMemory> storageData_;

    // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    // Pipeline
    // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    /** The pipeline to use for rendering. */
    VkPipeline pipeline_ = nullptr;
    /** The layout of the pipeline. */
    VkPipelineLayout pipelineLayout_ = nullptr;

    // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    // Descriptors
    // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    /** The layout of the descriptor. */
    VkDescriptorSetLayout descriptorLayout_ = nullptr;
    /** The pool of descriptors. */
    VkDescriptorPool descriptorPool_ = nullptr;
    /** The descriptor set. */
    std::vector<VkDescriptorSet> descriptorSet_;

    // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    // Delta Time
    // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    /** Time point for determining delta time between frames. */
    std::chrono::time_point<std::chrono::steady_clock> start_, stop_;
    float deltaTime_ = 0.0f;

    // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    // Devices
    // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    /** GPU device, this is a link to the vulkan instance. */
    VkDevice device_ = nullptr;
    /** GPU physical device, this is a link to the physical gpu. */
    VkPhysicalDevice physicalDevice_ = nullptr;

    // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    // Renderables
    // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    /** The renderable objects to render. */
    std::vector<std::shared_ptr<Renderable>> renderables_;
    /** The shaders to use for rendering. [shaderId](vert, frag) */
    std::vector<std::tuple<VkShaderModule, VkShaderModule>> shaders_;
    /** The current instance data. */
    ShaderModelData instanceData_;
    
    /** Creates the graphics pipeline. */
    void InitPipeline(unsigned Width, unsigned Height, unsigned MaxFrames);
    /** Cleans up the graphics pipeline. */
    void CleanUp() const;
    
public:
    void Init(const std::shared_ptr<GVulkanSurface>& VulkanSurface, unsigned Width, unsigned Height, unsigned MaxFrames);

    /** Binds a renderable to the renderer. */
    void BindRenderable(Renderable* NewRenderable);
    /** Compiles a shader and adds it to the known shaders. */
    void CompileShader(const std::string& VsFilePath, const std::string& PsFilePath, Shader& NewShader);
    
    /** Pushes the updates to the storage buffers. */
    void UpdateStorageBuffers(VkPhysicalDevice PhysicalDevice, unsigned& MaxFrames, bool Create);
    /** Used to get the current command buffers, instead of using this internally it is expected that the buffer is provided to other methods. */
    void GetCurrentBuffers(unsigned& CurrentBuffer, VkCommandBuffer& CommandBuffer) const;

    /** Updates the renderer. */
    void Render();
    /** Updates the renderer. */
    void Update();
    /** Gets the delta time between frames. */
    float GetDeltaTime() const;

};
