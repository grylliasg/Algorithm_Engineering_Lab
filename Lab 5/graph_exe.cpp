#include <iostream>
#include <LEDA/graph/graph.h>
#include <cstdlib>
using namespace std;
//using namespace leda;

void printGraph(leda::graph& gr, leda::node_array<string>& na, leda::edge_array<int>& ea)
{
    leda::node v;
    leda::edge e;

    forall_nodes(v, gr)
    {
        forall_adj_edges(e, v) 
            cout << na[v] << " ---- " << na[gr.opposite(v, e)] << "  " << ea[e] << endl;
    }

    cout << endl << endl;
}

int main()
{
    leda::graph G;

    leda::node a = G.new_node();
    leda::node b = G.new_node();
    leda::node c = G.new_node();
    leda::node d = G.new_node();
    leda::node e = G.new_node();
    leda::node f = G.new_node();

    leda::edge e1 = G.new_edge(a, d);
    leda::edge e2 = G.new_edge(a, b);
    leda::edge e3 = G.new_edge(b, d);
    leda::edge e4 = G.new_edge(d, c);
    leda::edge e5 = G.new_edge(b, c);
    leda::edge e6 = G.new_edge(d, f);
    leda::edge e7 = G.new_edge(c, f);
    leda::edge e8 = G.new_edge(b, e);
    leda::edge e9 = G.new_edge(e, f);

    leda::node_array<string> Name(G);
    leda::edge_array<int> weight(G);

    Name[a] = "A";
    Name[b] = "B";
    Name[c] = "C";
    Name[d] = "D";
    Name[e] = "E";
    Name[f] = "F";

    weight[e1] = 1;
    weight[e2] = 4;
    weight[e3] = 5;
    weight[e4] = 7;
    weight[e5] = 3;
    weight[e6] = 8;
    weight[e7] = 6;
    weight[e8] = 9;
    weight[e9] = 2;

    G.make_undirected();

    printGraph(G, Name, weight);

    // 1.3, 1.4 Etoimo grafhma apo th LEDA

    leda::graph H;

    random_simple_graph(H, 6, 7);

    Make_Connected(H);  // Synektiko grafhma

    H.make_undirected();

    leda::node_array<string> name(H);
    leda::edge_array<int> w(H);

    int index = 0;
    string labels[] = {"A", "B", "C", "D", "E", "F"};
    
    leda::node no;
    forall_nodes(no, H)
    {
        name[no] = labels[index++];
    }

    cout << "Edges of random graph H:" << endl;

    leda::edge ed;
    forall_edges(ed, H)
    {
        w[ed] = (rand() % 1000) + 1;
        cout << name[H.source(ed)] << " ---- " << name[H.target(ed)] << ", weight:" << w[ed] << endl;
    }

    cout << endl << "Graph H: ";

    H.print();

    return 0;
}