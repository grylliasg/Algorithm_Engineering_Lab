#include <LEDA/graph/graph.h>
#include <LEDA/graph/node_map.h>
#include <LEDA/graph/graph_alg.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <queue>
#include <ctime>
#include <sys/time.h>
#include <cstdlib>

using namespace std;
using namespace leda;

graph G;  // global graph

struct BBF {
    node_map<char> label;
    node_map<node> parent;

    BBF() : label(G), parent(G) {}

    node maketree(char c) {
        node n = G.new_node();
        parent[n] = nil;

        if (c != '\0') label[n] = c; // Vazoume labels mono sta round nodes

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
        //evert(x);
        G.new_edge(x, y); 
        parent[x] = y; // Make parent 
    }

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

    void condensepath(std::vector<node> condpath, char c) {
        node newNode = maketree(c);

        node n, x;

        for(int i = 0; i < condpath.size(); ++i) {
            n = condpath[i];

            forall_nodes(x, G) {
                if (parent[x] == n) link(x, newNode);
            }
        }

        for(int i = 0; i < condpath.size(); ++i) {
            n = condpath[i];
            G.del_node(n);
        }
    }

    // -----------------------
    // Bridge-block operations: -------------------------

    node make_vertex(char ch) {
        node u = maketree('\0');
        link(u, maketree(ch));
        
        return u;
    }

    char find_block(node u) {
        std::vector<node> path = findpath(u,u);
        node n = path[0];

        return label[n];
    }

    void insert_edge(node u, node v, char c) {
        // Find roots
        node root_u = u, root_v = v;
        while(parent[root_u] != nil) root_u = parent[root_u];
        while(parent[root_v] != nil) root_v = parent[root_v];

        if (root_u != root_v) { // different components

            node x = parent[u]; // same as x = findpath(u,u)
            node y = parent[v]; // same as y = findpath(v,v)

            // Compare the size of the components using BFS:
            node_array<int> compnum(G, -1);
            list<node> compU = BFS(G, u, compnum);
            list<node> compV = BFS(G, v, compnum);

            if(compU.size() <= compV.size()) {
                evert(x); link(x, y); 
            }
            else {
                evert(y); link(y, x);
            }
        }
        
        else { // same component
            std::vector<node> path = findpath(u, v);
            condensepath(path, c);
        }
    }
};

// // TESTING ON DIFFICULT SITUATIONS
// int main() {
//     BBF bbf;
//     G.make_undirected();

//     node u0 = bbf.make_vertex('A');
//     node u1 = bbf.make_vertex('B');
//     node u2 = bbf.make_vertex('C');
//     node u3 = bbf.make_vertex('D');

//     bbf.insert_edge(u0, u1, 'A');
//     bbf.insert_edge(u0, u1, 'A');
//     bbf.insert_edge(u2, u3, 'B');
//     bbf.insert_edge(u2, u3, 'B');
//     bbf.insert_edge(u0, u3, 'A');

//     // // Test !!
//     bbf.insert_edge(u3, u3, 'G');
//     cout << "B-Block(u0): " << bbf.find_block(u0) << endl;
//     cout << "B-Block(u1): " << bbf.find_block(u1) << endl;
//     cout << "B-Block(u2): " << bbf.find_block(u2) << endl;
//     cout << "B-Block(u3): " << bbf.find_block(u3) << endl;

//     return 0;
// }

// TESTING ON LARGE GRAPHS FOR EXPERIMENTAL EVALUATION:
int main() {
    BBF bbf;

    int numOfSqNodes = 8000;
    std::vector<node> u(numOfSqNodes);
    G.make_undirected();

    // Create trees
    for(int i = 0; i < numOfSqNodes; i++) {
        u[i] = bbf.make_vertex('A');
    }

    // Insert edges
    for(int i = 0; i < numOfSqNodes - 2; i++) {
        bbf.insert_edge(u[i], u[i+1], 'A');
        bbf.insert_edge(u[i], u[i+1], 'A');
        bbf.insert_edge(u[i], u[i+2], 'A');
    }

    srand(time(NULL));
    timeval start, stop;
    gettimeofday(&start, NULL); // start clock

    for (int i = 0; i < numOfSqNodes; i++) {
    char c = bbf.find_block(u[i]);
    }

    gettimeofday(&stop, NULL); // stop clock
    long duration = (stop.tv_sec - start.tv_sec) * 1000000L + (stop.tv_usec - start.tv_usec);
    cout << duration << " microseconds" << endl;

    return 0;
}


// // TESTING ON ARTICLE'S BRIDGE BLOCK FOREST:
// int main() {
//     G.make_undirected();


