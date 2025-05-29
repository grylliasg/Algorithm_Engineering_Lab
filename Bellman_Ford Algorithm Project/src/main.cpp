#include "bellmanFord.hpp"
#include "visitor_graph.hpp"
#include <LEDA/graph/graph.h>
#include <LEDA/graph/templates/shortest_path.h>
#include <ctime>

using namespace boost;

void LEDA_Graph_To_Boost(leda::graph& L_G, leda::edge_array<int>& wt, Graph& B_G) 
{
    leda::node_array<Vertex> Map(L_G);

    leda::edge e;
    leda::node n;

    forall_nodes(n, L_G)
    {
        Vertex v = add_vertex(B_G);
        Map[n] = v;
    }

    forall_edges(e, L_G)
    {
        Edge u = add_edge(Map[L_G.source(e)], Map[L_G.target(e)], B_G).first;
        B_G[u].weight = wt[e];
    }
}

bool randomGraph(int numNodes)
{
    leda::graph LG;
	Graph BG;

    int m = 20 * numNodes * (int)log(numNodes);

    leda::random_graph(LG, numNodes, m, true, true, true);
    leda::Make_Connected(LG);

	leda::edge_array<int> weight(LG);

    leda::edge e;
    forall_edges(e, LG)
    {
        weight[e] = -100 + rand()%(10000 + 100 + 1); // range = [-100, 10000]
    }

    LEDA_Graph_To_Boost(LG, weight, BG); // METATROPH TOU GRAPH APO LEDA SE BOOST (LG -> BG)

    std::vector<long> dist(numNodes);
    std::vector<Edge> pred(numNodes);

	srand(time(0));
    clock_t start1 = clock();

    leda::node start = LG.choose_node();
    Vertex startNode = LG.index(start);

    bool myResult = bellmanFord(BG, startNode, dist, pred);
    clock_t end1 = clock();
    std::cout << "\n-----------\nNegative Cycle: " << (myResult?"No":"Yes") << std::endl << std::endl;

// OPTIONAL: print distances from start node
	// VertexIterator vi, viEnd;
	// std::cout << "\nDistance of vertices:" << std::endl;
	// for(tie(vi, viEnd)=vertices(BG); vi != viEnd; ++vi)
	// {
	// 	std::cout << *vi << ": dist = " << dist[*vi] << std::endl;
	// }

	double duration1 = double(end1 - start1) / CLOCKS_PER_SEC;
	std::cout << "My function's duration: " << duration1 << " seconds\n";

    leda::node_array<int> distLEDA(LG);
 	leda::node_array<leda::edge> predLEDA(LG);

    clock_t start3 = clock();
	bool res = leda::BELLMAN_FORD_B_T(LG, start, weight, distLEDA, predLEDA);
    clock_t end3 = clock();
    std::cout << "\n-----------\nNegative Cycle: " << (res?"No":"Yes") << std::endl << std::endl;

	double duration3 = double(end3 - start3) / CLOCKS_PER_SEC;
	std::cout << "LEDA function's duration: " << duration3 << " seconds\n";

	if (myResult)
    {
        std::vector<int> distance(numNodes, (std::numeric_limits<short>::max)());
        std::vector<std::size_t> parent(numNodes);
        distance[startNode] = 0;

        clock_t start2 = clock();
        bool resBOOST = bellman_ford_shortest_paths(BG, numNodes, weight_map(get(&myEdge::weight, BG)).distance_map(&distance[0]).predecessor_map(&parent[0]));
        clock_t end2 = clock();
        std::cout << "\n-----------\nNegative Cycle: " << (resBOOST?"No":"Yes") << std::endl << std::endl;
        double duration2 = double(end2 - start2) / CLOCKS_PER_SEC;
	    std::cout << "Boost function's duration: " << duration2 << " seconds\n";
    }
}

