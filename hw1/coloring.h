#ifndef COLORING_H_
#define COLORING_H_

#include <vector>
#include "minisat/core/Solver.h"

using namespace std;

// ***************************************************************************
// A graph class. 
// Note that when adding an edge (n1,n2) n1 must be less or 
// equal to n2. This is only done for simplicity and a more compact 
// implementation.
// ***************************************************************************
class Graph {
public:
    Graph(int nNumberOfNodes) : m_nNumberOfNodes(nNumberOfNodes)
    {
        m_graph.resize(nNumberOfNodes);
    }

    int getNumberOfNodes() const { return m_nNumberOfNodes; }

    // Not efficient for large graphs
    vector<int> getEdgesForNode(int node) const
    {
        assert (node < m_nNumberOfNodes);
        assert (node < m_graph.size());
        return m_graph[node];
    }

    // For now allowing duplication
    void addEdge (int n1, int n2)
    {
        assert (n1 < m_nNumberOfNodes &&
                n2 < m_nNumberOfNodes);
        assert (n1 <= n2);

        // Make sure that the vector can contain the first node
        if (m_graph.size() <= n1)
            m_graph.resize(n1+1);

        // Take care of the first node
        m_graph[n1].push_back(n2);
    }

private:
    int m_nNumberOfNodes;
    // A vector of vectors to represent the adjacency list
    // The outer vector is mapping a node (by index) to its
    // vector which represents a container of adjacent nodes.
    vector<vector<int> > m_graph;
};

// ***************************************************************************
// A class modeling the k-coloring problem.
// ***************************************************************************
class Coloring {
public:
    Coloring(const Graph& g, int nNumberOfColors) :
          m_graph(g)
        , m_nNumberOfColors(nNumberOfColors)
        , m_solver()
    {
        // Prepare the solver with the needed variables
        int nodes = m_graph.getNumberOfNodes();
        for (int c = 0; c < m_nNumberOfColors; c++)
        {
            for (int n = 0; n < nodes; n++)
            {
                m_solver.newVar();
            }
        }
    }

    void addOneColorConstraints(int node) {
        assert (node < m_graph.getNumberOfNodes());

        // Add your code here
    }

    void addEdgeColoringConstraints(int n1, int n2) {
        assert (n1 < m_graph.getNumberOfNodes() &&
                n2 < m_graph.getNumberOfNodes());
        assert (n1 <= n2);

        // Add your code here

    }

    bool isColorable()
    {
        // Go over all nodes
        for (int n = 0; n < m_graph.getNumberOfNodes(); n++)
        {
            // Add the constraints for the node
            addOneColorConstraints(n);

            // Now add constraints for the edges
            vector<int> edges = m_graph.getEdgesForNode(n);
            for (int adjcent = 0; adjcent < edges.size(); adjcent++)
            {
                addEdgeColoringConstraints(n, edges[adjcent]);
            }
        }

        bool bResult = m_solver.solve();

        return bResult;
    }

    // The function gets allColoring by reference and returns
    // all k-coloring in this vector. Note that the inner vector
    // represents one assignment
    void givemeAllColoring(vector<vector<Minisat::lbool> >& allColoring) {
        // Go over all nodes
        for (int n = 0; n < m_graph.getNumberOfNodes(); n++)
        {
            // Add the constraints for the node
            addOneColorConstraints(n);

            // Now add constraints for the edges
            vector<int> edges = m_graph.getEdgesForNode(n);
            for (int adjcent = 0; adjcent < edges.size(); adjcent++)
            {
                addEdgeColoringConstraints(n, edges[adjcent]);
            }
        }

        // Add your code here
    }

private:
    Minisat::Var getNodeHasColorVar(int node, int color)
    {
        assert (node < m_graph.getNumberOfNodes() &&
                color < m_nNumberOfColors);

        return (color * m_graph.getNumberOfNodes()) + node;
    }

private:
    const Graph& m_graph;
    int m_nNumberOfColors;

    Minisat::Solver m_solver;
};

#endif // COLORING_H_
