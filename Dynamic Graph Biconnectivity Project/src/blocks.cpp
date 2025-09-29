#include <LEDA/graph/graph.h>
#include <LEDA/graph/node_map.h>
#include <LEDA/graph/graph_alg.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include <sys/time.h>
#include <ctime>


using namespace std;
using namespace leda;

graph G;

struct BF {
    node_map<char> label;   // labels for round nodes
    node_map<node> parent;  // parent pointers

    BF() : label(G), parent(G) {}

    node maketree(char c) {
        node n = G.new_node();
        parent[n] = nil;
        if (c != '\0') label[n] = c; // labels only for round nodes
        return n;
    }

    void evert(node n) {
        node prev = nil, curr = n;
        while (curr != nil) {
            node next = parent[curr];
            parent[curr] = prev;
            prev = curr;
            curr = next;
        }
    }

    void link(node x, node y) {
        G.new_edge(x, y);
        parent[x] = y;
    }

    // Finds the root from u to v (includes round and square nodes)

   std::vector<node> findpath(node u, node v) {
        std::vector<node> path_u, path_v, path;

        if (u == v) { // Same nodes
            path.push_back(parent[u]);
            return path;
        }

        // Route from u to root
        node temp = u;
        while (temp != nil) {
            path_u.push_back(temp);
            temp = parent[temp];
        }

        // Route from v to root
        temp = v;
        while (temp != nil) {
            path_v.push_back(temp);
            temp = parent[temp];
        }

        // Reverse paths
        std::reverse(path_u.begin(), path_u.end());
        std::reverse(path_v.begin(), path_v.end());

        // Find common
        size_t i = 0;
        while (i < path_u.size() && i < path_v.size() && path_u[i] == path_v[i]) {
            i++;
        }

        // Add the route from u to common ancestor to path vector
        for (size_t j = path_u.size(); j > i; j--) {
            path.push_back(path_u[j-1]);
        }

        // Add the route from common ancestor to v to path vector
        for (size_t j = i-1; j < path_v.size(); j++) { 
            path.push_back(path_v[j]);
        }

        // std::cout << "Final path from u to v: ";
        // for (size_t i = 0; i < path.size(); i++) {
        // std::cout << path[i] << " ";
        // }

        path.erase(path.begin());
        path.pop_back();

        return path;
    }


    // Merge round nodes 
    void condensepath(const std::vector<node>& condpath, char c) {
        node newNode = maketree(c);

        node n, x;
        bool firstChecked = false;

        for(int i = 0; i < condpath.size(); ++i) {
            n = condpath[i];

            if(label[n] != 0) { // only round nodes
                forall_nodes(x, G) {
                    if(parent[x] == n) link(x, newNode);
                }

                if(parent[n] != nil && firstChecked == false) {
                    link(newNode, parent[n]);
                    //firstChecked = true;
                }
                G.del_node(n);
            }
        }
    }

    // -------------------------
    // Block operations
    node make_vertex(char) {
        return maketree('\0'); // only square nodes
    }

    char find_block(node u, node v) {
        std::vector<node> path = findpath(u, v);
        
        if (path.size() > 1) return '\0';

        size_t i;
        for(i = 0; i < path.size(); i++) {
            node n = path[i];
            if(label[n] != 0) return label[n];
        }
    }

    void insert_edge(node u, node v, char c) {
        // Find roots
        node root_u = u, root_v = v;
        while(parent[root_u] != nil) root_u = parent[root_u];
        while(parent[root_v] != nil) root_v = parent[root_v];

        if (root_u != root_v) { // different components
            node r = maketree(c);

            // Compare the size of the components using BFS:
            node_array<int> compnum(G, -1);
            list<node> compU = BFS(G, u, compnum);
            list<node> compV = BFS(G, v, compnum);

            if(compU.size() <= compV.size()) {
                evert(u); link(u, r); link(r, v);
            }
            else {
                evert(v); link(v, r); link(r, u);
            }
        }
        
        else { // same component
            std::vector<node> path = findpath(u, v);
            condensepath(path, c);
        }
    }
};

// TESTING ON DIFFICULT SITUATIONS
// int main() {
//     BF bf;
//     G.make_undirected();

//     node u0 = bf.make_vertex('a');
//     node u1 = bf.make_vertex('b');
//     node u2 = bf.make_vertex('c');
//     node u3 = bf.make_vertex('d');
//     node u4 = bf.make_vertex('e');

//     bf.insert_edge(u0, u2, 'A');
//     bf.insert_edge(u2, u3, 'A');
//     bf.insert_edge(u0, u1, 'A');
//     bf.insert_edge(u1, u2, 'A');
//     bf.insert_edge(u2, u4, 'B');
//     bf.insert_edge(u3, u4, 'B');

//     // // Test !!
//     bf.insert_edge(u3, u3, 'G');
//     cout << "Block(u0, u1): " << bf.find_block(u0, u1) << endl;
//     cout << "Block(u1, u2): " << bf.find_block(u1, u2) << endl;
//     cout << "Block(u2, u3): " << bf.find_block(u2, u3) << endl;
//     cout << "Block(u3, u4): " << bf.find_block(u3, u4) << endl;

//     return 0;
// }


