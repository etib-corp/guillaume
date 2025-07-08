#pragma once

#include <algorithm>
#include <cmath>
#include <string>

namespace guigui {

class Vector {
private:
    float x;
    float y;

public:
    Vector(float x = 0, float y = 0)
        : x(x)
        , y(y)
    {
    }
    Vector(const Vector& other)
        : x(other.x)
        , y(other.y)
    {
    }
    ~Vector() { }
    float get_x() const { return x; }
    float get_y() const { return y; }
    void set_x(float new_x) { x = new_x; }
    void set_y(float new_y) { y = new_y; }
    Vector operator+(const Vector& other) const
    {
        return Vector(x + other.x, y + other.y);
    }
    Vector operator-(const Vector& other) const
    {
        return Vector(x - other.x, y - other.y);
    }
    Vector operator*(float scalar) const { return Vector(x * scalar, y * scalar); }
    Vector operator/(float scalar) const { return Vector(x / scalar, y / scalar); }
    bool operator==(const Vector& other) const
    {
        return (x == other.x && y == other.y);
    }
    bool operator!=(const Vector& other) const { return !(*this == other); }
    Vector& operator=(const Vector& other)
    {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }
    Vector& operator+=(const Vector& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    Vector& operator-=(const Vector& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Vector& operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    Vector& operator/=(float scalar)
    {
        if (scalar != 0) {
            x /= scalar;
            y /= scalar;
        }
        return *this;
    }
    float length() const
    {
        return static_cast<float>(sqrt(static_cast<double>(x * x + y * y)));
    }
    Vector normalized() const
    {
        float len = length();
        if (len == 0)
            return Vector(0, 0);
        return Vector(x / len, y / len);
    }
    Vector clamp(float minVal, float maxVal) const
    {
        return Vector(std::max(minVal, std::min(maxVal, x)),
            std::max(minVal, std::min(maxVal, y))

        );
    }
    Vector abs() const { return Vector(std::abs(x), std::abs(y)); }
    Vector floor() const
    {
        return Vector(static_cast<float>(std::floor(x)),
            static_cast<float>(std::floor(y)));
    }
    Vector ceil() const
    {
        return Vector(static_cast<float>(std::ceil(x)),
            static_cast<float>(std::ceil(y)));
    }
    Vector round() const
    {
        return Vector(static_cast<float>(std::round(x)),
            static_cast<float>(std::round(y)));
    }
    Vector min(const Vector& other) const
    {
        return Vector(std::min(x, other.x), std::min(y, other.y));
    }
    Vector max(const Vector& other) const
    {
        return Vector(std::max(x, other.x), std::max(y, other.y));
    }
    Vector operator-() const { return Vector(-x, -y); }
    std::string to_string() const
    {
        return "Vector(" + std::to_string(x) + ", " + std::to_string(y) + ")";
    }
};

} // namespace guigui
