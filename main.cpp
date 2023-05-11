#include "bot.hpp"

int main() {
    read_inputs();
    std::cout << "Running...\n";

    // TODO: only build once in the beginning
    build_layer();
    bfs();

    save_state();
}