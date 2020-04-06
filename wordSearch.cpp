#include "wordSearch.h"
#include <random>
#include "dictionary.h"
#include <iostream>
#include <iomanip>

namespace WordSearch
{
    // we process candidates to form data[direction][wordSize]
    // reason is that we want to pick direction uniformly at random
    using ProcessedCandidates = std::array<Candidates, (size_t)Direction::COUNT>;

    using Rand = std::uniform_int_distribution<size_t>;

    std::random_device g_rd;
    std::mt19937 g_mt(g_rd());

    // Number of times we will try to position random word on board before we fail.
    static constexpr size_t SAFETY_COUNT = 2000;
    // How fast we will decrease word length lower bound with unsuccessful attempts.
    static constexpr float WORD_SIZE_DECREMENT_FACTOR = 0.5f;

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
    template<class T, class BOARD>
    bool ApplyCharFunction(BOARD& board, const Candidate& position, const std::string& word, T function)
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
    bool VerifyWord(const Board& board, const Candidate& position, const std::string& word)
    {
        return ApplyCharFunction(board, position, word, [](const Board& board, int row, int col, char character)
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

    using ApplyIndices = std::vector<size_t>;
    ApplyIndices ApplyWordIndices(Board& board, const Candidate& position, const std::string& word)
    {
        ApplyIndices res;
        size_t index = 0;

        ApplyCharFunction(board, position, word, [&index, &res](Board& board, int row, int col, char character)
            {
                if (board[row][col] == 0)
                {
                    board[row][col] = character;
                    res.push_back(index);
                }

                index++;

                return true;
            });

        return res;
    }

    void UnapplyWordIndices(Board& board, const Candidate& position, const std::string& word, const ApplyIndices& indices)
    {
        size_t index = 0;
        size_t indicesIndex = 0;

        ApplyCharFunction(board, position, word, [&index, &indicesIndex, &indices](Board& board, int row, int col, char character)
            {
                if (indicesIndex < indices.size() && index == indices[indicesIndex])
                {
                    board[row][col] = 0;
                    indicesIndex++;
                }

                index++;

                return true;
            });
    }

    // Count number of empty cells word will take on position in board.
    size_t CountEmptyCells(const Board& board, const Candidate& position, const std::string& word)
    {
        size_t count = 0;

        ApplyCharFunction(board, position, word, [&count](const Board& board, int row, int col, char character)
            {
                count += board[row][col] == 0 ? 1 : 0;

                return true;
            });

        return count;
    }

    // Check if word is present on position in board.
    bool CheckWord(const Board& board, const Candidate& position, const std::string& word)
    {
        bool result = true;

        ApplyCharFunction(board, position, word, [&result](const Board& board, int row, int col, char character)
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

    bool IsAnyWordDuplicated(const Board& board, const Words& words, const Candidates& candidates)
    {
        for (const auto& word : words)
        {
            size_t count = 0;

            for (const auto& candidate : candidates[word.size()])
            {
                if (CheckWord(board, candidate, word))
                    count++;

                if (count > 1)
                    return true;
            }
        }

        return false;
    }

    bool VerifyDuplication(const Board& board, const Words& words, const Candidate& position, const std::string& newWord, const Candidates& candidates)
    {
        Board tempBoard(board);
        ApplyWord(tempBoard, position, newWord);
        if (IsAnyWordDuplicated(tempBoard, words, candidates) || IsAnyWordDuplicated(tempBoard, { newWord }, candidates))
            return false;

        return true;
    }

    // This is because we would like to position longer words first.
    Rand GetRandWordSize(size_t from, size_t to, size_t safetyCount)
    {
        return Rand(std::max((int)from, (int)to - 1 - (int)(safetyCount * WORD_SIZE_DECREMENT_FACTOR)), to);
    }

    bool PositionWordRandom(Dictionary::Data& data, Board& board, Words& words, ProcessedCandidates& candidates, size_t wordSizeFrom, size_t wordSizeTo, Rand randDir, const Candidates& checkCandidates)
    {
        size_t safetyCounter = 0;

        while (safetyCounter < SAFETY_COUNT)
        {
            auto direction = randDir(g_mt);
            auto word = Dictionary::GetRandomWord(data, GetRandWordSize(wordSizeFrom, wordSizeTo, safetyCounter)(g_mt));

            if (std::find(std::begin(words), std::end(words), *word) != std::end(words))
                continue;

            std::shuffle(std::begin(candidates[direction][word->size()]), std::end(candidates[direction][word->size()]), g_mt);

            for (auto it = std::begin(candidates[direction][word->size()]); it != std::end(candidates[direction][word->size()]); it++)
            {
                if (VerifyWord(board, *it, *word) && CountEmptyCells(board, *it, *word) != 0)
                {
                    if (!VerifyDuplication(board, words, *it, *word, checkCandidates))
                        continue;

                    ApplyWord(board, *it, *word);
                    RemoveInterceptingCandidates(*it, word->size(), candidates);
                    words.push_back(std::move(*word));

                    return true;
                }
            }

            safetyCounter++;
        }

        return false;
    }

    std::tuple<Board, Words> PositionWords(Dictionary::Data& data, size_t boardRows, size_t boardCols, size_t wordSizeFrom, size_t wordSizeTo)
    {
        Rand randDirStraight(0, (size_t)Direction::Right);
        Rand randDirDiagonal((size_t)Direction::UpLeft, (size_t)Direction::DownRight);

        size_t maxWordSizeTo = std::min(std::max(boardRows, boardCols), wordSizeTo);

        ProcessedCandidates candidates = ProcessCandidates(GetCandidates(boardRows, boardCols));
        Candidates checkCandidates = GetCandidates(boardRows, boardCols);

        Board board(boardRows, std::vector<uint8_t>(boardCols, 0));
        Words words;

        size_t totalCells = boardRows * boardCols;
        size_t freeCells = totalCells;
        // First is positioned with diagonal words.
        Rand currentRandDir = randDirDiagonal;

        while (PositionWordRandom(data, board, words, candidates, wordSizeFrom, maxWordSizeTo, currentRandDir, checkCandidates))
        {
            freeCells = GetFreeCellsCount(board);

            // After half of the cells are positioned we will switch to horizontal/vertical direction.
            if (freeCells < totalCells / 2)
                currentRandDir = randDirStraight;
        }

        return { board, words };
    }

    std::array<Direction, (size_t)Direction::COUNT> GetShuffledDirections()
    {
        std::array<Direction, (size_t)Direction::COUNT> dirs
        {
            Direction::Up, Direction::Down, Direction::Left, Direction::Right, Direction::UpLeft, Direction::UpRight, Direction::DownLeft, Direction::DownRight
        };
        std::shuffle(std::begin(dirs), std::end(dirs), g_mt);

        return dirs;
    }

    void CopyCandidates(const ProcessedCandidates& from, ProcessedCandidates& to)
    {
        for (size_t dir = 0; dir < from.size(); ++dir)
        {
            for (size_t size = 0; size < from[dir].size(); ++size)
            {
                if (to[dir][size].size() == from[dir][size].size())
                    continue;

                to[dir][size].resize(from[dir][size].size());
                for (size_t i = 0; i < from[dir][size].size(); ++i)
                    to[dir][size][i] = from[dir][size][i];
            }
        }
    }

    bool PositionWordsBacktrack(Board& board, const Words& words, size_t wordIndex, const ProcessedCandidates& candidates, const Candidates& checkCandidates)
    {
        if (words.size() == wordIndex)
            return true;

        ProcessedCandidates tempCandidates = candidates;

        for (auto dir : GetShuffledDirections())
        {
            for (auto it = std::begin(candidates[(size_t)dir][words[wordIndex].size()]); it != std::end(candidates[(size_t)dir][words[wordIndex].size()]); it++)
            {
                if (VerifyWord(board, *it, words[wordIndex]) && CountEmptyCells(board, *it, words[wordIndex]) != 0)
                {
                    // This takes long time, temporarily disabled
                    //if (!VerifyDuplication(board, words, *it, word, checkCandidates))
                    //    continue;

                    auto applyIndices = ApplyWordIndices(board, *it, words[wordIndex]);

                    CopyCandidates(candidates, tempCandidates);
                    RemoveInterceptingCandidates(*it, words[wordIndex].size(), tempCandidates);

                    if (auto resBoard = PositionWordsBacktrack(board, words, wordIndex + 1, tempCandidates, checkCandidates))
                        return resBoard;

                    UnapplyWordIndices(board, *it, words[wordIndex], applyIndices);
                }
            }
        }

        return false;
    }

    std::optional<Board> PositionWords(size_t boardRows, size_t boardCols, const Words& words)
    {
        ProcessedCandidates candidates = ProcessCandidates(GetCandidates(boardRows, boardCols));
        Candidates checkCandidates = GetCandidates(boardRows, boardCols);
        Board board(boardRows, std::vector<uint8_t>(boardCols, 0));

        if (PositionWordsBacktrack(board, words, 0, candidates, checkCandidates))
            return board;

        return std::nullopt;
    }

    size_t GetFreeCellsCount(const Board& board)
    {
        size_t res = 0;

        for (size_t r = 0; r < board.size(); ++r)
            for (size_t c = 0; c < board[r].size(); ++c)
                res += board[r][c] == 0 ? 1 : 0;
        return res;
    }

    void FillFreeCellsRandom(Board& board, const Words& words)
    {
        Rand randChar(97, 122);
        auto candidates = GetCandidates(board.size(), board[0].size());

        for (size_t r = 0; r < board.size(); ++r)
        {
            for (size_t c = 0; c < board[r].size(); ++c)
            {
                if (board[r][c] != 0)
                    continue;

                board[r][c] = (uint8_t)randChar(g_mt);
                while(IsAnyWordDuplicated(board, words, candidates))
                    board[r][c] = (uint8_t)randChar(g_mt);
            }
        }
    }
}
