#include "gram.h"
#include "parser/lrparser.h"

int main() {
  Grammar G("data/grammar3.in");
  // G.Print();
  // G.Debug();

  vector<int> vec({6, 8, 2, 8, 7, 4, 8});  // (num + num) * num

  LRParser p1(G);
  p1.extGram.Print();
  p1.extGram.Debug();
  p1.GetSLRTable();
  p1.PrintLRTable();
  p1.SLRAnalysis(vec);

  return 0;
}
