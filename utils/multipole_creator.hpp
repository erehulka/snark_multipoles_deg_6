#ifndef MULTIPOLE_CREATOR_HPP
#define MULTIPOLE_CREATOR_HPP

#include <iostream>
#include <vector>

#include <graphs.hpp>
#include <multipoles.hpp>
#include "../implementation.h"
#include "../create_multipole_deg6.hpp"

using namespace ba_graph;

std::vector<Multipole> create_all_multipoles_by_removing_3_edges(Graph &g, graph_props_to_delete &props) {
    std::vector<Multipole> result;
    if (props.locs.size() == 3) {
        try {
            Graph gCopy = copy_identical(g);
            Multipole m = create_by_removing_three_e(gCopy,props);
            result.push_back(m);
        } catch (const std::exception &e) {
            // no action
        }
    } else {
        for (int i = 0; i < max_number(g); i++) {
            for (int j = i+1; j < max_number(g); j++) {
                if (!g.contains(Location(i, j))) continue;
                add_edge_to_gprops(props, i, j);
                std::vector<Multipole> recursiveResult = create_all_multipoles_by_removing_3_edges(g, props);
                result.insert(result.end(), recursiveResult.begin(), recursiveResult.end());
                props.locs.pop_back();
            }
        }
    }

    return result;
}

std::vector<Multipole> create_all_multipoles_by_removing_2_vertices(Graph &g, graph_props_to_delete &props) {
    std::vector<Multipole> result;

    for (int i = 0; i < max_number(g); i++) {
        add_vertex_to_gprops(props, i);
        for (int j = i+1; j < max_number(g); j++) {
            add_vertex_to_gprops(props, j);

            try {
                Graph gCopy = copy_identical(g);
                Multipole m = create_by_removing_2_vertices(gCopy, props);
                result.push_back(m);
            } catch (const std::exception &e) {
                // no action
            }

            props.vertices.pop_back();
        }
        props.vertices.pop_back();
    }

    return result;
}

std::vector<Multipole> create_all_multipoles_by_removing_2_inc_vertices_and_edge(Graph &g, graph_props_to_delete &props) {
    std::vector<Multipole> result;

    if (props.vertices.size() == 2) {
        for (int i = 0; i < max_number(g); i++) {
            for (int j = i+1; j < max_number(g); j++) {
                if (!g.contains(Location(i, j))) continue;
                add_edge_to_gprops(props, i, j);

                try {
                    Graph gCopy = copy_identical(g);
                    Multipole m = create_by_removing_2_inc_vertices_and_edge(gCopy,props);
                    result.push_back(m);
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

                std::vector<Multipole> recursiveResult = create_all_multipoles_by_removing_2_inc_vertices_and_edge(g, props);
                result.insert(result.end(), recursiveResult.begin(), recursiveResult.end());

                props.vertices.pop_back();
            }
            props.vertices.pop_back();
        }
    }

    return result;
}

std::vector<Multipole> create_all_multipoles_by_removing_path_length_4(Graph &g, graph_props_to_delete &props) {
    std::vector<Multipole> result;

    if (props.vertices.size() == 4) {
        try {
            Graph gCopy = copy_identical(g);
            Multipole m = create_by_removing_path_length_4(gCopy,props);
            result.push_back(m);
        } catch (const std::exception &e) {
            // no action
        }
    } else if (props.vertices.size() == 0) {
        for (int i = 0; i < max_number(g); i++) {
            add_vertex_to_gprops(props, i);

            std::vector<Multipole> recursiveResult = create_all_multipoles_by_removing_path_length_4(g, props);
            result.insert(result.end(), recursiveResult.begin(), recursiveResult.end());

            remove_last_vertex_from_gprops(props);
        }
    } else {
        int last_vertex = props.vertices[props.vertices.size() - 1].to_int();
        for (Number neighbor : g[last_vertex].neighbours()) {
            if (!std::count(props.vertices.begin(), props.vertices.end(), neighbor)) {
                add_vertex_to_gprops(props, neighbor);

                std::vector<Multipole> recursiveResult = create_all_multipoles_by_removing_path_length_4(g, props);
                result.insert(result.end(), recursiveResult.begin(), recursiveResult.end());

                remove_last_vertex_from_gprops(props);
            }
        }
    }

    return result;
}

std::vector<Multipole> create_all_multipoles_by_removing_vertex_and_3_neighbours(Graph &g, graph_props_to_delete &props) {
    std::vector<Multipole> result;

    for (int vertex = 0; vertex <= max_number(g); vertex++) {
        add_vertex_to_gprops(props, vertex);

        for (Number neighbor : g[vertex].neighbours()) {
            add_vertex_to_gprops(props, neighbor);
        }

        try {
            Graph gCopy = copy_identical(g);
            Multipole m = create_by_removing_vertex_and_3_neighbours(gCopy,props);
            result.push_back(m);
        } catch (const std::exception &e) {
            // no action
        }

        clear_props(props);
    }

    return result;
}

std::vector<Multipole> create_all_multipoles_from_graph(Graph &g) {
    std::vector<Multipole> result;

    graph_props_to_delete props;
    clear_props(props);

    std::vector<Multipole> by_removing_3_edges = create_all_multipoles_by_removing_3_edges(g, props);
    result.insert(result.end(), by_removing_3_edges.begin(), by_removing_3_edges.end());
    clear_props(props);

    std::vector<Multipole> by_removing_2_vertices = create_all_multipoles_by_removing_2_vertices(g, props);
    result.insert(result.end(), by_removing_2_vertices.begin(), by_removing_2_vertices.end());
    clear_props(props);

    std::vector<Multipole> by_removing_2_in_vertices_and_edge = create_all_multipoles_by_removing_2_inc_vertices_and_edge(g, props);
    result.insert(result.end(), by_removing_2_in_vertices_and_edge.begin(), by_removing_2_in_vertices_and_edge.end());
    clear_props(props);

    std::vector<Multipole> by_removing_path_4 = create_all_multipoles_by_removing_path_length_4(g, props);
    result.insert(result.end(), by_removing_path_4.begin(), by_removing_path_4.end());
    clear_props(props);

    std::vector<Multipole> by_removing_vertex_3_neighbors = create_all_multipoles_by_removing_vertex_and_3_neighbours(g, props);
    result.insert(result.end(), by_removing_vertex_3_neighbors.begin(), by_removing_vertex_3_neighbors.end());
    clear_props(props);

    return result;
}

#endif
