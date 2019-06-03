//
// Created by redwan on 6/2/19.
//

#include "../include/HamiltonianCycle.h"

Adj HamiltonianCycle::gridAdjacency(int m, int n) {
/**@brief
 * Given mxn grid size create a adjacency matrix for the graph
 */
    row = m;
    col = n;
    int N = m*n;
    Adj M(N,vector<bool>(N,false));
    for (int j = 0; j < m; ++j) {
        for (int k = 0; k < n; ++k) {
            int i = j*n + k;
            if(k>0)
            {
                M[i-1][i] = M[i][i-1] = true;
            }
            if(j>0)
            {
                M[i-n][i] = M[i][i-n] = true;
            }
        }
    }

    return M;
}

bool HamiltonianCycle::hamCycle(Adj &graph)
{
    /**@brief
     * Let us put vertex 0 as the first vertex in the path. If there is \par
     * a Hamiltonian Cycle, then the path can be started from any point \par
     * of the cycle as the graph is undirected  \par
     */
    V = graph.size();
    vector<int>path(V,-1);
    foundSolution = false;
    path[0] = 0;
    if ( hamCycleUtil(graph, path, 1) == false )
    {
        cerr<<" Solution does not exist \n";
        return false;
    }
    foundSolution = true;
    indexes = path;
    for(auto &v:path)
        cout<<v<<"\t";
    cout<<endl<<endl;
    return true;

}

bool HamiltonianCycle::hamCycleUtil(Adj &graph, vector<int>&path, int pos)
{
    /* base case: If all vertices are included in Hamiltonian Cycle */
    if (pos == V)
    {
        // And if there is an edge from the last included vertex to the
        // first vertex
        if ( graph[ path[pos-1] ][ path[0] ] == 1 )
            return true;
        else
            return false;
    }

    // Try different vertices as a next candidate in Hamiltonian Cycle.
    // We don't try for 0 as we included 0 as starting point in in hamCycle()
    for (int v = 1; v < V; v++)
    {
        /* Check if this vertex can be added to Hamiltonian Cycle */
        if (isSafe(v, graph, path, pos))
        {
            path[pos] = v;

            /* recur to construct rest of the path */
            if (hamCycleUtil (graph, path, pos+1) == true)
                return true;

            /* If adding vertex v doesn't lead to a solution,
               then remove it */
            path[pos] = -1;
        }
    }

    /* If no vertex can be added to Hamiltonian Cycle constructed so far,
       then return false */
    return false;
}

bool HamiltonianCycle::isSafe(int v, vector<vector<bool>>&graph, vector<int>&path, int pos)
{
    /* Check if this vertex is an adjacent vertex of the previously
       added vertex. */
    if (graph [ path[pos-1] ][ v ] == 0)
        return false;

    /* Check if the vertex has already been included.
      This step can be optimized by creating an array of size V */
    for (int i = 0; i < pos; i++)
        if (path[i] == v)
            return false;

    return true;
}

PATH HamiltonianCycle::getTrajectory() {
    assert(foundSolution && "no solution found");
    PATH path;
    auto getSub = [&](int index)
    {
        return Point{index%row, index/row};
    };
    for(auto &i:indexes)
        path.push_back(getSub(i));
    return path;
}

HamiltonianCycle::HamiltonianCycle() {

}

