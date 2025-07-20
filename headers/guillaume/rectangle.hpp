/**
 * @file rectangle.hpp
 * @brief Rectangle geometry class for the Guillaume GUI framework
 * 
 * This file contains the Rectangle class which provides 2D rectangular
 * geometry representation and manipulation for layout and positioning
 * in the Guillaume framework.
 */

#pragma once

#include "guillaume/vector.hpp"

namespace guigui {

/**
 * @class Rectangle
 * @brief 2D rectangle representation with position and size
 * 
 * The Rectangle class provides comprehensive functionality for working with
 * rectangular areas in 2D space. It stores position and size information
 * and provides various mathematical operations, intersection testing,
 * and utility functions commonly needed for GUI layout and hit testing.
 * 
 * @details Features:
 * - Position and size representation using Vector objects
 * - Comprehensive arithmetic operations
 * - Intersection and containment testing
 * - Area and perimeter calculations
 * - Normalization and clamping utilities
 * - Union and intersection operations
 * 
 * @note The rectangle uses integer coordinates for pixel-perfect positioning
 * in GUI applications.
 */
class Rectangle {
private:
    Vector _position;  ///< Top-left corner position (x, y)
    Vector _size;      ///< Size dimensions (width, height)

protected:
public:
    /**
     * @brief Constructs a Rectangle with specified position and size vectors
     * 
     * @param position Top-left corner position (default: (0,0))
     * @param size Rectangle dimensions (default: (0,0))
     * 
     * @details Creates a rectangle using Vector objects for position and size.
     * Default parameters create a zero-sized rectangle at the origin.
     */
    Rectangle(const Vector& position = Vector(0, 0), const Vector& size = Vector(0, 0));
    
    /**
     * @brief Constructs a Rectangle with individual coordinate values
     * 
     * @param x X coordinate of the top-left corner
     * @param y Y coordinate of the top-left corner
     * @param width Width of the rectangle
     * @param height Height of the rectangle
     * 
     * @details Convenience constructor that takes individual integer values
     * for direct specification of rectangle bounds.
     */
    Rectangle(int x, int y, int width, int height);
    
    /**
     * @brief Copy constructor
     * 
     * @param other Rectangle to copy from
     * 
     * Creates a new Rectangle with the same position and size as the source.
     */
    Rectangle(const Rectangle& other);
    
    /**
     * @brief Destructor
     * 
     * Cleans up the Rectangle object (no special cleanup needed).
     */
    ~Rectangle();
    
    // Vector-based getters and setters
    
    /**
     * @brief Gets the position vector (top-left corner)
     * 
     * @return Vector Position of the rectangle's top-left corner
     */
    Vector get_position() const;
    
    /**
     * @brief Gets the size vector (width and height)
     * 
     * @return Vector Dimensions of the rectangle
     */
    Vector get_size() const;
    
    /**
     * @brief Sets the position vector
     * 
     * @param new_position New position for the rectangle's top-left corner
     */
    void set_position(const Vector& new_position);
    
    /**
     * @brief Sets the size vector
     * 
     * @param new_size New dimensions for the rectangle
     */
    void set_size(const Vector& new_size);
    
    // Individual component getters and setters
    
    /**
     * @brief Gets the X coordinate of the top-left corner
     * 
     * @return int X coordinate
     */
    int get_x() const;
    
    /**
     * @brief Gets the Y coordinate of the top-left corner
     * 
     * @return int Y coordinate
     */
    int get_y() const;
    
    /**
     * @brief Gets the width of the rectangle
     * 
     * @return int Width in pixels
     */
    int get_width() const;
    
    /**
     * @brief Gets the height of the rectangle
     * 
     * @return int Height in pixels
     */
    int get_height() const;
    
    /**
     * @brief Sets the X coordinate of the top-left corner
     * 
     * @param new_x New X coordinate
     */
    void set_x(int new_x);
    
    /**
     * @brief Sets the Y coordinate of the top-left corner
     * 
     * @param new_y New Y coordinate
     */
    void set_y(int new_y);
    
    /**
     * @brief Sets the width of the rectangle
     * 
     * @param new_width New width in pixels
     */
    void set_width(int new_width);
    
    /**
     * @brief Sets the height of the rectangle
     * 
     * @param new_height New height in pixels
     */
    void set_height(int new_height);
    
    // Arithmetic operators
    
    /**
     * @brief Adds two rectangles (position and size addition)
     * 
     * @param other Rectangle to add
     * @return Rectangle Result of component-wise addition
     */
    Rectangle operator+(const Rectangle& other) const;
    
    /**
     * @brief Subtracts two rectangles (position and size subtraction)
     * 
     * @param other Rectangle to subtract
     * @return Rectangle Result of component-wise subtraction
     */
    Rectangle operator-(const Rectangle& other) const;
    
    /**
     * @brief Multiplies rectangle by a scalar
     * 
     * @param scalar Value to multiply position and size by
     * @return Rectangle Scaled rectangle
     */
    Rectangle operator*(int scalar) const;
    
    /**
     * @brief Divides rectangle by a scalar
     * 
     * @param scalar Value to divide position and size by
     * @return Rectangle Scaled rectangle
     */
    Rectangle operator/(int scalar) const;
    
    // Comparison operators
    
