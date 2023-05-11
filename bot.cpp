#include "bot.hpp"

#include <queue>
#include <stack>

void read_inputs() {
    std::ifstream ifs(g::input_file.data());
    char _;
    ifs >> g::height >> g::width >> g::period;
    //  >> g::turn;
    // ifs >> g::cur.x >> g::cur.y >> _;

    g::map.resize(g::height);
    for (auto& row : g::map) {
        row.resize(g::width);
        for (auto& cell : row) {
            ifs >> cell;
        }
    }
}

void build_layer() {
    layer.resize(g::height);
    for (auto& row : layer) {
        row.resize(g::width);
    }

    for (int y = 0; y < g::height; ++y) {
        int limit;
        if (y < g::height / 2)
            limit = std::min(y + 1, (g::width + 1) / 2);
        else
            limit = std::min(g::height - y, (g::width + 1) / 2);

        int x;

        for (x = 0; x < limit; ++x) {
            layer[y][x] = layer[y][g::width - x - 1] = x;
        }

        for (; x < g::width - limit; ++x) {
            layer[y][x] = limit - 1;
        }
    }
}

void save_state() {
    std::ofstream ofs(g::state_file.data());

    State best_state{};

    for (int i = 0; i < 4; ++i) {
        for (int x = 0; x < g::height; ++x) {
            for (int y = 0; y < g::width; ++y) {
                if (dist[i][x][y] > best_state.step) {
                    best_state = {Pos{x, y}, i, dist[i][x][y]};
                }
            }
        }
    }

    for (;;) {
        auto& [pos, dir, step] = best_state;
        ofs << pos.x << ' ' << pos.y << ' ' << dir << std::endl;
        g::map[pos.x][pos.y] = '*';
        if (best_state.dir == g::off) break;

        best_state = trace[best_state.dir][best_state.pos.x][best_state.pos.y];
    }

    for (const auto& row : g::map) {
        for (const auto& cell : row) {
            ofs << cell;
        }
        ofs << std::endl;
    }
}

bool in_bound(int x, int y) {
    return x >= 0 && x < g::height && y >= 0 && y < g::width;
}

void bfs() {
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

    for (int i = 0; i < g::height; ++i) {
        for (int j = 0; j < g::width; ++j) {
            if (g::map[i][j] == '#') continue;

            dist[g::off][i][j] = 1;
            queue.push({{i, j}, g::off, dist[g::off][i][j]});
        }
    }

    while (!queue.empty()) {
        auto cur_state = queue.top();
        queue.pop();

        const auto& [pos, dir, step] = cur_state;

        if (step > g::period * (std::min(g::height, g::width) + 1) / 2) continue;
        if (step < dist[dir][pos.x][pos.y]) continue;

        for (int i = 0; i < 4; ++i) {
            if (dir == (i + 2) % 4) continue;

            int x = pos.x + g::dx[i];
            int y = pos.y + g::dy[i];
            if (!in_bound(x, y)) continue;
            if (g::map[x][y] == '#') continue;

            if ((step + 1) % g::period == 0 && layer[pos.x][pos.y] + 1 != layer[x][y]) {
                continue;
            } else if (layer[pos.x][pos.y] > layer[x][y]) {
                continue;
            }

            if (dist[i][x][y] < dist[dir][pos.x][pos.y] + 1) {
                dist[i][x][y] = dist[dir][pos.x][pos.y] + 1;
                trace[i][x][y] = cur_state;
                queue.push({{x, y}, i, dist[i][x][y]});
            }
        }
    }
}