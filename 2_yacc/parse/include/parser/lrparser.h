#pragma once
#include <vector>

#include "gram.h"
#include "parser/parser.h"

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

  // <state id, nonterminal id> -> next state id
  map<pair<int, int>, int> gotoTable;

  // <state id, terminal id> -> <action type, action code>
  map<pair<int, int>, pair<ActionType, int>> actionTable;

  // get extended grammar
  void GetExtG();

  // closure{nowItem} => nowSet
  void GetClosure(LR0Item item, LR0Set& set);
  // go(set, x) => newSet
  void GetGo(LR0Set nowSet, int x, LR0Set& newSet);
  void PrintLR0Item(const LR0Item& lR0Item, const char* end);
  void PrintLR0Set(const LR0Set& lR0Set, const char* end);
  void PrintStk(vector<pair<int, int>>& stk, const char* c);

 public:
  LRParser(Grammar gm) : Parser(gm) {
    isExt = false;
    GetExtG();
  };
  ~LRParser(){};
  // Get SLR(1) analysis table
  void GetSLRTable();
  void PrintLRTable();
  // print LR(0) set family and DFA
  void PrintLR0Fmly();
  // conduct a SLR(1) analysis
  bool SLRAnalysis(Item input);
};
