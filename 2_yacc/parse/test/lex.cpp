#include "gram.h"
#include "lexer.h"

using namespace std;
int main() {
  Grammar G("data/grammar.in");
  G.Print();
  Lexer lexer(G);
  vector<int> item;
  item = lexer.Analysis("data/expression.in");
  for (auto x : item) 
    printf("%d ", x);
  printf("\n");
  return 0;
}
