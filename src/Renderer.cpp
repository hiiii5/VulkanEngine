#include "Renderer.h"

#include "Engine.h"
#include "Filesystem.h"
#include "Renderable.h"
#include "Shader.h"
#include "shaderc/shaderc.h"
#ifdef _WIN32
#pragma comment(lib, "shaderc_combined.lib")
#endif

void Renderer::InitPipeline(unsigned Width, unsigned Height, unsigned MaxFrames)
{
    VkRenderPass renderPass;
    vulkanSurface_->GetRenderPass(reinterpret_cast<void**>(&renderPass));

    // We need to create a pipeline with a vertex and fragment shader.
    VkPipelineShaderStageCreateInfo shaderStages[2] = {};

    // Vertex shader
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    // TODO: DR HIGH Implement a default shader.
    shaderStages[0].module = std::get<0>(shaders_[0]);
    shaderStages[0].pName = "main";

    // Fragment shader
    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    // TODO: DR HIGH Implement a default shader.
    shaderStages[1].module = std::get<1>(shaders_[0]);
    shaderStages[1].pName = "main";

    // Assembly state, this is the type of geometry we are drawing. (triangles, lines, points)
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
    inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyState.primitiveRestartEnable = false;

    // Vertex input state
    VkVertexInputBindingDescription vertexInputBinding = {};
    vertexInputBinding.binding = 0;
    // TODO: DR HIGH Implement size of a vertex.
    vertexInputBinding.stride = sizeof(Vertex);
    vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    // Attributes describe shader input variables, which we will use to describe vertex data.
    VkVertexInputAttributeDescription vertexInputAttributes[3] = {
        {0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0}, // UV, Normal, etc.
        {1, 0, VK_FORMAT_R32G32B32_SFLOAT, 12},
        {2, 0, VK_FORMAT_R32G32B32_SFLOAT, 24}
    };

    VkPipelineVertexInputStateCreateInfo vertexInputState = {};
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputState.vertexBindingDescriptionCount = 1;
    vertexInputState.pVertexBindingDescriptions = &vertexInputBinding;
    vertexInputState.vertexAttributeDescriptionCount = 3;
    vertexInputState.pVertexAttributeDescriptions = vertexInputAttributes;

    // Viewport state (we still need to set this up even though we will overwrite the values)
    VkViewport viewport = {
        0, 0, static_cast<float>(Width), static_cast<float>(Height), 0, 1
    };

    VkRect2D scissor = {
        {0, 0}, {Width, Height}
    };
    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    // Rasterizer state
    VkPipelineRasterizationStateCreateInfo rasterizationState = {};
    rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizationState.rasterizerDiscardEnable = VK_FALSE;
    rasterizationState.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizationState.lineWidth = 1.0f;
    rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizationState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizationState.depthClampEnable = VK_FALSE;
    rasterizationState.depthBiasEnable = VK_FALSE;
    rasterizationState.depthBiasClamp = 0.0f;
    rasterizationState.depthBiasConstantFactor = 0.0f;
    rasterizationState.depthBiasSlopeFactor = 0.0f;

    // Multisampling state
    VkPipelineMultisampleStateCreateInfo multisampleState = {};
    multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampleState.sampleShadingEnable = VK_FALSE;
    multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampleState.minSampleShading = 1.0f;
    multisampleState.pSampleMask = VK_NULL_HANDLE;
    multisampleState.alphaToCoverageEnable = VK_FALSE;
    multisampleState.alphaToOneEnable = VK_FALSE;

    // Depth and stencil state
    VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
    depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencilState.depthTestEnable = VK_TRUE;
    depthStencilState.depthWriteEnable = VK_TRUE;
    depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencilState.depthBoundsTestEnable = VK_FALSE;
    depthStencilState.minDepthBounds = 0.0f;
    depthStencilState.maxDepthBounds = 1.0f;
    depthStencilState.stencilTestEnable = VK_FALSE;

    // Color blend state
    VkPipelineColorBlendAttachmentState colorBlendAttachmentState = {};
    colorBlendAttachmentState.colorWriteMask = 0xf;
    colorBlendAttachmentState.blendEnable = VK_FALSE;
    colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

    // Pipeline color blend state create info
    VkPipelineColorBlendStateCreateInfo colorBlendState = {};
    colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendState.logicOpEnable = VK_FALSE;
    colorBlendState.logicOp = VK_LOGIC_OP_COPY;
    colorBlendState.attachmentCount = 1;
    colorBlendState.pAttachments = &colorBlendAttachmentState;
    colorBlendState.blendConstants[0] = 0.0f;
    colorBlendState.blendConstants[1] = 0.0f;
    colorBlendState.blendConstants[2] = 0.0f;
    colorBlendState.blendConstants[3] = 0.0f;

    // Dynamic state
    VkDynamicState dynamicState[2] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
    dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicStateCreateInfo.dynamicStateCount = 2;
    dynamicStateCreateInfo.pDynamicStates = dynamicState;

    VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
    descriptorSetLayoutBinding.binding = 0;
    descriptorSetLayoutBinding.descriptorCount = 1;
    descriptorSetLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    descriptorSetLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    descriptorSetLayoutBinding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
    descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.flags = 0;
    descriptorSetLayoutCreateInfo.bindingCount = 1;
    descriptorSetLayoutCreateInfo.pBindings = &descriptorSetLayoutBinding;
    descriptorSetLayoutCreateInfo.pNext = nullptr;
    vkCreateDescriptorSetLayout(device_, &descriptorSetLayoutCreateInfo, nullptr, &descriptorLayout_);

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    VkDescriptorPoolSize poolSize{VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, MaxFrames};
    descriptorPoolCreateInfo.poolSizeCount = 1;
    descriptorPoolCreateInfo.pPoolSizes = &poolSize;
    descriptorPoolCreateInfo.maxSets = MaxFrames;
    descriptorPoolCreateInfo.flags = 0;
    descriptorPoolCreateInfo.pNext = nullptr;
    vkCreateDescriptorPool(device_, &descriptorPoolCreateInfo, nullptr, &descriptorPool_);

    VkDescriptorSetAllocateInfo allocateInfo = {};
    allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocateInfo.descriptorSetCount = 1;
    allocateInfo.pSetLayouts = &descriptorLayout_;
    allocateInfo.descriptorPool = descriptorPool_;
    allocateInfo.pNext = nullptr;
    descriptorSet_.resize(MaxFrames);
    for (unsigned int i = 0; i < MaxFrames; ++i)
    {
        vkAllocateDescriptorSets(device_, &allocateInfo, &descriptorSet_[i]);
    }

    VkWriteDescriptorSet writeDescriptorSet = {};
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.descriptorCount = 1;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.dstBinding = 0;
    writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    for (unsigned int i = 0; i < MaxFrames; ++i)
    {
        writeDescriptorSet.dstSet = descriptorSet_[i];
        VkDescriptorBufferInfo bufferInfo{storageHandle_[i], 0, VK_WHOLE_SIZE};
        writeDescriptorSet.pBufferInfo = &bufferInfo;
        vkUpdateDescriptorSets(device_, 1, &writeDescriptorSet, 0, nullptr);
    }

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
    pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutCreateInfo.setLayoutCount = 1;
    pipelineLayoutCreateInfo.pSetLayouts = &descriptorLayout_;

    VkPushConstantRange pushConstantRange = {VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(unsigned int)};
    pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
    pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;
    vkCreatePipelineLayout(device_, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout_);

    // Pipeline state... (Finally)
    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = 2;
    pipelineCreateInfo.pStages = shaderStages;
    pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
    pipelineCreateInfo.pVertexInputState = &vertexInputState;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pRasterizationState = &rasterizationState;
    pipelineCreateInfo.pMultisampleState = &multisampleState;
    pipelineCreateInfo.pDepthStencilState = &depthStencilState;
    pipelineCreateInfo.pColorBlendState = &colorBlendState;
    pipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;
    pipelineCreateInfo.layout = pipelineLayout_;
    pipelineCreateInfo.renderPass = renderPass;
    pipelineCreateInfo.subpass = 0;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    vkCreateGraphicsPipelines(device_, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &pipeline_);
}

