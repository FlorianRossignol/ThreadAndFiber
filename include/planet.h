//
// Created by efarhan on 20/02/2021.
//

#pragma once

#include <vector>

#include "vector3.h"

class PlanetSystem
{
public:
    explicit PlanetSystem(std::size_t length);
    void Update(float dt);
private:
    std::vector<maths::Vec3f> positions_;
};

class PlanetSystem4
{
public:
    explicit PlanetSystem4(std::size_t length);
    void Update(float dt);
private:
    std::vector<maths::FourVec3f> positionsSOA_;
    std::vector<maths::Vec3f> positions_;
};

class PlanetSystem8
{
public:
    explicit PlanetSystem8(std::size_t length);
    void Update(float dt);
private:
    std::vector<maths::EightVec3f> positionsSOA_;
    std::vector<maths::Vec3f> positions_;
};