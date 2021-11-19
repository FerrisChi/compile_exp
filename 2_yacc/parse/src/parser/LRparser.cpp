#include "parser/lrparser.h"

void LRParser::GetExtG() {
  if (isExt) return;
  extGram = gram;
  extGram.symbols.push_back(Symbol("S'", Symbol::NonTerminal));
  extGram.N.push_back(extGram.symbols.size() - 1);
  extGram.items.push_back(vector<int>({extGram.Start}));
  extGram.P.push_back(
      make_pair(extGram.symbols.size() - 1, extGram.items.size() - 1));
  extGram.Start = extGram.symbols.size() - 1;
  extGram.isCalcFirst = false;
  extGram.isCalcFollow = false;
  isExt = true;
}

void LRParser::PrintLR0Item(const LR0Item& lR0Item, const char* end = " ") {
  printf("[");
  extGram.symbols[lR0Item.first].print();
  printf("-> ");
  for (auto x : lR0Item.second) {
    if (x == -1)
      printf(".");
    else
      extGram.symbols[x].print("\0");
  }
  printf("]%s", end);
}

void LRParser::PrintLR0Set(const LR0Set& lR0Set, const char* end = "\n") {
  printf("{ ");
  for (auto lr0Item : lR0Set) {
    if (lr0Item == lR0Set.back())
      PrintLR0Item(lr0Item, "\0");
    else
      PrintLR0Item(lr0Item, ", ");
  }
  printf(" }%s", end);
}

void LRParser::PrintLR0Fmly() {
  printf("\nLR0 DFA:\n{\n");
  pair<int, int> pr;
  for (int i = 0; i < lR0Fmly.size(); i++) {
    printf("Set %d: ", i);
    PrintLR0Set(lR0Fmly[i], "\0");
    int fr = 1;
    for (int x = 0; x < extGram.symbols.size(); x++) {
      pr = make_pair(i, x);
      if (actionTable.find(pr) != actionTable.end() &&
          actionTable[pr].first == SHIFT) {
        if (fr) {
          fr = 0;
          printf(" |   read ");
        } else
          printf(", ");
        printf("\'");
        extGram.symbols[x].print("\' -> ");
        printf("set %d", actionTable[pr].second);
      }
    }
    printf(" \n");
  }
  printf("}\n");
}

void LRParser::GetClosure(LR0Item begItem, LR0Set& set) {
  set.clear();
  set.push_back(begItem);
  // PrintLR0Item(begItem, "\n");
  LR0Set newSet;
  int cnt = 0;
  do {
    newSet = set;
    for (auto lR0Item : newSet) {
      int x = 0;
      Item item = lR0Item.second;
      while (item[x] != DOT_ID) x++;
      if (x == item.size() - 1 || extGram.isTerminal(item[++x])) continue;
      // A -> α·Bη, B is nonterminal
      for (auto prod : extGram.P)
        if (prod.first == item[x]) {
          // add B -> ·η to current LR0 set
          LR0Item newLR0Item;
          newLR0Item.first = item[x];
          if (extGram.items[prod.second][0] == EPSILON_ID) {
            newLR0Item.second = vector<int>({DOT_ID});
          } else {
            newLR0Item.second = vector<int>(extGram.items[prod.second]);
            newLR0Item.second.insert(newLR0Item.second.begin(), DOT_ID);
          }
          int flag = 0;
          for (auto befItem : set)
            if (befItem == newLR0Item) {
              flag = 1;
              break;
            }
          if (!flag) {
            set.push_back(newLR0Item);
            // PrintLR0Item(newLR0Item, "\n");
          }
        }
    }
    // printf("%ld\n", set.size());
  } while (newSet != set);
}

