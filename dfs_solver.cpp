#include "dfs_solver.hpp"

#include <ranges>

void DfsSolver::save_state() {
    std::ofstream ofs(g::state_file.data());

    for (const auto& [pos, dir, step, layer] : best_stack) {
        ofs << pos.x << ' ' << pos.y << std::endl;
    }
}

void DfsSolver::find_path() {
    for (int i = 0; i < g::height; ++i) {
        for (int j = 0; j < g::width; ++j) {
            if (g::map[i][j] == '#') continue;
            dfs({i, j});
        }
    }

    std::cout << best_stack.size() << '\n';
    save_state();
}

void DfsSolver::dfs(Pos src) {
    for (auto& dist_dir : dist) {
        dist_dir.resize(g::height);
        for (auto& dist_row : dist_dir) {
            dist_row.assign(g::width, 0);
        }
    }

    visited.resize(g::height);
    for (auto& visited_row : visited) {
        visited_row.assign(g::width, false);
    }

    dist[g::off][src.x][src.y] = 1;
    stack.clear();

    dfs_single({src, g::off, 1, 0});
}

void DfsSolver::dfs_single(State u) {
    stack.push_back(u);

    const auto& [pos, dir, step, clayer] = u;
    std::vector<int> ord{0, 1, 2, 3};
    bool travelled = false;

    visited[pos.x][pos.y] = true;

    if (step > g::period * (std::min(g::height, g::width) + 1) / 2) goto exit;
    if (step < dist[dir][pos.x][pos.y]) goto exit;

    if (dir != g::off) {
        ord.erase(std::ranges::find(ord, dir));
        ord.insert(ord.begin(), dir);
    }

    for (int i : ord) {
        if (dir == (i + 2) % 4) continue;

        int x = pos.x + g::dx[i];
        int y = pos.y + g::dy[i];

        if (!in_bound(x, y)) continue;
        if (g::map[x][y] == '#') continue;
        if (visited[x][y]) continue;

        if (clayer > layer[x][y]) {
            continue;
        } else if ((step + 1) % g::period == 0 && clayer >= layer[x][y]) {
            continue;
        }

        if (dist[i][x][y] < step + 1) {
            dist[i][x][y] = step + 1;
            dfs_single({{x, y}, i, step + 1, layer[x][y]});
            travelled = true;
        }
    }

exit:
    if (travelled) {
        if (best_stack.size() < stack.size()) {
            best_stack = stack;
        }
    }

    stack.pop_back();
    visited[pos.x][pos.y] = false;
}
