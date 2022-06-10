#ifndef MULTIPOLE_CREATOR_HPP
#define MULTIPOLE_CREATOR_HPP

#include <iostream>
#include <vector>
#include <utility>

#include "../implementation.h"
#include <algorithms/cyclic_connectivity.hpp>
#include <graphs.hpp>
#include <multipoles.hpp>
#include <invariants.hpp>

#include "../create_multipole_deg6.hpp"

using namespace ba_graph;

struct multipole_creation {
    std::string g;
    Multipole *m;
    std::string g_original;
    graph_props_to_delete props;
    std::string deletion_method;

    multipole_creation() = delete;
    multipole_creation(Graph &g, Multipole &m, Graph &g_original, graph_props_to_delete &props, std::string deletion_m) {
        this->g = write_graph6(g, false);
        this->m = &m;
        this->deletion_method = deletion_m;

        this->g_original = write_graph6(g_original, false);

        graph_props_to_delete props_t;
        props_t.vertices = std::vector<Number>(props.vertices);
        props_t.locs = std::vector<Location>(props.locs);
        this->props = props_t;
    }
};

void renumber_g_by_isaacs(Graph &g, Graph &isaacs) {
    std::vector<Number> v;
    for(const auto &rot : g) v.push_back(rot.n());
    std::sort(v.begin(),v.end());
    Mapping<Number> map;
    int i_size = max_number(isaacs).to_int() + 1;
    for(const auto &e : v) map.set(e.to_int(),e.to_int() + i_size);
    renumber_vertices(g,map);
}

void connect_g_to_isaacs(Graph &g, std::pair<Graph*, Multipole*> isaacs_multipole) {
    renumber_g_by_isaacs(g, *(isaacs_multipole.first));
    Graph* isaacs = isaacs_multipole.first;

    std::vector<Number> connectors_from_g;
    std::vector<Number> to_delete_g;
    for(const auto &rot : g){
        if (rot.neighbours().size() == 1) {
            connectors_from_g.push_back(*(rot.neighbours().begin()));
            to_delete_g.push_back(rot.n());
        }
    }
    for (const auto &num_d : to_delete_g) {
        deleteV(g, num_d);
    }

    std::vector<Number> r;
    for(const auto &rot : *isaacs){
        if (rot.neighbours().size() == 3) {
            addV(g,rot.n());
            r.push_back(rot.n());
        }
    }

    for (const auto &num : r) {
        for (const Number n : (*isaacs)[num].neighbours()) {
            if (n > num && (*isaacs)[n].degree() == 3) addE(g, Location(num, n));
        }
    }

    std::vector<Number> isaacs_connectors = (isaacs_multipole.second)->connectors[0].numbers;
    for (int i = 0; i < 6; i++) {
        addE(g, Location(connectors_from_g[i], *((*isaacs)[isaacs_connectors[i]].neighbours().begin())));
    }
}

bool is_good_multipole(Graph &g, Multipole &m) {
    int girth_m = girth(g);
    if (girth_m < 6) return false;

    Graph isaacs = create_isaacs(6);
    graph_props_to_delete props;
    props.vertices = {0, 1, 2, 3, 4, 5};
    std::vector<Connector> connectors;
    for (Number i : props.vertices) {
        connectors.push_back(remove_vertex(isaacs, i));
    }
    Multipole m_isaacs = Multipole(get_correct_connectors(isaacs, connectors));
    m_isaacs.flatten();

    std::pair<Graph*, Multipole*> isaacs_multipole = {&isaacs, &m_isaacs};
    Graph gCopy = copy_identical(g);
    connect_g_to_isaacs(gCopy, isaacs_multipole);

    if (cyclic_connectivity(gCopy) < 6) return false;

    return true;
}

std::vector<multipole_creation> create_all_multipoles_by_removing_3_edges(Graph &g, graph_props_to_delete &props) {
    std::vector<multipole_creation> result;
    if (props.locs.size() == 3) {
        try {
            Graph gCopy = copy_identical(g);
            Multipole m = create_by_removing_three_e(gCopy,props);

            if (!is_good_multipole(gCopy, m)) return result;

            result.push_back(multipole_creation(gCopy, m, g, props, "by_3_edges"));
        } catch (const std::exception &e) {
            // no action
        }
    } else {
        for (int i = 0; i < max_number(g); i++) {
            for (int j = i+1; j < max_number(g); j++) {
                if (!g.contains(Location(i, j))) continue;
                add_edge_to_gprops(props, i, j);
                std::vector<multipole_creation> recursiveResult = create_all_multipoles_by_removing_3_edges(g, props);
                result.insert(result.end(), recursiveResult.begin(), recursiveResult.end());
                props.locs.pop_back();
            }
        }
    }

    return result;
}

std::vector<multipole_creation> create_all_multipoles_by_removing_2_vertices(Graph &g, graph_props_to_delete &props) {
    std::vector<multipole_creation> result;

    for (int i = 0; i < max_number(g); i++) {
        add_vertex_to_gprops(props, i);
        for (int j = i+1; j < max_number(g); j++) {
            add_vertex_to_gprops(props, j);

            try {
                Graph gCopy = copy_identical(g);
                Multipole m = create_by_removing_2_vertices(gCopy, props);

                if (!is_good_multipole(gCopy, m)) continue;

                Graph g_original = copy_identical(g);
                result.push_back(multipole_creation(gCopy, m, g_original, props, "by_2_vertices"));
            } catch (const std::exception &e) {
                // no action
            }

            props.vertices.pop_back();
        }
        props.vertices.pop_back();
    }

    return result;
}

