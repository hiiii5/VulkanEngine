#pragma once

/** Base class for all objects in the engine. */
class Object
{
protected:
    bool isInitialized_ = false;

public:
    Object() = default;
    virtual ~Object() = default;
    virtual void Destroy() = 0;

    virtual void Init();
    virtual void Update(float DeltaTime) = 0;

    // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    // | Copy and Move Constructors and Operators
    // +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    Object(const Object& Other);
    Object(Object&& Other) noexcept;

    Object& operator=(const Object& Other);
    Object& operator=(Object&& Other) noexcept;
};
