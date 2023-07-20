#pragma once
#include "vector3.h"

namespace maths
{
class Mat3f {
public:
    Mat3f() = default;

    ~Mat3f() = default;

    constexpr Mat3f(const Mat3f &m) noexcept : values_(m.values_)
    {
    }

    constexpr Mat3f(const std::array<Vec3f, 3> &v) : values_(v)
    {
    }

    Vec3f &operator[](std::size_t idx) { return values_[idx]; }

    const Vec3f &operator[](std::size_t idx) const { return values_[idx]; }

    float operator()(size_t column, size_t row) const { return values_[column][row]; }

    float &operator()(size_t column, size_t row) { return values_[column][row]; }

    [[nodiscard]] float Determinant() const;
    [[nodiscard]] float DeterminantOpti() const;

private:
    std::array<Vec3f, 3> values_{};
};
}