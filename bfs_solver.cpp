#include "bfs_solver.hpp"

#include <queue>
#include <ranges>

void BfsSolver::save_state() {
    std::ofstream ofs(g::state_file.data());

    State best_state{};

    for (int i = 0; i < 4; ++i) {
        for (int x = 0; x < g::height; ++x) {
            for (int y = 0; y < g::width; ++y) {
                if (best_dist[i][x][y] > best_state.step) {
                    best_state = {Pos{x, y}, i, best_dist[i][x][y]};
                }
            }
        }
    }

    std::cout << "Best state: " << best_state.pos.x << ' ' << best_state.pos.y
              << ' ' << best_state.dir << ' ' << best_state.step << std::endl;

    for (;;) {
        auto& [pos, dir, step, _] = best_state;
        ofs << pos.x << ' ' << pos.y << ' ' << dir << std::endl;
        g::map[pos.x][pos.y] = '*';
        if (best_state.dir == g::off) break;

        best_state = best_trace[dir][pos.x][pos.y];
    }

    for (const auto& row : g::map) {
        for (const auto& cell : row) {
            ofs << cell;
        }
        ofs << std::endl;
    }
}

void BfsSolver::find_path() {
    int best = 0;

    for (int i = 0; i < 1; ++i) {
        for (int j = 0; j < 1; ++j) {
            if (g::map[i][j] == '#') continue;

            DistCache dist;
            TraceCache trace;
            
            int len = bfs_single({i, j}, dist, trace);

            if (best < len) {
                best = len;
                best_dist = dist;
                best_trace = trace;
            }
        }
    }

    std::cout << "Best: " << best << std::endl;
}

int BfsSolver::bfs_single(Pos src, DistCache& dist, TraceCache& trace) {
    for (auto& dist_dir : dist) {
        dist_dir.resize(g::height);
        for (auto& dist_row : dist_dir) {
            dist_row.resize(g::width);
        }
    }
    for (auto& trace_dir : trace) {
        trace_dir.resize(g::height);
        for (auto& trace_row : trace_dir) {
            trace_row.resize(g::width);
        }
    }

    std::priority_queue<State> queue;

    dist[g::off][src.x][src.y] = 1;
    queue.push({src, g::off, dist[g::off][src.x][src.y], layer[src.x][src.y]});

    while (!queue.empty()) {
        auto cur_state = queue.top();
        queue.pop();

        const auto& [pos, dir, step, clayer] = cur_state;

        if (step > g::period * (std::min(g::height, g::width) + 1) / 2) continue;
        if (step < dist[dir][pos.x][pos.y]) continue;

        std::cout << pos.x << ' ' << pos.y << std::endl;

        std::vector<int> ord{0, 1, 2, 3};
        if (dir != g::off) {
            ord.erase(std::ranges::find(ord, dir));
            ord.insert(ord.begin(), dir);
        }

        for (int i : ord) {
            if (dir == (i + 2) % 4) continue;

            int x = pos.x + g::dx[i];
            int y = pos.y + g::dy[i];

            if (x == src.x && y == src.y) continue;

            if (!in_bound(x, y)) continue;
            if (g::map[x][y] == '#') continue;

            if (clayer > layer[x][y]) {
                continue;
            } else if ((step + 1) % g::period == 0 && clayer >= layer[x][y]) {
                continue;
            }

            if (dist[i][x][y] < step + 1) {
                dist[i][x][y] = step + 1;
                trace[i][x][y] = cur_state;
                queue.push({{x, y}, i, dist[i][x][y], layer[x][y]});
            }
        }
    }

    int best_dist = 0;
    for (int d = 0; d < 4; ++d) {
        for (int i = 0; i < g::height; ++i) {
            for (int j = 0; j < g::width; ++j) {
                best_dist = std::max(best_dist, dist[d][i][j]);
            }
        }
    }

    return best_dist;
}