//
// Created by efarhan on 2/22/21.
//

#include "matrix3.h"

namespace maths
{

float Mat3f::Determinant() const
{
    return
            values_[0][0] * values_[1][1] * values_[2][2] +
            values_[1][0] * values_[2][1] * values_[0][2] +
            values_[2][0] * values_[0][1] * values_[1][2] -
            values_[2][0] * values_[1][1] * values_[0][2] -
            values_[0][0] * values_[2][1] * values_[1][2] -
            values_[1][0] * values_[0][1] * values_[2][2];
}

float Mat3f::DeterminantOpti() const {
    return values_[0][0]*(values_[1][1]*values_[2][2]-values_[1][2]*values_[2][1])-
        values_[1][0]*(values_[0][1]*values_[2][2]-values_[2][1]*values_[0][2])+
        values_[2][0]*(values_[0][1]*values_[1][2]-values_[1][1]*values_[0][2]);
}
}