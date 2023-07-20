#pragma once

#include <vector>

#include "vector3.h"
#include "matrix4.h"

#include <random>


void FillRandom(maths::Vec3f& v);
template<std::size_t N>
void FillRandom(maths::NVec3f<N>& v)
{
    static std::random_device rd;  //Will be used to obtain a seed for the random number engine
    static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    static std::uniform_real_distribution<float> dis(-100.0f, 100.0f);
    for(auto& xs : v.Xs())
    {
        xs = dis(gen);
    }
    for(auto& ys : v.Ys())
    {
        ys = dis(gen);
    }
    for(auto& zs : v.Zs())
    {
        zs = dis(gen);
    }
}
void FillRandom(maths::Mat4f& m);
void FillRandom(std::vector<int>& v, int low, int high);