#pragma once
#include <string>

#include "Object.h"

class TestChild : public Object
{
private:
    int a_;
    int b_;
    std::string c_;
public:
    TestChild(int A, int B, std::string C);

    void Destroy() override;
    void Init() override;
    void Update(float DeltaTime) override;

    void Print() const;
};
