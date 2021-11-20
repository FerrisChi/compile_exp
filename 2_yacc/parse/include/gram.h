#pragma once
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <map>
#include <vector>
using namespace std;

const int DOLLAR_ID = 0;
const int EPSILON_ID = 1;                   // ε
const int MAX_ITEM_LEN = 6;                 // max item length
const int MAX_PROD_LEN = MAX_ITEM_LEN + 5;  // max production length

// 单个符号
class Symbol {
 public:
  Symbol(){};
  Symbol(const char *name, int type) {
    this->name = name;
    this->type = type == 0 ? Terminal : NonTerminal;
  }
  bool operator==(const Symbol &s) {
    return (this->name == s.name) && (this->type == s.type);
  }
  enum Type { Terminal, NonTerminal } type;
  std::string name;
  int print(const char *end = " ") {
    printf("%s%s", name.c_str(), end);
    int l = name.length() + !(strcmp(end, "\0") == 0);
    if (name == "ε") l--;
    return l;
  };
};

// 2型文法
class Grammar {
  typedef vector<int> Item;

 private:
  void Init(const char *inp);

 public:
  bool isCalcFirst;
  bool isCalcFollow;
  int Start;
  vector<Symbol> symbols;
  vector<Item> items;
  vector<int> T;
  vector<int> N;
  // <id of symbol, id of item>
  vector<pair<int, int>> P;
  vector<vector<int>> first, follow;
  void CalcFirst();
  void CalcFollow();
  void Print();
  void Debug();

  // print production {id} and return number of character printed
  int PrintProd(int prodId, const char *c);
  int PrintItem(Item item, const char *c);
  void PrintFirst();
  void PrintFollow();
  // get first set of item
  void GetFirst(Item item, vector<int> &f);
  // whether id is a terminal character
  bool isTerminal(int id);
  bool isNonNullTerminal(int id);
  bool isNonterminal(int id);
  // whether id is in first{item}
  bool isInFirst(int id, Item item);
  bool isInFollow(int termId, int nontermId);
  // judge if {nonterminal -> item} is a valid production.
  bool isProduction(int Nid, Item &item);

  Grammar(){};
  Grammar(const char *inp);
  ~Grammar();
};
