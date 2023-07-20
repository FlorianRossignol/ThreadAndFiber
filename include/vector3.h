#pragma once
#include <array>
#include <cmath>
#include "instrinsics.h"
#include "vectorize.h"

namespace maths
{
union Vec3f
{
    struct
    {
        float x,y,z;
    };
    std::array<float, 3> values{};

    constexpr Vec3f() : x(0), y(0), z(0){}
    constexpr Vec3f(float x, float y, float z) : x(x), y(y), z(z){}
    Vec3f(const float* ptr);

    Vec3f operator+(const Vec3f& v) const;
    Vec3f operator-(const Vec3f& v) const;
    Vec3f operator*(float f) const;
    Vec3f operator/(float f) const;

    float& operator[](std::size_t idx) { return values[idx]; }
    const float& operator[](std::size_t idx) const { return values[idx]; }

    [[nodiscard]] float Magnitude() const;
    [[nodiscard]] float SqrMagnitude() const;
    [[nodiscard]] Vec3f Normalized() const;
    static constexpr Vec3f zero(){return {};}
    static constexpr Vec3f one(){return {1,1,1};}
};

float Dot(const Vec3f& v1, const Vec3f& v2);

template<std::size_t N>
class NVec3f
{
public:
    NVec3f() = default;
    NVec3f(const Vec3f *ptr);
    NVec3f(Vec3f v);
    NVec3f(const std::array<float, N>& xs, const std::array<float, N>& ys, const std::array<float, N>& zs) :
        xs(xs), ys(ys), zs(zs){}
    NVec3f(const FloatArray<N> & xs, const FloatArray<N> & ys, const FloatArray<N> & zs) :
        xs(xs.array()), ys(ys.array()), zs(zs.array()){}

    [[nodiscard]] FloatArray<N> Magnitude() const;
    [[nodiscard]] FloatArray<N> SqrMagnitude() const;
    static FloatArray<N> Dot(const NVec3f &v1, const NVec3f &v2);
    [[nodiscard]] NVec3f Normalized() const;

    [[nodiscard]] FloatArray<N> Xs() const {return xs;}
    [[nodiscard]] FloatArray<N> Ys() const {return ys;}
    [[nodiscard]] FloatArray<N> Zs() const {return zs;}

    [[nodiscard]] std::array<float, N>& Xs() {return xs;}
    [[nodiscard]] std::array<float, N>& Ys() {return ys;}
    [[nodiscard]] std::array<float, N>& Zs() {return zs;}

    NVec3f operator+(const NVec3f& v) const;
    NVec3f operator-(const NVec3f& v) const;
    NVec3f operator*(const FloatArray<N> & rhs) const;
    NVec3f operator*(float value) const;
    NVec3f operator/(const FloatArray<N> & rhs) const;

    [[nodiscard]] std::array<Vec3f, N> vectors() const;
private:
    std::array<float, N> xs{};
    std::array<float, N> ys{};
    std::array<float, N> zs{};
};

template<std::size_t N>
NVec3f<N>::NVec3f(const Vec3f *ptr)
{
    for(std::size_t i = 0; i < N; i++)
    {
        xs[i] = ptr[i].x;
        ys[i] = ptr[i].y;
        zs[i] = ptr[i].z;
    }
}
template<std::size_t N>
NVec3f<N>::NVec3f(Vec3f v)
{
    for(std::size_t i = 0; i < N; i++)
    {
        xs[i] = v.x;
        ys[i] = v.y;
        zs[i] = v.z;
    }
}
template<std::size_t N>
FloatArray<N> NVec3f<N>::Dot(const NVec3f<N> &v1, const NVec3f<N> &v2)
{
    FloatArray<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i] = v1.xs[i]*v2.xs[i]+v1.ys[i]*v2.ys[i]+v1.zs[i]*v2.zs[i];
    }
    return result;
}
template<std::size_t N>
NVec3f<N> NVec3f<N>::operator+(const NVec3f &v) const
{
    NVec3f<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result.xs[i] = xs[i]+v.xs[i];
        result.ys[i] = ys[i]+v.ys[i];
        result.zs[i] = zs[i]+v.zs[i];
    }
    return result;
}
template<std::size_t N>
NVec3f<N> NVec3f<N>::operator-(const NVec3f &v) const
{
    NVec3f<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result.xs[i] = xs[i]-v.xs[i];
        result.ys[i] = ys[i]-v.ys[i];
        result.zs[i] = zs[i]-v.zs[i];
    }
    return result;
}

