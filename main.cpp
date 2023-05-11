#include "bot.hpp"
#include "dfs_solver.hpp"

int main() {
    read_inputs();

    // TODO: only build once in the beginning
    build_layer();

    DfsSolver solver;
    solver.find_path();
}