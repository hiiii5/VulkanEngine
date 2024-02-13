#include "Renderable.h"

#include "Renderer.h"

Renderable::Renderable() = default;
Renderable::~Renderable() = default;

void Renderable::Bind()
{
    if (isBound_)
    {
        return;
    }

    // Binds the renderable to the renderer to hook into the render loop.
    Renderer::GRenderer().BindRenderable(this);
    isBound_ = true;
}

void Renderable::Render()
{
}

Renderable::Renderable(const Renderable& Other) = default;

Renderable::Renderable(Renderable&& Other) noexcept:
    isRenderableInitialized_(Other.isRenderableInitialized_),
    isWireframe_(Other.isWireframe_),
    vertexCount_(Other.vertexCount_),
    indexCount_(Other.indexCount_),
    meshCount_(Other.meshCount_),
    vertexBufferHandle_(Other.vertexBufferHandle_),
    vertexBufferMemory_(Other.vertexBufferMemory_),
    indexBufferHandle_(Other.indexBufferHandle_),
    indexBufferMemory_(Other.indexBufferMemory_)
{
}

Renderable& Renderable::operator=(const Renderable& Other)
{
    if (this == &Other)
        return *this;
    isRenderableInitialized_ = Other.isRenderableInitialized_;
    isWireframe_ = Other.isWireframe_;
    vertexCount_ = Other.vertexCount_;
    indexCount_ = Other.indexCount_;
    meshCount_ = Other.meshCount_;
    vertexBufferHandle_ = Other.vertexBufferHandle_;
    vertexBufferMemory_ = Other.vertexBufferMemory_;
    indexBufferHandle_ = Other.indexBufferHandle_;
    indexBufferMemory_ = Other.indexBufferMemory_;
    return *this;
}

Renderable& Renderable::operator=(Renderable&& Other) noexcept
{
    if (this == &Other)
        return *this;
    isRenderableInitialized_ = Other.isRenderableInitialized_;
    isWireframe_ = Other.isWireframe_;
    vertexCount_ = Other.vertexCount_;
    indexCount_ = Other.indexCount_;
    meshCount_ = Other.meshCount_;
    vertexBufferHandle_ = Other.vertexBufferHandle_;
    vertexBufferMemory_ = Other.vertexBufferMemory_;
    indexBufferHandle_ = Other.indexBufferHandle_;
    indexBufferMemory_ = Other.indexBufferMemory_;
    return *this;
}
