#include "guillaume/color.hpp"
#include <string>

namespace guigui {

Color::Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha)
    : _red(red)
    , _green(green)
    , _blue(blue)
    , _alpha(alpha)
{
}

Color::Color(const Color& other)
    : _red(other._red)
    , _green(other._green)
    , _blue(other._blue)
    , _alpha(other._alpha)
{
}

Color::~Color()
{
}

std::uint8_t Color::get_red() const
{
    return _red;
}

std::uint8_t Color::get_green() const
{
    return _green;
}

std::uint8_t Color::get_blue() const
{
    return _blue;
}

std::uint8_t Color::get_alpha() const
{
    return _alpha;
}

void Color::set_red(std::uint8_t new_red)
{
    _red = new_red;
}

void Color::set_green(std::uint8_t new_green)
{
    _green = new_green;
}

void Color::set_blue(std::uint8_t new_blue)
{
    _blue = new_blue;
}

void Color::set_alpha(std::uint8_t new_alpha)
{
    _alpha = new_alpha;
}

Color Color::operator+(const Color& other) const
{
    return Color(_red + other._red, _green + other._green, _blue + other._blue, _alpha + other._alpha);
}

Color Color::operator-(const Color& other) const
{
    return Color(_red - other._red, _green - other._green, _blue - other._blue, _alpha - other._alpha);
}

Color Color::operator*(std::uint8_t scalar) const
{
    return Color(_red * scalar, _green * scalar, _blue * scalar, _alpha * scalar);
}

Color Color::operator/(std::uint8_t scalar) const
{
    if (scalar != 0) {
        return Color(_red / scalar, _green / scalar, _blue / scalar, _alpha / scalar);
    }
    return *this; // Avoid division by zero
}

bool Color::operator==(const Color& other) const
{
    return (_red == other._red && _green == other._green && _blue == other._blue && _alpha == other._alpha);
}

bool Color::operator!=(const Color& other) const
{
    return !(*this == other);
}

Color& Color::operator=(const Color& other)
{
    if (this != &other) {
        _red = other._red;
        _green = other._green;
        _blue = other._blue;
        _alpha = other._alpha;
    }
    return *this;
}

Color& Color::operator+=(const Color& other)
{
    _red += other._red;
    _green += other._green;
    _blue += other._blue;
    _alpha += other._alpha;
    return *this;
}

Color& Color::operator-=(const Color& other)
{
    _red -= other._red;
    _green -= other._green;
    _blue -= other._blue;
    _alpha -= other._alpha;
    return *this;
}

Color& Color::operator*=(std::uint8_t scalar)
{
    _red *= scalar;
    _green *= scalar;
    _blue *= scalar;
    _alpha *= scalar;
    return *this;
}

Color& Color::operator/=(std::uint8_t scalar)
{
    if (scalar != 0) {
        _red /= scalar;
        _green /= scalar;
        _blue /= scalar;
        _alpha /= scalar;
    }
    return *this;
}

std::string Color::to_string() const
{
    return "Color(" + std::to_string(_red) + "r, " + std::to_string(_green) + "g, " + std::to_string(_blue) + "b, " + std::to_string(_alpha) + "a)";
}

std::uint32_t Color::to_int() const
{
    return (_red << 24) | (_green << 16) | (_blue << 8) | _alpha;
}

Color Color::from_int(std::uint32_t value)
{
    return Color((value >> 24) & 0xFF, (value >> 16) & 0xFF, (value >> 8) & 0xFF, value & 0xFF);
}

} // namespace guigui
