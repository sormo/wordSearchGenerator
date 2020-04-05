#include "dictionary.h"
#include <fstream>
#include <random>

namespace Dictionary
{
    std::random_device g_rd;
    std::mt19937 g_mt(g_rd());

    Data ReadDictionary(const std::string& path)
    {
        Data result;
        std::ifstream file(path);

        while (file)
        {
            std::string word;
            file >> word;

            if (word.size() >= result.size())
                continue;

            result[word.size()].push_back(word);
        }

        return result;
    }

    std::optional<std::string> GetRandomWord(const Data& data, size_t wordSize)
    {
        size_t wordCount = data[wordSize].size();
        if (wordCount == 0)
            return std::nullopt;

        std::uniform_int_distribution<size_t> rand(0, wordCount - 1);

        auto it = std::begin(data[wordSize]);
        std::advance(it, rand(g_mt));

        return *it;
    }

    std::vector<std::string> GetRandomWords(const Data& data, size_t wordCount, size_t wordSizeFrom, size_t wordSizeTo)
    {
        std::uniform_int_distribution<size_t> randWordSize(wordSizeFrom, wordSizeTo);

        std::vector<std::string> result;
        while (result.size() != wordCount)
        {
            if (auto word = GetRandomWord(data, randWordSize(g_mt)))
            {
                if (std::find(std::begin(result), std::end(result), *word) != std::end(result))
                    continue;

                result.push_back(std::move(*word));
            }
        }

        return result;
    }
}
