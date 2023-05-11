#ifndef BFS_SOLVER_HPP
#define BFS_SOLVER_HPP

#include "bot.hpp"

class BfsSolver {
private:
    DistCache best_dist;
    TraceCache best_trace;

    int bfs_single(Pos src, DistCache& dist, TraceCache& trace);
    void save_state();

public:
    void find_path();
};

#endif // BFS_SOLVER_HPP