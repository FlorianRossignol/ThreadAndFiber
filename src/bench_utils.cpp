//
// Created by efarhan on 19/02/2021.
//

#include "bench_utils.h"

#include <random>

void FillRandom(maths::Vec3f& v)
{
    static std::random_device rd;  //Will be used to obtain a seed for the random number engine
    static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    static std::uniform_real_distribution<float> dis(-100.0f, 100.0f);
    v[0] = dis(gen);
    v[1] = dis(gen);
    v[2] = dis(gen);
}

void FillRandom(std::vector<int> &v, int low, int high)
{
    static std::random_device rd;  //Will be used to obtain a seed for the random number engine
    static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<int> dis(low, high);
    std::for_each(v.begin(), v.end(), [&dis](int& n){n = dis(gen);});
}

void FillRandom(maths::Mat4f &m)
{
    static std::random_device rd;  //Will be used to obtain a seed for the random number engine
    static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    static std::uniform_real_distribution<float> dis(-100.0f, 100.0f);
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            m[i][j] = dis(gen);
        }
    }
}
