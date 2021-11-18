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
    this->type = type == 1 ? Terminal : NonTerminal;
  }
  enum Type { Terminal, NonTerminal } type;
  std::string name;
  int print(char end = ' ') {
    printf("%s%c", name.c_str(), end);
    int l = name.length() + (end != '\0');
    if (name == "ε") l--;
    return l;
  };
};

class Item {
 public:
  vector<Symbol> s;
  int print(char end = ' ') {
    int cnt = 0;
    for (auto x : s) cnt += x.print('\0');
    printf("%c", end);
    return cnt + (end != '\0');
  }
};

// 2型文法
class Grammar {
 private:
  void Init(std::string fpgram);
  bool isCalcFirst;
  bool isCalcFollow;

 public:
  int Start;
  vector<Symbol> symbols;
  vector<vector<int>> items;
  vector<int> T;
  vector<int> N;
  vector<pair<int,int>> P;  // <id of symbol, id of item>
  vector<vector<int>> first, follow;
  void CalcFirst();
  void CalcFollow();
  void Print();
  void Debug();
  void GetFirst(vector<int> item, vector<int> &f);  // get first set of item
  bool isTerminal(int id);  // whether id is a terminal character
  bool isNonNullTerminal(int id);
  bool isNonterminal(int id);
  bool isInFirst(int id, vector<int> item);  // whether id is in first{item}
  bool isInFollow(int termId, int nontermId);
  bool isProduction(int Nid,
                    vector<int> &item);  // judge if {nonterminal -> item} is a
                                         // valid production.

  Grammar();
  ~Grammar();
};
