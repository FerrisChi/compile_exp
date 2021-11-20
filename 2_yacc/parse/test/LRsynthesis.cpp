#include "gram.h"
#include "lexer.h"
#include "parser/lrparser.h"

using namespace std;
int main() {
  Grammar G("data/grammar.in");
  G.Print();
  Lexer lexer(G);
  vector<int> item;
  item = lexer.Analysis("data/expression.in");
  LRParser lrParser(G);
  lrParser.extGram.Debug(); 
  lrParser.GetSLRTable();
  lrParser.SLRAnalysis(item);
  return 0;
}
