#include "test.h"
#include "wordSearch.h"
#include <optional>
#include <cassert>

namespace WordSearch
{
    bool IsOnLine(const Candidate& candidate, int row, int col);
    std::optional<int> GetDirectedLineDistance(const Candidate& candidate, int row, int col);
    bool IsInterceptingCandidate(const Candidate& candidate1, size_t size1, const Candidate& candidate2, size_t size2);
}

namespace Test
{
    template<class T1, class T2>
    void ASSERT(T1 value, T2 excepted)
    {
        assert(value == excepted);
    }

    void Execute()
    {
        ASSERT(WordSearch::IsOnLine({ 1, 2, WordSearch::Direction::Down }, 2, 3), false);
        ASSERT(WordSearch::IsOnLine({ 1, 2, WordSearch::Direction::Down }, 0, 4), false);
        ASSERT(WordSearch::IsOnLine({ 1, 2, WordSearch::Direction::Down }, 0, 0), false);
        ASSERT(WordSearch::IsOnLine({ 1, 2, WordSearch::Direction::Down }, 1, 0), false);
        ASSERT(WordSearch::IsOnLine({ 1, 2, WordSearch::Direction::Down }, 0, 2), true);
        ASSERT(WordSearch::IsOnLine({ 1, 2, WordSearch::Direction::Down }, 1, 2), true);
        ASSERT(WordSearch::IsOnLine({ 1, 2, WordSearch::Direction::Down }, 4, 2), true);

        ASSERT(WordSearch::IsOnLine({ 3, 4, WordSearch::Direction::Up }, 3, 2), false);
        ASSERT(WordSearch::IsOnLine({ 3, 4, WordSearch::Direction::Up }, 1, 2), false);
        ASSERT(WordSearch::IsOnLine({ 3, 4, WordSearch::Direction::Up }, 0, 4), true);
        ASSERT(WordSearch::IsOnLine({ 3, 4, WordSearch::Direction::Up }, 4, 4), true);

        ASSERT(WordSearch::IsOnLine({ 3, 4, WordSearch::Direction::Left }, 1, 4), false);
        ASSERT(WordSearch::IsOnLine({ 3, 4, WordSearch::Direction::Left }, 1, 2), false);
        ASSERT(WordSearch::IsOnLine({ 3, 4, WordSearch::Direction::Left }, 3, 1), true);
        ASSERT(WordSearch::IsOnLine({ 3, 4, WordSearch::Direction::Left }, 3, 8), true);

        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::Right }, 3, 2), true);
        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::Right }, 1, 2), false);
        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::Right }, 3, 1), true);
        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::Right }, 3, 8), true);

        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::UpLeft }, 2, 1), true);
        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::UpLeft }, 1, 0), true);
        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::UpLeft }, 2, 0), false);
        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::UpLeft }, 4, 4), false);

        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::DownRight }, 2, 1), true);
        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::DownRight }, 1, 0), true);
        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::DownRight }, 1, 1), false);
        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::DownRight }, 5, 4), true);

        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::UpRight }, 2, 1), false);
        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::UpRight }, 5, 4), false);
        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::UpRight }, 1, 4), true);
        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::UpRight }, 4, 1), true);

        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::DownLeft }, 2, 1), false);
        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::DownLeft }, 1, 0), false);
        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::DownLeft }, 5, 0), true);
        ASSERT(WordSearch::IsOnLine({ 3, 2, WordSearch::Direction::DownLeft }, 5, 4), false);

        //

        ASSERT(WordSearch::GetDirectedLineDistance({ 1, 2, WordSearch::Direction::Down }, 3, 2), 2);
        ASSERT(WordSearch::GetDirectedLineDistance({ 1, 2, WordSearch::Direction::Down }, 3, 3), std::nullopt);
        ASSERT(WordSearch::GetDirectedLineDistance({ 1, 2, WordSearch::Direction::Down }, 0, 2), -1);

        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 2, WordSearch::Direction::Up }, 5, 2), -2);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 2, WordSearch::Direction::Up }, 0, 2), 3);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 2, WordSearch::Direction::Up }, 3, 5), std::nullopt);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 2, WordSearch::Direction::Up }, 2, 0), std::nullopt);

        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::Left }, 3, 1), 3);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::Left }, 3, 5), -1);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::Left }, 1, 4), std::nullopt);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::Left }, 5, 2), std::nullopt);

        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::Right }, 3, 1), -3);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::Right }, 3, 5), 1);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::Right }, 1, 4), std::nullopt);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::Right }, 5, 2), std::nullopt);

        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::UpLeft }, 1, 2), 2);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::UpLeft }, 5, 6), -2);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::UpLeft }, 1, 4), std::nullopt);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::UpLeft }, 5, 2), std::nullopt);

        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::DownRight }, 1, 2), -2);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::DownRight }, 5, 6), 2);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::DownRight }, 1, 4), std::nullopt);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::DownRight }, 5, 2), std::nullopt);

        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::UpRight }, 2, 5), 1);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::UpRight }, 4, 3), -1);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::UpRight }, 4, 4), std::nullopt);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::UpRight }, 5, 2), -2);

        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::DownLeft }, 2, 5), -1);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::DownLeft }, 4, 3), 1);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::DownLeft }, 4, 4), std::nullopt);
        ASSERT(WordSearch::GetDirectedLineDistance({ 3, 4, WordSearch::Direction::DownLeft }, 5, 2), 2);

        //

        ASSERT(WordSearch::IsInterceptingCandidate({ 1, 2, WordSearch::Direction::Down }, 4, { 3, 4, WordSearch::Direction::Up }, 3), false);
        ASSERT(WordSearch::IsInterceptingCandidate({ 1, 2, WordSearch::Direction::Down }, 4, { 3, 2, WordSearch::Direction::Up }, 3), true);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::Down }, 4, { 1, 2, WordSearch::Direction::Up }, 3), false);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::Down }, 4, { 1, 4, WordSearch::Direction::Up }, 2), false);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::Down }, 4, { 1, 4, WordSearch::Direction::Down }, 3), true);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::Up }, 3, { 1, 4, WordSearch::Direction::Up }, 2), true);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::Up }, 3, { 1, 4, WordSearch::Direction::Left }, 2), false);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::Right }, 3, { 1, 4, WordSearch::Direction::Up }, 2), false);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::Right }, 3, { 1, 4, WordSearch::Direction::Left }, 2), false);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::Right }, 3, { 1, 4, WordSearch::Direction::Right }, 2), false);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::Right }, 3, { 3, 1, WordSearch::Direction::Right }, 3), false);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::Right }, 3, { 3, 1, WordSearch::Direction::Right }, 4), true);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::Left }, 3, { 3, 1, WordSearch::Direction::Left }, 2), false);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::Left }, 4, { 3, 1, WordSearch::Direction::Left }, 2), true);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::Left }, 3, { 3, 1, WordSearch::Direction::Right }, 2), true);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::Right }, 4, { 3, 1, WordSearch::Direction::Left }, 2), false);

        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::UpRight }, 4, { 3, 1, WordSearch::Direction::Left }, 2), false);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::DownRight }, 4, { 3, 1, WordSearch::Direction::Right }, 2), false);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::UpLeft }, 4, { 3, 1, WordSearch::Direction::Up }, 2), false);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::DownLeft }, 4, { 3, 1, WordSearch::Direction::Down }, 2), false);

        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::UpRight }, 4, { 4, 3, WordSearch::Direction::DownLeft }, 2), false);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::DownRight }, 4, { 1, 2, WordSearch::Direction::DownRight }, 2), false);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::UpLeft }, 1, { 2, 3, WordSearch::Direction::UpLeft }, 2), false);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::DownLeft }, 4, { 4, 3, WordSearch::Direction::UpLeft }, 2), false);

        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::UpRight }, 4, { 1, 6, WordSearch::Direction::DownLeft }, 2), true);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::DownRight }, 4, { 5, 6, WordSearch::Direction::DownRight }, 2), true);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::UpLeft }, 3, { 1, 2, WordSearch::Direction::UpLeft }, 2), true);
        ASSERT(WordSearch::IsInterceptingCandidate({ 3, 4, WordSearch::Direction::DownLeft }, 4, { 4, 3, WordSearch::Direction::UpRight }, 2), true);
    }
}
