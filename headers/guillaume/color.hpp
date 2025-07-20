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
    Color(std::uint8_t red = 0, std::uint8_t green = 0, std::uint8_t blue = 0, std::uint8_t alpha = 255);
    Color(const Color& other);
    ~Color();
    
    std::uint8_t get_red() const;
    std::uint8_t get_green() const;
    std::uint8_t get_blue() const;
    std::uint8_t get_alpha() const;
    
    void set_red(std::uint8_t new_red);
    void set_green(std::uint8_t new_green);
    void set_blue(std::uint8_t new_blue);
    void set_alpha(std::uint8_t new_alpha);
    
    Color operator+(const Color& other) const;
    Color operator-(const Color& other) const;
    Color operator*(std::uint8_t scalar) const;
    Color operator/(std::uint8_t scalar) const;
    
    bool operator==(const Color& other) const;
    bool operator!=(const Color& other) const;
    
    Color& operator=(const Color& other);
    Color& operator+=(const Color& other);
    Color& operator-=(const Color& other);
    Color& operator*=(std::uint8_t scalar);
    Color& operator/=(std::uint8_t scalar);
    
    std::string to_string() const;
    std::uint32_t to_int() const;
    static Color from_int(std::uint32_t value);
};
} // namespace guigui