void Renderer::CleanUp() const
{
    // Wait until everything has completed.
    vkDeviceWaitIdle(device_);

    // Inform the window that we are shutting down.
    Engine::GEngine().GetWindow()->Shutdown();
    
    // Release allocated buffers, shaders, and pipeline.
    for (const VkBuffer buffer : storageHandle_)
    {
        vkDestroyBuffer(device_, buffer, nullptr);
    }
    for (const std::tuple<VkShaderModule, VkShaderModule>& shader : shaders_)
    {
        vkDestroyShaderModule(device_, std::get<0>(shader), nullptr);
        vkDestroyShaderModule(device_, std::get<1>(shader), nullptr);
    }
    for (const VkDeviceMemory handle : storageData_)
    {
        vkFreeMemory(device_, handle, nullptr);
    }

    vkDestroyDescriptorSetLayout(device_, descriptorLayout_, nullptr);
    vkDestroyDescriptorPool(device_, descriptorPool_, nullptr);
    vkDestroyPipelineLayout(device_, pipelineLayout_, nullptr);
    vkDestroyPipeline(device_, pipeline_, nullptr);
}

void Renderer::Init(const std::shared_ptr<GVulkanSurface>& VulkanSurface, const unsigned Width, const unsigned Height, unsigned MaxFrames)
{
    vulkanSurface_ = VulkanSurface;
    renderables_ = std::vector<std::shared_ptr<Renderable>>();
    
    /***************** GEOMETRY INTIALIZATION ******************/
    // Grab the device & physical device so we can allocate some stuff
    vulkanSurface_->GetPhysicalDevice(reinterpret_cast<void**>(&physicalDevice_));
    vulkanSurface_->GetDevice(reinterpret_cast<void**>(&device_));
    
    UpdateStorageBuffers(physicalDevice_, MaxFrames, true);

    auto defaultShader = Shader{};
    const std::string shadersPath = Filesystem::GetShadersPath();
    CompileShader(shadersPath + "\\vs.hlsl", shadersPath + "\\ps.hlsl", defaultShader);
    InitPipeline(Width, Height, MaxFrames);

    shutdown_.Create(*vulkanSurface_, [&]
    {
        if (+shutdown_.Find(GVulkanSurface::Events::RELEASE_RESOURCES, true))
        {
            CleanUp();
        }
    });
}