void gridGraph(int n)
{
    leda::graph LG;

    leda::node nodes[n][n];

    // Prosthikh komvwn
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            nodes[i][j] = LG.new_node();
        }
    }

    std::vector<leda::edge> area1_edges;
    std::vector<leda::edge> area2_edges;
    std::vector<leda::edge> area3_edges;

    srand(time(NULL));

    // Prosthikh akmwn
    // Area 1:
    for (int i = 0; i <= n/2; i++)
    {
        for (int j = 0; j <= n-1; j++)
        {

            if (i == n/2 && j == n-1) continue; // Katw deksia komvos ths area 1

            if (i == n/2)
            {
                leda::edge e = LG.new_edge(nodes[i][j], nodes[i][j+1]); // Mono pros ta deksia
                area1_edges.push_back(e);
            } 

            if (j == n-1)
            {
                leda::edge e = LG.new_edge(nodes[i][j], nodes[i+1][j]); // Mono pros ta katw
                area1_edges.push_back(e);
            }

            if (i < n/2 && j < n-1)
            {
                leda::edge e = LG.new_edge(nodes[i][j], nodes[i+1][j]); // Pros ta katw
                leda::edge ee = LG.new_edge(nodes[i][j], nodes[i][j+1]); // kai pros ta deksia
                area1_edges.push_back(e); area1_edges.push_back(ee);
            }
            
        }
    }

    // Area 2:
    for (int i = n/2; i <= n-1; i++)
    {
        for (int j = n/2; j <= n-1; j++)
        {
            if (i == n-1 && j == n-1) continue; // Katw deksia komvos tou whole grid

            if (i == n/2)
            {
                leda::edge e = LG.new_edge(nodes[i][j], nodes[i+1][j]); // Mono pros ta katw
                area2_edges.push_back(e);
            }

            else if (i == n-1) 
            {
                leda::edge e = LG.new_edge(nodes[i][j], nodes[i][j+1]); // Mono pros ta deksia
                area2_edges.push_back(e);
            }

            if (j == n-1 && i > n/2) 
            {
                leda::edge e = LG.new_edge(nodes[i][j], nodes[i+1][j]); // Mono pros ta katw
                area2_edges.push_back(e);
            }

            else if (j == n/2 && i < n-1 && i > n/2) 
            {
                leda::edge e = LG.new_edge(nodes[i][j], nodes[i+1][j]); // Mono pros ta katw
                area2_edges.push_back(e);
            }

            if (j == n/2 && i > n/2 && i < n-1) 
            {
                leda::edge e = LG.new_edge(nodes[i][j], nodes[i][j+1]); // Mono pros ta deksia
                area2_edges.push_back(e);
            }

            if (i > n/2 && i < n-1 && j < n-1 && j > n/2)
            {
                leda::edge e = LG.new_edge(nodes[i][j], nodes[i+1][j]); // Pros ta katw
                leda::edge ee = LG.new_edge(nodes[i][j], nodes[i][j+1]); // kai pros ta deksia
                area2_edges.push_back(e); area2_edges.push_back(ee);
            }
        }
    }

    // Area 3:
    leda::edge ped1;
    leda::edge ped2;

    for (int i = n/2; i <= n-1; i++)
    {
        for (int j = 0; j <= n/2-1; j++)
        {
            if (i == n/2 && j < n/2-1)
            {
                if (rand() % 2 == 0)
                {
                    leda::edge e = LG.new_edge(nodes[i][j], nodes[i+1][j]); // Mono pros ta katw
                    area3_edges.push_back(e);
                }
                else
                {
                    leda::edge e = LG.new_edge(nodes[i+1][j], nodes[i][j]); // Mono pros ta panw
                    area3_edges.push_back(e);
                }
            }

            if (i == n-1) 
            {
                if (rand() % 2 == 0)
                {
                    leda::edge e = LG.new_edge(nodes[i][j+1], nodes[i][j]); // Pros ta aristera
                    area3_edges.push_back(e);
                }
                else
                {
                   leda::edge e = LG.new_edge(nodes[i][j], nodes[i][j+1]); // Pros ta deksia
                   area3_edges.push_back(e);
                }
            }

            if (i > n/2 && i < n-1)
            {
                if (i == n/2+1 && j == n/2-1) // Komvos pou vasizontai oi eidikes akmes
                {
                    ped1 = LG.new_edge(nodes[n/2+1][n/2], nodes[n/2+1][n/2-1]); // eidikh akmh "ped1"
                    ped2 = LG.new_edge(nodes[n/2+1][n/2-1], nodes[n/2][n/2-1]); // eidikh akmh "ped2"

                    if (rand() % 2 == 0)
                    {
                        leda::edge e = LG.new_edge(nodes[i][j], nodes[i+1][j]); // Mono pros ta katw
                        area3_edges.push_back(e);
                    }
                    else
                    {
                        leda::edge e = LG.new_edge(nodes[i+1][j], nodes[i][j]); // or mono pros ta panw
                        area3_edges.push_back(e);
                    }

                    continue;
                }

                if (rand() % 2 == 0)
                {
                    leda::edge e = LG.new_edge(nodes[i][j], nodes[i+1][j]); // Mono pros ta katw
                    area3_edges.push_back(e);
                }
                else
                {
                    leda::edge e = LG.new_edge(nodes[i+1][j], nodes[i][j]); // or mono pros ta panw
                    area3_edges.push_back(e);
                }

                if (rand() % 2 == 0)
                {
                    leda::edge e = LG.new_edge(nodes[i][j+1], nodes[i][j]); // Pros ta aristera
                    area3_edges.push_back(e);
                }
                else
                {
                   leda::edge e = LG.new_edge(nodes[i][j], nodes[i][j+1]); // or pros ta deksia
                   area3_edges.push_back(e);
                }
            }

        }
    }


    leda::edge_array<int> weight(LG);
	leda::node_array<int> dist(LG);
	leda::node_array<leda::edge> pred(LG);

    // Prosthiki varwn stis akmes 
    weight[ped1] = -100000;
    weight[ped2] = -100000;

    leda::edge e;
    for (int i = 0; i < area1_edges.size(); ++i) // VARH PERIOXHS 1
    {
       e = area1_edges[i];
       weight[e] = rand() % 10001;
    }

    for (int i = 0; i < area2_edges.size(); ++i) // VARH PERIOXHS 2
    {
       e = area2_edges[i];
       weight[e] = rand() % 10001;
    }

    for (int i = 0; i < area3_edges.size(); ++i) // VARH PERIOXHS 3
    {
       e = area3_edges[i];
       weight[e] = -100 + rand() % (10000 + 100 + 1);
    }

