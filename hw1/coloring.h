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

        // node has a color: or(for_color_c( var (node, c) ))
        Minisat::vec<Minisat::Lit> clause;
        for (int c=0; c < m_nNumberOfColors; c++) {
            Minisat::Var v = getNodeHasColorVar(node, c); // = node n has the color c;
            clause.push(Minisat::mkLit(v, false));
        }
        m_solver.addClause(clause);

        // node doesn't have more than one color: for_color_c(for_color_k_not_c( or(not(var(node, c)), not(var(node,k))))
        for (int c=0; c < m_nNumberOfColors-1; c++) {
            for (int k = c + 1; k < m_nNumberOfColors; k++) {
                Minisat::Lit node_is_not_c = Minisat::mkLit(getNodeHasColorVar(node, c), true)
                Minisat::Lit node_is_not_k = Minisat::mkLit(getNodeHasColorVar(node, k), true)
                m_solver.addClause(node_is_not_c, node_is_not_k);
            }
        }
    }

    void addEdgeColoringConstraints(int n1, int n2) {
        assert (n1 < m_graph.getNumberOfNodes() &&
                n2 < m_graph.getNumberOfNodes());
        assert (n1 <= n2);

        // Add your code here

        //nodes n1 n2 have different colors: for_color_c(or(not(var(n1, c)), not(var(n2,c))))
        for (int c=0; c < m_nNumberOfColors-1; c++) {
            Minisat::Lit n1_is_not_c = Minisat::mkLit(getNodeHasColorVar(n1, c), true)
            Minisat::Lit n2_is_not_c = Minisat::mkLit(getNodeHasColorVar(n2, c), true)
            m_solver.addClause(n1_is_not_c, n2_is_not_c);
        }
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
