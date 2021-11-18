#include "implementation.h"
#include <vector>

#include <graphs.hpp>

using namespace ba_graph;

struct graph_props_to_delete {
  std::vector<Location> locs;
  int n_of_edges = 0;
};

void create_by_removing_three_e(Graph *g, struct graph_props_to_delete *props) {
  if (props->n_of_edges != 3) {
    throw std::range_error( "Number of edges must be 3." );
  }
  // TODO Overit ci nahodou neodstranujeme 3 hrany incidentne s jednym vrcholom.
  for (int i = 0; i < props->n_of_edges; i++) {
    if (!g->contains( props->locs[i] )) {
      throw std::invalid_argument( "Graph does not contain one of provided edges." );
    } else {
      deleteE(*g, props->locs[i]);
    }
  }
}

void add_edge_to_gprops(struct graph_props_to_delete *props, int from, int to) {
  props->locs.push_back(Location(from, to));
  props->n_of_edges++;
}

void delete_gprops(struct graph_props_to_delete *props) {
  delete props;
}