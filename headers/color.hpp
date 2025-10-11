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

#include <algorithm>
#include <cstdint>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

/**
 * @class Color
 * @brief Represents an RGBA color with floating-point channels in [0, 1].
 *
 * The Color class stores red, green, blue, and alpha components as floats
 * clamped to the [0,1] range. It provides convenient constructors, conversion
 * utilities to and from 8-bit channel values and hexadecimal strings, as well
 * as basic color operations.
 */
class Color {
private:
  float _r; ///< Red   component in [0,1]
  float _g; ///< Green component in [0,1]
  float _b; ///< Blue  component in [0,1]
  float _a; ///< Alpha component in [0,1]

  static constexpr float _clamp01(float v) {
    return v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v);
  }

public:
  /**
   * @brief Construct a fully-opaque black color (0,0,0,1).
   */
  Color(void) : _r(0.0f), _g(0.0f), _b(0.0f), _a(1.0f) {}

  /**
   * @brief Construct from RGB, alpha defaults to 1.0.
   */
  Color(float r, float g, float b)
      : _r(_clamp01(r)), _g(_clamp01(g)), _b(_clamp01(b)), _a(1.0f) {}

  /**
   * @brief Construct from RGBA.
   */
  Color(float r, float g, float b, float a)
      : _r(_clamp01(r)), _g(_clamp01(g)), _b(_clamp01(b)), _a(_clamp01(a)) {}

  /**
   * @brief Red component in [0,1].
   */
  float r(void) const { return _r; }
  /**
   * @brief Green component in [0,1].
   */
  float g(void) const { return _g; }
  /**
   * @brief Blue component in [0,1].
   */
  float b(void) const { return _b; }
  /**
   * @brief Alpha component in [0,1].
   */
  float a(void) const { return _a; }

  /**
   * @brief Setters (values are clamped to [0,1]).
   */
  /**
   * @brief Set red component.
   * @param v Red value, clamped to [0,1]
   */
  void setR(float v) { _r = _clamp01(v); }
  /**
   * @brief Set green component.
   * @param v Green value, clamped to [0,1]
   */
  void setG(float v) { _g = _clamp01(v); }
  /**
   * @brief Set blue component.
   * @param v Blue value, clamped to [0,1]
   */
  void setB(float v) { _b = _clamp01(v); }
  /**
   * @brief Set alpha component.
   * @param v Alpha value, clamped to [0,1]
   */
  void setA(float v) { _a = _clamp01(v); }

  /**
   * @brief Return a copy with a different alpha (keeps RGB).
   */
  Color withAlpha(float alpha) const { return Color(_r, _g, _b, alpha); }

  /**
   * @brief Linear interpolation between two colors (component-wise).
   * @param to Target color
   * @param t Interpolation factor in [0,1]
   */
  Color lerp(const Color &to, float t) const {
    float u = _clamp01(t);
    return Color(_r + (to._r - _r) * u, _g + (to._g - _g) * u,
                 _b + (to._b - _b) * u, _a + (to._a - _a) * u);
  }

  /**
   * @brief Premultiplied alpha representation.
   * @return Color where rgb are multiplied by alpha, alpha unchanged.
   */
  Color premultiplied() const { return Color(_r * _a, _g * _a, _b * _a, _a); }

  /**
   * @brief Alpha blend this color over a background color.
   * Uses standard source-over compositing in linear space.
   */
  Color over(const Color &bg) const {
    float outA = _a + bg._a * (1.0f - _a);
    if (outA <= 0.0f)
      return Color(0, 0, 0, 0);
    float outR = (_r * _a + bg._r * bg._a * (1.0f - _a)) / outA;
    float outG = (_g * _a + bg._g * bg._a * (1.0f - _a)) / outA;
    float outB = (_b * _a + bg._b * bg._a * (1.0f - _a)) / outA;
    return Color(outR, outG, outB, outA);
  }

  /**
   * @brief 8-bit RGBA packed representation.
   *
   * A simple POD structure holding 8-bit per channel red, green, blue and
   * alpha components.
   */
  struct RGBA8 {
    /// Red channel (0..255)
    uint8_t r;
    /// Green channel (0..255)
    uint8_t g;
    /// Blue channel (0..255)
    uint8_t b;
    /// Alpha channel (0..255)
    uint8_t a;
  };

  /**
   * @brief Convert to 8-bit per channel RGBA.
   * @return An RGBA8 struct containing channels in 0..255.
   */
  RGBA8 toRGBA8() const {
    auto to8 = [](float v) -> uint8_t {
      float cl = v <= 0.0f ? 0.0f : (v >= 1.0f ? 1.0f : v);
      return static_cast<uint8_t>(std::round(cl * 255.0f));
    };
    return RGBA8{to8(_r), to8(_g), to8(_b), to8(_a)};
  }

  /**
   * @brief Create from 8-bit per channel RGBA values.
   */
  static Color fromRGBA8(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) {
    auto toF = [](uint8_t v) -> float {
      return static_cast<float>(v) / 255.0f;
    };
    return Color(toF(r), toF(g), toF(b), toF(a));
  }

  /**
   * @brief Convert to hexadecimal string.
   * Formats as "#RRGGBB" if a==1 else "#RRGGBBAA".
   */
  std::string toHex(bool includeAlphaIfOpaque = false) const {
    auto p = toRGBA8();
    std::ostringstream oss;
    oss << '#' << std::uppercase << std::hex << std::setfill('0')
        << std::setw(2) << static_cast<int>(p.r) << std::setw(2)
        << static_cast<int>(p.g) << std::setw(2) << static_cast<int>(p.b);
    if (includeAlphaIfOpaque || p.a != 255) {
      oss << std::setw(2) << static_cast<int>(p.a);
    }
    return oss.str();
  }

  /**
   * @brief Parse from hexadecimal string.
   *
   * Accepts formats (leading '#' is optional):
   * - \c RGB
   * - \c RRGGBB
   * - \c RGBA
   * - \c RRGGBBAA
   *
   * Throws std::invalid_argument on invalid format.
   */
  static Color fromHex(const std::string &hex) {
    auto isHex = [](char c) {
      return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
             (c >= 'A' && c <= 'F');
    };

    std::string s = hex;
    if (!s.empty() && s[0] == '#')
      s.erase(0, 1);
    if (s.size() != 3 && s.size() != 4 && s.size() != 6 && s.size() != 8)
      throw std::invalid_argument("Invalid hex color length");
    for (char c : s) {
      if (!isHex(c))
        throw std::invalid_argument("Invalid hex color character");
    }

    auto hexToByte = [](const std::string &hh) -> uint8_t {
      return static_cast<uint8_t>(std::stoul(hh, nullptr, 16));
    };

    if (s.size() == 3 || s.size() == 4) {
      // short form: expand nibbles: #RGB -> #RRGGBB
      uint8_t r = hexToByte(std::string{s[0], s[0]});
      uint8_t g = hexToByte(std::string{s[1], s[1]});
      uint8_t b = hexToByte(std::string{s[2], s[2]});
      uint8_t a = 0xFF;
      if (s.size() == 4)
        a = hexToByte(std::string{s[3], s[3]});
      return fromRGBA8(r, g, b, a);
    }

    // long form
    uint8_t r = hexToByte(s.substr(0, 2));
    uint8_t g = hexToByte(s.substr(2, 2));
    uint8_t b = hexToByte(s.substr(4, 2));
    uint8_t a = 0xFF;
    if (s.size() == 8)
      a = hexToByte(s.substr(6, 2));
    return fromRGBA8(r, g, b, a);
  }

  /**
   * @brief Equality operators (exact component-wise equality).
   */
  bool operator==(const Color &other) const {
    return _r == other._r && _g == other._g && _b == other._b && _a == other._a;
  }
  /**
   * @brief Inequality operator.
   * @return true if any component differs, false otherwise.
   */
  bool operator!=(const Color &other) const { return !(*this == other); }

  /**
   * @brief Component-wise addition (clamped).
   */
  Color operator+(const Color &o) const {
    return Color(_clamp01(_r + o._r), _clamp01(_g + o._g), _clamp01(_b + o._b),
                 _clamp01(_a + o._a));
  }

  /**
   * @brief Component-wise subtraction (clamped to [0,1]).
   */
  Color operator-(const Color &o) const {
    return Color(_clamp01(_r - o._r), _clamp01(_g - o._g), _clamp01(_b - o._b),
                 _clamp01(_a - o._a));
  }

  /**
   * @brief Scalar multiplication (scales RGB; alpha unchanged by default).
   */
  Color operator*(float s) const {
    return Color(_clamp01(_r * s), _clamp01(_g * s), _clamp01(_b * s), _a);
  }

  /**
   * @brief Common named colors (opaque unless specified).
   */
  /// Opaque black (0,0,0,1)
  static Color Black() { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
  /// Opaque white (1,1,1,1)
  static Color White() { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
  /// Opaque red (1,0,0,1)
  static Color Red() { return Color(1.0f, 0.0f, 0.0f, 1.0f); }
  /// Opaque green (0,1,0,1)
  static Color Green() { return Color(0.0f, 1.0f, 0.0f, 1.0f); }
  /// Opaque blue (0,0,1,1)
  static Color Blue() { return Color(0.0f, 0.0f, 1.0f, 1.0f); }
  /// Fully transparent black (0,0,0,0)
  static Color Transparent() { return Color(0.0f, 0.0f, 0.0f, 0.0f); }
};
