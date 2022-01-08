#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-return-braced-init-list"

#include <vector>
#include <string>
#include <algorithm>
#include <set>

#include <graphs.hpp>
#include <multipoles.hpp>

#include "implementation.h"
#include "utils/connector_validator.hpp"

using namespace ba_graph;

struct graph_props_to_delete {
  std::vector<Location> locs;
  std::vector<Number> vertices;
};

/**
 * Functions to modify struct graph_props_to_delete
 * Add edge, vertex, or clear all data from it.
 * TODO add these modifiers to different file, folder utils
 */

void add_edge_to_gprops(struct graph_props_to_delete &props, int from, int to) {
    props.locs.emplace_back(from, to);
}

void add_edge_to_gprops(struct graph_props_to_delete &props, Location edge) {
    props.locs.push_back(edge);
}

void add_vertex_to_gprops(struct graph_props_to_delete &props, const int v_index) {
    props.vertices.emplace_back(v_index);
}

void add_vertex_to_gprops(struct graph_props_to_delete &props, const Number vertex) {
    props.vertices.push_back(vertex);
}

void clear_props(struct graph_props_to_delete &props) {
    props.vertices = std::vector<Number>();
    props.locs = std::vector<Location>();
}

/**
 * Functions to remove vertex or edge from graph.
 * Returned values are connectors, with newly created vertices.
 */

Connector remove_edge(Graph &g, Location &edge) {
    if (!g.contains( edge )) {
        throw std::invalid_argument( "Graph does not contain one of provided edges." );
    } else if (g[edge.n1()].degree() != 3 || g[edge.n2()].degree() != 3) {
        throw std::invalid_argument("You can't remove connector vertex.");
    }

    Number first = addMultipleV(g,1);
    Number second = addMultipleV(g,1);
    addE(g,Location(edge.n1(), first));
    addE(g,Location(edge.n2(), second));

    deleteE(g, edge);
    return Connector(std::vector<Number>{first, second});
}

Connector remove_vertex(Graph &g, Number &vertex) {
    if (!g.contains(vertex)) {
        throw std::invalid_argument("Graph does not contain one of provided vertices.");
    } else if (g[vertex].degree() != 3) {
        throw std::invalid_argument("You can't remove connector vertex.");
    }

    std::vector<Number> connecting_vertices;
    for (auto neighbor : g[vertex].neighbours()) {
        if (g[neighbor].degree() == 1) {
            deleteV(g, neighbor);
        } else {
            Number added = addMultipleV(g,1);
            addE(g, Location(neighbor, added));
            connecting_vertices.push_back(added);
        }
    }

    deleteV(g, vertex);
    return Connector(connecting_vertices);
}

/**
 * Simple function to check if vertices in graph props form a path in given graph.
 */
bool is_path(const Graph &g, const struct graph_props_to_delete &props) {
    for (int i = 0; i < (props.vertices.size() - 1); i++) {
        if (!g.contains( Location(props.vertices[i], props.vertices[i+1]) ))
            return false;
    }
    return true;
}

/**
 * Functions to modify graphs, to return multipoles with width 6.
 */

Multipole create_by_removing_three_e(Graph &g, struct graph_props_to_delete &props) {
    if (props.locs.size() != 3) {
        char result_error[100];
        sprintf(result_error, "Number of edges must be 3, %lu provided.", props.locs.size());
        std::string error_msg = result_error;
        throw std::range_error(error_msg);
    }

    std::vector<Connector> connectors;
    for (int i = 0; i < 3; i++) {
        connectors.push_back(remove_edge(g, props.locs[i]));
    }

    return Multipole(get_correct_connectors(g, connectors));
}