//     BBF bbf;

//     // Create Article's BBF dynamically

//     // At first, create 13 trees:
//     node s1 = bbf.make_vertex('A'); 
//     node s2 = bbf.make_vertex('B'); 
//     node s3 = bbf.make_vertex('C'); 
//     node s4 = bbf.make_vertex('D'); 
//     node s5 = bbf.make_vertex('E'); 
//     node s6 = bbf.make_vertex('F'); 
//     node s7 = bbf.make_vertex('G'); 
//     node s8 = bbf.make_vertex('H'); 
//     node s9 = bbf.make_vertex('I'); 
//     node s10 = bbf.make_vertex('D'); 
//     node s11 = bbf.make_vertex('K'); 
//     node s12 = bbf.make_vertex('L'); 
//     node s13 = bbf.make_vertex('M'); 

//     // bridge-block C: 
//     bbf.insert_edge(s5, s6, 'n'); 
//     bbf.insert_edge(s5, s6, 'n'); // condense
//     bbf.insert_edge(s7, s8, 'n');
//     bbf.insert_edge(s7, s8, 'n'); // condense
//     bbf.insert_edge(s6, s7, 'n');
//     bbf.insert_edge(s6, s7, 'n'); // condense
//     bbf.insert_edge(s8, s9, 'C');
//     bbf.insert_edge(s8, s9, 'C'); // condense

//     // bridge-block B:
//     bbf.insert_edge(s2, s3, 'C'); 
//     bbf.insert_edge(s2, s3, 'C'); // condense
//     bbf.insert_edge(s3, s4, 'C');
//     bbf.insert_edge(s3, s4, 'B'); // condense

//     // bridge-block E:
//     bbf.insert_edge(s11, s12, 'A'); 
//     bbf.insert_edge(s11, s12, 'A'); // condense
//     bbf.insert_edge(s12, s13, 'A'); 
//     bbf.insert_edge(s12, s13, 'E'); // condense

//     bbf.insert_edge(s1, s2, 'A'); 
//     bbf.insert_edge(s4, s5, 'A'); 
//     bbf.insert_edge(s9, s10, 'A'); 


//     // Print the Bridge Block of each square node of the BBF:
//     cout << "find_block(1): " << bbf.find_block(s1) << endl; 
//     cout << "find_block(2): " << bbf.find_block(s2) << endl; 
//     cout << "find_block(3): " << bbf.find_block(s3) << endl; 
//     cout << "find_block(4): " << bbf.find_block(s4) << endl; 
//     cout << "find_block(5): " << bbf.find_block(s5) << endl; 
//     cout << "find_block(6): " << bbf.find_block(s6) << endl; 
//     cout << "find_block(7): " << bbf.find_block(s7) << endl; 
//     cout << "find_block(8): " << bbf.find_block(s8) << endl; 
//     cout << "find_block(9): " << bbf.find_block(s9) << endl; 
//     cout << "find_block(10): " << bbf.find_block(s10) << endl; 
//     cout << "find_block(11): " << bbf.find_block(s11) << endl; 
//     cout << "find_block(12): " << bbf.find_block(s12) << endl; 
//     cout << "find_block(13): " << bbf.find_block(s13) << endl; 

//     // Make the two insert_edge operations (Article's Fig. 2):
//     bbf.insert_edge(s1, s5, 'F');
//     bbf.insert_edge(s9, s11, 'E');
//     cout << "\nAfter operating insert_edge(s1,s5,'F') and insert_edge(s9,s11,'E'):\n";

//     // Print the Bridge Block of each square node of the BBF:
//     cout << "find_block(1): " << bbf.find_block(s1) << endl; 
//     cout << "find_block(2): " << bbf.find_block(s2) << endl; 
//     cout << "find_block(3): " << bbf.find_block(s3) << endl; 
//     cout << "find_block(4): " << bbf.find_block(s4) << endl; 
//     cout << "find_block(5): " << bbf.find_block(s5) << endl; 
//     cout << "find_block(6): " << bbf.find_block(s6) << endl; 
//     cout << "find_block(7): " << bbf.find_block(s7) << endl; 
//     cout << "find_block(8): " << bbf.find_block(s8) << endl; 
//     cout << "find_block(9): " << bbf.find_block(s9) << endl; 
//     cout << "find_block(10): " << bbf.find_block(s10) << endl; 
//     cout << "find_block(11): " << bbf.find_block(s11) << endl; 
//     cout << "find_block(12): " << bbf.find_block(s12) << endl; 
//     cout << "find_block(13): " << bbf.find_block(s13) << endl; 
    
//     return 0;
// }