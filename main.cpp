#include "test.h"
#include "dictionary.h"
#include "wordSearch.h"
#include <iostream>

int main()
{
    Test::Execute();

    auto data = Dictionary::ReadDictionary("dict\\sk.txt");
    auto words1 = Dictionary::GetRandomWords(data, 12, 4, 6);
    auto board1 = WordSearch::PositionWords(6, 6, words1);

    WordSearch::PrintBoard(board1);
    for (const auto& word : words1)
        std::cout << word << "\n";

    auto [board2, words2] = WordSearch::PositionWords(data, 15, 20, 45, 4);
    std::cout << "Free cells: " << WordSearch::GetFreeCellsCount(board2) << "\n";
    WordSearch::PrintBoard(board2);

    WordSearch::FillFreeCellsRandom(board2, words2);

    WordSearch::PrintBoard(board2);
    for (const auto& word : words2)
        std::cout << word << "\n";

    return 0;
}
