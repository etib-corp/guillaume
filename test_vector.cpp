#include "headers/guillaume/vector.hpp"
#include <iostream>

int main() {
    // Test backward compatibility with Vector (2D float)
    guigui::Vector v2d_old(3.0f, 4.0f);
    std::cout << "Original Vector: " << v2d_old.to_string() << std::endl;
    std::cout << "Length: " << v2d_old.length() << std::endl;
    std::cout << "X: " << v2d_old.get_x() << ", Y: " << v2d_old.get_y() << std::endl;
    
    // Test new templated vectors
    guigui::Vector2f v2f(1.0f, 2.0f);
    std::cout << "\nVector2f: " << v2f.to_string() << std::endl;
    
    guigui::Vector3f v3f(1.0f, 2.0f, 3.0f);
    std::cout << "Vector3f: " << v3f.to_string() << std::endl;
    std::cout << "X: " << v3f.get_x() << ", Y: " << v3f.get_y() << ", Z: " << v3f.get_z() << std::endl;
    
    guigui::Vector4d v4d(1.0, 2.0, 3.0, 4.0);
    std::cout << "Vector4d: " << v4d.to_string() << std::endl;
    
    guigui::Vector2i v2i(10, 20);
    std::cout << "Vector2i: " << v2i.to_string() << std::endl;
    
    // Test arithmetic operations
    guigui::Vector2f a(1.0f, 2.0f);
    guigui::Vector2f b(3.0f, 4.0f);
    
    auto sum = a + b;
    auto diff = a - b;
    auto scaled = a * 2.0f;
    
    std::cout << "\nArithmetic operations:" << std::endl;
    std::cout << "a = " << a.to_string() << std::endl;
    std::cout << "b = " << b.to_string() << std::endl;
    std::cout << "a + b = " << sum.to_string() << std::endl;
    std::cout << "a - b = " << diff.to_string() << std::endl;
    std::cout << "a * 2 = " << scaled.to_string() << std::endl;
    
    // Test with different dimensions
    guigui::VectorN<5, float> v5f(1.0f, 2.0f, 3.0f, 4.0f, 5.0f);
    std::cout << "\n5D Vector: " << v5f.to_string() << std::endl;
    std::cout << "Length: " << v5f.length() << std::endl;
    
    // Test array access
    std::cout << "\nArray access:" << std::endl;
    for (std::size_t i = 0; i < 5; ++i) {
        std::cout << "v5f[" << i << "] = " << v5f[i] << std::endl;
    }
    
    return 0;
}
