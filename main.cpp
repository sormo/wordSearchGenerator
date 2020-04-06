#include "test.h"
#include "dictionary.h"
#include "wordSearch.h"
#include <iostream>

int main()
{
    Test::Execute();

    auto data = Dictionary::ReadDictionary("dict\\cz.txt");

    // Backtracking implementation is slow.
    //auto words1 = Dictionary::GetRandomWords(data, 6, 4, 6);
    //if (auto board1 = WordSearch::PositionWords(6, 6, words1))
    //{
    //    WordSearch::PrintBoard(*board1);
    //    for (const auto& word : words1)
    //        std::cout << word << "\n";
    //}

    auto [board2, words2] = WordSearch::PositionWords(data, 10, 10, 4);
    std::cout << "Free cells: " << WordSearch::GetFreeCellsCount(board2) << "\n";
    WordSearch::PrintBoard(board2);

    WordSearch::FillFreeCellsRandom(board2, words2);

    WordSearch::PrintBoard(board2);
    for (const auto& word : words2)
        std::cout << word << "\n";

    size_t freeCells = 0, wordCount = 0;
    for (size_t i = 0; i < 100; ++i)
    {
        auto [board3, words3] = WordSearch::PositionWords(data, 10, 10, 4);
        freeCells += WordSearch::GetFreeCellsCount(board3);
        wordCount += words3.size();
    }
    std::cout << "Average free cells: " << freeCells / 100 << "\n";
    std::cout << "Average word count: " << wordCount / 100 << "\n";

    return 0;
}
