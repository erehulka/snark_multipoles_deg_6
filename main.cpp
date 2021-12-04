#include "implementation.h"

#include <graphs.hpp>
#include <snarks/colouring_cvd.hpp>
#include "create_multipole_deg6.hpp"

using namespace ba_graph;


int main() {
    graph_props_to_delete some_edges;
    add_edge_to_gprops(some_edges, 1, 2);
    add_edge_to_gprops(some_edges, Location(Number(8), Number(3)));
    add_edge_to_gprops(some_edges, Location(4, 9));

    Graph g(create_petersen());
    std::cout << g << std::endl;
    create_by_removing_three_e(g, some_edges);
    std::cout << g << std::endl;

    Graph g2(create_petersen());
    std::cout << g2 << std::endl;
    graph_props_to_delete some_edges2;
    add_vertex_to_gprops(some_edges2, 4);
    add_vertex_to_gprops(some_edges2, 0);
    add_vertex_to_gprops(some_edges2, Number(3));
    add_vertex_to_gprops(some_edges2, Number(9));
    create_by_removing_vertex_and_3_neighbours(g2, some_edges2);
    std::cout << g2 << std::endl;

    return 0;
}