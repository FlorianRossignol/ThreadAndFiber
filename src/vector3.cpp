#include <vector3.h>

#include <cmath>

namespace maths
{

Vec3f::Vec3f(const float *ptr)
{
    values[0] = ptr[0];
    values[1] = ptr[1];
    values[2] = ptr[2];
}
float Vec3f::Magnitude() const
{
    return std::sqrt(x*x+y*y+z*z);
}

Vec3f Vec3f::operator+(const Vec3f &v) const
{
    const Vec3f result(x+v.x, y+v.y, z+v.z);
    return result;
}

Vec3f Vec3f::operator-(const Vec3f &v) const
{
    const Vec3f result(x-v.x, y-v.y, z-v.z);
    return result;
}

Vec3f Vec3f::operator*(float f) const
{
    const Vec3f result(x*f, y*f, z*f);
    return result;
}

Vec3f Vec3f::operator/(float f) const
{
    const Vec3f result(x/f, y/f, z/f);
    return result;
}
Vec3f Vec3f::Normalized() const
{
    return *this/Magnitude();
}
float Vec3f::SqrMagnitude() const
{
    return Dot(*this, *this);
}

float Dot(const Vec3f &v1, const Vec3f &v2)
{
    return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}

#if defined(__SSE__)

template<>
FourVec3f FourVec3f::operator+(const FourVec3f& v) const
{
    FourVec3f fv3f;
    auto x1 = _mm_loadu_ps(xs.data());
    auto y1 = _mm_loadu_ps(ys.data());
    auto z1 = _mm_loadu_ps(zs.data());

    const auto x2 = _mm_loadu_ps(v.xs.data());
    const auto y2 = _mm_loadu_ps(v.ys.data());
    const auto z2 = _mm_loadu_ps(v.zs.data());

    x1 = _mm_add_ps(x1, x2);
    y1 = _mm_add_ps(y1, y2);
    z1 = _mm_add_ps(z1, z2);

    _mm_storeu_ps(fv3f.xs.data(), x1);
    _mm_storeu_ps(fv3f.ys.data(), y1);
    _mm_storeu_ps(fv3f.zs.data(), z1);
    return fv3f;
}


template<>
FourVec3f FourVec3f::operator-(const FourVec3f& v) const
{
    FourVec3f fv3f;
    auto x1 = _mm_loadu_ps(xs.data());
    auto y1 = _mm_loadu_ps(ys.data());
    auto z1 = _mm_loadu_ps(zs.data());

    const auto x2 = _mm_loadu_ps(v.xs.data());
    const auto y2 = _mm_loadu_ps(v.ys.data());
    const auto z2 = _mm_loadu_ps(v.zs.data());

    x1 = _mm_sub_ps(x1, x2);
    y1 = _mm_sub_ps(y1, y2);
    z1 = _mm_sub_ps(z1, z2);

    _mm_storeu_ps(fv3f.xs.data(), x1);
    _mm_storeu_ps(fv3f.ys.data(), y1);
    _mm_storeu_ps(fv3f.zs.data(), z1);
    return fv3f;
}


template<>
FourVec3f FourVec3f::operator*(const FourFloat& rhs) const
{
    FourVec3f result;
    auto x = _mm_loadu_ps(xs.data());
    auto y = _mm_loadu_ps(ys.data());
    auto z = _mm_loadu_ps(zs.data());
    const auto v = _mm_loadu_ps(&rhs[0]);

    x = _mm_mul_ps(x, v);
    y = _mm_mul_ps(y, v);
    z = _mm_mul_ps(z, v);

    _mm_storeu_ps(result.Xs().data(), x);
    _mm_storeu_ps(result.Ys().data(), y);
    _mm_storeu_ps(result.Zs().data(), z);
    return result;

}

template<>
FourVec3f FourVec3f::operator/(const FourFloat& rhs) const
{
    FourVec3f result;
    auto x = _mm_loadu_ps(xs.data());
    auto y = _mm_loadu_ps(ys.data());
    auto z = _mm_loadu_ps(zs.data());
    const auto v = _mm_loadu_ps(&rhs[0]);

    x = _mm_div_ps(x, v);
    y = _mm_div_ps(y, v);
    z = _mm_div_ps(z, v);

    _mm_storeu_ps(result.Xs().data(), x);
    _mm_storeu_ps(result.Ys().data(), y);
    _mm_storeu_ps(result.Zs().data(), z);
    return result;
}

template<>
FourVec3f FourVec3f::operator*(float value) const
{
    FourVec3f result;
    auto x = _mm_loadu_ps(xs.data());
    auto y = _mm_loadu_ps(ys.data());
    auto z = _mm_loadu_ps(zs.data());
    const auto v = _mm_load1_ps(&value);

    x = _mm_mul_ps(x, v);
    y = _mm_mul_ps(y, v);
    z = _mm_mul_ps(z, v);

    _mm_storeu_ps(result.Xs().data(), x);
    _mm_storeu_ps(result.Ys().data(), y);
    _mm_storeu_ps(result.Zs().data(), z);
    return result;
}
template<>
FourFloat FourVec3f::Dot(const FourVec3f& v1, const FourVec3f& v2)
{
    auto x1 = _mm_loadu_ps(v1.xs.data());
    auto y1 = _mm_loadu_ps(v1.ys.data());
    auto z1 = _mm_loadu_ps(v1.zs.data());

    auto x2 = _mm_loadu_ps(v2.xs.data());
    auto y2 = _mm_loadu_ps(v2.ys.data());
    auto z2 = _mm_loadu_ps(v2.zs.data());

    x1 = _mm_mul_ps(x1, x2);
    y1 = _mm_mul_ps(y1, y2);
    z1 = _mm_mul_ps(z1, z2);

    x1 = _mm_add_ps(x1, y1);
    x1 = _mm_add_ps(x1, z1);

    FourFloat result;
    _mm_storeu_ps(&result[0], x1);
    return result;
}
template<>
FourFloat FourVec3f::Magnitude() const
{
    auto x1 = _mm_loadu_ps(xs.data());
    auto y1 = _mm_loadu_ps(ys.data());
    auto z1 = _mm_loadu_ps(zs.data());

    x1 = _mm_mul_ps(x1, x1);
    y1 = _mm_mul_ps(y1, y1);
    z1 = _mm_mul_ps(z1, z1);

    x1 = _mm_add_ps(x1, y1);
    x1 = _mm_add_ps(x1, z1);
    x1 = _mm_sqrt_ps(x1);

    FourFloat result;
    _mm_storeu_ps(&result[0], x1);
    return result;
}
#endif


#if defined(__aarch64__)
template<>
FourVec3f FourVec3f::operator+(const FourVec3f& v) const
{
    FourVec3f fv3f;
    auto x1 = vld1q_f32(xs.data());
    auto y1 = vld1q_f32(ys.data());
    auto z1 = vld1q_f32(zs.data());

    const auto x2 = vld1q_f32(v.xs.data());
    const auto y2 = vld1q_f32(v.ys.data());
    const auto z2 = vld1q_f32(v.zs.data());

    x1 = vaddq_f32(x1, x2);
    y1 = vaddq_f32(y1, y2);
    z1 = vaddq_f32(z1, z2);

    vst1q_f32(fv3f.xs.data(), x1);
    vst1q_f32(fv3f.ys.data(), y1);
    vst1q_f32(fv3f.zs.data(), z1);
    return fv3f;
}

template<>
FourVec3f FourVec3f::operator-(const FourVec3f& v) const
{
    FourVec3f fv3f;
    auto x1 = vld1q_f32(xs.data());
    auto y1 = vld1q_f32(ys.data());
    auto z1 = vld1q_f32(zs.data());

    const auto x2 = vld1q_f32(v.xs.data());
    const auto y2 = vld1q_f32(v.ys.data());
    const auto z2 = vld1q_f32(v.zs.data());

    x1 = vsubq_f32(x1, x2);
    y1 = vsubq_f32(y1, y2);
    z1 = vsubq_f32(z1, z2);

    vst1q_f32(fv3f.xs.data(), x1);
    vst1q_f32(fv3f.ys.data(), y1);
    vst1q_f32(fv3f.zs.data(), z1);
    return fv3f;
}

template<>
FourVec3f FourVec3f::operator*(const FourFloat& values) const
{
    FourVec3f fv3f;
    auto x1 = vld1q_f32(xs.data());
    auto y1 = vld1q_f32(ys.data());
    auto z1 = vld1q_f32(zs.data());

    const auto x2 = vld1q_f32(&values[0]);
    const auto y2 = vld1q_f32(&values[0]);
    const auto z2 = vld1q_f32(&values[0]);

    x1 = vmulq_f32(x1, x2);
    y1 = vmulq_f32(y1, y2);
    z1 = vmulq_f32(z1, z2);

    vst1q_f32(fv3f.xs.data(), x1);
    vst1q_f32(fv3f.ys.data(), y1);
    vst1q_f32(fv3f.zs.data(), z1);
    return fv3f;

}

template<>
FourVec3f FourVec3f::operator/(const FourFloat& values) const
{
    FourVec3f fv3f;
    auto x1 = vld1q_f32(xs.data());
    auto y1 = vld1q_f32(ys.data());
    auto z1 = vld1q_f32(zs.data());

    const auto x2 = vld1q_f32(&values[0]);
    const auto y2 = vld1q_f32(&values[0]);
    const auto z2 = vld1q_f32(&values[0]);

    x1 = vdivq_f32(x1, x2);
    y1 = vdivq_f32(y1, y2);
    z1 = vdivq_f32(z1, z2);

    vst1q_f32(fv3f.xs.data(), x1);
    vst1q_f32(fv3f.ys.data(), y1);
    vst1q_f32(fv3f.zs.data(), z1);
    return fv3f;
}

template<>
FourVec3f FourVec3f::operator*(float value) const
{
    FourVec3f fv3f;
    auto x1 = vld1q_f32(xs.data());
    auto y1 = vld1q_f32(ys.data());
    auto z1 = vld1q_f32(zs.data());

    const auto v = vld1q_dup_f32(&value);

    x1 = vmulq_f32(x1, v);
    y1 = vmulq_f32(y1, v);
    z1 = vmulq_f32(z1, v);

    vst1q_f32(fv3f.xs.data(), x1);
    vst1q_f32(fv3f.ys.data(), y1);
    vst1q_f32(fv3f.zs.data(), z1);
    return fv3f;
}

template<>
FourFloat FourVec3f::Dot(const FourVec3f& v1, const FourVec3f& v2)
{
    FourVec3f fv3f;
    auto x1 = vld1q_f32(v1.xs.data());
    auto y1 = vld1q_f32(v1.ys.data());
    auto z1 = vld1q_f32(v1.zs.data());

    const auto x2 = vld1q_f32(v2.xs.data());
    const auto y2 = vld1q_f32(v2.ys.data());
    const auto z2 = vld1q_f32(v2.zs.data());


    x1 = vmulq_f32(x1, x2);
    y1 = vmulq_f32(y1, y2);
    z1 = vmulq_f32(z1, z2);

    x1 = vaddq_f32(x1, y1);
    x1 = vaddq_f32(x1, z1);

    FourFloat result;
    vst1q_f32(&result[0], x1);
    return result;
}
template<>
FourFloat FourVec3f::Magnitude() const
{
    auto x1 = vld1q_f32(xs.data());
    auto y1 = vld1q_f32(ys.data());
    auto z1 = vld1q_f32(zs.data());

    x1 = vmulq_f32(x1, x1);
    y1 = vmulq_f32(y1, y1);
    z1 = vmulq_f32(z1, z1);

    x1 = vaddq_f32(x1, y1);
    x1 = vaddq_f32(x1, z1);
    x1 = vsqrtq_f32(x1);

    FourFloat result;
    vst1q_f32(&result[0], x1);
    return result;
}

#endif


#if defined(__AVX2__)
template<>
EightVec3f EightVec3f::operator+(const EightVec3f& v) const
{
    EightVec3f result;

    auto x1 = _mm256_loadu_ps(xs.data());
    auto y1 = _mm256_loadu_ps(xs.data());
    auto z1 = _mm256_loadu_ps(xs.data());

    const auto x2 = _mm256_loadu_ps(v.xs.data());
    const auto y2 = _mm256_loadu_ps(v.xs.data());
    const auto z2 = _mm256_loadu_ps(v.xs.data());

    x1 = _mm256_add_ps(x1, x2);
    y1 = _mm256_add_ps(y1, y2);
    z1 = _mm256_add_ps(z1, z2);

    _mm256_storeu_ps(result.xs.data(), x1);
    _mm256_storeu_ps(result.ys.data(), y1);
    _mm256_storeu_ps(result.zs.data(), z1);

    return result;
}
template<>
EightVec3f EightVec3f::operator-(const EightVec3f& v) const
{
    EightVec3f result;

    auto x1 = _mm256_loadu_ps(xs.data());
    auto y1 = _mm256_loadu_ps(xs.data());
    auto z1 = _mm256_loadu_ps(xs.data());

    const auto x2 = _mm256_loadu_ps(v.xs.data());
    const auto y2 = _mm256_loadu_ps(v.xs.data());
    const auto z2 = _mm256_loadu_ps(v.xs.data());

    x1 = _mm256_sub_ps(x1, x2);
    y1 = _mm256_sub_ps(y1, y2);
    z1 = _mm256_sub_ps(z1, z2);

    _mm256_storeu_ps(result.xs.data(), x1);
    _mm256_storeu_ps(result.ys.data(), y1);
    _mm256_storeu_ps(result.zs.data(), z1);

    return result;
}
template<>
EightVec3f EightVec3f::operator*(const EightFloat& rhs) const
{
    EightVec3f result;
    auto x = _mm256_loadu_ps(xs.data());
    auto y = _mm256_loadu_ps(ys.data());
    auto z = _mm256_loadu_ps(zs.data());
    const auto v = _mm256_loadu_ps(&rhs[0]);

    x = _mm256_mul_ps(x, v);
    y = _mm256_mul_ps(y, v);
    z = _mm256_mul_ps(z, v);

    _mm256_storeu_ps(result.Xs().data(), x);
    _mm256_storeu_ps(result.Ys().data(), y);
    _mm256_storeu_ps(result.Zs().data(), z);
    return result;
}
template<>
EightVec3f EightVec3f::operator*(float value) const
{
    EightVec3f result;
    auto x = _mm256_loadu_ps(xs.data());
    auto y = _mm256_loadu_ps(ys.data());
    auto z = _mm256_loadu_ps(zs.data());
    const auto v = _mm256_broadcast_ss(&value);

    x = _mm256_mul_ps(x, v);
    y = _mm256_mul_ps(y, v);
    z = _mm256_mul_ps(z, v);

    _mm256_storeu_ps(result.Xs().data(), x);
    _mm256_storeu_ps(result.Ys().data(), y);
    _mm256_storeu_ps(result.Zs().data(), z);
    return result;
}
template<>
EightVec3f EightVec3f::operator/(const EightFloat& values) const
{
    EightVec3f result;
    auto x = _mm256_loadu_ps(xs.data());
    auto y = _mm256_loadu_ps(ys.data());
    auto z = _mm256_loadu_ps(zs.data());
    const auto v = _mm256_loadu_ps(&values[0]);

    x = _mm256_div_ps(x, v);
    y = _mm256_div_ps(y, v);
    z = _mm256_div_ps(z, v);

    _mm256_storeu_ps(result.Xs().data(), x);
    _mm256_storeu_ps(result.Ys().data(), y);
    _mm256_storeu_ps(result.Zs().data(), z);
    return result;
}
template<>
EightFloat EightVec3f::Dot(const EightVec3f& v1, const EightVec3f& v2)
{
    auto x1 = _mm256_loadu_ps(v1.xs.data());
    auto y1 = _mm256_loadu_ps(v1.ys.data());
    auto z1 = _mm256_loadu_ps(v1.zs.data());

    auto x2 = _mm256_loadu_ps(v2.xs.data());
    auto y2 = _mm256_loadu_ps(v2.ys.data());
    auto z2 = _mm256_loadu_ps(v2.zs.data());

    x1 = _mm256_mul_ps(x1, x2);
    y1 = _mm256_mul_ps(y1, y2);
    z1 = _mm256_mul_ps(z1, z2);

    x1 = _mm256_add_ps(x1, y1);
    x1 = _mm256_add_ps(x1, z1);

    EightFloat result;
    _mm256_storeu_ps(&result[0], x1);
    return result;
}
template<>
EightFloat EightVec3f::Magnitude() const
{
    auto x1 = _mm256_loadu_ps(xs.data());
    auto y1 = _mm256_loadu_ps(ys.data());
    auto z1 = _mm256_loadu_ps(zs.data());

    x1 = _mm256_mul_ps(x1, x1);
    y1 = _mm256_mul_ps(y1, y1);
    z1 = _mm256_mul_ps(z1, z1);

    x1 = _mm256_add_ps(x1, y1);
    x1 = _mm256_add_ps(x1, z1);
    x1 = _mm256_sqrt_ps(x1);

    EightFloat result;
    _mm256_storeu_ps(&result[0], x1);
    return result;
}


#endif



}
