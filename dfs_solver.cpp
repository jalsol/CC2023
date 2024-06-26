#include "dfs_solver.hpp"

#include <algorithm>
#include <cctype>

void DfsSolver::save_state() {
    std::ofstream ofs(g::state_file.data());

    ofs << best_stack.size() << '\n';

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

    freq.clear();

    for (int i = 0; i < g::height; ++i) {
        for (int j = 0; j < g::width; ++j) {
            if (std::tolower(g::map[i][j]) == std::tolower(g::color)) {
                visited[i][j] = true;
            }
            ++freq[std::toupper(g::map[i][j])];
        }
    }

    dist[g::off][src.x][src.y] = 1;
    stack.clear();

    dfs_single({src, g::off, g::turn + 1, 0});
}

void DfsSolver::dfs_single(State u) {
    stack.push_back(u);

    const auto& [pos, dir, step, clayer] = u;
    std::vector<int> ord{0, 1, 2, 3};
    bool travelled = false;

    visited[pos.x][pos.y] = true;

    if (step > g::height * g::width) goto exit;
    if (step < dist[dir][pos.x][pos.y]) goto exit;

    if (dir != g::off) {
        ord.erase(std::find(ord.begin(), ord.end(), (dir + 2) % 4));
        ord.erase(std::find(ord.begin(), ord.end(), dir));
        ord.insert(ord.begin(), dir);
    }

    std::sort(ord.begin(), ord.end(), [&](int i, int j) {
        char lhs =
            in_bound(pos.x + g::dx[i], pos.y + g::dy[i])
            ? g::map[pos.x + g::dx[i]][pos.y + g::dy[i]]
            : '\0';

        char rhs =
            in_bound(pos.x + g::dx[j], pos.y + g::dy[j])
            ? g::map[pos.x + g::dx[j]][pos.y + g::dy[j]]
            : '\0';

        return freq[lhs] > freq[rhs];
    });

    for (int i : ord) {
        int x = pos.x + g::dx[i];
        int y = pos.y + g::dy[i];

        if (!in_bound(x, y)) continue;
        if (g::map[x][y] == '#' || std::islower(g::map[x][y])) continue;
        if (visited[x][y]) continue;

        if (clayer > layer[x][y]) {
            continue;
        }

        if (step % g::period == 0 && layer[x][y] <= (step / g::period) - 1) {
            continue;
        }

        if (dist[i][x][y] < step + 1) {
            dist[i][x][y] = step + 1;
            dfs_single({{x, y}, i, step + 1, layer[x][y]});
            travelled = true;
        }
    }

exit:
    if (!travelled) {
        if (best_stack.size() < stack.size()) {
            best_stack = stack;
        } else if (best_stack.size() == stack.size() && std::rand() % 2) {
            best_stack = stack;
        }
    }

    stack.pop_back();
    visited[pos.x][pos.y] = false;
}
