#pragma once
#include <vulkan/vulkan_core.h>
#include "gateware/Gateware.h"

enum
{
      MaxSubMeshPerDraw = 1024
};
struct Vector { 
      float x, y, z; 
};
struct Vertex { 
      Vector pos, uvw, nrm; 
};
struct alignas(void*) Attributes {
      Vector Kd; float d;
      Vector Ks; float Ns;
      Vector Ka; float sharpness;
      Vector Tf; float Ni;
      Vector Ke; unsigned illum;
};

struct ShaderModelData {
      GW::MATH::GVECTORF SunDirection, SunColor, EyePos;
      GW::MATH::GMATRIXF ViewMatrix, ProjectionMatrix;
      GW::MATH::GMATRIXF Transforms[MaxSubMeshPerDraw];
      Attributes Attributes[MaxSubMeshPerDraw];
};

class Renderable
{
protected:
      bool isRenderableInitialized_ = false;
      bool isWireframe_ = false;
      
      unsigned vertexCount_ = 0;
      unsigned indexCount_ = 0;
      unsigned meshCount_ = 0;
      
      VkBuffer vertexBufferHandle_ = nullptr;
      VkDeviceMemory vertexBufferMemory_ = nullptr;
      VkBuffer indexBufferHandle_ = nullptr;
      VkDeviceMemory indexBufferMemory_ = nullptr;

      bool isBound_ = false;
      
public:
      Renderable();
      virtual ~Renderable();

      void Bind();
      virtual void Render();

      /** Copy and Move */
      Renderable(const Renderable& Other);
      Renderable(Renderable&& Other) noexcept;

      Renderable& operator=(const Renderable& Other);
      Renderable& operator=(Renderable&& Other) noexcept;
};

