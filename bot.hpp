#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Pos {
    int x, y;
};

namespace g {

constexpr std::string_view input_file = "MAP.INP";
constexpr std::string_view output_file = "MOVE.OUT";
constexpr std::string_view state_file = "STATE.DAT";

inline int width, height;
inline int turn, tick;
inline Pos cur;
inline char team_color;
inline std::vector<std::vector<char>> map;

} // namespace g

inline std::vector<std::vector<int>> layer;

void read_inputs();
void build_layer();
void save_state();

#endif // BOT_HPP