#include "bot.hpp"
#include "dfs_solver.hpp"

#include <cstdlib>
#include <ctime>

int main() {
    read_inputs();

    if (g::turn == 0) {
        build_layer();
        DfsSolver solver;
        solver.find_path();
    }

    response();

    if (g::turn > 0 && g::turn % g::period == 0) {
        build_layer();
        DfsSolver solver;
        solver.dfs(g::cur);
        solver.save_state();
    }
}