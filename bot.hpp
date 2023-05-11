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

    bool operator<(const State& other) const {
        return step < other.step;
    }
};

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
inline std::vector<std::vector<char>> map;

} // namespace g

inline std::vector<std::vector<int>> layer;
inline std::array<std::vector<std::vector<int>>, 5> dist;
inline std::array<std::vector<std::vector<State>>, 4> trace;

void read_inputs();
void build_layer();
void save_state();
void bfs();
bool in_bound(int x, int y);

#endif // BOT_HPP