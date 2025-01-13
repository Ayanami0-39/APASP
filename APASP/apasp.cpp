#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <cassert>

#include "Matrix.h"
#include "AdjList.h"

using namespace std;

Matrix<int> matrixFromAdjList(const AdjList& G)
{
    int n = G.size();
    Matrix<int> d(n, n, n);
    for (int u = 0; u < n; ++u)
        for (int v : G[u])
            d(u, v) = 1;
    return d;
}

// Theorem 3.1
Matrix<int> apasp_a(AdjList &G)
{
    int n = G.size();       // V
    int m = G.edge_size();  // E
    m /= 2;

    int s1 = lround(sqrt(m / n));

    AdjList E2 = G.get_edge_subset(s1);

    vector<int> D1;
    AdjList Es;
    tie(D1, Es) = G.dominate(s1);

    Matrix<int> d = matrixFromAdjList(G);

    vector<bool> is_visited(n);
    for (int u : D1)
    {
        is_visited[u] = true;
        G.dijkstra(d, u);
    }

    E2.merge_graphs_from(Es);
    for (int u = 0; u < n; ++u)
    {
        if (is_visited[u])
            continue;

        AdjList new_graph = E2;
        for (int v : D1)
            new_graph.add_edge(u, v);

        new_graph.dijkstra(d, u);
    }

    return d;
}

// Theorem 3.2
Matrix<int> apasp_b(AdjList &G)
{
    int n = G.size(); // V
    int s1 = lround(pow(n, 2. / 3));
    int s2 = lround(pow(n, 1. / 3));

    AdjList E2 = G.get_edge_subset(s1);
    AdjList E3 = G.get_edge_subset(s2);

    vector<int> D1, D2;
    AdjList Es1, Es2;
    tie(D1, Es1) = G.dominate(s1);
    tie(D2, Es2) = G.dominate(s2);
    Es1.merge_graphs_from(Es2);

    Matrix<int> d = matrixFromAdjList(G);

    for (int u : D1)
        G.dijkstra(d, u);

    for (int u : D2)
        E2.dijkstra(d, u);

    E3.merge_graphs_from(Es1);
    for (int u : D1)
        for (int v = 0; v < n; ++v)
            if (u != v)
                E3.add_edge(u, v);
    
    for (int u : D2)
        for (int v : D2)
            if (u != v)
                E3.add_edge(u, v);

    for (int u = 0; u < n; ++u)
    {
        AdjList new_graph = E3;
        for (int v : D2)
            if (u != v)
                new_graph.add_edge(u, v);
        new_graph.dijkstra(d, u);
    }

    return d;
}

int main(int argc, char *argv[])
{
    int node_count, edge_count;
    cin >> node_count >> edge_count;

    AdjList G(node_count);

    for (int i = 0; i < edge_count; ++i)
    {
        int u, v;
        cin >> u >> v;
        G.add_edge(u, v);
    }

    Matrix<int> d;

    bool sparse = (pow(node_count, 1.5)*pow(edge_count, 0.5) < pow(node_count, 2.333));
    d = sparse ? apasp_a(G) : apasp_b(G);

    d.print();
}