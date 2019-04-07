#include "coloring.h"

// ***************************************************************************
// A main file that uses the Coloring and Graph classes.
// Note that you can create your own graphs and test their k-colorability for
// different values of k.
// ***************************************************************************
int main()
{
    printf("\n====================================================\n\n");
    printf("The k-coloring problem\n");
    printf("\n====================================================\n\n\n");

    int k = 2; // The number of colors

    // Create a graph with 3 nodes.
    Graph g(3);

    // Add edges to the graph - this is a simple triangle.
    g.addEdge(0,1);
    g.addEdge(0,2);
    g.addEdge(1,2);
    
    // Model the problem for k.
    Coloring c(g, k);

    // Now ask if it is colorable
    bool bResult = c.isColorable();

    if (bResult) {
        printf("\tThe graph is %d-colorable.\n\n", k);
    }
    else {
        printf("\tNo %d-coloring is found!\n\n", k);
    }
    return 0;
}
