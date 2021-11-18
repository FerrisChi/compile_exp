#include "gram.h"
#include "parser.h"

int main() {
  // Grammar *G = new Grammar();
  Grammar G;
  G.Print();
  G.Debug();
  G.CalcFirst();
  G.CalcFollow();

  LL1Parser p(G);
  p.GetLL1Table();
  p.PrintTable();
  vector<int> vec({6, 8, 2, 8, 7, 4, 8});
  p.LL1Analysis(vec);

  //   vector<int> v;
  //   G.GetFirst(vector<int>({12, 13, 12, 13, 5}), v);
  //   for (auto x : v) printf("%d ", x);
  return 0;
}
