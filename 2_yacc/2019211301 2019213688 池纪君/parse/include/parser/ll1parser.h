#pragma once
#include <stack>
#include <vector>

#include "gram.h"
#include "parser/parser.h"

class LL1Parser : public Parser {
  typedef vector<int> Item;

 private:
  // id of Nonterminal, Terminal and production
  map<pair<int, int>, int> LL1Table;
  void PrintStack(stack<int>& stk, const char* end);
  void PrintVec(vector<int>& vec, int start, const char* end);

 public:
  static const int ERROR = -2;
  static const int SYNC = -1;
  LL1Parser(Grammar gm) : Parser(gm){};
  bool GetLL1Table();  // return false means conflict
  void PrintTable();
  bool LL1Analysis(Item input);
};