    /**
     * @brief Checks if two rectangles are equal
     * 
     * @param other Rectangle to compare with
     * @return bool True if position and size are equal
     */
    bool operator==(const Rectangle& other) const;
    
    /**
     * @brief Checks if two rectangles are not equal
     * 
     * @param other Rectangle to compare with
     * @return bool True if position or size differ
     */
    bool operator!=(const Rectangle& other) const;
    
    // Assignment operators
    
    /**
     * @brief Assignment operator
     * 
     * @param other Rectangle to copy from
     * @return Rectangle& Reference to this rectangle
     */
    Rectangle& operator=(const Rectangle& other);
    
    /**
     * @brief Add-assign operator
     * 
     * @param other Rectangle to add
     * @return Rectangle& Reference to this rectangle
     */
    Rectangle& operator+=(const Rectangle& other);
    
    /**
     * @brief Subtract-assign operator
     * 
     * @param other Rectangle to subtract
     * @return Rectangle& Reference to this rectangle
     */
    Rectangle& operator-=(const Rectangle& other);
    
    /**
     * @brief Multiply-assign operator
     * 
     * @param scalar Scalar to multiply by
     * @return Rectangle& Reference to this rectangle
     */
    Rectangle& operator*=(int scalar);
    
    /**
     * @brief Divide-assign operator
     * 
     * @param scalar Scalar to divide by
     * @return Rectangle& Reference to this rectangle
     */
    Rectangle& operator/=(int scalar);
    
    // Utility methods
    
    /**
     * @brief Calculates the area of the rectangle
     * 
     * @return int Area in square pixels (width * height)
     */
    int area() const;
    
    /**
     * @brief Calculates the perimeter of the rectangle
     * 
     * @return int Perimeter in pixels (2 * (width + height))
     */
    int perimeter() const;
    
    /**
     * @brief Returns a normalized rectangle (positive width and height)
     * 
     * @return Rectangle Rectangle with positive dimensions
     * 
     * @details Ensures width and height are positive by adjusting
     * position if necessary.
     */
    Rectangle normalized() const;
    
    /**
     * @brief Clamps rectangle components to a range
     * 
     * @param minVal Minimum value for all components
     * @param maxVal Maximum value for all components
     * @return Rectangle Rectangle with clamped values
     */
    Rectangle clamp(int minVal, int maxVal) const;
    /**
     * @brief Returns a rectangle with absolute (positive) values
     * 
     * @return Rectangle Rectangle with all positive components
     * 
     * @details Converts negative width/height to positive values
     * and adjusts position accordingly.
     */
    Rectangle abs() const;
    
    /**
     * @brief Returns a rectangle with floor values
     * 
     * @return Rectangle Rectangle with components rounded down
     * 
     * @details Applies floor operation to all components.
     */
    Rectangle floor() const;
    
    /**
     * @brief Returns a rectangle with ceiling values
     * 
     * @return Rectangle Rectangle with components rounded up
     * 
     * @details Applies ceiling operation to all components.
     */
    Rectangle ceil() const;
    
    /**
     * @brief Returns a rectangle with rounded values
     * 
     * @return Rectangle Rectangle with components rounded to nearest integer
     * 
     * @details Applies standard rounding to all components.
     */
    Rectangle round() const;
    
    /**
     * @brief Returns the component-wise minimum of two rectangles
     * 
     * @param other Rectangle to compare with
     * @return Rectangle Rectangle with minimum components
     */
    Rectangle min(const Rectangle& other) const;
    
    /**
     * @brief Returns the component-wise maximum of two rectangles
     * 
     * @param other Rectangle to compare with
     * @return Rectangle Rectangle with maximum components
     */
    Rectangle max(const Rectangle& other) const;
    
    /**
     * @brief Unary negation operator
     * 
     * @return Rectangle Rectangle with negated position and size
     */
    Rectangle operator-() const;
    
    /**
     * @brief Converts the rectangle to a string representation
     * 
     * @return std::string String in format "Rectangle(x,y,w,h)"
     * 
     * @details Useful for debugging and logging rectangle values.
     */
    std::string to_string() const;
    
    // Geometric operations
    
    /**
     * @brief Checks if a point is inside the rectangle
     * 
     * @param point Point to test for containment
     * @return bool True if the point is inside the rectangle bounds
     * 
     * @details Uses inclusive bounds testing for precise hit detection.
     */
    bool contains(const Vector& point) const;
    
    /**
     * @brief Checks if this rectangle intersects with another
     * 
     * @param other Rectangle to test intersection with
     * @return bool True if the rectangles overlap
     * 
     * @details Returns true if any part of the rectangles overlap,
     * including edge touching.
     */
    bool intersects(const Rectangle& other) const;
    
    /**
     * @brief Calculates the intersection area of two rectangles
     * 
     * @param other Rectangle to intersect with
     * @return Rectangle Rectangle representing the overlapping area
     * 
     * @details Returns a rectangle with zero area if no intersection exists.
     */
    Rectangle intersection(const Rectangle& other) const;
    
    /**
     * @brief Calculates the union (bounding box) of two rectangles
     * 
     * @param other Rectangle to union with
     * @return Rectangle Smallest rectangle that contains both input rectangles
     * 
     * @details Creates the minimal bounding rectangle that encompasses
     * both input rectangles completely.
     */
    Rectangle union_rect(const Rectangle& other) const;
};
} // namespace guigui
