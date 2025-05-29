#include "bellmanFord.hpp"
#include "visitor_graph.hpp"
#include <queue>

using namespace boost;
//using namespace std;

void Update_pred(const Graph& BG, Vertex v, const std::vector<bool>& in_R, std::vector<bool>& reached_from_node_in_U, std::vector<Edge>& pred)
{
	reached_from_node_in_U[v] = true;

	OutEdgeIterator outed_f, outed_s;
	for (tie(outed_f, outed_s) = out_edges(v, BG); outed_f != outed_s; ++outed_f)
	{
		Vertex w = target(*outed_f, BG);
		if (!reached_from_node_in_U[w])
		{
			if (in_R[w]) pred[w] = *outed_f;
			Update_pred(BG, w, in_R, reached_from_node_in_U, pred);
		}
	}
}

bool bellmanFord(Graph& BG, Vertex s, std::vector<long>& dist, std::vector<Edge>& pred)
{
	
	Edge edge_nil = *(edges(BG).second);
	Vertex vertex_nil = graph_traits<Graph>::null_vertex();

	int n = num_vertices(BG); 
	int phase_count = 0;

	std::queue<Vertex> Q;
	std::vector<bool> in_Q(n, false);
	Vertex u, v;

	VertexIterator vit_first, vit_sec;

	for (tie(vit_first, vit_sec) = vertices(BG); vit_first != vit_sec; ++vit_first)
	{
		pred[*vit_first] = edge_nil;
	}

	dist[s] = 0;
	Q.push(s); in_Q[s] = true;
	Q.push(vertex_nil);

	while (phase_count < n)
	{
		u = Q.front();
		Q.pop();
		if (u == vertex_nil)
		{
			phase_count++;
			if (Q.empty()) return true;
			Q.push(vertex_nil);
			continue;
		}
		else in_Q[u] = false;

		long du = dist[u];

		OutEdgeIterator outEdgeItFirst, outEdgeItSec;

		for (tie(outEdgeItFirst, outEdgeItSec) = out_edges(u, BG); outEdgeItFirst != outEdgeItSec; ++outEdgeItFirst)
		{
			v = target(*outEdgeItFirst, BG);
			long d = du + BG[*outEdgeItFirst].weight;
			if ((pred[v] == edge_nil && v != s) || d < dist[v])
			{
				dist[v] = d; pred[v] = *outEdgeItFirst;
				
				if (!in_Q[v]) {Q.push(v); in_Q[v] = true;}
			}
		}
	}
	
	// Postprocessing

	if (pred[s] != edge_nil) return false;

	std::vector<bool> in_R(n, false);

	EdgeIterator ed_f, ed_s;

	std::vector<Edge> removed_vertices;

	for (tie(ed_f, ed_s) = edges(BG); ed_f != ed_s; ++ed_f)
	{
		if (*ed_f != pred[target(*ed_f, BG)])
		{
			removed_vertices.push_back(*ed_f);
		} 
	}

	for (int i = 0; i < removed_vertices.size(); i++)
	{
		remove_edge(removed_vertices[i], BG); 
	}
	
	my_dfs_visitor vis(BG);

	boost::depth_first_search(BG, visitor(vis));
	in_R = vis.return_vector();
	
	for (int i = 0; i < removed_vertices.size(); i++)
	{
		Vertex src, trt;
		src = source(removed_vertices[i], BG);
		trt = target(removed_vertices[i], BG);
		add_edge(src, trt, BG);
	}

	std::vector<bool> reached_from_node_in_U(n, false);

	for (tie(vit_first, vit_sec) = vertices(BG); vit_first != vit_sec; ++vit_first)
	{
		if (in_Q[*vit_first] && !reached_from_node_in_U[*vit_first])
		Update_pred(BG, *vit_first, in_R, reached_from_node_in_U, pred);
	}
	
	return false;
}