#include "wordSearch.h"
#include <random>
#include "dictionary.h"
#include <iostream>
#include <iomanip>

namespace WordSearch
{
    using Candidates = std::array<std::vector<Candidate>, Dictionary::MAX_WORD_SIZE>;
    // we process candidates to form data[direction][wordSize]
    // reason is that we want to pick direction uniformly at random
    using ProcessedCandidates = std::array<Candidates, (size_t)Direction::COUNT>;

    std::random_device g_rd;
    std::mt19937 g_mt(g_rd());

    static constexpr size_t SAFETY_COUNT = 2000;

    std::vector<Candidate> GenerateCandidates(int boardRows, int boardCols, int wordSize)
    {
        std::vector<Candidate> result;
        int countRow = (boardRows - wordSize + 1) * boardCols;
        int countDiag = (boardRows - wordSize + 1) * (boardCols - wordSize + 1);

        // Up
        for (int r = wordSize - 1; r < boardRows; ++r)
            for (int c = 0; c < boardCols; ++c)
                result.push_back({ r, c, Direction::Up });

        // Down
        for (int r = 0; r < boardRows - wordSize + 1; ++r)
            for (int c = 0; c < boardCols; ++c)
                result.push_back({ r, c, Direction::Down });

        // Left
        for (int r = 0; r < boardRows; ++r)
            for (int c = wordSize - 1; c < boardCols; ++c)
                result.push_back({ r, c, Direction::Left });

        // Right
        for (int r = 0; r < boardRows; ++r)
            for (int c = 0; c < boardCols - wordSize + 1; ++c)
                result.push_back({ r, c, Direction::Right });

        // UpLeft
        for (int r = wordSize - 1; r < boardRows; ++r)
            for (int c = wordSize - 1; c < boardCols; ++c)
                result.push_back({ r, c, Direction::UpLeft });

        // UpRight
        for (int r = wordSize - 1; r < boardRows; ++r)
            for (int c = 0; c < boardCols - wordSize + 1; ++c)
                result.push_back({ r, c, Direction::UpRight });

        // DownLeft
        for (int r = 0; r < boardRows - wordSize + 1; ++r)
            for (int c = wordSize - 1; c < boardCols; ++c)
                result.push_back({ r, c, Direction::DownLeft });

        // DownRight
        for (int r = 0; r < boardRows - wordSize + 1; ++r)
            for (int c = 0; c < boardCols - wordSize + 1; ++c)
                result.push_back({ r, c, Direction::DownRight });

        return result;
    }

    using CharFunctionPrototype = bool(Board&, int row, int col, char character);

    // Call function for each row, col and char from word.
    template<class T>
    bool ApplyCharFunction(Board& board, const Candidate& position, const std::string& word, T function)
    {
        if (position.dir == Direction::Up)
        {
            for (size_t i = 0; i < word.size(); ++i)
            {
                if (!function(board, position.row - i, position.col, word[i]))
                    return false;
            }
        }
        else if (position.dir == Direction::Down)
        {
            for (size_t i = 0; i < word.size(); ++i)
            {
                if (!function(board, position.row + i, position.col, word[i]))
                    return false;
            }
        }
        else if (position.dir == Direction::Left)
        {
            for (size_t i = 0; i < word.size(); ++i)
            {
                if (!function(board, position.row, position.col - i, word[i]))
                    return false;
            }
        }
        else if (position.dir == Direction::Right)
        {
            for (size_t i = 0; i < word.size(); ++i)
            {
                if (!function(board, position.row, position.col + i, word[i]))
                    return false;
            }
        }
        else if (position.dir == Direction::UpLeft)
        {
            for (size_t i = 0; i < word.size(); ++i)
            {
                if (!function(board, position.row - i, position.col - i, word[i]))
                    return false;
            }
        }
        else if (position.dir == Direction::UpRight)
        {
            for (size_t i = 0; i < word.size(); ++i)
            {
                if (!function(board, position.row - i, position.col + i, word[i]))
                    return false;
            }
        }
        else if (position.dir == Direction::DownLeft)
        {
            for (size_t i = 0; i < word.size(); ++i)
            {
                if (!function(board, position.row + i, position.col - i, word[i]))
                    return false;
            }
        }
        else if (position.dir == Direction::DownRight)
        {
            for (size_t i = 0; i < word.size(); ++i)
            {
                if (!function(board, position.row + i, position.col + i, word[i]))
                    return false;
            }
        }

        return true;
    }

