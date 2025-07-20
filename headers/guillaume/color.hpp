/**
 * @file color.hpp
 * @brief Color representation class for the Guillaume GUI framework
 * 
 * This file contains the Color class which provides RGBA color representation
 * and manipulation functionality for the Guillaume framework.
 */

#pragma once

#include <cstdint>
#include <string>

namespace guigui {

/**
 * @class Color
 * @brief RGBA color representation with 8-bit components
 * 
 * The Color class provides a convenient way to represent and manipulate colors
 * using red, green, blue, and alpha (transparency) components. Each component
 * is stored as an 8-bit unsigned integer (0-255).
 * 
 * @details Features:
 * - RGBA color representation with 8-bit precision
 * - Arithmetic operations for color manipulation
 * - Comparison operations for color matching
 * - String and integer conversion utilities
 * - Copy constructor and assignment operators
 * 
 * @note Alpha value of 255 means fully opaque, 0 means fully transparent
 */
class Color {
private:
    std::uint8_t _red;    ///< Red component (0-255)
    std::uint8_t _green;  ///< Green component (0-255)
    std::uint8_t _blue;   ///< Blue component (0-255)
    std::uint8_t _alpha;  ///< Alpha/transparency component (0-255, 255 = opaque)

protected:
public:
    /**
     * @brief Constructs a Color with specified RGBA components
     * 
     * @param red Red component value (0-255, default: 0)
     * @param green Green component value (0-255, default: 0)
     * @param blue Blue component value (0-255, default: 0)
     * @param alpha Alpha component value (0-255, default: 255 for opaque)
     * 
     * @details Creates a color with the specified component values.
     * Default parameters create an opaque black color.
     */
    Color(std::uint8_t red = 0, std::uint8_t green = 0, std::uint8_t blue = 0, std::uint8_t alpha = 255);
    
    /**
     * @brief Copy constructor
     * 
     * @param other Color to copy from
     * 
     * Creates a new Color with the same RGBA values as the source color.
     */
    Color(const Color& other);
    
    /**
     * @brief Destructor
     * 
     * Cleans up the Color object (no special cleanup needed for this class).
     */
    ~Color();
    
    // Component getters
    
    /**
     * @brief Gets the red component value
     * 
     * @return std::uint8_t Red component (0-255)
     */
    std::uint8_t get_red() const;
    
    /**
     * @brief Gets the green component value
     * 
     * @return std::uint8_t Green component (0-255)
     */
    std::uint8_t get_green() const;
    
    /**
     * @brief Gets the blue component value
     * 
     * @return std::uint8_t Blue component (0-255)
     */
    std::uint8_t get_blue() const;
    
    /**
     * @brief Gets the alpha component value
     * 
     * @return std::uint8_t Alpha component (0-255, 255 = opaque)
     */
    std::uint8_t get_alpha() const;
    
    // Component setters
    
    /**
     * @brief Sets the red component value
     * 
     * @param new_red New red component value (0-255)
     */
    void set_red(std::uint8_t new_red);
    
    /**
     * @brief Sets the green component value
     * 
     * @param new_green New green component value (0-255)
     */
    void set_green(std::uint8_t new_green);
    
    /**
     * @brief Sets the blue component value
     * 
     * @param new_blue New blue component value (0-255)
     */
    void set_blue(std::uint8_t new_blue);
    
    /**
     * @brief Sets the alpha component value
     * 
     * @param new_alpha New alpha component value (0-255)
     */
    void set_alpha(std::uint8_t new_alpha);
    
    // Arithmetic operators
    
    /**
     * @brief Adds two colors component-wise
     * 
     * @param other Color to add
     * @return Color Result of component-wise addition (clamped to 255)
     * 
     * @details Adds corresponding components and clamps results to valid range.
     */
    Color operator+(const Color& other) const;
    
    /**
     * @brief Subtracts two colors component-wise
     * 
     * @param other Color to subtract
     * @return Color Result of component-wise subtraction (clamped to 0)
     * 
     * @details Subtracts corresponding components and clamps results to valid range.
     */
    Color operator-(const Color& other) const;
    
    /**
     * @brief Multiplies color by a scalar value
     * 
     * @param scalar Value to multiply each component by
     * @return Color Result of scalar multiplication (clamped to 255)
     * 
     * @details Multiplies each component by the scalar and clamps to valid range.
     */
    Color operator*(std::uint8_t scalar) const;
    
    /**
     * @brief Divides color by a scalar value
     * 
     * @param scalar Value to divide each component by
     * @return Color Result of scalar division
     * 
     * @details Divides each component by the scalar. Dividing by zero is undefined.
     */
    Color operator/(std::uint8_t scalar) const;
    
    // Comparison operators
    
    /**
     * @brief Checks if two colors are equal
     * 
     * @param other Color to compare with
     * @return bool True if all components are equal, false otherwise
     */
    bool operator==(const Color& other) const;
    
    /**
     * @brief Checks if two colors are not equal
     * 
     * @param other Color to compare with
     * @return bool True if any component differs, false otherwise
     */
    bool operator!=(const Color& other) const;
    
    // Assignment operators
    
    /**
     * @brief Assignment operator
     * 
     * @param other Color to copy from
     * @return Color& Reference to this color
     * 
     * @details Copies all RGBA components from the source color.
     */
    Color& operator=(const Color& other);
    
    /**
     * @brief Add-assign operator
     * 
     * @param other Color to add
     * @return Color& Reference to this color
     * 
     * @details Adds the other color's components to this color (clamped).
     */
    Color& operator+=(const Color& other);
    
    /**
     * @brief Subtract-assign operator
     * 
     * @param other Color to subtract
     * @return Color& Reference to this color
     * 
     * @details Subtracts the other color's components from this color (clamped).
     */
    Color& operator-=(const Color& other);
    
    /**
     * @brief Multiply-assign operator
     * 
     * @param scalar Value to multiply by
     * @return Color& Reference to this color
     * 
     * @details Multiplies all components by the scalar (clamped).
     */
    Color& operator*=(std::uint8_t scalar);
    
    /**
     * @brief Divide-assign operator
     * 
     * @param scalar Value to divide by
     * @return Color& Reference to this color
     * 
     * @details Divides all components by the scalar.
     */
    Color& operator/=(std::uint8_t scalar);
    
    // Conversion utilities
    
    /**
     * @brief Converts the color to a string representation
     * 
     * @return std::string String in format "Color(r,g,b,a)"
     * 
     * @details Useful for debugging and logging color values.
     */
    std::string to_string() const;
    
    /**
     * @brief Converts the color to a 32-bit integer
     * 
     * @return std::uint32_t Color as RGBA packed into 32 bits
     * 
     * @details Packs the RGBA components into a single 32-bit value.
     * The exact bit layout depends on the implementation.
     */
    std::uint32_t to_int() const;
    
    /**
     * @brief Creates a Color from a 32-bit integer
     * 
     * @param value 32-bit integer containing packed RGBA values
     * @return Color New color with components extracted from the integer
     * 
     * @details Unpacks RGBA components from a 32-bit integer value.
     * This is the inverse operation of to_int().
     */
    static Color from_int(std::uint32_t value);
};
} // namespace guigui
