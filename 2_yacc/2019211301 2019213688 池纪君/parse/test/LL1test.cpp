#include "gram.h"
#include "parser/ll1parser.h"

int main() {
  Grammar G("data/grammar3.in");
  // G.Print();
  // G.Debug();

  vector<int> vec({6, 8, 2, 8, 7, 4, 8});  // (num + num) * num

  LL1Parser p(G);
  p.GetLL1Table();
  p.PrintTable();
  p.LL1Analysis(vec);

  return 0;
}
