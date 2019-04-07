// ********************************************************************
// Modeling the Pigeon Hole Problem with MiniSAT
// Can n+1 pigeons fit in n holes?
//
// Assuming we have 3 pigeons and 2 holes:
// We introduce a variable for each possible position of the pigeon.
// This means 6 variables:
// piegon 1: v_1_1 and v_1_2
// piegon 2: v_2_1 and v_2_2
// piegon 3: v_3_1 and v_3_2
//
// Now the constraints:
// 1. We want to make sure that a pigeon is in a hole:
//    (v_1_1 or v_1_2)
//    (v_2_1 or v_2_2)
//    (v_3_1 or v_3_2)
// 2. We want to make sure only one pigeon is placed in a given hole:
//    (!v_1_1 or !v_2_1) and (!v_1_1 or !v_3_1) and (!v_2_1 or !v_3_1)
//    (!v_1_2 or !v_2_2) and (!v_1_2 or !v_3_2) and (!v_2_2 or !v_3_2)
// ********************************************************************



#include <stdlib.h>
#include <stdio.h>

//#include "minisat/core/Solver.h"
#include "../extavy/minisat/utils/System.h"
#include "../extavy/minisat/core/Solver.h"

int main(int argc, char** argv) {

  if (argc != 2)
    exit(1);

  int n = atoi(argv[1]);

  if (n < 1)
    exit(1);


  Minisat::Solver s;
  s.verbosity = 1;
  // Creating the variables
  for (int i=0; i < n+1; i++) {
    for (int j=0; j < n; j++)
      Minisat::Var v = s.newVar(); // Var n*i + j = pigeon i has a place in hole j;
  }

  // n+1 clauses - a pigeon has to be placed in some hole
  for (int i=0; i < n+1; i++) {
    Minisat::vec<Minisat::Lit> clause;
    for (int j=0; j < n; j++) {
      Minisat::Var v = n*i + j; // = pigeon i has a place in hole j;
      clause.push(Minisat::mkLit(v, false));
    }
    s.addClause(clause);
  }

  // For each hole make sure that only one pigeon is placed in it.
  for (int j=0; j < n; j++)
    for (int i=0; i < n; i++)
      for (int k=i+1; k < n+1; k++)
        s.addClause(Minisat::mkLit(n*i + j, true), Minisat::mkLit(n*k + j, true));

  double initial_time = Minisat::cpuTime();

  s.toDimacs("cnf.cnf");
  bool res = s.solve();

  if (res) printf("This cannot be!\n");
  else printf("As expected!\n");

  printf("Time to solve: %12.2f s\n", Minisat::cpuTime() - initial_time);
  return 0;
}
