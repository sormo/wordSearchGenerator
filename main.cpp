#include "test.h"
#include "dictionary.h"
#include "wordSearch.h"
#include <iostream>

int main()
{
    Test::Execute();

    auto data = Dictionary::ReadDictionary("dict\\sk.txt");
    auto words1 = Dictionary::GetRandomWords(data, 12, 4, WordSearch::BOARD_SIZE);
    auto board1 = WordSearch::PositionWords(words1);

    WordSearch::PrintBoard(board1);
    for (const auto& word : words1)
        std::cout << word << "\n";

    auto [board2, words2] = WordSearch::PositionWords(data, 12, 4);

    WordSearch::PrintBoard(board2);
    for (const auto& word : words2)
        std::cout << word << "\n";

    return 0;
}
