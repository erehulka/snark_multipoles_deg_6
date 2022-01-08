#include <vector>
#include <string>
#include <algorithm>
#include <set>

#include <graphs.hpp>
#include <multipoles.hpp>

#include "../implementation.h"

using namespace ba_graph;

/**
 * Functions that validates and fixes vector of connectors,
 * that will be passed as a parameter to Multipole constructor.
 */

void process_connector(const Graph &g, Connector &connector) {
    std::vector<Number> existing;
    for (auto vertex : connector.numbers) {
        if (g.contains(vertex))
            existing.push_back(vertex);
    }

    connector = Connector(existing);
}

std::vector<Connector> get_correct_connectors(const Graph &g, std::vector<Connector> connectors) {
    std::vector<Connector> to_return;

    for (auto connector : connectors) {
        process_connector(g, connector);
        if (connector.size() > 0)
            to_return.push_back(connector);
    }

    if (to_return.empty())
        throw std::invalid_argument("Error when creating connectors.");

    return to_return;
}
