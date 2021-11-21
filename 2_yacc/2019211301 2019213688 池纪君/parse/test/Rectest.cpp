#include "gram.h"
#include "parser/recparser.h"

int main() {
  Grammar G("data/grammar.in");
  // G.Print();
  // G.Debug();

  vector<int> vec({6, 8, 2, 8, 7, 4, 8});  // (num + num) * num
  RecurParser recurParser(G);
  recurParser.Analysis(vec);

  return 0;
}
