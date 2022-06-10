#ifndef GEN_ALL_MULTIPOLE_DEG_6
#define GEN_ALL_MULTIPOLE_DEG_6

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <graphs.hpp>

#include "implementation.h"

using namespace ba_graph;

void generate_all_and_write_to_file(const std::string input, const std::string output_file, bool is_file = true) {
    std::vector<Graph> result = is_file
            ? generate_graphs_from_file(input)
            : generate_graphs_from_webpage(input);

    std::vector<multipole_creation> allMulti;
    for (int i = 0; i < result.size(); i++) {
        std::vector<multipole_creation> thisMulti = create_all_multipoles_from_graph(result[i]);
        allMulti.insert(allMulti.end(), thisMulti.begin(), thisMulti.end());
    }

    std::ofstream output(output_file);
    if (output.is_open()) {
        for (const auto &oneRes : allMulti) {
            output << oneRes.deletion_method << ' ' << oneRes.g_original << ' ' << oneRes.g << " edges: " << oneRes.props.locs << ", vertices: " << oneRes.props.vertices << std::endl;
        }
    } else std::cerr << "Unable to open file" << std::endl;
}

#endif