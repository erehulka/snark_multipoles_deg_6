#include <cassert>

#include "implementation.h"
#include <graphs.hpp>
#include <snarks/colouring_cvd.hpp>
#include "./utils/webpage_parser.hpp"
#include "./utils/multipole_creator.hpp"
#include "create_multipole_deg6.hpp"

using namespace ba_graph;

template<class T>
void assert_exception(T func, Graph &g, struct graph_props_to_delete &props) {
    bool caught = false;
    try {
        func(g, props);
    } catch (const std::exception &e) {
        caught = true;
    }

    assert(caught);
}

void test_create_by_removing_three_edges() {
    Graph g(create_petersen());

    graph_props_to_delete some_edges;
    add_edge_to_gprops(some_edges, 1, 2);
    add_edge_to_gprops(some_edges, Location(Number(8), Number(3)));
    add_edge_to_gprops(some_edges, Location(4, 9));

    Multipole result = create_by_removing_three_e(g, some_edges);
    assert(result.size() == 3);
    for (const auto& con : result.connectors)
        assert(con.size() == 2);

    assert(g.contains(Location(1, 10)));
    assert(!g.contains(Location(1, 2)));

    Graph g2(create_petersen());
    clear_props(some_edges);
    add_edge_to_gprops(some_edges, 1, 2);
    add_edge_to_gprops(some_edges, 3, 2);
    add_edge_to_gprops(some_edges, 1, 4);
    add_edge_to_gprops(some_edges, 9, 8);
    assert_exception(create_by_removing_three_e, g2, some_edges);
}

void test_create_by_removing_vertex_and_3_neighbours() {
    Graph g(create_petersen());
    graph_props_to_delete props;
    add_vertex_to_gprops(props, 4);
    add_vertex_to_gprops(props, 0);
    add_vertex_to_gprops(props, Number(3));
    add_vertex_to_gprops(props, Number(9));
    Multipole result = create_by_removing_vertex_and_3_neighbours(g, props);

    assert(result.size() == 3);
    for (const auto &con : result.connectors)
        assert(con.size() == 2);

    clear_props(props);
    add_vertex_to_gprops(props, 0);
    add_vertex_to_gprops(props, Number(3));
    add_vertex_to_gprops(props, 4);
    Graph g2(create_petersen());

    assert_exception(create_by_removing_vertex_and_3_neighbours, g2, props);
    add_vertex_to_gprops(props, 4);
    assert_exception(create_by_removing_vertex_and_3_neighbours, g2, props);
    props.vertices[3] = Number(9);
    assert_exception(create_by_removing_vertex_and_3_neighbours, g2, props);

    clear_props(props);
    add_vertex_to_gprops(props, 7);
    add_vertex_to_gprops(props, 2);
    add_vertex_to_gprops(props, 5);
    add_vertex_to_gprops(props, 9);
    Graph g3(create_petersen());
    result = create_by_removing_vertex_and_3_neighbours(g3, props);

    assert(result.size() == 3);
    assert(result.check_in(g3));
    assert(!result.check_in(g2));
    for (const auto &con : result.connectors)
        assert(con.size() == 2);
}

void test_create_by_removing_2_vertices() {
    Graph g(create_petersen());
    graph_props_to_delete props;
    add_vertex_to_gprops(props, 4);
    add_vertex_to_gprops(props, 8);

    Multipole result = create_by_removing_2_vertices(g, props);

    assert(result.size() == 2);
    for (const auto &con : result.connectors)
        assert(con.size() == 3);
    assert(result.connectors[0].numbers[0] == 10);
    assert(result.connectors[1].numbers[0] == 13);

    clear_props(props);
    Graph g2(create_petersen());
    add_vertex_to_gprops(props, 4);
    assert_exception(create_by_removing_2_vertices, g2, props);
    add_vertex_to_gprops(props, 0);
    assert_exception(create_by_removing_2_vertices, g2, props);
}

void test_create_by_removing_2_inc_vertices_and_edge() {
    Graph g(create_petersen());
    graph_props_to_delete props;
    add_vertex_to_gprops(props, 2);
    add_vertex_to_gprops(props, 3);
    add_edge_to_gprops(props, 9, 6);

    assert(!g.contains(10));
    assert(g.size() == 15);
    Multipole result = create_by_removing_2_inc_vertices_and_edge(g, props);

    assert(result.size() == 3);
    assert(g.contains(10));
    for (const auto &con : result.connectors)
        assert(con.size() == 2);

    clear_props(props);
    Graph g2(create_petersen());
    add_vertex_to_gprops(props, 2);
    assert_exception(create_by_removing_2_inc_vertices_and_edge, g, props);
    add_vertex_to_gprops(props, 2);
    assert_exception(create_by_removing_2_inc_vertices_and_edge, g, props);
    add_edge_to_gprops(props, 9, 6);
    assert_exception(create_by_removing_2_inc_vertices_and_edge, g, props);
    props.vertices[1] = Number(9);
    assert_exception(create_by_removing_2_inc_vertices_and_edge, g, props);

    clear_props(props);
    add_vertex_to_gprops(props, 3);
    add_vertex_to_gprops(props, 2);
    add_edge_to_gprops(props, 2, 1);
    assert_exception(create_by_removing_2_inc_vertices_and_edge, g, props);

    clear_props(props);
    add_vertex_to_gprops(props, 1);
    add_edge_to_gprops(props, 5, 7);
    add_vertex_to_gprops(props, 0);
    result = create_by_removing_2_inc_vertices_and_edge(g2, props);
    assert(result.size() == 3);
}

void test_create_by_removing_path_length_4() {
    Graph g(create_petersen());
    graph_props_to_delete props;
    add_vertex_to_gprops(props, 2);
    add_vertex_to_gprops(props, 3);
    add_vertex_to_gprops(props, 8);
    add_vertex_to_gprops(props, 6);

    Multipole result = create_by_removing_path_length_4(g, props);

    assert(result.size() == 4);
    assert(result.check_in(g));
    for (const auto &con : result.connectors)
        assert(con.size() == 2 || con.size() == 1);

    Graph g2(create_petersen());
    clear_props(props);
    add_vertex_to_gprops(props, 2);
    add_vertex_to_gprops(props, 3);
    add_vertex_to_gprops(props, 8);
    assert_exception(create_by_removing_path_length_4, g2, props);
    add_vertex_to_gprops(props, 0);
    assert_exception(create_by_removing_path_length_4, g2, props);
    props.vertices[3] = Number(5);

    result = create_by_removing_path_length_4(g2, props);
    assert(result.size() == 4);
}


int main() {
    std::vector<Graph> result = generate_graphs_from_webpage("https://hog.grinvin.org/data/snarks/Generated_graphs.22.05.sn.cyc4.g6");
    std::vector<std::pair<Graph*, Multipole*> > allMulti = create_all_multipoles_from_graph(result[0]);
    std::cout << allMulti.size() << std::endl;

    test_create_by_removing_three_edges();
    test_create_by_removing_vertex_and_3_neighbours();
    test_create_by_removing_2_vertices();
    test_create_by_removing_2_inc_vertices_and_edge();
    test_create_by_removing_path_length_4();

    std::cout << "All tests ran - asserts true." << std::endl;
    return 0;
}
