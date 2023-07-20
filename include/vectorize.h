//
// Created by efarhan on 23/02/2021.
//

#pragma once

#include <array>
#include <cstdlib>
#include <cmath>

#include "instrinsics.h"

namespace maths
{
template<std::size_t N>
class FloatArray
{
public:
    FloatArray() = default;
    FloatArray(const std::array<float, N>& arg): values_(arg){}
    FloatArray(const float* arg)
    {
        for(std::size_t i = 0; i < N; i++)
        {
            values_[i] = arg[i];
        }
    }
    float& operator[](std::size_t idx) { return values_[idx]; }
    const float& operator[](std::size_t idx) const { return values_[idx]; }

    FloatArray operator+(const FloatArray& rhs) const;
    FloatArray operator-(const FloatArray& rhs) const;
    FloatArray operator-() const;
    FloatArray operator*(const FloatArray& rhs) const;
    FloatArray operator*(float rhs) const;
    static FloatArray Sqrt(const FloatArray& rhs);
    static FloatArray Reciprocal(const FloatArray& rhs);

    [[nodiscard]] const std::array<float, N>& array() const {return values_;}
private:
    std::array<float, N> values_;
};

template<std::size_t N>
FloatArray<N> FloatArray<N>::operator+(const FloatArray &rhs) const
{
    FloatArray<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i] = values_[i]+rhs[i];
    }
    return result;
}

template<std::size_t N>
FloatArray<N> FloatArray<N>::operator-(const FloatArray &rhs) const
{
    FloatArray<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i] = values_[i]-rhs[i];
    }
    return result;
}

template<std::size_t N>
FloatArray<N> FloatArray<N>::operator*(const FloatArray &rhs) const
{
    FloatArray<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i] = values_[i]*rhs[i];
    }
    return result;
}

template<std::size_t N>
FloatArray<N> FloatArray<N>::operator*(float rhs) const
{
    FloatArray<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i] = values_[i]*rhs;
    }
    return result;
}

template<std::size_t N>
FloatArray<N> FloatArray<N>::Sqrt(const FloatArray &rhs)
{
    FloatArray<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i] = std::sqrt(rhs[i]);
    }
    return result;
}
template<std::size_t N>
FloatArray<N> FloatArray<N>::Reciprocal(const FloatArray &rhs)
{
    FloatArray<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i] = 1.0f / rhs[i];
    }
    return result;
}

template<std::size_t N>
FloatArray<N> FloatArray<N>::operator-() const
{
    FloatArray<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i] = -values_[i];
    }
    return result;
}

using FourFloat = FloatArray<4>;
using EightFloat = FloatArray<8>;


#if defined(__SSE__)
template<>
inline FourFloat FourFloat::Sqrt(const FloatArray<4>& rhs)
{
    auto vs = _mm_loadu_ps(&rhs[0]);
    vs = _mm_sqrt_ps(vs);

    FourFloat result;
    _mm_storeu_ps(&result[0], vs);
    return result;
}

template<>
inline FourFloat FourFloat::operator*(const FloatArray<4>& rhs) const
{
    auto v1s = _mm_loadu_ps(values_.data());
    auto v2s = _mm_loadu_ps(rhs.values_.data());
    v1s = _mm_mul_ps(v1s, v2s);

    FourFloat result;
    _mm_storeu_ps(result.values_.data(), v1s);
    return result;
}

template<>
inline FourFloat FourFloat::operator*(float rhs) const
{
    auto v1s = _mm_loadu_ps(values_.data());
    auto v2 = _mm_load1_ps(&rhs);
    v1s = _mm_mul_ps(v1s, v2);

    FourFloat result;
    _mm_storeu_ps(result.values_.data(), v1s);
    return result;
}
#endif

#if defined(__AVX2__)
template<>
inline EightFloat EightFloat::Sqrt(const EightFloat& rhs)
{
    auto vs = _mm256_loadu_ps(rhs.values_.data());
    vs = _mm256_sqrt_ps(vs);

    EightFloat result;
    _mm256_storeu_ps(result.values_.data(), vs);
    return result;
}

template<>
inline EightFloat EightFloat::operator*(const EightFloat& rhs) const
{
    auto v1s = _mm256_loadu_ps(values_.data());
    auto v2s = _mm256_loadu_ps(rhs.values_.data());
    v1s = _mm256_mul_ps(v1s, v2s);

    EightFloat result;
    _mm256_storeu_ps(result.values_.data(), v1s);
    return result;
}

template<>
inline EightFloat EightFloat::operator*(float rhs) const
{
    auto v1s = _mm256_loadu_ps(values_.data());
    auto v2 = _mm256_broadcast_ss(&rhs);
    v1s = _mm256_mul_ps(v1s, v2);

    EightFloat result;
    _mm256_storeu_ps(result.values_.data(), v1s);
    return result;
}
#endif

#if defined(__aarch64__)
template<>
inline FourFloat FourFloat::Sqrt(const FloatArray<4>& rhs)
{
    auto vs = vld1q_f32(rhs.values_.data());
    vs = vsqrtq_f32(vs);

    FourFloat result;
    vst1q_f32(&result[0], vs);
    return result;
}

template<>
inline FourFloat FourFloat::operator*(const FloatArray<4>& rhs) const
{
    auto v1s = vld1q_f32(values_.data());
    auto v2s = vld1q_f32(rhs.values_.data());
    v1s = vmulq_f32(v1s, v2s);

    FourFloat result;
    vst1q_f32(&result[0], v1s);
    return result;
}

template<>
inline FourFloat FourFloat::operator*(float rhs) const
{
    auto v1s = vld1q_f32(values_.data());
    auto v2 = vld1q_dup_f32(&rhs);
    v1s = vmulq_f32(v1s, v2);

    FourFloat result;
    vst1q_f32(&result[0], v1s);
    return result;
}
#endif

}