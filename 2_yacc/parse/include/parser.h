#pragma once
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "gram.h"
#include "utils.h"

class Parser {
 public:
  Grammar gram;
  Parser(Grammar gram) : gram(gram){};
};

class LL1Parser : public Parser {
  typedef vector<int> Item;

 private:
  map<pair<int, int>, int>
      LL1Table;  // id of Nonterminal, Terminal and production
  void PrintStack(stack<int>& stk, const char* end);
  void PrintVec(vector<int>& vec, int start, const char* end);

 public:
  static const int ERROR = -2;
  static const int SYNC = -1;
  LL1Parser(Grammar gm) : Parser(gm){};
  bool GetLL1Table();  // false means conflict
  void PrintTable();
  bool LL1Analysis(Item input);
};

class LRParser : public Parser {
  typedef vector<int> Item;
  typedef pair<int, Item> LR0Item;  // LR(0) item: id of nonterminal, Item
  typedef vector<LR0Item> LR0Set;   // LR(0) item set
  typedef vector<LR0Set> LR0Fmly;   // LR(0) item set specification family

  const int DOT_ID = -1;

 private:
  bool isExt;

 public:
  enum ActionType { SHIFT, REDUCE, ACCEPT, ERROR } actionType;
  Grammar extGram;

 private:
  LR0Fmly lR0Fmly;
  map<pair<int, int>, int>
      gotoTable;  // id of state, nonterminal and next state
  map<pair<int, int>, pair<ActionType, int>>
      actionTable;  // id of state, id of terminal, action type, action code
  void GetExtG();
  void GetClosure(LR0Item item, LR0Set& set);  // closure{nowItem} => nowSet
  void GetGo(LR0Set nowSet, int x, LR0Set& newSet);
  void PrintLR0Item(const LR0Item& lR0Item, const char* end);
  void PrintLR0Set(const LR0Set& lR0Set, const char* end);
  void PrintLR0Fmly();

 public:
  LRParser(Grammar gm) : Parser(gm) {
    isExt = false;
    GetExtG();
  };
  ~LRParser(){};
  void GetSLRTable();
  void PrintLRTable();
  bool LRAnalysis(Item input);
};
