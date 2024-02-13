#include "TestChild2.h"
#include <iostream>

#include "Engine.h"
#include "TestChild.h"

TestChild2::TestChild2(TestChild* TestChild, const int A, std::string B) : a_(A), b_(std::move(B)), testChild_(TestChild)
{
}

void TestChild2::Print() const
{
    VK_LOG(LogCategory::Log, "TestChild2::Print()")
    testChild_->Print();
    VK_LOG(LogCategory::Warning, "a: " + std::to_string(a_) + " b: " + b_)
}

void TestChild2::Destroy()
{
}

void TestChild2::Init()
{
    Object::Init();
}

void TestChild2::Update(float DeltaTime)
{
    VK_LOG(LogCategory::Log, "TestChild2::Update()")
}
