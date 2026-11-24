#pragma once

#include <vector>
#include <string>
#include <cstdint>

using Bytes = std::vector<std::uint8_t>;
using Digest = std::vector<std::uint8_t>;

struct DataSet
{
    std::string description;
    std::vector<Bytes> data;
};

inline Bytes TextBytes(const std::string& text)
{
    return Bytes(text.begin(), text.end());
}

inline Digest Digest32(std::uint32_t value)
{
    return {
        static_cast<std::uint8_t>(value >> 24),
        static_cast<std::uint8_t>(value >> 16),
        static_cast<std::uint8_t>(value >> 8),
        static_cast<std::uint8_t>(value)
    };
}


