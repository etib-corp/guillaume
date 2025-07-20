#pragma once

#include "guillaume/vector.hpp"

namespace guigui {

class Rectangle {
private:
    Vector _position;
    Vector _size;

protected:
public:
    Rectangle(const Vector& position = Vector(0, 0), const Vector& size = Vector(0, 0));
    Rectangle(int x, int y, int width, int height);
    Rectangle(const Rectangle& other);
    ~Rectangle();
    
    Vector get_position() const;
    Vector get_size() const;
    void set_position(const Vector& new_position);
    void set_size(const Vector& new_size);
    
    int get_x() const;
    int get_y() const;
    int get_width() const;
    int get_height() const;
    void set_x(int new_x);
    void set_y(int new_y);
    void set_width(int new_width);
    void set_height(int new_height);
    
    Rectangle operator+(const Rectangle& other) const;
    Rectangle operator-(const Rectangle& other) const;
    Rectangle operator*(int scalar) const;
    Rectangle operator/(int scalar) const;
    
    bool operator==(const Rectangle& other) const;
    bool operator!=(const Rectangle& other) const;
    
    Rectangle& operator=(const Rectangle& other);
    Rectangle& operator+=(const Rectangle& other);
    Rectangle& operator-=(const Rectangle& other);
    Rectangle& operator*=(int scalar);
    Rectangle& operator/=(int scalar);
    
    int area() const;
    int perimeter() const;
    Rectangle normalized() const;
    Rectangle clamp(int minVal, int maxVal) const;
    Rectangle abs() const;
    Rectangle floor() const;
    Rectangle ceil() const;
    Rectangle round() const;
    Rectangle min(const Rectangle& other) const;
    Rectangle max(const Rectangle& other) const;
    Rectangle operator-() const;
    std::string to_string() const;
    
    bool contains(const Vector& point) const;
    bool intersects(const Rectangle& other) const;
    Rectangle intersection(const Rectangle& other) const;
    Rectangle union_rect(const Rectangle& other) const;
};
} // namespace guigui