void Renderer::BindRenderable(Renderable* NewRenderable)
{
    renderables_.push_back(std::shared_ptr<Renderable>(NewRenderable));
}

void Renderer::CompileShader(const std::string& VsFilePath, const std::string& PsFilePath, Shader& NewShader)
{
    const shaderc_compiler_t compiler = shaderc_compiler_initialize();
    const shaderc_compile_options_t options = shaderc_compile_options_initialize();
    shaderc_compile_options_set_source_language(options, shaderc_source_language_hlsl);
    shaderc_compile_options_set_invert_y(options, false);

#ifndef NDEBUG
    shaderc_compile_options_set_generate_debug_info(options);
#endif

    // Create the vertex shader.
    const std::string vsShader = Shader::ReadFile(VsFilePath);
    const shaderc_compilation_result_t vsResult = shaderc_compile_into_spv(compiler, vsShader.c_str(), strlen(vsShader.c_str()), shaderc_vertex_shader, "main.vert", "main", options);
    if (shaderc_result_get_compilation_status(vsResult) != shaderc_compilation_status_success)
    {
        VK_LOG(LogCategory::Error, "Failed to compile vertex shader: " + VsFilePath)
        return;
    }
    VkShaderModule vertexShader;
    GvkHelper::create_shader_module(device_, shaderc_result_get_length(vsResult), const_cast<char*>(shaderc_result_get_bytes(vsResult)), &vertexShader);
    shaderc_result_release(vsResult);

    // Create the pixel shader.
    const std::string psShader = Shader::ReadFile(PsFilePath);
    const shaderc_compilation_result_t psResult = shaderc_compile_into_spv(compiler, psShader.c_str(), strlen(psShader.c_str()), shaderc_fragment_shader, "main.frag", "main", options);
    if (shaderc_result_get_compilation_status(psResult) != shaderc_compilation_status_success)
    {
        VK_LOG(LogCategory::Error, "Failed to compile pixel shader: " + PsFilePath)
        return;
    }
    VkShaderModule pixelShader;
    GvkHelper::create_shader_module(device_, shaderc_result_get_length(psResult), const_cast<char*>(shaderc_result_get_bytes(psResult)), &pixelShader);
    shaderc_result_release(psResult);
    
    // Free runtime shader compiler resources
    shaderc_compile_options_release(options);
    shaderc_compiler_release(compiler);

    // Add the shader to the known shaders.
    shaders_.emplace_back(vertexShader, pixelShader);
    NewShader = Shader(VsFilePath, PsFilePath, shaders_.size() - 1);
}

