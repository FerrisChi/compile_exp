#include "gram.h"
#include "parser/lrparser.h"

int main() {
  Grammar G("data/grammar.in");
  // G.Print();
  // G.Debug();

  vector<int> vec({6, 8, 2, 8, 7, 4, 8});  // (num + num) * num

  LRParser p1(G);
  p1.extGram.Print();
  p1.extGram.Debug();
  p1.extGram.PrintFirst();
  p1.extGram.PrintFollow();
  p1.GetSLRTable();
  p1.PrintLR0Fmly();
  p1.PrintLRTable();
  p1.SLRAnalysis(vec);

  return 0;
}
