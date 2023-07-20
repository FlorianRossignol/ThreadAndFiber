//
// Created by efarhan on 2/22/21.
//

#pragma once

#include <cmath>
#include <array>
#include <vector>

namespace maths
{
inline float Abs(float v)
{
    return std::abs(v);
}

inline bool Equal(float a, float b, float epsilon = 0.0000001f) {
    return Abs(a - b) < epsilon;
}
template<typename T>
float Mod(T a, T b)
{
    return a<b?a:a%b;
}

constexpr bool is_prime_naive(unsigned number)
{
    if (number < 2)
        return false;
    if(number == 2)
    {
        return true;
    }
    for(unsigned n = 3; n*n < number; n++)
    {
        if(number%n == 0)
        {
            return false;
        }
    }
    return true;
}

std::vector<unsigned> generate_prime_under(unsigned number)
{
    std::vector<unsigned> prime_numbers;
    prime_numbers.push_back(2u);
    for(unsigned n = 3; n < number; n+=2)
    {
        if(is_prime_naive(n))
        {
            prime_numbers.push_back(n);
        }
    }
    return prime_numbers;
}
}
