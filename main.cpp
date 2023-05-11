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
}