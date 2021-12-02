#include <vector>
#include <string>
#include <algorithm>

#include <graphs.hpp>
#include "implementation.h"

using namespace ba_graph;

struct graph_props_to_delete {
  std::vector<Location> locs;
  std::vector<Number> vertices;
};

// TODO: Vracat default multipol s connectormi ako rozumne rozdelenie visiacich hran
// TODO Odstranenie hrany/vrchola spravi hranu s vrcholom st. 1 (visiaca)

void remove_edge(Graph &g, Location &edge) {
  if (!g.contains( edge )) {
    throw std::invalid_argument( "Graph does not contain one of provided edges." );
  }
  if (g[edge.n1()].degree() == 1 || g[edge.n2()].degree() == 1) {
    // TODO odstranit moze byt aj nesuvisly
    throw std::invalid_argument( "By deleting this edge the graph would break into multiple components." );
  }

  deleteE(g, edge);
}

void remove_vertex(Graph &g, Number &vertex) {
  if (!g.contains(vertex)) {
    throw std::invalid_argument("Graph does not contain one of provided vertices.");
  }
  for (auto n : g[vertex].neighbours()) {
		if (g[n].degree() == 1) {
      throw std::invalid_argument("By deleting this vertex the graph would break into multiple components.");
    }
	}

  deleteV(g, vertex);
}

void create_by_removing_three_e(Graph &g, struct graph_props_to_delete &props) {
  if (props.locs.size() != 3) {
    char result_error[100];
    sprintf(result_error, "Number of edges must be 3, %d provided.", props.locs.size());
    std::string error_msg = result_error;
    throw std::range_error(error_msg);
  }
  for (int i = 0; i < 3; i++) {
    remove_edge(g, props.locs[i]);
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

  for (int i = 0; i < 2; i++) {
    remove_vertex(g, props.vertices[i]);
  }
}

void create_by_removing_2_inc_vertices_and_edge(Graph &g, struct graph_props_to_delete &props) {

  // ----------- BEGIN correctness check -----------

  if (props.vertices.size() != 2) {
    char result_error[100];
    sprintf(result_error, "Number of vertices must be 2, provided: %d.", props.vertices.size());
    std::string error_msg = result_error; 
    throw std::range_error(error_msg);
  }
  
  if (props.locs.size() != 3) {
    char result_error[100];
    sprintf(result_error, "Number of edges must be 1, %d provided.", props.locs.size());
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

  // ----------- END corectness check -----------

  for (int i = 0; i < 2; i++) {
    remove_vertex(g, props.vertices[i]);
  }

  remove_edge(g, props.locs[0]);
}

void create_by_removing_path_length_4(Graph &g, struct graph_props_to_delete &props) {
  // TODO Assert že dané vrcholy naozaj tvoria cestu dĺžky 4 v grafe g
  // Donutit usera zadat v poradi a overit ci existuju vsetky 3 hrany
  for (auto vertex : props.vertices)
    remove_vertex(g, vertex);
}

// TODO metoda na odstranenie vrcholu a jeho 3 susedov

void add_edge_to_gprops(struct graph_props_to_delete &props, int from, int to) {
  props.locs.push_back( Location(from, to) );
}

void add_edge_to_gprops(struct graph_props_to_delete &props, Location edge) {
  props.locs.push_back(edge);
}

void add_vertex_to_props(struct graph_props_to_delete &props, int v_index) {
  props.vertices.push_back( Number(v_index) );
}

void add_vertex_to_props(struct graph_props_to_delete &props, Number vertex) {
  props.vertices.push_back(vertex);
}
