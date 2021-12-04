#include "implementation.h"

#include <graphs.hpp>
#include <snarks/colouring_cvd.hpp>
#include "create_multipole_deg6.hpp"

using namespace ba_graph;


int main() {
    graph_props_to_delete some_edges;
    add_edge_to_gprops(some_edges, 1, 2);
    add_edge_to_gprops(some_edges, 8, 3);
    add_edge_to_gprops(some_edges, 4, 9);

    Graph g(create_petersen());
    std::cout << g << std::endl;
    create_by_removing_three_e(g, some_edges);
    std::cout << g << std::endl;

    return 0;
}