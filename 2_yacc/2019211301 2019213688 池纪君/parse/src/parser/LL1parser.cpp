#include "parser/ll1parser.h"

bool LL1Parser::GetLL1Table() {
  gram.CalcFirst();
  gram.CalcFollow();
  int nowId, termId;
  vector<int> nowFirst;
  for (int prodId = 0; prodId < gram.P.size(); prodId++) {
    nowId = gram.P[prodId].first;
    gram.GetFirst(gram.items[gram.P[prodId].second], nowFirst);
    for (int i = 0; i < nowFirst.size(); i++) {
      termId = nowFirst[i];
      if (termId != EPSILON_ID) {
        if (LL1Table.find(make_pair(nowId, termId)) != LL1Table.end()) {
          printf("Conflict!\n");
          return false;
        }

        LL1Table[make_pair(nowId, termId)] = prodId;
        // printf("Add %d to table(%d, %d)\n", prodId, nowId, termId);
      }
    }
    if (gram.isInFirst(EPSILON_ID, nowFirst)) {
      for (int i = 0; i < gram.follow[nowId].size(); i++) {
        termId = gram.follow[nowId][i];
        if (LL1Table.find(make_pair(nowId, termId)) != LL1Table.end()) {
          printf("Conflict!\n");
          return false;
        }
        LL1Table[make_pair(nowId, termId)] = prodId;
        // printf("Add %d to table(%d, %d)\n", prodId, nowId, termId);
      }
    }
  }
  // add "sync" tag (-1)
  for (auto nontermId : gram.N)
    for (auto termId : gram.T)
      if (LL1Table.find(make_pair(nontermId, termId)) == LL1Table.end()) {
        if (gram.isInFollow(termId, nontermId))
          LL1Table[make_pair(nontermId, termId)] = SYNC;
        else
          LL1Table[make_pair(nontermId, termId)] = ERROR;
      }

  return true;
}
void LL1Parser::PrintTable() {
  printf("\nLL1 analysis table:\n");
  PrintBlank(MAX_PROD_LEN - 2);
  printf("| ");
  for (auto termId : gram.T) {
    int cnt = gram.symbols[termId].print("\0");
    PrintBlank(MAX_PROD_LEN - cnt);
  }
  printf("\n");
  for (int i = 0; i < (gram.T.size() + 1) * MAX_PROD_LEN; i++) printf("-");
  printf("\n");

  for (auto nontermId : gram.N) {
    int cnt = gram.symbols[nontermId].print("\0");
    PrintBlank(MAX_PROD_LEN - cnt - 2);
    printf("| ");
    for (auto termId : gram.T) {
      cnt = 0;
      if (LL1Table[make_pair(nontermId, termId)] == SYNC) {
        cnt = 4;
        printf("SYNC");
      } else if (LL1Table[make_pair(nontermId, termId)] != ERROR) {
        cnt = gram.symbols[nontermId].print("\0") + 4;
        printf(" -> ");
        int itemId = LL1Table[make_pair(nontermId, termId)];
        for (auto t : gram.items[itemId]) {
          cnt += gram.symbols[t].print("\0");
        }
      }
      PrintBlank(MAX_PROD_LEN - cnt);
    }
    printf("\n");
  }
  printf("\n");
}

void LL1Parser::PrintStack(stack<int> &stk, const char *end = "\n") {
  printf("Stack: ");
  stack<int> tmpStk(stk);
  while (!tmpStk.empty()) {
    gram.symbols[tmpStk.top()].print();
    tmpStk.pop();
  }
  printf("%s", end);
}

void LL1Parser::PrintVec(vector<int> &vec, int start, const char *end = "\n") {
  printf("Input: ");
  for (int x = start; x < vec.size(); x++) gram.symbols[vec[x]].print();
  printf("%s", end);
}

bool LL1Parser::LL1Analysis(Item inp) {
  printf("\nStart nonrecursive analysis.\n");
  stack<int> stk;
  stk.push(DOLLAR_ID);
  stk.push(gram.Start);
  inp.push_back(DOLLAR_ID);
  int nowId, p = 0;
  do {
    nowId = stk.top();
    PrintStack(stk, "\t");
    PrintVec(inp, p, "\t");
    if (gram.isTerminal(nowId) || nowId == DOLLAR_ID) {  // nowId is terminal
      if (nowId == inp[p]) {
        stk.pop();
        p++;
        printf("eliminate ");
        gram.symbols[nowId].print("\n");
      } else {
        printf("Error\n");
        return false;
      }
    } else {  // nowId is nonterminal
      if (LL1Table[make_pair(nowId, inp[p])] >= 0) {
        stk.pop();
        int prodId = LL1Table[make_pair(nowId, inp[p])];
        int itemId = gram.P[prodId].second;
        if (gram.items[itemId][0] != EPSILON_ID) {
          for (int i = gram.items[itemId].size() - 1; i >= 0; i--) {
            stk.push(gram.items[itemId][i]);
          }
        }
        // output production
        printf("Output: ");
        gram.symbols[nowId].print();
        printf("-> ");
        for (int i = 0; i < gram.items[itemId].size(); i++)
          gram.symbols[gram.items[itemId][i]].print();
        printf("\n");
      } else if (LL1Table[make_pair(nowId, inp[p])] == SYNC) {
        // M[nowId, inp[p]] = synch
        printf("Error, pop out ");
        stk.pop();
        gram.symbols[nowId].print("\n");
      } else {  // M[nowId, inp[p]] = error
        printf("Error, skip ");
        gram.symbols[inp[p++]].print("\n");
        return false;
      }
    }
  } while (nowId != DOLLAR_ID);
  printf("Analysis succeed.\n");
  return true;
}