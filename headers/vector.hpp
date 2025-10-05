/*
 Copyright (c) 2025 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#pragma once

#include <array>
#include <cmath>
#include <initializer_list>

/**
 * @brief A generic N-dimensional vector class template
 *
 * This class provides a mathematical vector implementation with support for
 * basic vector operations including addition, subtraction, scalar
 * multiplication, dot product, magnitude calculation, and normalization.
 *
 * @tparam Type The numeric type of vector components (e.g., float, double, int)
 * @tparam Dimension The number of dimensions/components in the vector
 */
template <typename Type, size_t Dimension> class Vector {
private:
  std::array<Type, Dimension> _data; ///< Array to hold vector components

public:
  /**
   * @brief Default constructor - initializes all components to zero
   */
  Vector() : _data{} {}

  /**
   * @brief Constructor from initializer list
   *
   * Initializes vector components from the provided values. If fewer values
   * are provided than the dimension, remaining components are zero-initialized.
   * Extra values beyond the dimension are ignored.
   *
   * @param values Initializer list of component values
   */
  Vector(std::initializer_list<Type> values) {
    size_t i = 0;
    for (const auto &value : values) {
      if (i >= Dimension)
        break;
      _data[i++] = value;
    }
  }

  /**
   * @brief Non-const element access operator
   *
   * @param index The index of the component to access
   * @return Reference to the component at the specified index
   */
  Type &operator[](size_t index) { return _data[index]; }

  /**
   * @brief Const element access operator
   *
   * @param index The index of the component to access
   * @return Const reference to the component at the specified index
   */
  const Type &operator[](size_t index) const { return _data[index]; }

  /**
   * @brief Vector addition operator
   *
   * @param other The vector to add to this vector
   * @return A new vector containing the component-wise sum
   */

  Vector operator+(const Vector &other) const {
    Vector result;
    for (size_t i = 0; i < Dimension; ++i) {
      result[i] = _data[i] + other[i];
    }
    return result;
  }

  /**
   * @brief Vector subtraction operator
   *
   * @param other The vector to subtract from this vector
   * @return A new vector containing the component-wise difference
   */
  Vector operator-(const Vector &other) const {
    Vector result;
    for (size_t i = 0; i < Dimension; ++i) {
      result[i] = _data[i] - other[i];
    }
    return result;
  }

  /**
   * @brief Scalar multiplication operator
   *
   * @param scalar The scalar value to multiply each component by
   * @return A new vector with all components scaled by the scalar
   */
  Vector operator*(Type scalar) const {
    Vector result;
    for (size_t i = 0; i < Dimension; ++i) {
      result[i] = _data[i] * scalar;
    }
    return result;
  }

  /**
   * @brief Compute dot product with another vector
   *
   * @param other The vector to compute the dot product with
   * @return The dot product (scalar) result
   */
  Type dot(const Vector &other) const {
    Type result = Type{};
    for (size_t i = 0; i < Dimension; ++i) {
      result += _data[i] * other[i];
    }
    return result;
  }

  /**
   * @brief Calculate the magnitude (length) of the vector
   *
   * @return The Euclidean magnitude of the vector
   */
  Type magnitude() const { return std::sqrt(dot(*this)); }

  /**
   * @brief Create a normalized (unit) vector
   *
   * Returns a vector with the same direction but magnitude of 1.
   * If the vector has zero magnitude, returns the original vector unchanged.
   *
   * @return A new normalized vector
   */
  Vector normalize() const {
    Type mag = magnitude();
    return (mag != Type{}) ? (*this * (Type{1} / mag)) : *this;
  }

  /**
   * @brief Get the number of dimensions/components
   *
   * @return The dimension of the vector (compile-time constant)
   */
  constexpr size_t size() const { return Dimension; }
};
