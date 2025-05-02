#pragma once
#include <iostream>
#include <cmath>

class Vec2 {
public:
    float x, y;

    // Default constructor
    Vec2() : x(0), y(0) {}

    // Parameterized constructor
    Vec2(float x, float y) : x(x), y(y) {}

    // Method to compute the magnitude (length) of the vector
    float length() const {
        return std::sqrt(x * x + y * y);
    }

    // Method to normalize the vector (make it unit length)
    Vec2 normalize() const {
        float mag = length();
        if (mag > 0) {
            return Vec2(x / mag, y / mag);
        }
        return *this;  // Return the vector itself if it's a zero vector
    }

    // Method to compute the dot product of this vector and another
    float dot(const Vec2& other) const {
        return x * other.x + y * other.y;
    }

    // Method to compute the distance between this vector and another
    float distance(const Vec2& other) const {
        return std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }
    
    float manathan_distance(const Vec2& other) const {
        return std::abs(x - other.x) + std::abs(y - other.y);
    }

    // Operator to add two vectors
    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }

    // Operator to subtract two vectors
    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }

    // Operator to scale a vector by a scalar
    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    // Operator to print the vector (useful for debugging)
    friend std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
        os << "(" << vec.x << ", " << vec.y << ")";
        return os;
    }
};
