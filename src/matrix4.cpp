#include <vector3.h>
#include <math_utils.h>
#include "matrix4.h"
#include "instrinsics.h"
#include "matrix3.h"

namespace maths
{

Mat4f Mat4f::operator+(const Mat4f &rhs) const
{
    Mat4f result;
    for(int i = 0; i < 4; i++)
    {
        result[i] = rhs[i] + values_[i];
    }
    return result;
}

Mat4f Mat4f::operator*(const Mat4f& rhs) const noexcept
{
    std::array<Vec4f, 4> v;
    for (int column = 0; column < 4; column++)
    {
        for (int row = 0; row < 4; row++)
        {
            v[column][row] = 0;
            for (int i = 0; i < 4; i++)
            {
                v[column][row] += values_[i][row] * rhs[column][i];
            }
        }
    }
    return Mat4f(v);
}

Mat4f Mat4f::Inverse() const
{
    Mat4f inverse = zero();
    const float determinant = Determinant();
    if (Equal(determinant, 0.0f))
        return inverse;
    //Calculate the cofactor matrix
    for (int col = 0; col < 4; col++)
    {
        for (int row = 0; row < 4; row++)
        {
            const Mat3f sub(std::array<Vec3f, 3>
                                               {
                                                       Vec3f(values_[(col + 1) % 4][(row + 1) % 4],
                                                             values_[(col + 1) % 4][(row + 2) % 4],
                                                             values_[(col + 1) % 4][(row + 3) % 4]),
                                                       Vec3f(values_[(col + 2) % 4][(row + 1) % 4],
                                                             values_[(col + 2) % 4][(row + 2) % 4],
                                                             values_[(col + 2) % 4][(row + 3) % 4]),
                                                       Vec3f(values_[(col + 3) % 4][(row + 1) % 4],
                                                             values_[(col + 3) % 4][(row + 2) % 4],
                                                             values_[(col + 3) % 4][(row + 3) % 4])
                                               });
            inverse[col][row] = ((col + row) % 2 == 0 ? 1.0f : -1.0f) * sub.Determinant();
        }
    }
    inverse = inverse.Transpose();
    inverse = inverse * (1.0f / determinant);
    return inverse;
}

float Mat4f::Determinant() const
{
    float result = 0.0f;
    for (int i = 0; i < 4; i++)
    {
        const Mat3f m = Mat3f(std::array<Vec3f, 3>
                                      {
                                              Vec3f(&values_[(i + 1) % 4][1]),
                                              Vec3f(&values_[(i + 2) % 4][1]),
                                              Vec3f(&values_[(i + 3) % 4][1])
                                      });
        result += (i % 2 == 1 ? -1.0f : 1.0f) * values_[i][0] * m.Determinant();
    }
    return result;
}



Mat4f Mat4f::InverseOpti() const
{
    float determinant4 = 0.0f;
    std::array<float, 4> subDetFirstLine;
    for (int i = 0; i < 4; i++)
    {
        const Mat3f m = Mat3f(std::array<Vec3f, 3>
                                      {
                                              Vec3f(&values_[(i + 1) % 4][1]),
                                              Vec3f(&values_[(i + 2) % 4][1]),
                                              Vec3f(&values_[(i + 3) % 4][1])
                                      });
        const float det = m.DeterminantOpti();
        subDetFirstLine[i] = det;
        determinant4 += (i % 2 == 1 ? -1.0f : 1.0f) * values_[i][0] * det;
    }

    Mat4f inverse = zero();
    if (Equal(determinant4, 0.0f))
        return inverse;
    if(Equal(determinant4, 1.0f))
        return Transpose();
    //Calculate the cofactor matrix
    for (int col = 0; col < 4; col++)
    {
        inverse[col][0] = (col % 2 == 0 ? 1.0f : -1.0f) * subDetFirstLine[col];
        for (int row = 1; row < 4; row++)
        {
            const Mat3f sub(std::array<Vec3f, 3>
                                    {
                                            Vec3f(values_[(col + 1) % 4][(row + 1) % 4],
                                                  values_[(col + 1) % 4][(row + 2) % 4],
                                                  values_[(col + 1) % 4][(row + 3) % 4]),
                                            Vec3f(values_[(col + 2) % 4][(row + 1) % 4],
                                                  values_[(col + 2) % 4][(row + 2) % 4],
                                                  values_[(col + 2) % 4][(row + 3) % 4]),
                                            Vec3f(values_[(col + 3) % 4][(row + 1) % 4],
                                                  values_[(col + 3) % 4][(row + 2) % 4],
                                                  values_[(col + 3) % 4][(row + 3) % 4])
                                    });
            inverse[col][row] = ((col + row) % 2 == 0 ? 1.0f : -1.0f) * sub.DeterminantOpti();
        }
    }
    inverse = inverse.Transpose();
    inverse = inverse * (1.0f / determinant4);
    return inverse;
}

Mat4f Mat4f::Transpose() const
{
    std::array<Vec4f, 4> v;
    for (int column = 0; column < 4; column++)
    {
        for (int row = 0; row < 4; row++)
        {
            v[row][column] = values_[column][row];
        }
    }
    return Mat4f(v);
}

Mat4f Mat4f::operator*(float rhs) const noexcept
{
    Mat4f m;
    for(int col = 0; col < 4; col++)
    {
        for(int row = 0; row < 4 ; row++)
        {
            m[col][row] = values_[col][row] * rhs;
        }
    }
    return m;
}
Mat4f Mat4f::operator/(float rhs) const noexcept
{
    Mat4f m;
    for(int col = 0; col < 4; col++)
    {
        for(int row = 0; row < 4 ; row++)
        {
            m[col][row] = values_[col][row] / rhs;
        }
    }
    return m;
}

float Mat4f::MatrixDiff(const Mat4f &m1, const Mat4f &m2)
{
    float result = 0.0f;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            result += Abs(m1[i][j]-m2[i][j]);
        }
    }
    return result;
}

