#include "Object.h"

#include <algorithm>

void Object::Init()
{
    isInitialized_ = true;
}

Object::Object(const Object& Other)
{
    this->isInitialized_ = Other.isInitialized_;
}

Object::Object(Object&& Other) noexcept
{
    this->isInitialized_ = Other.isInitialized_;
}

Object& Object::operator=(const Object& Other)
{
    if (this == &Other)
        return *this;
    isInitialized_ = Other.isInitialized_;
    return *this;
}

Object& Object::operator=(Object&& Other) noexcept
{
    if (this == &Other)
        return *this;
    isInitialized_ = Other.isInitialized_;
    return *this;
}
