#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <initializer_list>
#include <string>
#include <type_traits>

namespace guigui {

template<std::size_t N, typename T = float>
class VectorN {
private:
    std::array<T, N> _components;

public:
    // Constructors
    VectorN() : _components{} {}
    
    template<typename... Args>
    VectorN(Args... args) : _components{static_cast<T>(args)...} {
        static_assert(sizeof...(Args) == N, "Number of arguments must match vector dimension");
    }
    
    VectorN(std::initializer_list<T> init) {
        std::copy_n(init.begin(), std::min(N, init.size()), _components.begin());
    }
    
    VectorN(const VectorN& other) = default;
    ~VectorN() = default;
    
    // Component access
    T& operator[](std::size_t index) { return _components[index]; }
    const T& operator[](std::size_t index) const { return _components[index]; }
    
    T& at(std::size_t index) { return _components.at(index); }
    const T& at(std::size_t index) const { return _components.at(index); }
    
    // For 2D compatibility
    template<std::size_t D = N>
    typename std::enable_if<D >= 1, T>::type get_x() const { return _components[0]; }
    
    template<std::size_t D = N>
    typename std::enable_if<D >= 2, T>::type get_y() const { return _components[1]; }
    
    template<std::size_t D = N>
    typename std::enable_if<D >= 3, T>::type get_z() const { return _components[2]; }
    
    template<std::size_t D = N>
    typename std::enable_if<D >= 4, T>::type get_w() const { return _components[3]; }
    
    template<std::size_t D = N>
    typename std::enable_if<D >= 1>::type set_x(T new_x) { _components[0] = new_x; }
    
    template<std::size_t D = N>
    typename std::enable_if<D >= 2>::type set_y(T new_y) { _components[1] = new_y; }
    
    template<std::size_t D = N>
    typename std::enable_if<D >= 3>::type set_z(T new_z) { _components[2] = new_z; }
    
    template<std::size_t D = N>
    typename std::enable_if<D >= 4>::type set_w(T new_w) { _components[3] = new_w; }
    
    // Arithmetic operators
    VectorN operator+(const VectorN& other) const {
        VectorN result;
        for (std::size_t i = 0; i < N; ++i) {
            result._components[i] = _components[i] + other._components[i];
        }
        return result;
    }
    
    VectorN operator-(const VectorN& other) const {
        VectorN result;
        for (std::size_t i = 0; i < N; ++i) {
            result._components[i] = _components[i] - other._components[i];
        }
        return result;
    }
    
    VectorN operator*(T scalar) const {
        VectorN result;
        for (std::size_t i = 0; i < N; ++i) {
            result._components[i] = _components[i] * scalar;
        }
        return result;
    }
    
    VectorN operator/(T scalar) const {
        VectorN result;
        for (std::size_t i = 0; i < N; ++i) {
            result._components[i] = _components[i] / scalar;
        }
        return result;
    }
    
    // Comparison operators
    bool operator==(const VectorN& other) const {
        return _components == other._components;
    }
    
    bool operator!=(const VectorN& other) const {
        return !(*this == other);
    }
    
    // Assignment operators
    VectorN& operator=(const VectorN& other) = default;
    
    VectorN& operator+=(const VectorN& other) {
        for (std::size_t i = 0; i < N; ++i) {
            _components[i] += other._components[i];
        }
        return *this;
    }
    
    VectorN& operator-=(const VectorN& other) {
        for (std::size_t i = 0; i < N; ++i) {
            _components[i] -= other._components[i];
        }
        return *this;
    }
    
    VectorN& operator*=(T scalar) {
        for (std::size_t i = 0; i < N; ++i) {
            _components[i] *= scalar;
        }
        return *this;
    }
    
    VectorN& operator/=(T scalar) {
        for (std::size_t i = 0; i < N; ++i) {
            _components[i] /= scalar;
        }
        return *this;
    }
    
    // Mathematical operations
    T length() const {
        T sum = T{};
        for (const auto& component : _components) {
            sum += component * component;
        }
        return static_cast<T>(std::sqrt(static_cast<double>(sum)));
    }
    
    VectorN normalized() const {
        T len = length();
        if (len == T{}) {
            return VectorN{};
        }
        return *this / len;
    }
    
    VectorN clamp(T minVal, T maxVal) const {
        VectorN result;
        for (std::size_t i = 0; i < N; ++i) {
            result._components[i] = std::max(minVal, std::min(maxVal, _components[i]));
        }
        return result;
    }
    
    VectorN abs() const {
        VectorN result;
        for (std::size_t i = 0; i < N; ++i) {
            result._components[i] = std::abs(_components[i]);
        }
        return result;
    }
    
    VectorN floor() const {
        VectorN result;
        for (std::size_t i = 0; i < N; ++i) {
            result._components[i] = static_cast<T>(std::floor(_components[i]));
        }
        return result;
    }
    
    VectorN ceil() const {
        VectorN result;
        for (std::size_t i = 0; i < N; ++i) {
            result._components[i] = static_cast<T>(std::ceil(_components[i]));
        }
        return result;
    }
    
    VectorN round() const {
        VectorN result;
        for (std::size_t i = 0; i < N; ++i) {
            result._components[i] = static_cast<T>(std::round(_components[i]));
        }
        return result;
    }
    
    VectorN min(const VectorN& other) const {
        VectorN result;
        for (std::size_t i = 0; i < N; ++i) {
            result._components[i] = std::min(_components[i], other._components[i]);
        }
        return result;
    }
    
    VectorN max(const VectorN& other) const {
        VectorN result;
        for (std::size_t i = 0; i < N; ++i) {
            result._components[i] = std::max(_components[i], other._components[i]);
        }
        return result;
    }
    
    VectorN operator-() const {
        VectorN result;
        for (std::size_t i = 0; i < N; ++i) {
            result._components[i] = -_components[i];
        }
        return result;
    }
    
    std::string to_string() const {
        std::string result = "Vector" + std::to_string(N) + "(";
        for (std::size_t i = 0; i < N; ++i) {
            if (i > 0) result += ", ";
            result += std::to_string(_components[i]);
        }
        result += ")";
        return result;
    }
    
    // Iterator support
    auto begin() { return _components.begin(); }
    auto end() { return _components.end(); }
    auto begin() const { return _components.begin(); }
    auto end() const { return _components.end(); }
    
    // Dimension and type info
    static constexpr std::size_t dimension() { return N; }
    using value_type = T;
};

// Type aliases for common vector types
using Vector2f = VectorN<2, float>;
using Vector3f = VectorN<3, float>;
using Vector4f = VectorN<4, float>;

using Vector2d = VectorN<2, double>;
using Vector3d = VectorN<3, double>;
using Vector4d = VectorN<4, double>;

using Vector2i = VectorN<2, int>;
using Vector3i = VectorN<3, int>;
using Vector4i = VectorN<4, int>;

// Backward compatibility alias - this maintains the original Vector interface
using Vector = Vector2f;

} // namespace guigui
