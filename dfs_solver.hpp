#ifndef DFS_SOLVER_HPP
#define DFS_SOLVER_HPP

#include "bot.hpp"

#include <vector>

class DfsSolver {
private:
    DistCache dist;
    std::vector<State> stack;
    std::vector<State> best_stack;
    std::vector<std::vector<bool>> visited;

    void dfs(Pos src);
    void dfs_single(State u);
    void save_state();

public:
    void find_path();
};

#endif // DFS_SOLVER_HPP