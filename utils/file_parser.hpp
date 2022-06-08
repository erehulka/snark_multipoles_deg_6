#include <graphs.hpp>
#include "../implementation.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace ba_graph;

std::vector<Graph> generate_graphs_from_file(const std::string& input_file) {
    std::vector<Graph> result;

    std::ifstream infile(input_file);
    std::string line;
    while (std::getline(infile, line)) {
        try {
            result.emplace_back(read_graph6_line(line));
        } catch (const std::invalid_argument& e) {
            // pass: invalid graph argument
        }
    }

    return result;
}