Multipole create_by_removing_2_vertices(Graph &g, struct graph_props_to_delete &props) {
  if (props.vertices.size() != 2) {
    char result_error[100];
    sprintf(result_error, "Number of vertices must be 2, provided: %lu.", props.vertices.size());
    std::string error_msg = result_error;
    throw std::range_error(error_msg);
  }

  if (props.vertices[0].to_int() == props.vertices[1].to_int()) {
    throw std::invalid_argument("Both of the vertices are the same.");
  }

  if (g.contains(Location( props.vertices[0].to_int(), props.vertices[1].to_int() ))) {
    throw std::invalid_argument("Vertices are incidental. Hint: Use create_by_removing_2_inc_vertices_and_edge, \
      with one these two vertices plus one edge.");
  }

    std::vector<Connector> connectors;
    for (int i = 0; i < 2; i++) {
        connectors.push_back(remove_vertex(g, props.vertices[i]));
    }

    return Multipole(get_correct_connectors(g, connectors));
}

Multipole create_by_removing_2_inc_vertices_and_edge(Graph &g, struct graph_props_to_delete &props) {

  // ----------- BEGIN correctness check -----------

  if (props.vertices.size() != 2) {
    char result_error[100];
    sprintf(result_error, "Number of vertices must be 2, provided: %lu.", props.vertices.size());
    std::string error_msg = result_error;
    throw std::range_error(error_msg);
  }

  if (props.locs.size() != 1) {
    char result_error[100];
    sprintf(result_error, "Number of edges must be 1, %lu provided.", props.locs.size());
    std::string error_msg = result_error;
    throw std::range_error(error_msg);
  }

  if (props.vertices[0].to_int() == props.vertices[1].to_int()) {
    throw std::invalid_argument("Both of the vertices are the same.");
  }

  if (!g.contains(Location(props.vertices[0], props.vertices[1]))) {
    throw std::invalid_argument("The 2 vertices must be incidental.");
  }

  if (props.locs[0].n1().to_int() == props.vertices[0].to_int() ||
      props.locs[0].n2().to_int() == props.vertices[1].to_int() ||
      props.locs[0].n1().to_int() == props.vertices[1].to_int() ||
      props.locs[0].n2().to_int() == props.vertices[0].to_int()
  ) {
    throw std::invalid_argument("The edge can\'t be incidental with any of the 2 vertices.");
  }

  // ----------- END correctness check -----------

    std::vector<Connector> connectors;
    for (int i = 0; i < 2; i++) {
        connectors.push_back(remove_vertex(g, props.vertices[i]));
    }

    connectors.push_back(remove_edge(g, props.locs[0]));
    return Multipole(get_correct_connectors(g, connectors));
}

Multipole create_by_removing_path_length_4(Graph &g, struct graph_props_to_delete &props) {
    if (props.vertices.size() != 4) {
        char result_error[100];
        sprintf(result_error, "Number of vertices must be 4, provided: %lu.", props.vertices.size());
        std::string error_msg = result_error;
        throw std::range_error(error_msg);
    }
    if (!is_path(g, props)) {
        throw std::invalid_argument("These 4 vertices do not form a path in provided graph. Make sure "
                                     "you have passed them in correct order");
    }
    std::vector<Connector> connectors;
    for (auto vertex : props.vertices)
        connectors.push_back(remove_vertex(g, vertex));

    return Multipole(get_correct_connectors(g, connectors));
}

Multipole create_by_removing_vertex_and_3_neighbours(Graph &g, const struct graph_props_to_delete &props) {
    if (props.vertices.size() != 4) {
        char result_error[100];
        sprintf(result_error, "Number of vertices must be 4, provided: %lu.", props.vertices.size());
        std::string error_msg = result_error;
        throw std::range_error(error_msg);
    }

    std::set<int> unique_vertices;
    unique_vertices.insert(props.vertices[0].to_int());
    for (int i = 1; i < 4; i++) {
        if (!g.contains(Location(props.vertices[0], props.vertices[i])))
            throw std::invalid_argument("These vertices are not in correct form. Pass them as: "
                                        "one vertex first and its 3 neighbors after that");
        unique_vertices.insert(props.vertices[i].to_int());
    }

    if (unique_vertices.size() != 4) {
        throw std::invalid_argument("Two of the vertices you have passed are the same. Pass them as: "
                                    "one vertex first and its 3 neighbors after that");
    }

    std::vector<Connector> connectors;
    for (auto vertex : props.vertices)
        connectors.push_back(remove_vertex(g, vertex));

    return Multipole(get_correct_connectors(g, connectors));
}

#pragma clang diagnostic pop