#pragma once
#include <array>

namespace maths
{

union Vec4f
{
    struct
    {
        float x, y, z, w;
    };
    std::array<float, 4> data;

    constexpr Vec4f() : x(0), y(0), z(0), w(1) {}
    constexpr Vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vec4f(const float* ptr);

    Vec4f operator+(const Vec4f& v) const;
    Vec4f operator-(const Vec4f& v) const;
    Vec4f operator*(float f) const;
    Vec4f operator/(float f) const;

    float& operator[](std::size_t idx) { return data[idx]; }
    const float& operator[](std::size_t idx) const { return data[idx]; }

    static constexpr Vec4f zero() {return Vec4f();}
    static constexpr Vec4f one() {return Vec4f(1,1,1,1);}
};


}