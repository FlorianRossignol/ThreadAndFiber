#pragma once

#include <cmath>


class Shape
{
public:
    Shape() = default;
    virtual ~Shape() = default;
    Shape(const Shape&) = default;
    Shape& operator=(const Shape& other) = default;
    Shape(Shape&& other) noexcept = default;
    Shape& operator=(Shape&& other) noexcept = default;

    [[nodiscard]] virtual float Area() const = 0;
    [[nodiscard]] virtual float Perimeter() const = 0;

};

class Circle final : public Shape
{
public:
    Circle(float radius) : radius_(radius)
    {
    }
    [[nodiscard]] float Area() const override
    {
        static constexpr float pi = M_PI;
        return radius_ * radius_ * pi;
    }
    [[nodiscard]] float Perimeter() const override
    {
        static constexpr float pi = M_PI;
        return 2.0f * radius_ * pi;
    }
private:
    float radius_ = 0.0f;
};

class Rect final : public Shape
{
public:
    Rect(float width, float height) : width_(width), height_(height)
    {

    }
    [[nodiscard]] float Area() const override
    {
        return width_ * height_;
    }
    [[nodiscard]] float Perimeter() const override
    {
        return 2.0f * (width_ + height_);
    }
private:
    float width_ = 0.0f;
    float height_ = 0.0f;
};