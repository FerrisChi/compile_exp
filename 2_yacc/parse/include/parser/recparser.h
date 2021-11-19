#pragma once
#include <iostream>

#include "gram.h"

class RecurParser {
  typedef vector<int> Item;

 public:
  Grammar gram;
  RecurParser(Grammar gm) : gram(gm) {}
  RecurParser() {}
  bool Analysis(Item inp);

 private:
  Item input;
  Symbol nowS;
  int pos;
  inline void Error() {
    printf("error at position %d : ", pos);
    nowS.print("\n");
    exit(0);
  }
  void ProcE();
  void ProcF();
  void ProcT();
  inline void fp() { nowS = gram.symbols[input[pos++]]; }
};
