#include "bot.hpp"

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

bool in_bound(int x, int y) {
    return x >= 0 && x < g::height && y >= 0 && y < g::width;
}

