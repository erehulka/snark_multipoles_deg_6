#include "implementation.h"
#include <vector>
#include <string>

#include <graphs.hpp>

using namespace ba_graph;

struct graph_props_to_delete {
  std::vector<Location> locs;
  std::vector<Number> vertices;
};

void create_by_removing_three_e(Graph &g, struct graph_props_to_delete &props) {
  if (props.locs.size() != 3) {
    char result_error[100];
    sprintf(result_error, "Number of edges must be 3, %d provided.", props.locs.size());
    std::string error_msg = result_error;
    throw std::range_error(error_msg);
  }
  // TODO Overit ci nahodou neodstranujeme 3 hrany incidentne s jednym vrcholom.
  for (int i = 0; i < (int) props.locs.size(); i++) {
    if (!g.contains( props.locs[i] )) {
      throw std::invalid_argument( "Graph does not contain one of provided edges." );
    } else {
      deleteE(g, props.locs[i]);
    }
  }
}

void create_by_removing_2_vertices(Graph &g, struct graph_props_to_delete &props) {
  if (props.vertices.size() != 2) {
    char result_error[100];
    sprintf(result_error, "Number of vertices must be 2, provided: %d.", props.vertices.size());
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

  for (int i = 0; i < (int) props.vertices.size(); i++) {
    if (!g.contains(props.vertices[i])) {
      throw std::invalid_argument("Graph does not contain one of provided vertices.");
    } else {
      deleteV(g, props.vertices[i]);
    }
  }
}

void add_edge_to_gprops(struct graph_props_to_delete &props, int from, int to) {
  props.locs.push_back( Location(from, to) );
}

void add_vertex_to_props(struct graph_props_to_delete &props, int v_index) {
  props.vertices.push_back( Number(v_index) );
}