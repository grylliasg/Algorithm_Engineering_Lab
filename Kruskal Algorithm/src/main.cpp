#include "kruskal.hpp"
#include <vector>
#include <cmath>
#include <ctime>
#include <sstream>
using namespace std;

bool Checker(leda::graph& G, leda::edge_array<bool>& treeEd, leda::edge_array<int>& weight, leda::node_array<std::string>& Name) 
{
    // TIMER FOR THE CHECKER
    clock_t start_check_time, end_check_time;
    double check_time; 

    start_check_time = clock();

    leda::edge ed;
    leda::list<leda::edge> nonMSTEdges;

    // Vres ta edges pou den anhkoun sto MST
    forall_edges(ed, G)
    {
        if (!treeEd[ed])
        {
            nonMSTEdges.push_back(ed);
        }
    }

    // Hide ta edges pou den anhkoun sto MST
    leda::edge edg;
    forall_edges(edg, G)
    {
        if (!treeEd[edg]) G.hide_edge(edg);
    }

    // Se auto to shmeio, to grafhma G apoteleitai mono apo tis MST akmes !

    // Elegxos an yparxei kyklos sto MST (Wrong...)
    forall_edges(ed, G) 
    {
        G.hide_edge(ed); // Hide thn akmh gia na doume an yparxei allh diadromh 
        leda::node startNodeG = G.source(ed);
        leda::node endNodeG = G.target(ed);

        leda::node_array<int> distG(G, -1);
        leda::node_array<leda::edge> predG(G);
        leda::list<leda::node> LNG = BFS(G, startNodeG, distG, predG); // Apply BFS xwris thn akmh ed

        if (distG[endNodeG] != -1) // An yparxei diadromh apo source(ed) se target(ed) einai wrong...
        {
            cout << "There is a cycle in the MST..." << endl;
            return false;
        }

        G.restore_edge(ed);
    }


    // Elegxos gia kyklo stis akmes non-MST
    forall(ed, nonMSTEdges)
    {
        leda::node startNode = G.source(ed);
        leda::node endNode = G.target(ed);

        leda::node_array<int> dist(G, -1);
        leda::node_array<leda::edge> pred(G);
        leda::list<leda::node> LN3 = BFS(G, startNode, dist, pred); // Kanoume BFS sto MST apo to startNode

        if (dist[endNode] != -1) // Vrethike monopati, ara dhmiourgeitai kyklos
        {
            //cout << "Edge " << Name[startNode] << "----" << Name[endNode] << " creates a cycle via:" << endl;

            int maxWeight = -1;
            leda::node currentNode = endNode;
            leda::edge edd = pred[currentNode];

            while (G.source(edd) != startNode && G.target(edd) != startNode) // Diatrexoume ta edges tou kyklou gia na paroume ta weights
            {
                edd = pred[currentNode];
                //cout << Name[G.source(edd)] << "----" << Name[G.target(edd)] << endl;

                if (weight[edd] > maxWeight)
                {
                    maxWeight = weight[edd];
                }

                if (currentNode != G.source(edd))
                    currentNode = G.source(edd);
                else
                    currentNode = G.target(edd);
            }

            if (weight[ed] >= maxWeight) 
                continue;
                //cout << "holding the biggest weight (" << weight[ed] <<") ✅" << endl << endl;
            else 
            {
                G.restore_all_edges();
                return false;
            }
        }
        else 
        {
            G.restore_all_edges();
            return false;
        }
    }
    G.restore_all_edges();

    end_check_time = clock();
    check_time = double(end_check_time - start_check_time) / CLOCKS_PER_SEC;
    cout << "Execution of Check Time: " << check_time << " seconds." << endl;

    return true;
}



