#include <bits/stdc++.h>
using namespace std;
typedef int8_t i8;
typedef vector<pair<int, int>> vii;

class Player
{
public:
    i8* state;
    int row, col;
    int player, winLength;

    i8 indexAt(const int r, const int c) const
    {
        return state[r * col + c];
    }

    bool winFromLastMove(const i8 setPlayer, const int lastR, const int lastC) const
    {
        const pair<int, int> directions[] = {
            {0, 1}, {1, 0}, {1, 1}, {1, -1}
        };

        for (auto [dx, dy] : directions)
        {
            int count = 1;

            // Check in positive direction
            for (int i = 1; i < winLength; i++)
            {
                int nr = lastR + i * dx;
                int nc = lastC + i * dy;
                if (nr >= 0 && nr < row && nc >= 0 && nc < col && indexAt(nr, nc) == setPlayer)
                    count++;
                else
                    break;
            }

            // Check in negative direction
            for (int i = 1; i < winLength; i++)
            {
                int nr = lastR - i * dx;
                int nc = lastC - i * dy;
                if (nr >= 0 && nr < row && nc >= 0 && nc < col && indexAt(nr, nc) == setPlayer)
                    count++;
                else
                    break;
            }

            if (count >= winLength)
                return true;
        }
        return false;
    }

    // lastR, lastC are the most recent move coordinates (-1 if none)
    int minimaxABprune(const bool turn, const int alpha, const int beta, vii& free,
                       int depth, int lastR, int lastC)
    {
        const i8 opponent = (player == 1) ? 2 : 1;
        const i8 checkPlayer = turn ? player : opponent;

        // Terminal conditions - only check from last move
        if (lastR != -1 && lastC != -1)
        {
            if (winFromLastMove(checkPlayer, lastR, lastC)) return turn ? 1 : -1;
        }

        if (free.empty()) return 0;
        if (depth <= 0) return 0;

        if (turn)
        {
            int best = INT_MIN;
            for (int i = 0; i < 2; i++)
            {
                auto [x, y] = free[i];
                state[x * col + y] = player;

                swap(free[i], free.back());
                free.pop_back();

                best = max(best, minimaxABprune(true, alpha, beta, free, depth - 1, x, y));

                free.push_back({x, y});
                swap(free[i], free.back());

                state[x * col + y] = 0;

                if (best > beta) break;
            }
            return best;
        }

        int best = INT_MAX;
        for (int i = 0; i < 2; i++)
        {
            auto [x, y] = free[i];
            state[x * col + y] = opponent;

            swap(free[i], free.back());
            free.pop_back();

            best = min(best, minimaxABprune(false, alpha, beta, free, depth - 1, x, y));

            free.push_back({x, y});
            swap(free[i], free.back());

            state[x * col + y] = 0;

            if (best < alpha) break;
        }

        return best;
    }

    Player(i8* s, const int r, const int c, const int id, const int len)
    {
        state = s;
        row = r;
        col = c;
        player = id;
        winLength = len;
    }
};

extern "C" {

void chooseMove(
    i8* state,
    const int row, const int col,
    const int playerID,
    const int winLength,
    int* outrow, int* outcol)
{
    Player p{state, row, col, playerID, winLength};
    vii free;

    for (int r = 0; r < row; r++)
        for (int c = 0; c < col; c++)
            if (state[r * col + c] == 0)
                free.emplace_back(r, c);

    if (free.empty()) return;

    int bestScore = INT_MIN;
    int bestR = -1, bestC = -1;

    for (int i = 0; i < free.size(); i++)
    {
        auto [r, c] = free[i];
        state[r * col + c] = playerID;

        swap(free[i], free.back());
        free.pop_back();

        int score = p.minimaxABprune(true, INT_MIN, INT_MAX, free, 4, r, c);

        free.push_back({r, c});
        swap(free[i], free.back());

        state[r * col + c] = 0;

        if (score > bestScore)
        {
            bestScore = score;
            bestR = r;
            bestC = c;
        }
    }

    *outrow = bestR;
    *outcol = bestC;
}

}