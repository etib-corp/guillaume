#include "guillaume/rectangle.hpp"
#include <algorithm>

namespace guigui {

Rectangle::Rectangle(const Vector& position, const Vector& size)
    : _position(position)
    , _size(size)
{
}

Rectangle::Rectangle(int x, int y, int width, int height)
    : _position(Vector(x, y))
    , _size(Vector(width, height))
{
}

Rectangle::Rectangle(const Rectangle& other)
    : _position(other._position)
    , _size(other._size)
{
}

Rectangle::~Rectangle() 
{
}

Vector Rectangle::get_position() const 
{ 
    return _position; 
}

Vector Rectangle::get_size() const 
{ 
    return _size; 
}

void Rectangle::set_position(const Vector& new_position) 
{ 
    _position = new_position; 
}

void Rectangle::set_size(const Vector& new_size) 
{ 
    _size = new_size; 
}

int Rectangle::get_x() const 
{ 
    return _position.get_x(); 
}

int Rectangle::get_y() const 
{ 
    return _position.get_y(); 
}

int Rectangle::get_width() const 
{ 
    return _size.get_x(); 
}

int Rectangle::get_height() const 
{ 
    return _size.get_y(); 
}

void Rectangle::set_x(int new_x) 
{ 
    _position.set_x(new_x); 
}

void Rectangle::set_y(int new_y) 
{ 
    _position.set_y(new_y); 
}

void Rectangle::set_width(int new_width) 
{ 
    _size.set_x(new_width); 
}

void Rectangle::set_height(int new_height) 
{ 
    _size.set_y(new_height); 
}

Rectangle Rectangle::operator+(const Rectangle& other) const
{
    return Rectangle(_position + other._position, _size + other._size);
}

Rectangle Rectangle::operator-(const Rectangle& other) const
{
    return Rectangle(_position - other._position, _size - other._size);
}

Rectangle Rectangle::operator*(int scalar) const
{
    return Rectangle(_position * scalar, _size * scalar);
}

Rectangle Rectangle::operator/(int scalar) const
{
    return Rectangle(_position / scalar, _size / scalar);
}

bool Rectangle::operator==(const Rectangle& other) const
{
    return (_position == other._position && _size == other._size);
}

bool Rectangle::operator!=(const Rectangle& other) const 
{ 
    return !(*this == other); 
}

Rectangle& Rectangle::operator=(const Rectangle& other)
{
    if (this != &other) {
        _position = other._position;
        _size = other._size;
    }
    return *this;
}

Rectangle& Rectangle::operator+=(const Rectangle& other)
{
    _position += other._position;
    _size += other._size;
    return *this;
}

Rectangle& Rectangle::operator-=(const Rectangle& other)
{
    _position -= other._position;
    _size -= other._size;
    return *this;
}

Rectangle& Rectangle::operator*=(int scalar)
{
    _position *= scalar;
    _size *= scalar;
    return *this;
}

Rectangle& Rectangle::operator/=(int scalar)
{
    if (scalar != 0) {
        _position /= scalar;
        _size /= scalar;
    }
    return *this;
}

int Rectangle::area() const 
{ 
    return _size.get_x() * _size.get_y(); 
}

int Rectangle::perimeter() const 
{ 
    return 2 * (_size.get_x() + _size.get_y()); 
}

Rectangle Rectangle::normalized() const
{
    int len = _size.length();
    if (len == 0)
        return Rectangle(Vector(0, 0), Vector(0, 0));
    return Rectangle(_position / len, _size / len);
}

Rectangle Rectangle::clamp(int minVal, int maxVal) const
{
    return Rectangle(_position.clamp(minVal, maxVal), _size.clamp(minVal, maxVal));
}

Rectangle Rectangle::abs() const 
{ 
    return Rectangle(_position.abs(), _size.abs()); 
}

Rectangle Rectangle::floor() const
{
    return Rectangle(_position.floor(), _size.floor());
}

Rectangle Rectangle::ceil() const 
{ 
    return Rectangle(_position.ceil(), _size.ceil()); 
}

Rectangle Rectangle::round() const
{
    return Rectangle(_position.round(), _size.round());
}

Rectangle Rectangle::min(const Rectangle& other) const
{
    return Rectangle(_position.min(other._position), _size.min(other._size));
}

Rectangle Rectangle::max(const Rectangle& other) const
{
    return Rectangle(_position.max(other._position), _size.max(other._size));
}

Rectangle Rectangle::operator-() const 
{ 
    return Rectangle(-_position, -_size); 
}

std::string Rectangle::to_string() const
{
    return "Rectangle(" + _position.to_string() + ", " + _size.to_string() + ")";
}

bool Rectangle::contains(const Vector& point) const
{
    return (point.get_x() >= _position.get_x() && 
            point.get_x() < _position.get_x() + _size.get_x() && 
            point.get_y() >= _position.get_y() && 
            point.get_y() < _position.get_y() + _size.get_y());
}

bool Rectangle::intersects(const Rectangle& other) const
{
    return !(_position.get_x() + _size.get_x() <= other._position.get_x() || 
             _position.get_x() >= other._position.get_x() + other._size.get_x() || 
             _position.get_y() + _size.get_y() <= other._position.get_y() || 
             _position.get_y() >= other._position.get_y() + other._size.get_y());
}

Rectangle Rectangle::intersection(const Rectangle& other) const
{
    if (!intersects(other)) {
        return Rectangle(Vector(0, 0), Vector(0, 0));
    }
    int x1 = std::max(_position.get_x(), other._position.get_x());
    int y1 = std::max(_position.get_y(), other._position.get_y());
    int x2 = std::min(_position.get_x() + _size.get_x(), other._position.get_x() + other._size.get_x());
    int y2 = std::min(_position.get_y() + _size.get_y(), other._position.get_y() + other._size.get_y());
    return Rectangle(Vector(x1, y1), Vector(x2 - x1, y2 - y1));
}

Rectangle Rectangle::union_rect(const Rectangle& other) const
{
    int x1 = std::min(_position.get_x(), other._position.get_x());
    int y1 = std::min(_position.get_y(), other._position.get_y());
    int x2 = std::max(_position.get_x() + _size.get_x(), other._position.get_x() + other._size.get_x());
    int y2 = std::max(_position.get_y() + _size.get_y(), other._position.get_y() + other._size.get_y());
    return Rectangle(Vector(x1, y1), Vector(x2 - x1, y2 - y1));
}

} // namespace guigui
