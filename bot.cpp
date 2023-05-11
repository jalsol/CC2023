#include "bot.hpp"

void read_inputs() {
    std::ifstream ifs(g::input_file.data());
    ifs >> g::height >> g::width >> g::period >> g::turn;
    ifs >> g::cur.x >> g::cur.y >> g::color;

    int _t;
    ifs >> _t;
    while (_t--) {
        int _x, _y;
        char _c;
        ifs >> _x >> _y >> _c;
    }

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

bool in_bound(int x, int y) {
    return x >= 0 && x < g::height && y >= 0 && y < g::width;
}

void response() {
    std::ifstream ifs(g::state_file.data());
    std::ofstream ofs(g::output_file.data());

    if (g::turn > 0 && (g::cur.x < 0 || g::cur.y < 0)) {
        ofs << "-1\n";
        return;
    }

    int size; ifs >> size;

    std::vector<Pos> ans(size);
    for (auto& [x, y] : ans) ifs >> x >> y;

    if (g::turn >= ans.size()) {
        ofs << "-2\n";
    } else {
        g::cur = ans[g::turn];
        ofs << g::cur.x << ' ' << g::cur.y << '\n';
    }
}