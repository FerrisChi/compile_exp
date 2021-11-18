#include "gram.h"
#include "parser.h"

int main() {
  // Grammar *G = new Grammar();
  Grammar G("../data/grammar.in");
  G.Print();
  G.Debug();

  // LL1Parser p(G);
  // p.GetLL1Table();
  // p.PrintTable();
  // vector<int> vec({6, 8, 2, 8, 7, 4, 8});  // (num + num) * num grammar
  // p.LL1Analysis(vec);

  LRParser p1(G);
  p1.extGram.Print();
  p1.extGram.Debug();
  p1.GetSLRTable();
  p1.PrintLRTable();
  // p1.LRAnalysis(vec);

  //   vector<int> v;
  //   G.GetFirst(vector<int>({12, 13, 12, 13, 5}), v);
  //   for (auto x : v) printf("%d ", x);
  return 0;
}
