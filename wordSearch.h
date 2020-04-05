#pragma once
#include <vector>
#include <string>
#include "dictionary.h"

namespace WordSearch
{
    using Board = std::vector<std::vector<uint8_t>>;
    using Words = std::vector<std::string>;

    std::optional<Board> PositionWords(size_t boardRows, size_t boardCols, const Words& words);
    std::tuple<Board, Words> PositionWords(Dictionary::Data& data, size_t boardRows, size_t boardCols,
        size_t wordSizeFrom = Dictionary::MIN_WORD_SIZE, size_t wordSizeTo = Dictionary::MAX_WORD_SIZE);

    void PrintBoard(const Board& board);

    size_t GetFreeCellsCount(const Board& board);
    void FillFreeCellsRandom(Board& board, const Words& words);

    // detail

    // !!! this is here only because of test ):
    enum class Direction
    {
        Up = 0,
        Down,
        Left,
        Right,
        UpLeft,
        UpRight,
        DownLeft,
        DownRight,

        COUNT
    };

    struct Candidate
    {
        int row;
        int col;

        Direction dir;
    };
    using Candidates = std::array<std::vector<Candidate>, Dictionary::MAX_WORD_SIZE>;
}
