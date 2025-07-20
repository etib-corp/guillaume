#include "guillaume/vector.hpp"
#include <algorithm>
#include <cmath>
#include <string>

namespace guigui {

Vector::Vector(float x, float y)
    : x(x)
    , y(y)
{
}

Vector::Vector(const Vector& other)
    : x(other.x)
    , y(other.y)
{
}

Vector::~Vector() 
{
}

float Vector::get_x() const 
{ 
    return x; 
}

float Vector::get_y() const 
{ 
    return y; 
}

void Vector::set_x(float new_x) 
{ 
    x = new_x; 
}

void Vector::set_y(float new_y) 
{ 
    y = new_y; 
}

Vector Vector::operator+(const Vector& other) const
{
    return Vector(x + other.x, y + other.y);
}

Vector Vector::operator-(const Vector& other) const
{
    return Vector(x - other.x, y - other.y);
}

Vector Vector::operator*(float scalar) const 
{ 
    return Vector(x * scalar, y * scalar); 
}

Vector Vector::operator/(float scalar) const 
{ 
    return Vector(x / scalar, y / scalar); 
}

bool Vector::operator==(const Vector& other) const
{
    return (x == other.x && y == other.y);
}

bool Vector::operator!=(const Vector& other) const 
{ 
    return !(*this == other); 
}

Vector& Vector::operator=(const Vector& other)
{
    if (this != &other) {
        x = other.x;
        y = other.y;
    }
    return *this;
}

Vector& Vector::operator+=(const Vector& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Vector& Vector::operator-=(const Vector& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector& Vector::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector& Vector::operator/=(float scalar)
{
    if (scalar != 0) {
        x /= scalar;
        y /= scalar;
    }
    return *this;
}

float Vector::length() const
{
    return static_cast<float>(sqrt(static_cast<double>(x * x + y * y)));
}

Vector Vector::normalized() const
{
    float len = length();
    if (len == 0)
        return Vector(0, 0);
    return Vector(x / len, y / len);
}

Vector Vector::clamp(float minVal, float maxVal) const
{
    return Vector(std::max(minVal, std::min(maxVal, x)), std::max(minVal, std::min(maxVal, y)));
}

Vector Vector::abs() const 
{ 
    return Vector(std::abs(x), std::abs(y)); 
}

Vector Vector::floor() const
{
    return Vector(static_cast<float>(std::floor(x)), static_cast<float>(std::floor(y)));
}

Vector Vector::ceil() const
{
    return Vector(static_cast<float>(std::ceil(x)), static_cast<float>(std::ceil(y)));
}

Vector Vector::round() const
{
    return Vector(static_cast<float>(std::round(x)), static_cast<float>(std::round(y)));
}

Vector Vector::min(const Vector& other) const
{
    return Vector(std::min(x, other.x), std::min(y, other.y));
}

Vector Vector::max(const Vector& other) const
{
    return Vector(std::max(x, other.x), std::max(y, other.y));
}

Vector Vector::operator-() const 
{ 
    return Vector(-x, -y); 
}

std::string Vector::to_string() const
{
    return "Vector(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}

} // namespace guigui
