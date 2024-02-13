#pragma once
#include <memory>

#include "Logger.h"
#include "Window.h"

class Object;
class Renderer;

#define VK_LOG(category, msg) \
    { \
        G_LOG(Engine::GEngine().GetLogger(), category, msg) \
    }

class Engine
{
    std::mutex engineMutex_;
    
public:
    static Engine& GEngine()
    {
        static Engine inst;
        std::lock_guard lock(inst.engineMutex_);
        return inst;
    }
    Engine(const Engine& Other) = delete;
    Engine(Engine&& Other) noexcept = delete;
    Engine& operator=(const Engine& Other) = delete;
    Engine& operator=(Engine&& Other) noexcept = delete;
private:
    Engine(){} // Private constructor for singleton
    ~Engine() = default; // Private destructor for singleton

    std::unique_ptr<Window> window_;

    bool isRunning_ = false;

    std::vector<std::shared_ptr<Object>> objects_;
    std::unique_ptr<Logger> logger_;
    
public:
    void Init(const char* Title, int X, int Y, int Width, int Height);

    void Start();
    void Update();

    /** Get the primary window. */
    [[nodiscard]] std::shared_ptr<GW::SYSTEM::GWindow> GetRawWindow() const { return window_->GetWindow(); }
    [[nodiscard]] Window* GetWindow() const { return window_.get(); }

    // Templated add object function for different types of objects with constructors.
    template <typename T = Object, typename... Args>
    std::shared_ptr<T> AddObject(Args&&... ConstructorArgs);

    [[nodiscard]] Logger* GetLogger() const;
};

