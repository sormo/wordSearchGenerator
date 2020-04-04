#pragma once
#include <map>
#include <vector>
#include <string>
#include <array>
#include <optional>

namespace Dictionary
{
    static constexpr size_t MIN_WORD_SIZE = 3;
    static constexpr size_t MAX_WORD_SIZE = 25;

    using Data = std::array<std::vector<std::string>, MAX_WORD_SIZE>;

    Data ReadDictionary(const std::string& path);

    std::optional<std::string> GetRandomWord(const Data& data, size_t wordSize);
    std::vector<std::string> GetRandomWords(const Data& data, size_t wordCount, size_t wordSizeFrom = MIN_WORD_SIZE, size_t wordSizeTo = MAX_WORD_SIZE);
}