void Renderer::UpdateStorageBuffers(VkPhysicalDevice PhysicalDevice, unsigned& MaxFrames, bool Create)
{
    vulkanSurface_->GetSwapchainImageCount(MaxFrames);

    if (storageHandle_.size() < MaxFrames)
    {
        storageHandle_.resize(MaxFrames);
    }
    if (storageData_.size() < MaxFrames)
    {
        storageData_.resize(MaxFrames);
    }

    // For every frame, make a matching buffer to hold initial date for the shaders.
    for (unsigned int i = 0; i < MaxFrames; i++)
    {
        if (Create)
        {
            GvkHelper::create_buffer(PhysicalDevice, device_, sizeof(ShaderModelData),
                                 VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &storageHandle_[i], &storageData_[i]);
        }
  
		GvkHelper::write_to_buffer(device_, storageData_[i], &instanceData_, sizeof(ShaderModelData));
    }
}

void Renderer::GetCurrentBuffers(unsigned& CurrentBuffer, VkCommandBuffer& CommandBuffer) const
{
    vulkanSurface_->GetSwapchainCurrentImage(CurrentBuffer);
    vulkanSurface_->GetCommandBuffer(CurrentBuffer, reinterpret_cast<void**>(&CommandBuffer));
}

void Renderer::Render()
{
    start_ = std::chrono::high_resolution_clock::now();

    unsigned currentBuffer;
    VkCommandBuffer commandBuffer;
    GetCurrentBuffers(currentBuffer, commandBuffer);

    unsigned int width, height;
    Engine::GEngine().GetRawWindow()->GetClientWidth(width);
    Engine::GEngine().GetRawWindow()->GetClientHeight(height);

    const VkViewport viewport = {
        0, 0, static_cast<float>(width), static_cast<float>(height), 0, 1
    };
    const VkRect2D scissor = {
        {0, 0}, {width, height}
    };

    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout_, 0, 1, &descriptorSet_[currentBuffer], 0, nullptr);

    for (const auto& renderable : renderables_)
    {
        renderable->Render();
    }

    stop_ = std::chrono::high_resolution_clock::now();
    using MS = std::chrono::duration<float, std::milli>;
    deltaTime_ = std::chrono::duration_cast<MS>(stop_ - start_).count();
}

void Renderer::Update()
{
    unsigned maxFrames;
    UpdateStorageBuffers(physicalDevice_, maxFrames, false);
}

float Renderer::GetDeltaTime() const
{
    return deltaTime_;
}
