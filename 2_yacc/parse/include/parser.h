#pragma once
#include <map>
#include <stack>

#include "gram.h"

class Parser {
 private:
 public:
  Grammar gram;
  Parser(Grammar gram) : gram(gram){};
};

class LL1Parser : public Parser {
  typedef vector<int> Item;

 private:
  map<pair<int, int>, int>
      LL1Table;  // id of Nonterminal, Terminal and production
  void PrintStack(stack<int>& stk, char end);
  void PrintVec(vector<int>& vec, int start, char end);

 public:
  static const int ERROR = -2;
  static const int SYNC = -1;
  LL1Parser(Grammar gm) : Parser(gm){};
  bool GetLL1Table();  // false means conflict
  void PrintTable();
  bool LL1Analysis(Item input);
};

class LR1Parser : public Parser {
 public:
  enum ActionType { SHIFT, REDUCE, ACCEPT, ERROR } actionType;

 private:
  map<pair<int, int>, int> gotoTable;  // id of state, nonterminal and next state
  map<pair<int,int>,pair<ActionType,int>> actionTable; // id of state, id of terminal, action type, action code
 public:
  LR1Parser(Grammar gm) : Parser(gm){};
  ~LR1Parser(){};
};
