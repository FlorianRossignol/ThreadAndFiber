//
// Created by efarhan on 20/02/2021.
//

#include "planet.h"
#include <random>

#include <cmath>


const float gravityConst = 1000.0f;
const float centerMass = 1000.0f;
const float asteroidMass = 1.0f;

static void PlanetInitPosition(maths::Vec3f& pos)
{
    static std::random_device rd;  //Will be used to obtain a seed for the random number engine
    static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    static std::uniform_real_distribution<float> dis(50.0f, 300.0f);
    static std::uniform_real_distribution<float> disY(-5.0f, 5.0f);
    static std::uniform_real_distribution<float> disAngle(0.0f, 2.0f*M_PI);

    const auto dist = dis(gen);
    const auto angle = disAngle(gen);
    pos = maths::Vec3f(std::cos(angle)*dist, disY(gen), std::sin(angle)*dist);
}

PlanetSystem::PlanetSystem(std::size_t length)
{
    positions_.resize(length);
    for(auto& pos: positions_)
    {
        PlanetInitPosition(pos);
    }
}

void PlanetSystem::Update(float dt)
{
    for(auto & position : positions_)
    {
        const auto deltaToCenter = maths::Vec3f() - position;
        const auto r = deltaToCenter.Magnitude();
        const auto forceScalar = gravityConst * centerMass * asteroidMass / (r * r);
        const auto forceVector = deltaToCenter / r * forceScalar;
        auto velDir = maths::Vec3f(-deltaToCenter.z, 0.0f, deltaToCenter.x);
        velDir = velDir.Normalized();

        const auto speed = std::sqrt(forceVector.Magnitude() / asteroidMass * deltaToCenter.Magnitude());
        const auto velocity = velDir * speed;
        position = position + velocity * dt;
    }
}

PlanetSystem4::PlanetSystem4(std::size_t length)
{
    positions_.resize(length);
    for(auto& pos: positions_)
    {
        PlanetInitPosition(pos);
    }
    positionsSOA_.resize(length/4);
    for(std::size_t i = 0; i < length/4; i++)
    {
        positionsSOA_[i] = maths::FourVec3f(&positions_[i*4]);
    }
}

void PlanetSystem4::Update(float dt)
{
    for(std::size_t i = 0; i < positionsSOA_.size(); i++)
    {
        const auto sunPos = maths::FourVec3f(maths::Vec3f());
        const auto deltaToCenter = sunPos - positionsSOA_[i];
        const auto r = deltaToCenter.Magnitude();
        const auto forceScalar = maths::FourFloat::Reciprocal(r*r) *
            (gravityConst * centerMass * asteroidMass);
        const auto forceVector = deltaToCenter / r * forceScalar;
        auto velDir = maths::FourVec3f(-(deltaToCenter.Zs()), maths::FourFloat(), deltaToCenter.Xs());
        velDir = velDir.Normalized();

        const auto speed = maths::FourFloat::Sqrt(forceVector.Magnitude()*deltaToCenter.Magnitude()*(
            1.0f / asteroidMass));
        const auto velocity = velDir * speed;
        positionsSOA_[i] = positionsSOA_[i] + velocity * dt;
        // Putting back the SoA in AoS format
        const auto pos = positionsSOA_[i].vectors();
        for(std::size_t j = 0; j < pos.size(); j++)
        {
            positions_[i*4+j] = pos[j];
        }
    }
}

PlanetSystem8::PlanetSystem8(std::size_t length)
{
    positions_.resize(length);
    for(auto& pos: positions_)
    {
        PlanetInitPosition(pos);
    }
    positionsSOA_.resize(length/8);
    for(std::size_t i = 0; i < length/8; i++)
    {
        positionsSOA_[i] = maths::EightVec3f(&positions_[i*8]);
    }
}

void PlanetSystem8::Update(float dt)
{
    for(std::size_t i = 0; i < positionsSOA_.size(); i++)
    {
        const auto sunPos = maths::EightVec3f(maths::Vec3f());
        const auto deltaToCenter = sunPos - positionsSOA_[i];
        const auto r = deltaToCenter.Magnitude();
        const auto forceScalar = maths::EightFloat::Reciprocal(r*r)*(
            gravityConst * centerMass * asteroidMass);
        const auto forceVector = deltaToCenter / r * forceScalar;
        auto velDir = maths::EightVec3f(-deltaToCenter.Zs(), maths::EightFloat(), deltaToCenter.Xs());
        velDir = velDir.Normalized();

        const auto speed = maths::EightFloat::Sqrt(forceVector.Magnitude()*deltaToCenter.Magnitude()*
            (1.0f / asteroidMass));
        const auto velocity = velDir * speed;
        positionsSOA_[i] = positionsSOA_[i] + velocity * dt;
        // Putting back the SoA in AoS format
        const auto pos = positionsSOA_[i].vectors();
        for(std::size_t j = 0; j < pos.size(); j++)
        {
            positions_[i*8+j] = pos[j];
        }
    }
}
