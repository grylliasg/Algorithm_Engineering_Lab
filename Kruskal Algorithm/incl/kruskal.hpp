#include "LEDA/core/list.h"
#include "LEDA/graph/graph.h"
#include <iostream>
#include <LEDA/graph/graph_alg.h>

void Kruskal(leda::graph& G, const leda::edge_array<int>& wt, leda::edge_array<bool>& TreeEdges);
bool Checker(leda::graph& G, leda::edge_array<bool>& treeEd, leda::edge_array<int>& weight, leda::node_array<std::string>& Name);