std::vector<multipole_creation> create_all_multipoles_by_removing_2_inc_vertices_and_edge(Graph &g, graph_props_to_delete &props) {
    std::vector<multipole_creation> result;

    if (props.vertices.size() == 2) {
        for (int i = 0; i < max_number(g); i++) {
            for (int j = i+1; j < max_number(g); j++) {
                if (!g.contains(Location(i, j))) continue;
                add_edge_to_gprops(props, i, j);

                try {
                    Graph gCopy = copy_identical(g);
                    Multipole m = create_by_removing_2_inc_vertices_and_edge(gCopy,props);

                    if (!is_good_multipole(gCopy, m)) continue;

                    Graph g_original = copy_identical(g);
                    result.push_back(multipole_creation(gCopy, m, g_original, props, "2_inc_vertices_and_edge"));
                } catch (const std::exception &e) {
                    // no action
                }

                props.locs.pop_back();
            }
        }
    } else {
        for (int i = 0; i < max_number(g); i++) {
            add_vertex_to_gprops(props, i);
            for (int j = i+1; j < max_number(g); j++) {
                add_vertex_to_gprops(props, j);

                std::vector<multipole_creation> recursiveResult = create_all_multipoles_by_removing_2_inc_vertices_and_edge(g, props);
                result.insert(result.end(), recursiveResult.begin(), recursiveResult.end());

                props.vertices.pop_back();
            }
            props.vertices.pop_back();
        }
    }

    return result;
}

std::vector<multipole_creation> create_all_multipoles_by_removing_path_length_4(Graph &g, graph_props_to_delete &props) {
    std::vector<multipole_creation> result;

    if (props.vertices.size() == 4) {
        try {
            Graph gCopy = copy_identical(g);
            Multipole m = create_by_removing_path_length_4(gCopy,props);

            if (!is_good_multipole(gCopy, m)) return result;

            Graph g_original = copy_identical(g);
            result.push_back(multipole_creation(gCopy, m, g_original, props, "path_len_4"));
        } catch (const std::exception &e) {
            // no action
        }
    } else if (props.vertices.size() == 0) {
        for (int i = 0; i < max_number(g); i++) {
            add_vertex_to_gprops(props, i);

            std::vector<multipole_creation> recursiveResult = create_all_multipoles_by_removing_path_length_4(g, props);
            result.insert(result.end(), recursiveResult.begin(), recursiveResult.end());

            remove_last_vertex_from_gprops(props);
        }
    } else {
        int last_vertex = props.vertices[props.vertices.size() - 1].to_int();
        for (Number neighbor : g[last_vertex].neighbours()) {
            if (!std::count(props.vertices.begin(), props.vertices.end(), neighbor)) {
                add_vertex_to_gprops(props, neighbor);

                std::vector<multipole_creation> recursiveResult = create_all_multipoles_by_removing_path_length_4(g, props);
                result.insert(result.end(), recursiveResult.begin(), recursiveResult.end());

                remove_last_vertex_from_gprops(props);
            }
        }
    }

    return result;
}

std::vector<multipole_creation> create_all_multipoles_by_removing_vertex_and_3_neighbours(Graph &g, graph_props_to_delete &props) {
    std::vector<multipole_creation> result;

    for (int vertex = 0; vertex <= max_number(g); vertex++) {
        add_vertex_to_gprops(props, vertex);

        for (Number neighbor : g[vertex].neighbours()) {
            add_vertex_to_gprops(props, neighbor);
        }

        try {
            Graph gCopy = copy_identical(g);
            Multipole m = create_by_removing_vertex_and_3_neighbours(gCopy,props);

            if (!is_good_multipole(gCopy, m)) continue;

            Graph g_original = copy_identical(g);
            result.push_back(multipole_creation(gCopy, m, g_original, props, "vertex_and_3_neighbors"));
        } catch (const std::exception &e) {
            // no action
        }

        clear_props(props);
    }

    return result;
}

std::vector<multipole_creation> create_all_multipoles_by_functions(Graph &g) {
    std::vector<multipole_creation> result;

    graph_props_to_delete props;
    clear_props(props);

    std::vector<multipole_creation> by_removing_3_edges = create_all_multipoles_by_removing_3_edges(g, props);
    result.insert(result.end(), by_removing_3_edges.begin(), by_removing_3_edges.end());
    clear_props(props);

    std::vector<multipole_creation> by_removing_2_vertices = create_all_multipoles_by_removing_2_vertices(g, props);
    result.insert(result.end(), by_removing_2_vertices.begin(), by_removing_2_vertices.end());
    clear_props(props);

    std::vector<multipole_creation> by_removing_2_in_vertices_and_edge = create_all_multipoles_by_removing_2_inc_vertices_and_edge(g, props);
    result.insert(result.end(), by_removing_2_in_vertices_and_edge.begin(), by_removing_2_in_vertices_and_edge.end());
    clear_props(props);

    std::vector<multipole_creation> by_removing_path_4 = create_all_multipoles_by_removing_path_length_4(g, props);
    result.insert(result.end(), by_removing_path_4.begin(), by_removing_path_4.end());
    clear_props(props);

    std::vector<multipole_creation> by_removing_vertex_3_neighbors = create_all_multipoles_by_removing_vertex_and_3_neighbours(g, props);
    result.insert(result.end(), by_removing_vertex_3_neighbors.begin(), by_removing_vertex_3_neighbors.end());
    clear_props(props);

    return result;
}

std::vector<multipole_creation> create_all_multipoles_from_graph(Graph &g) {
    std::vector<multipole_creation> by_functions = create_all_multipoles_by_functions(g);
    return by_functions;
}

#endif