void LRParser::GetGo(LR0Set nowSet, int x, LR0Set& newSet) {
  // printf("Calc GO(I,%d:%s)\n", x, extGram.symbols[x].name.c_str());
  // PrintLR0Set(nowSet, "\n");
  newSet.clear();
  LR0Set tmpSet;
  for (auto lR0Item : nowSet) {
    int p = 0;
    Item item = lR0Item.second;
    while (item[p] != DOT_ID) p++;
    if (p == item.size() - 1 || item[++p] != x) continue;
    LR0Item tmpLR0Item = lR0Item;
    auto it = tmpLR0Item.second.begin();
    tmpLR0Item.second.erase(it + p - 1);
    tmpLR0Item.second.insert(it + p, DOT_ID);
    GetClosure(tmpLR0Item, tmpSet);
    LR0Set uniSet(extGram.symbols.size() * extGram.symbols.size());
    auto itt = set_union(tmpSet.begin(), tmpSet.end(), newSet.begin(),
                         newSet.end(), uniSet.begin());
    uniSet.resize(itt - uniSet.begin());
    newSet = uniSet;
  }
  // printf("result: ");
  // PrintLR0Set(newSet, "\n");
}

void LRParser::GetSLRTable() {
  extGram.CalcFirst();
  extGram.CalcFollow();

  printf("\nGenerating LR(0) item sets.\n");
  LR0Item beginItem =
      make_pair(extGram.Start, vector<int>({DOT_ID, gram.Start}));
  LR0Item nowItem = beginItem, succItem = beginItem;
  succItem.second.erase(succItem.second.begin());
  succItem.second.push_back(DOT_ID);
  LR0Set newSet;
  LR0Fmly newFamily;
  GetClosure(nowItem, newSet);
  lR0Fmly.push_back(newSet);
  do {
    newFamily = lR0Fmly;
    for (int i = 0; i < newFamily.size(); i++) {  // I_i
      LR0Set nowSet = newFamily[i];
      for (int x = 0; x < extGram.symbols.size(); x++)
        if (x != EPSILON_ID) {
          GetGo(nowSet, x, newSet);
          if (!newSet.size()) continue;  // set is {}
          int in = -1;
          for (int j = 0; j < lR0Fmly.size(); j++) {  // I_j
            LR0Set befSet = lR0Fmly[j];
            if (befSet == newSet) {
              in = j;
              break;
            }
          }
          if (in == -1) {
            // add newSet to LR(0) sets family
            lR0Fmly.push_back(newSet);
            in = lR0Fmly.size() - 1;
            for (auto lR0Item : newSet) {
              if (lR0Item == succItem) {
                // printf("action[%d, %d]=ACC\n", in, DOLLAR_ID);
                actionTable[make_pair(in, DOLLAR_ID)] = make_pair(ACCEPT, 0);
              } else if (lR0Item.second.back() == DOT_ID) {
                // action[i,a]=R (A->α)
                LR0Item tmp = lR0Item;
                tmp.second.pop_back();
                int prodId;
                for (int p = 0; p < extGram.P.size(); p++)
                  if (tmp.first == extGram.P[p].first) {
                    if (tmp.second == extGram.items[extGram.P[p].second] ||
                        (tmp.second.empty() &&
                         extGram.items[extGram.P[p].second][0] == EPSILON_ID)) {
                      prodId = p;
                      break;
                    }
                  }
                for (auto x : extGram.follow[lR0Item.first]) {
                  actionTable[make_pair(in, x)] = make_pair(REDUCE, prodId);
                  // printf("action[%d, %d]=R%d\n", in, x, prodId);
                }
              }
            }
          }
          if (extGram.isTerminal(x)) {
            actionTable[make_pair(i, x)] = make_pair(SHIFT, in);
            // printf("action[%d, %d]=S%d\n", i, x, in);
          } else {
            gotoTable[make_pair(i, x)] = in;
            // printf("goto[%d, %d]=%d\n", i, x, in);
          }
        }
    }
  } while (lR0Fmly != newFamily);
  printf("LR(0) item sets generated.\n");
  PrintLR0Fmly();
}