// // TESTING ON LARGE GRAPHS FOR EXPERIMENTAL EVALUATION:
int main() {
    BF bf;

    int numOfSqNodes = 5000;
    std::vector<node> u(numOfSqNodes);
    G.make_undirected();

    // Create square nodes
    for(int i = 0; i < numOfSqNodes; i++) {
        u[i] = bf.make_vertex('A');
    }

    // Insert edges
    for(int i = 0; i < numOfSqNodes - 2; i+=2) {
        bf.insert_edge(u[i], u[i+2], 'A');
    }

    for(int i = 0; i < numOfSqNodes - 2; i+=2) {
        bf.insert_edge(u[i], u[i+1], 'A');
        bf.insert_edge(u[i+1], u[i+2], 'A'); // condense
    }

    timeval start, stop;
    gettimeofday(&start, NULL); // start clock

    for (int i = 0; i < numOfSqNodes - 1; i++) {
    char c = bf.find_block(u[i], u[i+1]);
    }

    gettimeofday(&stop, NULL); // stop clock
    long duration = (stop.tv_sec - start.tv_sec) * 1000000L + (stop.tv_usec - start.tv_usec);
    cout << duration << " microseconds" << endl;


    return 0;
}


// TESTING ON ARTICLE'S BF:
// int main() {
//     G.make_undirected();

//     BF bf;

//     node s1 = bf.make_vertex('a');
//     node s2 = bf.make_vertex('a');
//     node s3 = bf.make_vertex('a');
//     node s4 = bf.make_vertex('a');
//     node s5 = bf.make_vertex('a');
//     node s6 = bf.make_vertex('a');
//     node s7 = bf.make_vertex('a');
//     node s8 = bf.make_vertex('a');
//     node s9 = bf.make_vertex('a');
//     node s10 = bf.make_vertex('a');
//     node s11 = bf.make_vertex('a');
//     node s12 = bf.make_vertex('a');
//     node s13 = bf.make_vertex('a');

//     // Blocks
//     bf.insert_edge(s4, s5, 'C');  
//     bf.insert_edge(s4, s2, 'B');  
//     bf.insert_edge(s4, s3, 'B');  
//     bf.insert_edge(s2, s3, 'B');  
//     bf.insert_edge(s2, s1, 'A');  
//     bf.insert_edge(s5, s6, 'D');  
//     bf.insert_edge(s5, s7, 'D');  
//     bf.insert_edge(s6, s7, 'D');  
//     bf.insert_edge(s7, s10, 'E');  
//     bf.insert_edge(s7, s8, 'F');  
//     bf.insert_edge(s7, s9, 'F');  
//     bf.insert_edge(s8, s9, 'F');  
//     bf.insert_edge(s11, s13, 'G');  
//     bf.insert_edge(s11, s12, 'G');  
//     bf.insert_edge(s12, s13, 'G');  

//     // Find blocks:
//     cout << "find_block(1,2): " << bf.find_block(s1, s2) << endl; 
//     cout << "find_block(2,4): " << bf.find_block(s2, s4) << endl; 
//     cout << "find_block(3,4): " << bf.find_block(s3, s4) << endl; 
//     cout << "find_block(4,5): " << bf.find_block(s4, s5) << endl; 
//     cout << "find_block(5,6): " << bf.find_block(s5, s6) << endl; 
//     cout << "find_block(5,7): " << bf.find_block(s5, s7) << endl; 
//     cout << "find_block(7,10): " << bf.find_block(s7, s10) << endl; 
//     cout << "find_block(7,8): " << bf.find_block(s7, s8) << endl; 
//     cout << "find_block(7,9): " << bf.find_block(s7, s9) << endl; 
//     cout << "find_block(11,12): " << bf.find_block(s11, s12) << endl; 
//     cout << "find_block(12,13): " << bf.find_block(s12, s13) << endl; 
//     cout << "find_block(11,13): " << bf.find_block(s11, s13) << endl; 

//     // -- FIG.3 OPERATIONS :
//     bf.insert_edge(s8, s10, 'H');  
//     bf.insert_edge(s5, s11, 'I');  
//     cout << "\nAfter fig.3 operations:\n"; 

//     // Find blocks:
//     cout << "find_block(1,2): " << bf.find_block(s1, s2) << endl; 
//     cout << "find_block(2,4): " << bf.find_block(s2, s4) << endl; 
//     cout << "find_block(3,4): " << bf.find_block(s3, s4) << endl; 
//     cout << "find_block(4,5): " << bf.find_block(s4, s5) << endl; 
//     cout << "find_block(5,6): " << bf.find_block(s5, s6) << endl; 
//     cout << "find_block(5,7): " << bf.find_block(s5, s7) << endl; 
//     cout << "find_block(7,10): " << bf.find_block(s7, s10) << endl; 
//     cout << "find_block(7,8): " << bf.find_block(s7, s8) << endl; 
//     cout << "find_block(7,9): " << bf.find_block(s7, s9) << endl; 
//     cout << "find_block(11,12): " << bf.find_block(s11, s12) << endl; 
//     cout << "find_block(12,13): " << bf.find_block(s12, s13) << endl; 
//     cout << "find_block(11,13): " << bf.find_block(s11, s13) << endl; 
//     cout << "find_block(5,11): " << bf.find_block(s5, s11) << endl; 
// }




