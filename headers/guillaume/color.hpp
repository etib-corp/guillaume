#pragma once

#include <cstdint>
#include <string>

namespace guigui {
class Color {
private:
    std::uint8_t _red;
    std::uint8_t _green;
    std::uint8_t _blue;
    std::uint8_t _alpha;

protected:
public:
    Color(std::uint8_t red = 0, std::uint8_t green = 0, std::uint8_t blue = 0, std::uint8_t alpha = 255)
        : _red(red)
        , _green(green)
        , _blue(blue)
        , _alpha(alpha)
    {
    }
    Color(const Color& other)
        : _red(other._red)
        , _green(other._green)
        , _blue(other._blue)
        , _alpha(other._alpha)
    {
    }
    ~Color() { }
    std::uint8_t get_red() const { return _red; }
    std::uint8_t get_green() const { return _green; }
    std::uint8_t get_blue() const { return _blue; }
    std::uint8_t get_alpha() const { return _alpha; }
    void set_red(std::uint8_t new_red) { _red = new_red; }
    void set_green(std::uint8_t new_green) { _green = new_green; }
    void set_blue(std::uint8_t new_blue) { _blue = new_blue; }
    void set_alpha(std::uint8_t new_alpha) { _alpha = new_alpha; }
    Color operator+(const Color& other) const
    {
        return Color(_red + other._red, _green + other._green, _blue + other._blue,
            _alpha + other._alpha);
    }
    Color operator-(const Color& other) const
    {
        return Color(_red - other._red, _green - other._green, _blue - other._blue,
            _alpha - other._alpha);
    }
    Color operator*(std::uint8_t scalar) const
    {
        return Color(_red * scalar, _green * scalar, _blue * scalar,
            _alpha * scalar);
    }
    Color operator/(std::uint8_t scalar) const
    {
        if (scalar != 0) {
            return Color(_red / scalar, _green / scalar, _blue / scalar,
                _alpha / scalar);
        }
        return *this; // Avoid division by zero
    }
    bool operator==(const Color& other) const
    {
        return (_red == other._red && _green == other._green && _blue == other._blue && _alpha == other._alpha);
    }
    bool operator!=(const Color& other) const { return !(*this == other); }
    Color& operator=(const Color& other)
    {
        if (this != &other) {
            _red = other._red;
            _green = other._green;
            _blue = other._blue;
            _alpha = other._alpha;
        }
        return *this;
    }
    Color& operator+=(const Color& other)
    {
        _red += other._red;
        _green += other._green;
        _blue += other._blue;
        _alpha += other._alpha;
        return *this;
    }
    Color& operator-=(const Color& other)
    {
        _red -= other._red;
        _green -= other._green;
        _blue -= other._blue;
        _alpha -= other._alpha;
        return *this;
    }
    Color& operator*=(std::uint8_t scalar)
    {
        _red *= scalar;
        _green *= scalar;
        _blue *= scalar;
        _alpha *= scalar;
        return *this;
    }
    Color& operator/=(std::uint8_t scalar)
    {
        if (scalar != 0) {
            _red /= scalar;
            _green /= scalar;
            _blue /= scalar;
            _alpha /= scalar;
        }
        return *this;
    }
    std::string to_string() const
    {
        return "Color(" + std::to_string(_red) + ", " + std::to_string(_green) + ", " + std::to_string(_blue) + ", " + std::to_string(_alpha) + ")";
    }
    std::uint32_t to_int() const
    {
        return (_red << 24) | (_green << 16) | (_blue << 8) | _alpha;
    }
    static Color from_int(std::uint32_t value)
    {
        return Color((value >> 24) & 0xFF, (value >> 16) & 0xFF,
            (value >> 8) & 0xFF, value & 0xFF);
    }
};
} // namespace guigui
