#include "vector4.h"

namespace maths
{
Vec4f Vec4f::operator+(const Vec4f &v) const
{
    const Vec4f result(x+v.x, y+v.y, z+v.z, w+v.w);
    return result;
}
}