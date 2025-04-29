#include "kruskal.hpp"
#include <LEDA/core/array.h>
using namespace std;

void Kruskal(leda::graph& G, const leda::edge_array<int>& wt, leda::edge_array<bool>& TreeEdges)
{
	// Ταξινόμηση των ακμών με βάση το βάρος τους
	G.sort_edges(wt);

	// Δείκτης first για κάθε κόμβο
	leda::node_array<leda::node> first(G);

	// Το id της λίστας που βρίσκεται ο κάθε κόμβος
	leda::node_array<int> my_list(G);

	// Array που αποθηκεύει όλες τις συνιστώσες (λίστες). Αρχικά κάθε κόμβος βρίσκεται σε μία ξεχωριστή λίστα
	leda::array< leda::list<leda::node> > components(G.number_of_nodes());

	// Εισαγωγή τιμών στις συνιστώσες και στα node_arrays (first, my_list)
	leda::node v;
	int list_id = 0;

	// Για κάθε κόμβο v
	forall_nodes(v, G)
	{
		leda::list<leda::node> L; 	// Δημιουργούμε την λίστα του κόμβου v
		L.append(v);				// Εισάγουμε τον v στην λίστα
		components[list_id] = L;	// Εισάγουμε την λίστα με id = list_id στο array που αποθηκεύει όλες τις λίστες
		first[v] = v;				// Στην αρχή όπου κάθε κόμβος βρίσκεται σε μια ξεχωριστή λίστα, first[v] = v
		my_list[v] = list_id;		// Το id της λίστα είναι το list_id
		++list_id;					// Αυξάνουμε το list_id για την επόμενη επανάληψη
	}


	leda::edge ed;

	forall_edges(ed, G)
	{
		if (first[G.source(ed)] == first[G.target(ed)])
		{
			continue;
		}

		else
		{
			TreeEdges[ed] = true;

			if (components[my_list[G.source(ed)]].size() <= components[my_list[G.target(ed)]].size())
			{
				int prevId = my_list[G.source(ed)];
				forall (v, components[my_list[G.source(ed)]])
				{
					first[v] = first[G.target(ed)];
					my_list[v] = my_list[G.target(ed)];
				}

				components[my_list[G.target(ed)]].conc(components[prevId]);
			}
			else
			{
				int prevId = my_list[G.target(ed)];
				forall (v, components[my_list[G.target(ed)]])
				{
					first[v] = first[G.source(ed)];
					my_list[v] = my_list[G.source(ed)];
				}
				components[my_list[G.source(ed)]].conc(components[prevId]);
			}
			
		}
	}
}