#if defined(__SSE__)
Mat4f Mat4f::MultIntrinsics(const Mat4f &rhs) const noexcept
{
    std::array<Vec4f, 4> v;
    __m128 c1 = _mm_load_ps(&this->values_[0][0]);
    __m128 c2 = _mm_load_ps(&this->values_[1][0]);
    __m128 c3 = _mm_load_ps(&this->values_[2][0]);
    __m128 c4 = _mm_load_ps(&this->values_[3][0]);
    for (int column = 0; column < 4; column++)
    {
        __m128 x = _mm_load1_ps(&rhs.values_[column][0]);
        __m128 y = _mm_load1_ps(&rhs.values_[column][1]);
        __m128 z = _mm_load1_ps(&rhs.values_[column][2]);
        __m128 w = _mm_load1_ps(&rhs.values_[column][3]);
        x = _mm_mul_ps(c1, x);
        y = _mm_mul_ps(c2, y);
        z = _mm_mul_ps(c3, z);
        w = _mm_mul_ps(c4, w);

        x = _mm_add_ps(x, y);
        z = _mm_add_ps(z, w);
        x = _mm_add_ps(x, z);
        _mm_store_ps(&v[column][0], x);
    }
    return {v};
}

#endif

#if defined(__aarch64__)
Mat4f Mat4f::MultIntrinsics(const Mat4f &rhs) const noexcept
{
    std::array<Vec4f, 4> v;
    auto c1 = vld1q_f32(&this->values_[0][0]);
    auto c2 = vld1q_f32(&this->values_[1][0]);
    auto c3 = vld1q_f32(&this->values_[2][0]);
    auto c4 = vld1q_f32(&this->values_[3][0]);
    for (int column = 0; column < 4; column++)
    {
        auto x = vld1q_dup_f32(&rhs.values_[column][0]);
        auto y = vld1q_dup_f32(&rhs.values_[column][1]);
        auto z = vld1q_dup_f32(&rhs.values_[column][2]);
        auto w = vld1q_dup_f32(&rhs.values_[column][3]);
        x = vmulq_f32(c1, x);
        y = vmulq_f32(c2, y);
        z = vmulq_f32(c3, z);
        w = vmulq_f32(c4, w);

        x = vaddq_f32(x, y);
        z = vaddq_f32(z, w);
        x = vaddq_f32(x, z);
        vst1q_f32(&v[column][0], x);
    }
    return {v};
}

#endif



}
