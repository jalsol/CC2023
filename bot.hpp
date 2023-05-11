#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>

struct Pos {
    int x, y;
};

struct State {
    Pos pos;
    int dir;
    int step;
    int layer;

    bool operator<(const State& other) const {
        if (layer != other.layer)
            return layer > other.layer;
        return step < other.step;
    }
};

using DistCache = std::array<std::vector<std::vector<int>>, 5>;
using TraceCache = std::array<std::vector<std::vector<State>>, 4>;

namespace g {

constexpr std::string_view input_file = "MAP.INP";
constexpr std::string_view output_file = "MOVE.OUT";
constexpr std::string_view state_file = "STATE.DAT";

constexpr int dx[] = {0, 1, 0, -1};
constexpr int dy[] = {-1, 0, 1, 0};

constexpr int off = 4;

inline int width, height;
inline int turn, period;
inline Pos cur;
inline char color;
inline std::vector<std::vector<char>> map;

} // namespace g

inline std::vector<std::vector<int>> layer;

void read_inputs();
void build_layer();
bool in_bound(int x, int y);
void response();
void out_of_path();

#endif // BOT_HPP