    // Verify if it's possible to place word on position in board.
    bool VerifyWord(Board& board, const Candidate& position, const std::string& word)
    {
        return ApplyCharFunction(board, position, word, [](Board& board, int row, int col, char character)
            {
                return board[row][col] == 0 || board[row][col] == character;
            });
    }

    // Place word on position in board.
    void ApplyWord(Board& board, const Candidate& position, const std::string& word)
    {
        ApplyCharFunction(board, position, word, [](Board& board, int row, int col, char character)
            {
                board[row][col] = character;

                return true;
            });
    }

    // Count number of empty cells word will take on position in board.
    size_t CountEmptyCells(Board& board, const Candidate& position, const std::string& word)
    {
        size_t count = 0;

        ApplyCharFunction(board, position, word, [&count](Board& board, int row, int col, char character)
            {
                count += board[row][col] == 0 ? 1 : 0;

                return true;
            });

        return count;
    }

    // Check if word is present on position in board.
    bool CheckWord(Board& board, const Candidate& position, const std::string& word)
    {
        bool result = true;

        ApplyCharFunction(board, position, word, [&result](Board& board, int row, int col, char character)
            {
                result = board[row][col] == character;

                return result;
            });

        return result;
    }

    void PrintBoard(const Board& board)
    {
        for (size_t r = 0; r < board.size(); ++r)
        {
            for (size_t c = 0; c < board[r].size(); ++c)
            {
                std::cout << std::setw(4) << (char)(board[r][c] == 0 ? '.' : board[r][c]);
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    ProcessedCandidates ProcessCandidates(Candidates&& candidates)
    {
        ProcessedCandidates result;

        for (size_t wordSize = 0; wordSize < candidates.size(); ++wordSize)
        {
            for (auto&& candidate : candidates[wordSize])
            {
                result[(size_t)candidate.dir][wordSize].push_back(std::move(candidate));
            }
        }

        return result;
    }

    Candidates GetCandidates(size_t rows, size_t cols)
    {
        Candidates result;

        for (size_t i = Dictionary::MIN_WORD_SIZE; i < Dictionary::MAX_WORD_SIZE; ++i)
        {
            result[i] = GenerateCandidates(rows, cols, i);
            std::shuffle(std::begin(result[i]), std::end(result[i]), g_mt);
        }

        return result;
    }

    Direction GetOpositeDirection(Direction dir)
    {
        switch (dir)
        {
        case Direction::Up:
            return Direction::Down;
        case Direction::Down:
            return Direction::Up;
        case Direction::Left:
            return Direction::Right;
        case Direction::Right:
            return Direction::Left;
        case Direction::UpLeft:
            return Direction::DownRight;
        case Direction::UpRight:
            return Direction::DownLeft;
        case Direction::DownLeft:
            return Direction::UpRight;
        case Direction::DownRight:
            return Direction::UpLeft;
        }

        return Direction::COUNT;
    }

    bool IsOnLine(const Candidate& candidate, int row, int col)
    {
        switch (candidate.dir)
        {
        case Direction::Up:
        case Direction::Down:
            return col == candidate.col;
        case Direction::Left:
        case Direction::Right:
            return row == candidate.row;
        case Direction::UpLeft:
        case Direction::DownRight:
            return candidate.row - row == candidate.col - col;
        case Direction::UpRight:
        case Direction::DownLeft:
            return candidate.row - row == col - candidate.col;
        }

        return true;
    }

    // Return distance of [row, col] from start cell of candidate. Distance is positive if [row, col]
    // is in direction of candidate, zero if it lies on start cell or it is negative.
    // If [row, col] does not lie on the same line as candidate, return nullopt.
    std::optional<int> GetDirectedLineDistance(const Candidate& candidate, int row, int col)
    {
        // check if on line
        if (!IsOnLine(candidate, row, col))
            return std::nullopt;

        switch (candidate.dir)
        {
        case Direction::Up:
        case Direction::UpLeft:
        case Direction::UpRight:
            return candidate.row - row;
        case Direction::Down:
        case Direction::DownLeft:
        case Direction::DownRight:
            return row - candidate.row;
        case Direction::Left:
            return candidate.col - col;
        case Direction::Right:
            return col - candidate.col;
        }

        return std::nullopt;
    }

    std::tuple<int, int> GetEndpoint(const Candidate& candidate, size_t size)
    {
        switch (candidate.dir)
        {
        case Direction::Up:
            return { candidate.row - size + 1, candidate.col };
        case Direction::Down:
            return { candidate.row + size - 1, candidate.col };
        case Direction::Left:
            return { candidate.row, candidate.col - size + 1 };
        case Direction::Right:
            return { candidate.row, candidate.col + size - 1 };
        case Direction::UpLeft:
            return { candidate.row - size + 1, candidate.col - size + 1 };
        case Direction::UpRight:
            return { candidate.row - size + 1, candidate.col + size - 1 };
        case Direction::DownLeft:
            return { candidate.row + size - 1, candidate.col - size + 1 };
        case Direction::DownRight:
            return { candidate.row + size - 1, candidate.col + size - 1 };
        }

        return { 0, 0 };
    }

    bool IsInterceptingCandidate(const Candidate& candidate1, size_t size1, const Candidate& candidate2, size_t size2)
    {
        // check if start point lies on line
        auto startDist = GetDirectedLineDistance(candidate1, candidate2.row, candidate2.col);
        if (!startDist)
            return false;

        // check if end point lies on line
        auto [endRow, endCol] = GetEndpoint(candidate2, size2);
        auto endDist = GetDirectedLineDistance(candidate1, endRow, endCol);
        if (!endDist)
            return false;

        // check if overlaps start point
        if ((startDist >= 0 && endDist <= 0) || (startDist <= 0 && endDist >= 0))
            return true;

        // otherwise check if any point is inside
        if ((*startDist >= 0 && *startDist < (int)size1) || (*endDist >= 0 && *endDist < (int)size1))
            return true;

        return false;
    }

    void RemoveInterceptingCandidates(const Candidate& candidate, size_t candidateSize, ProcessedCandidates& candidates)
    {
        auto removeItercepting = [&](size_t dir)
        {
            for (size_t itSize = 0; itSize < candidates[dir].size(); ++itSize)
            {
                for (auto it = std::begin(candidates[dir][itSize]); it != std::end(candidates[dir][itSize]);)
                {
                    if (IsInterceptingCandidate(candidate, candidateSize, *it, itSize))
                    {
                        it = candidates[dir][itSize].erase(it);
                    }
                    else
                    {
                        it++;
                    }
                }
            }
        };

        // same direction
        removeItercepting((size_t)candidate.dir);
        removeItercepting((size_t)GetOpositeDirection(candidate.dir));
    }

    std::tuple<Board, Words> PositionWords(Dictionary::Data& data, size_t boardRows, size_t boardCols, size_t wordCount, size_t wordSizeFrom, size_t wordSizeTo)
    {
        std::uniform_int_distribution<size_t> randDirection(0, (size_t)Direction::COUNT - 1);
        std::uniform_int_distribution<size_t> randWordSize(wordSizeFrom, std::min(std::max(boardRows, boardCols), wordSizeTo));
        ProcessedCandidates candidates = ProcessCandidates(GetCandidates(boardRows, boardCols));

        Board board(boardRows, std::vector<uint8_t>(boardCols, 0));
        Words words;

        size_t safetyCounter = 0;
        auto resetProgress = [&]()
        {
            safetyCounter = 0;
            words.clear();
            board = Board(boardRows, std::vector<uint8_t>(boardCols, 0));
            candidates = ProcessCandidates(GetCandidates(boardRows, boardCols));
        };

        while (words.size() != wordCount)
        {
            if (safetyCounter == SAFETY_COUNT)
                resetProgress();

            auto word = Dictionary::GetRandomWord(data, randWordSize(g_mt));
            auto direction = randDirection(g_mt);

            if (std::find(std::begin(words), std::end(words), *word) != std::end(words))
                continue;

            for (auto it = std::begin(candidates[direction][word->size()]); it != std::end(candidates[direction][word->size()]); it++)
            {
                if (VerifyWord(board, *it, *word) && CountEmptyCells(board, *it, *word) != 0)
                {
                    ApplyWord(board, *it, *word);

                    RemoveInterceptingCandidates(*it, word->size(), candidates);

                    words.push_back(std::move(*word));

                    safetyCounter = 0;

                    break;
                }
            }

            safetyCounter++;
        }

        return { board, words };
    }

    Board PositionWords(size_t boardRows, size_t boardCols, std::vector<std::string>& words)
    {
        std::uniform_int_distribution<size_t> randDirection(0, (size_t)Direction::COUNT - 1);
        ProcessedCandidates candidates = ProcessCandidates(GetCandidates(boardRows, boardCols));
        std::array<Direction, (size_t)Direction::COUNT> dirs
        {
            Direction::Up, Direction::Down, Direction::Left, Direction::Right, Direction::UpLeft, Direction::UpRight, Direction::DownLeft, Direction::DownRight
        };

        Board board( boardRows, std::vector<uint8_t>( boardCols, 0 ) );

        for (auto itWord = std::begin(words); itWord != std::end(words);)
        {
            std::shuffle(std::begin(dirs), std::end(dirs), g_mt);

            bool wordPlaced = false;

            for (auto dir : dirs)
            {
                for (auto it = std::begin(candidates[(size_t)dir][itWord->size()]); it != std::end(candidates[(size_t)dir][itWord->size()]); it++)
                {
                    if (VerifyWord(board, *it, *itWord) && CountEmptyCells(board, *it, *itWord) != 0)
                    {
                        ApplyWord(board, *it, *itWord);

                        RemoveInterceptingCandidates(*it, itWord->size(), candidates);

                        wordPlaced = true;

                        break;
                    }
                }

                if (wordPlaced)
                    break;
            }

            if (!wordPlaced)
                itWord = words.erase(itWord);
            else
                itWord++;
        }

        return board;
    }

    size_t GetFreeCellsCount(const Board& board)
    {
        size_t res = 0;

        for (size_t r = 0; r < board.size(); ++r)
            for (size_t c = 0; c < board[r].size(); ++c)
                res += board[r][c] == 0 ? 1 : 0;
        return res;
    }

    void FillFreeCells(Board& board, const std::string& message)
    {
        size_t messageIndex = 0;
        for (size_t r = 0; r < board.size(); ++r)
            for (size_t c = 0; c < board[r].size(); ++c)
                board[r][c] = board[r][c] == 0 ? message[messageIndex++] : board[r][c];
    }

    void FillFreeCellsRandom(Board& board)
    {
        std::uniform_int_distribution<size_t> randChar(97, 122);
        for (size_t r = 0; r < board.size(); ++r)
            for (size_t c = 0; c < board[r].size(); ++c)
                board[r][c] = board[r][c] == 0 ? (uint8_t)randChar(g_mt) : board[r][c];
    }
}
