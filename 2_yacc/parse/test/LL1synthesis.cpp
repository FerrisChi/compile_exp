#include "gram.h"
#include "lexer.h"
#include "parser/ll1parser.h"

using namespace std;
int main() {
  Grammar G("data/grammar3.in");
  G.Print();
  Lexer lexer(G);
  vector<int> item;
  item = lexer.Analysis("data/expression.in");
  LL1Parser ll1Parser(G);
  ll1Parser.GetLL1Table();
  ll1Parser.LL1Analysis(item);
  return 0;
}