// OPTIONAL: PRINT THE GRAPH
    //LG.print();


// LEDA:

    leda::node startNodeGrid = LG.first_node();
    Vertex startNodeBoost = LG.index(startNodeGrid);

    leda::node_array<int> distleda(LG);
 	leda::node_array<leda::edge> predleda(LG);

    clock_t startl = clock();
	bool resul = leda::BELLMAN_FORD_B_T(LG, startNodeGrid, weight, distleda, predleda);
    clock_t endl = clock();
    std::cout << "\n-----------\nNegative Cycle: " << (resul?"No":"Yes") << std::endl << std::endl;

	double durationl = double(endl - startl) / CLOCKS_PER_SEC;
	std::cout << "LEDA function's duration: " << durationl << " seconds\n";


// OPTIONAL: Print the weights of each edge in LG
    // std::cout << "\n--- LEDA Edges and Weights ---\n";
    // leda::edge it;
    // forall_edges(it, LG) {
    //     leda::node src = LG.source(it);
    //     leda::node tgt = LG.target(it);
    //     std::cout << "Edge (" << LG.index(src) << " -> " << LG.index(tgt) << ")"
    //             << " Weight: " << weight[it] << std::endl;
    // }


// METATROPH GRAFHMATOS

    Graph BG;
    LEDA_Graph_To_Boost(LG, weight, BG); // METATROPH TOU GRAPH APO LEDA SE BOOST (LG -> BG)

    // Efarmogh My Bellman Ford:
    std::vector<long> distt(n*n);
    std::vector<Edge> predd(n*n);

	srand(time(0));
    clock_t start4 = clock();
    bool myResult = bellmanFord(BG, startNodeBoost, distt, predd);
    clock_t end4 = clock();
    std::cout << "\n-----------\nNegative Cycle: " << (myResult?"No":"Yes") << std::endl << std::endl;


//Optional: Print the distance of each node from start node:
    // VertexIterator vi, viEnd;
	// std::cout << "\nDistance of vertices:" << std::endl;
	// for(tie(vi, viEnd)=vertices(BG); vi != viEnd; ++vi)
	// {
	// 	std::cout << *vi << ": dist = " << distt[*vi] << std::endl;
	// }


    double duration4 = double(end4 - start4) / CLOCKS_PER_SEC;
	std::cout << "My function's duration for Grid: " << duration4 << " seconds\n";


// BOOST:

    if (myResult) // An den exei arnitiko kyklo trexoume kai boost 
    {
        std::vector<int> distance(n*n, (std::numeric_limits<short>::max)());
        std::vector<std::size_t> parent(n*n);
        distance[startNodeBoost] = 0;

        clock_t startb = clock();
        bool resBOOSTT = bellman_ford_shortest_paths(BG, n, weight_map(get(&myEdge::weight, BG)).distance_map(&distance[0]).predecessor_map(&parent[0]));
        clock_t endb = clock();
        std::cout << "\n-----------\nNegative Cycle: " << (resBOOSTT?"No":"Yes") << std::endl << std::endl;
        double durationb = double(endb - startb) / CLOCKS_PER_SEC;
        std::cout << "Boost function's duration: " << durationb << " seconds\n";
    }
}

int main()
{
    int choice;
    int n;

    std::cout << "Choose type of Graph:\n";
    std::cout << "1. Random Graph (Choices: 1000, 4000, 8000)\n" ;
    std::cout << "2. Grid Graph (Choices: 100, 200, 300)\n" ;
    std::cout << "Choise: ";
    std::cin >> choice;

    if (choice == 1)
    {
        std::cout << "\nChoose size for the Random Graph (1000, 4000, 8000): " ;
        std::cin >> n;

        if (n == 1000 || n == 4000 || n == 8000)
        {
            randomGraph(n);
        }
        else
        {
            std::cout << "Invalid value. Acceptable: 1000, 4000, or 8000.\n" ;
        }

    } 
    else if (choice == 2)
    {
        std::cout << "Choose size for the Grid Graph (100, 200, 300): " ;
        std::cin >> n;

        if (n == 100 || n == 200 || n == 300)
        {
            gridGraph(n);
        }
        else
        {
            std::cout << "Invalid value. Acceptable: 100, 200, or 300.\n" ;
        }

    }
    else
    {
        std::cout << "Invalid choise, please select 1 or 2.\n" ;
    }

    return 0;
}