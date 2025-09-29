#ifndef BRIDGE_BLOCKS_HPP
#define BRIDGE_BLOCKS_HPP

#include <LEDA/graph/graph.h>
#include <LEDA/graph/node_map.h>
#include <vector>

using namespace leda;

// Global graph and maps
extern graph G;
extern node_map<char> label;
extern node_map<node> parent;

struct BBF
{
    node maketree(char c);

    void evert(node x);

    void link(node x, node y);

    std::vector<node> findpath(node u, node v);

    node condensepath(const std::vector<node>& P, char A);

    bool is_square_node(node x);
};

#endif // BRIDGE_BLOCKS_HPP
