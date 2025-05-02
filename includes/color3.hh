#pragma once

#include <iostream>
#include <algorithm> // for std::clamp
#include <string>
#include <sstream>

class Color3 {
public:
    unsigned char r, g, b;

    // Constructors
    Color3() : r(0), g(0), b(0) {}
    Color3(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}

    // Addition
    Color3 operator+(const Color3& other) const {
        return Color3(
            std::min(255, r + other.r),
            std::min(255, g + other.g),
            std::min(255, b + other.b)
        );
    }

    // Subtraction
    Color3 operator-(const Color3& other) const {
        return Color3(
            std::max(0, r - other.r),
            std::max(0, g - other.g),
            std::max(0, b - other.b)
        );
    }

    // Multiplication by scalar
    Color3 operator*(float scalar) const {
        return Color3(
            static_cast<unsigned char>(std::clamp(static_cast<int>(r * scalar), 0, 255)),
            static_cast<unsigned char>(std::clamp(static_cast<int>(g * scalar), 0, 255)),
            static_cast<unsigned char>(std::clamp(static_cast<int>(b * scalar), 0, 255))
        );
    }

    // Equality
    bool operator==(const Color3& other) const {
        return r == other.r && g == other.g && b == other.b;
    }

    bool operator!=(const Color3& other) const {
        return !(*this == other);
    }

    // Output
    friend std::ostream& operator<<(std::ostream& os, const Color3& color) {
        os << "Color3(" << static_cast<int>(color.r) << ", "
                       << static_cast<int>(color.g) << ", "
                       << static_cast<int>(color.b) << ")";
        return os;
    }

    // Create from hex string like "#FFAABB" or "FFAABB"
    static Color3 fromHex(const std::string& hex) {
        std::string cleaned = (hex[0] == '#') ? hex.substr(1) : hex;

        unsigned int value;
        std::stringstream ss;
        ss << std::hex << cleaned;
        ss >> value;

        return Color3(
            static_cast<unsigned char>((value >> 16) & 0xFF),
            static_cast<unsigned char>((value >> 8) & 0xFF),
            static_cast<unsigned char>(value & 0xFF)
        );
    }
};