void LRParser::PrintLRTable() {
  printf("\nSLR(1) Table:\n");
  int blank1 = (extGram.T.size() - 1) * 8 - 5,
      blank2 = (extGram.N.size() - 1) * 8 - 4;
  printf("State\t");
  PrintBlank(blank1 / 2);
  printf("action");
  PrintBlank(blank1 - blank1 / 2 - 1);
  printf("|");
  PrintBlank(blank2 / 2);
  printf("goto");
  PrintBlank(blank2 - blank2 / 2 - 1);
  printf("|\n\t| ");

  for (auto x : extGram.T)
    if (extGram.isNonNullTerminal(x)) {
      extGram.symbols[x].print("\t");
    }
  printf("| ");
  for (auto x : extGram.N)
    if (x != extGram.Start) {
      extGram.symbols[x].print("\t");
    }
  printf("|\n");
  for (int i = 0; i < lR0Fmly.size(); i++) {
    printf("%d\t| ", i);
    for (auto x : extGram.T)
      if (extGram.isNonNullTerminal(x)) {
        pair<int, int> pr = make_pair(i, x);
        if (actionTable.find(pr) != actionTable.end()) {
          pair<ActionType, int> acpr = actionTable[pr];
          if (acpr.first == SHIFT)
            printf("S%d", acpr.second);
          else if (acpr.first == REDUCE)
            printf("R%d", acpr.second);
          else if (acpr.first == ACCEPT)
            printf("ACC");
        }
        printf("\t");
      }
    printf("| ");
    for (auto x : extGram.N)
      if (x != extGram.Start) {
        pair<int, int> pr = make_pair(i, x);
        if (gotoTable.find(pr) != gotoTable.end()) {
          int nexSet = gotoTable[pr];
          printf("%d", nexSet);
        }
        printf("\t");
      }
    printf("|\n");
  }
  printf("\n");
}

void LRParser::PrintStk(vector<pair<int, int>>& stk, const char* c = "\n") {
  printf("{ ");
  for (auto pr : stk) {
    printf("<");
    printf("%d, ", pr.first);
    if (pr.second >= 0) extGram.symbols[pr.second].print("\0");
    printf(">");
    if (pr != stk.back()) printf(", ");
  }
  printf(" }%s", c);
}

bool LRParser::SLRAnalysis(Item inp) {
  int pos = 0, state, symbolId;
  vector<pair<int, int>> stk;  // <state, symbol>
  stk.push_back(make_pair(0, -1));
  pair<int, int> stpr, ckpr;
  pair<ActionType, int> acpr;
  inp.push_back(DOLLAR_ID);
  printf("\nStarting analysis:\n");
  int cnt = 0;
  do {
    cnt++;
    printf("Step %d:\n", cnt);
    PrintStk(stk, "\n");
    extGram.PrintItem(Item(inp.begin() + pos, inp.end()), "\n");

    stpr = stk.back();
    symbolId = inp[pos];
    ckpr = make_pair(stpr.first, symbolId);
    if (actionTable.find(ckpr) ==
        actionTable.end()) {  // not found in action table;
      printf("Error\n");
      return false;
    }
    acpr = actionTable[ckpr];
    if (acpr.first == SHIFT) {
      stk.push_back(make_pair(acpr.second, symbolId));
      pos++;
      printf("shift %d\n", acpr.second);
    } else if (acpr.first == REDUCE) {
      // pop |β| pair, push <goto[S',A], A>, output A->β
      int itemId = extGram.P[acpr.second].second;
      if (extGram.items[itemId][0] != EPSILON_ID) {  // avoid ε
        int popCnt = extGram.items[itemId].size();
        for (int _ = 0; _ < popCnt; _++) stk.pop_back();
      }
      pair<int, int> nxpr;
      int A = extGram.P[acpr.second].first, S_ = stk.back().first;
      nxpr.first = gotoTable[make_pair(S_, A)];
      nxpr.second = A;
      stk.push_back(nxpr);

      printf("reduce by: ");
      extGram.PrintProd(acpr.second, ", ");
      printf("goto[%d,", S_);
      extGram.symbols[A].print("]=");
      printf("%d\n", nxpr.first);
    } else if (acpr.first == ACCEPT) {
      printf("ACC\n");
      return true;
    }
    printf("\n");
  } while (1);
  return false;
}