template<std::size_t N>
NVec3f<N> NVec3f<N>::operator*(const FloatArray<N> &rhs) const
{
    NVec3f<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result.xs[i] = xs[i]*rhs[i];
        result.ys[i] = ys[i]*rhs[i];
        result.zs[i] = zs[i]*rhs[i];
    }
    return result;
}

template<std::size_t N>
NVec3f<N> NVec3f<N>::operator*(float rhs) const
{
    NVec3f<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result.xs[i] = xs[i]*rhs;
        result.ys[i] = ys[i]*rhs;
        result.zs[i] = zs[i]*rhs;
    }
    return result;
}

template<std::size_t N>
NVec3f<N> NVec3f<N>::operator/(const FloatArray<N> &rhs) const
{
    NVec3f<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result.xs[i] = xs[i]/rhs[i];
        result.ys[i] = ys[i]/rhs[i];
        result.zs[i] = zs[i]/rhs[i];
    }
    return result;
}
template<std::size_t N>
FloatArray<N> NVec3f<N>::SqrMagnitude() const
{
    return Dot(*this, *this);
}

template<std::size_t N>
FloatArray<N> NVec3f<N>::Magnitude() const
{
    return FloatArray<N>::Sqrt(SqrMagnitude());
}

template<std::size_t N>
NVec3f<N> NVec3f<N>::Normalized() const
{
    return *this/Magnitude();
}

template<std::size_t N>
std::array<Vec3f, N> NVec3f<N>::vectors() const
{
    std::array<Vec3f, N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i].x = xs[i];
        result[i].y = ys[i];
        result[i].z = zs[i];
    }
    return result;
}

using FourVec3f = NVec3f<4>;
using EightVec3f = NVec3f<8>;

#if defined(__SSE__)

template<>
FourVec3f FourVec3f::operator+(const FourVec3f& v) const;

template<>
FourVec3f FourVec3f::operator-(const FourVec3f& v) const;

template<>
FourVec3f FourVec3f::operator*(const FourFloat& rhs) const;

template<>
FourVec3f FourVec3f::operator/(const FourFloat& rhs) const;

template<>
FourVec3f FourVec3f::operator*(float value) const;

template<>
FourFloat FourVec3f::Dot(const FourVec3f& v1, const FourVec3f& v2);

template<>
FourFloat FourVec3f::Magnitude() const;
#endif


#if defined(__aarch64__)
template<>
FourVec3f FourVec3f::operator+(const FourVec3f& v) const;

template<>
FourVec3f FourVec3f::operator-(const FourVec3f& v) const;

template<>
FourVec3f FourVec3f::operator*(const FourFloat& values) const;

template<>
FourVec3f FourVec3f::operator/(const FourFloat& values) const;

template<>
FourVec3f FourVec3f::operator*(float value) const;

template<>
FourFloat FourVec3f::Dot(const FourVec3f& v1, const FourVec3f& v2);
template<>
FourFloat FourVec3f::Magnitude() const;

#endif


#if defined(__AVX2__)
template<>
EightVec3f EightVec3f::operator+(const EightVec3f& v) const;
template<>
EightVec3f EightVec3f::operator-(const EightVec3f& v) const;
template<>
EightVec3f EightVec3f::operator*(const EightFloat& rhs) const;
template<>
EightVec3f EightVec3f::operator*(float value) const;
template<>
EightVec3f EightVec3f::operator/(const EightFloat& values) const;
template<>
EightFloat EightVec3f::Dot(const EightVec3f& v1, const EightVec3f& v2);
template<>
EightFloat EightVec3f::Magnitude() const;


#endif
}