int main()
{
    clock_t start_time, end_time;
    double kruskal_time, leda_time;
    leda::edge ed;

	// leda::graph G;

    // leda::node a = G.new_node();
    // leda::node b = G.new_node();
    // leda::node c = G.new_node();
    // leda::node d = G.new_node();
    // leda::node e = G.new_node();
    // leda::node f = G.new_node();

    // leda::edge e1 = G.new_edge(a, d);
    // leda::edge e2 = G.new_edge(a, b);
    // leda::edge e3 = G.new_edge(b, d);
    // leda::edge e4 = G.new_edge(b, c);
    // leda::edge e5 = G.new_edge(c, d);
    // leda::edge e6 = G.new_edge(b, e);
    // leda::edge e7 = G.new_edge(f, d);
    // leda::edge e8 = G.new_edge(c, f);
    // leda::edge e9 = G.new_edge(e, f);

    // Make_Connected(G);

    // leda::node_array<std::string> Name(G);
    // leda::edge_array<int> weight(G);

    // Name[a] = "A";
    // Name[b] = "B";
    // Name[c] = "C";
    // Name[d] = "D";
    // Name[e] = "E";
    // Name[f] = "F";


    // weight[e1] = 1;
    // weight[e2] = 2;
    // weight[e3] = 4;
    // weight[e4] = 7;
    // weight[e5] = 9;
    // weight[e6] = 5;
    // weight[e7] = 3;
    // weight[e8] = 2;
    // weight[e9] = 6;

    // G.make_undirected();


// RANDOM GRAPHS

    // int n = 500; 
    // int m = 2 * n * log2(n); 

    // leda::graph G;
    // leda::random_source rnd(10, 10000); // random numbers

    // leda::random_graph(G, n, m, true, true, true);
    // Make_Connected(G);
    // G.make_undirected();

    // // Dinoume onomata sta nodes
    // leda::node_array<std::string> Name(G);
    // leda::node v;
    // int id = 0;
    // forall_nodes(v, G)
    // {
    //     std::stringstream ss;
    //     ss << "Node_" << id;
    //     Name[v] = ss.str();
    //     id++;
    // }

    // // Dinoume weights sta edges
    // leda::edge_array<int> weight(G);
    // leda::edge e;
    // forall_edges(e, G)
    // {
    //     weight[e] = rnd();
    // }

    

// GRID

    // leda::graph G;
    // leda::grid_graph(G, 300);

    // leda::random_source rnd(10, 10000); // random numbers

    // G.make_undirected();

    // // Dinoume onomata sta nodes
    // leda::node_array<std::string> Name(G);
    // leda::node v;

    // int id = 0;
    // forall_nodes(v, G)
    // {
    //     std::stringstream ss;
    //     ss << "Node_" << id;
    //     Name[v] = ss.str();
    //     id++;
    // }

    // // Dinoume weights sta edges
    // leda::edge_array<int> weight(G);
    // leda::edge e;
    // forall_edges(e, G) {
    //     weight[e] = rnd();
    // }
    

// SYNTHETIKA GRAFHMATA - WORST CASE - COMPLETE GRAPH ME ISO VAROS AKMWN

    leda::graph G;
    leda::complete_graph(G, 3000);

    Make_Connected(G);
    G.make_undirected();

    // Dinoume onomata sta nodes
    leda::node_array<std::string> Name(G);
    leda::node v;

    int id = 0;
    forall_nodes(v, G)
    {
        std::stringstream ss;
        ss << "Node_" << id;
        Name[v] = ss.str();
        id++;
    }

    // Dinoume weights sta edges
    leda::edge_array<int> weight(G);
    leda::edge e;
    forall_edges(e, G) {
        weight[e] = 1; // OLES TA EDGES ME WEIGHT = 1
    }



    //G.print();

    start_time = clock();
    leda::list<leda::edge> krEdges = leda::MIN_SPANNING_TREE(G, weight);
    end_time = clock();
    leda_time = double(end_time - start_time) / CLOCKS_PER_SEC;
    cout << "\n\nExecution Time - Leda time: " << leda_time << " seconds." << endl;

    leda::edge_array<bool> treeEd(G, false);

    start_time = clock();
	Kruskal(G, weight, treeEd); // Apply kruskal algorithm...
    end_time = clock();
    kruskal_time = double(end_time - start_time) / CLOCKS_PER_SEC;
    cout << "\n\nExecution Time - My Kruskal: " << kruskal_time << " seconds." << endl;

	int totalCost = 0;

	cout << "\nMST Edges: " << endl;

	forall_edges(ed, G)
	{
		if (treeEd[ed]) 
		{
			//cout << Name[G.source(ed)] << " ---- " << Name[G.target(ed)] << " ";
			totalCost += weight[ed];
		}
		
	}
	cout << "\nTotal Cost MST: " << totalCost << endl << endl;

    bool check = Checker(G, treeEd, weight, Name);

    if (check) cout << "Checker returned TRUE!" << endl << endl;
    else cout << "Checker returned FALSE :(" << endl << endl;

	return 0;
}