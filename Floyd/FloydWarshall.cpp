#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> readGraph(int V, int E)
{
    vector<vector<int>> d(V, vector<int>(V, -1));

    int v1, v2;
    for (int i = 0; i < E; i++)
    {
        cin >> v1 >> v2;
        d[v1][v2] = 1;
        d[v2][v1] = 1;
    }

    for(int i = 0; i < V; i++)
        d[i][i] = 0;

    return d;
}

int main()
{
    int V, E;
    cin >> V >> E;
    auto d = readGraph(V, E);

    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            for (int k = 0; k < V; k++)
            {
                int tmp = (d[j][i] == -1 || d[i][k] == -1) ? -1 : d[j][i] + d[i][k];

                if (d[j][k] == -1)
                    d[j][k] = tmp;
                else if (tmp != -1 && d[j][k] > tmp)
                    d[j][k] = tmp;
            }

    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
            std::cout << d[i][j] << " ";
        std::cout << std::endl;
    }
    return 0;
}
