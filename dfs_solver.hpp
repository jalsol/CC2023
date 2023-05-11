#ifndef DFS_SOLVER_HPP
#define DFS_SOLVER_HPP

#include "bot.hpp"

#include <vector>
#include <unordered_map>

class DfsSolver {
private:
    DistCache dist;
    std::vector<State> stack;
    std::vector<State> best_stack;
    std::vector<std::vector<bool>> visited;
    std::unordered_map<char, int> freq;

    void dfs_single(State u);

public:
    void save_state();
    void dfs(Pos src);
    void find_path();
};

#endif // DFS_SOLVER_HPP