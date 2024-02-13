#pragma once
#include <string>

#include "Object.h"

class TestChild;

class TestChild2 : public Object
{
private:
    int a_;
    std::string b_;
    TestChild* testChild_;
    
public:
    TestChild2(TestChild* TestChild, int A, std::string B);
    void Print() const;

    void Destroy() override;
    void Init() override;
    void Update(float DeltaTime) override;
};
