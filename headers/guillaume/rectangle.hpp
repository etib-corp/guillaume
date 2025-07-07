#pragma once

#include "guillaume/vector.hpp"

namespace guillaume {

class Rectangle {
private:
    Vector _position;
    Vector _size;

protected:
public:
    Rectangle(const Vector& position = Vector(0, 0),
        const Vector& size = Vector(0, 0))
        : _position(position)
        , _size(size)
    {
    }
    Rectangle(int x, int y, int width, int height)
        : _position(Vector(x, y))
        , _size(Vector(width, height))
    {
    }
    Rectangle(const Rectangle& other)
        : _position(other._position)
        , _size(other._size)
    {
    }
    ~Rectangle() { }
    Vector get_position() const { return _position; }
    Vector get_size() const { return _size; }
    void set_position(const Vector& new_position) { _position = new_position; }
    void set_size(const Vector& new_size) { _size = new_size; }
    int get_x() const { return _position.get_x(); }
    int get_y() const { return _position.get_y(); }
    int get_width() const { return _size.get_x(); }
    int get_height() const { return _size.get_y(); }
    void set_x(int new_x) { _position.set_x(new_x); }
    void set_y(int new_y) { _position.set_y(new_y); }
    void set_width(int new_width) { _size.set_x(new_width); }
    void set_height(int new_height) { _size.set_y(new_height); }
    Rectangle operator+(const Rectangle& other) const
    {
        return Rectangle(_position + other._position, _size + other._size);
    }
    Rectangle operator-(const Rectangle& other) const
    {
        return Rectangle(_position - other._position, _size - other._size);
    }
    Rectangle operator*(int scalar) const
    {
        return Rectangle(_position * scalar, _size * scalar);
    }
    Rectangle operator/(int scalar) const
    {
        return Rectangle(_position / scalar, _size / scalar);
    }
    bool operator==(const Rectangle& other) const
    {
        return (_position == other._position && _size == other._size);
    }
    bool operator!=(const Rectangle& other) const { return !(*this == other); }
    Rectangle& operator=(const Rectangle& other)
    {
        if (this != &other) {
            _position = other._position;
            _size = other._size;
        }
        return *this;
    }
    Rectangle& operator+=(const Rectangle& other)
    {
        _position += other._position;
        _size += other._size;
        return *this;
    }
    Rectangle& operator-=(const Rectangle& other)
    {
        _position -= other._position;
        _size -= other._size;
        return *this;
    }
    Rectangle& operator*=(int scalar)
    {
        _position *= scalar;
        _size *= scalar;
        return *this;
    }
    Rectangle& operator/=(int scalar)
    {
        if (scalar != 0) {
            _position /= scalar;
            _size /= scalar;
        }
        return *this;
    }
    int area() const { return _size.get_x() * _size.get_y(); }
    int perimeter() const { return 2 * (_size.get_x() + _size.get_y()); }
    Rectangle normalized() const
    {
        int len = _size.length();
        if (len == 0)
            return Rectangle(Vector(0, 0), Vector(0, 0));
        return Rectangle(_position / len, _size / len);
    }
    Rectangle clamp(int minVal, int maxVal) const
    {
        return Rectangle(_position.clamp(minVal, maxVal),
            _size.clamp(minVal, maxVal));
    }
    Rectangle abs() const { return Rectangle(_position.abs(), _size.abs()); }
    Rectangle floor() const
    {
        return Rectangle(_position.floor(), _size.floor());
    }
    Rectangle ceil() const { return Rectangle(_position.ceil(), _size.ceil()); }
    Rectangle round() const
    {
        return Rectangle(_position.round(), _size.round());
    }
    Rectangle min(const Rectangle& other) const
    {
        return Rectangle(_position.min(other._position), _size.min(other._size));
    }
    Rectangle max(const Rectangle& other) const
    {
        return Rectangle(_position.max(other._position), _size.max(other._size));
    }
    Rectangle operator-() const { return Rectangle(-_position, -_size); }
    std::string to_string() const
    {
        return "Rectangle(" + _position.to_string() + ", " + _size.to_string() + ")";
    }
    bool contains(const Vector& point) const
    {
        return (point.get_x() >= _position.get_x() && point.get_x() < _position.get_x() + _size.get_x() && point.get_y() >= _position.get_y() && point.get_y() < _position.get_y() + _size.get_y());
    }
    bool intersects(const Rectangle& other) const
    {
        return !(
            _position.get_x() + _size.get_x() <= other._position.get_x() || _position.get_x() >= other._position.get_x() + other._size.get_x() || _position.get_y() + _size.get_y() <= other._position.get_y() || _position.get_y() >= other._position.get_y() + other._size.get_y());
    }
    Rectangle intersection(const Rectangle& other) const
    {
        if (!intersects(other)) {
            return Rectangle(Vector(0, 0), Vector(0, 0));
        }
        int x1 = std::max(_position.get_x(), other._position.get_x());
        int y1 = std::max(_position.get_y(), other._position.get_y());
        int x2 = std::min(_position.get_x() + _size.get_x(),
            other._position.get_x() + other._size.get_x());
        int y2 = std::min(_position.get_y() + _size.get_y(),
            other._position.get_y() + other._size.get_y());
        return Rectangle(Vector(x1, y1), Vector(x2 - x1, y2 - y1));
    }
    Rectangle union_rect(const Rectangle& other) const
    {
        int x1 = std::min(_position.get_x(), other._position.get_x());
        int y1 = std::min(_position.get_y(), other._position.get_y());
        int x2 = std::max(_position.get_x() + _size.get_x(),
            other._position.get_x() + other._size.get_x());
        int y2 = std::max(_position.get_y() + _size.get_y(),
            other._position.get_y() + other._size.get_y());
        return Rectangle(Vector(x1, y1), Vector(x2 - x1, y2 - y1));
    }
};
} // namespace guillaume
