#include "bridge-blocks.hpp"
#include <vector>
#include <cmath>
#include <ctime>
#include <sstream>
using namespace std;

int main()
{

    leda::graph G;

    BBF bbf;

    // 1. Δημιουργία square κόμβων
    leda::node s1 = bbf.maketree('\0');  // square node
    leda::node s2 = bbf.maketree('\0');  // square node

    // 2. Δημιουργία round κόμβων και σύνδεση με square
    leda::node r1 = bbf.maketree('A');   // round node A
    leda::node r2 = bbf.maketree('B');   // round node B

    parent[s1] = r1; G.new_edge(r1, s1);  // r1 → s1
    parent[s2] = r2; G.new_edge(r2, s2);  // r2 → s2

    cout << ">>> Initial structure:\n";
    cout << "s1: ID " << G.index(s1) << ", parent: " << G.index(parent[s1]) << "\n";
    cout << "s2: ID " << G.index(s2) << ", parent: " << G.index(parent[s2]) << "\n";

    return 0;
}