#include <bits/stdc++.h>

using namespace std;
#define FastIO                                                                 \
  std::ios_base::sync_with_stdio(false);                                       \
  cin.tie(NULL);
typedef int8_t i8;
typedef pair<int, int> pii;

extern "C" {
constexpr int INF = 1000000000;

struct Cell {
  int f, g;
  int x, y;

  bool operator>(const Cell &b) const {
    return f > b.f;
  }
};

static bool valid(const int x, const int y, const int rows, const int cols) {
  return x >= 0 && x < rows && y >= 0 && y < cols;
}

static int manhattan_h(const pii &start, const pii &end) {
  return abs(start.first - end.first) + abs(start.second - end.second);
}

static int chebyshev_h(const pii &start, const pii &end) {
  // allegedly more accuate for 8 directional planning
  return max(abs(start.first - end.first), abs(start.second - end.second));
}

void aStar(const i8 *data, const int rows, const int cols, const int startX,
           const int startY, const int endX, const int endY, int *resPath,
           int *resCnt) {
  // cpp lambda function; stores [cols] by value
  // this is bad but... contiguous array indexing
  auto idx = [cols](const int x, const int y) { return x * cols + y; };

  *resCnt = 0; // len of result path, is pointer to "return" to python together
               // with path array
  const int goal = idx(endX, endY);
  auto hasGoal = false;
  const int n = rows * cols;
  vector score(n, INF);
  vector parent(n, -1);
  vector<uint8_t> closed(n, 0);

  priority_queue<Cell, vector<Cell>, greater<>> open; // (type, container, compare by)
  score[idx(startX, startY)] = 0;
  const Cell s = {
    chebyshev_h({startX, startY}, {endX, endY}),
    0,
    startX,startY};
  open.push(s);

  while (!open.empty()) {
    const Cell cnt = open.top();
    open.pop();

    const int cidx = idx(cnt.x, cnt.y);
    if (closed[cidx])
      continue;
    closed[cidx] = 1;
    if (cidx == goal) {
      hasGoal = true;
      break;
    }

    for (int next = 0; next < 8; next++) {
      constexpr int dirY[8] = {0, 0, 1, -1, 1, -1, 1, -1};
      constexpr int dirX[8] = {1, -1, 0, 0, 1, 1, -1, -1};

      int nx = cnt.x + dirX[next];
      int ny = cnt.y + dirY[next];
      if (!valid(nx, ny, rows, cols))
        continue;

      const int nidx = idx(nx, ny);
      if (closed[nidx])
        continue;
      if (data[nidx] != 0)
        continue;

      if (const int ng = score[cidx] + 1; ng < score[nidx]) {
        score[nidx] = ng;
        parent[nidx] = cidx;
        const int f = ng + manhattan_h({nx, ny}, {endX, endY});
        open.push({f, ng, nx, ny});
      }
    }
  }
  if (!hasGoal)
    return;

  // trace back path
  vector<pii> path;
  int cnt = goal;
  while (cnt != -1) {
    int x = cnt / cols;
    int y = cnt % cols;
    path.emplace_back(x, y);
    cnt = parent[cnt];
  }
  ranges::reverse(path);
  int out = 0;
  for (auto &[x, y] : path) {
    resPath[out++] = x;
    resPath[out++] = y;
  }
  *resCnt = static_cast<int>(path.size());
}
}
