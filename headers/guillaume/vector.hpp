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
    Vector(float x = 0, float y = 0);
    Vector(const Vector& other);
    ~Vector();
    
    float get_x() const;
    float get_y() const;
    void set_x(float new_x);
    void set_y(float new_y);
    
    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator*(float scalar) const;
    Vector operator/(float scalar) const;
    
    bool operator==(const Vector& other) const;
    bool operator!=(const Vector& other) const;
    
    Vector& operator=(const Vector& other);
    Vector& operator+=(const Vector& other);
    Vector& operator-=(const Vector& other);
    Vector& operator*=(float scalar);
    Vector& operator/=(float scalar);
    
    float length() const;
    Vector normalized() const;
    Vector clamp(float minVal, float maxVal) const;
    Vector abs() const;
    Vector floor() const;
    Vector ceil() const;
    Vector round() const;
    Vector min(const Vector& other) const;
    Vector max(const Vector& other) const;
    Vector operator-() const;
    std::string to_string() const;
};

} // namespace guigui
