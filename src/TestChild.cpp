#include "TestChild.h"

#include <iostream>

#include "Engine.h"

TestChild::TestChild(const int A, const int B, std::string C) : a_(A), b_(B), c_(std::move(C)) 
{
}

void TestChild::Destroy()
{
}

void TestChild::Init()
{
    Object::Init();
}

void TestChild::Update(float DeltaTime)
{
    VK_LOG(LogCategory::Log, "TestChild::Update()")
}

void TestChild::Print() const
{
    VK_LOG(LogCategory::Log, "a: " + std::to_string(a_) + " b: " + std::to_string(b_) + " c: " + c_)
}
