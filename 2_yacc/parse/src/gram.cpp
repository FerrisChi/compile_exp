#include "gram.h"

Grammar::Grammar() {
  this->Init(string("../data/grammar3.in"));
  isCalcFirst = false;
  isCalcFollow = false;
}

Grammar::~Grammar() {}

void Grammar::Init(std::string fpgram) {
  FILE *fgram = fopen(fpgram.c_str(), "r");
  int state = 0, nowSymbolId;
  char s[1005];
  symbols.push_back(Symbol("$", 1));
  T.push_back(0);
  symbols.push_back(Symbol("ε", 1));
  T.push_back(1);
  while (1) {
    if (fscanf(fgram, "%s", s) == EOF) break;
    // printf("%s\n", s);
    if (strcmp(s, "Terminal:") == 0) {
      state = 1;
    } else if (strcmp(s, "Nonterminal:") == 0) {
      state = 2;
    } else if (strcmp(s, "Production:") == 0) {
      state = 3;
    } else if (strcmp(s, "Start:") == 0) {
      state = 6;
    } else {
      switch (state) {
        case 1:  // Terminal
        {
          symbols.push_back(Symbol(s, 1));
          T.push_back(symbols.size() - 1);
          break;
        }
        case 2:  // Nonterminal
        {
          symbols.push_back(Symbol(s, 0));
          N.push_back(symbols.size() - 1);
          break;
        }
        case 3:  // Production
        {
          for (int i = 0; i < N.size(); i++)
            if (strcmp(s, symbols[N[i]].name.c_str()) == 0) {
              nowSymbolId = N[i];
              break;
            }
          state = 4;
          break;
        }
        case 4: {
          state = 5;
          break;
        }
        case 5: {
          int len = strlen(s);
          std::string tmp("");
          vector<int> nowItem;
          for (int i = 0; i < len; i++) {
            tmp += s[i];
            // **CAUSION: initially doesn't support Nonterminal characters with
            // same suffix.(e.g. {A, A'} ). Special check if X and X' exist.
            if (i < len - 1 && s[i + 1] == '\'') {
              string ttmp = tmp + "'";
              int flag = 0;
              for (int p = 0; p < symbols.size(); p++)
                if (ttmp == symbols[p].name) {
                  nowItem.push_back(p);
                  tmp = "";
                  flag = 1;
                  break;
                }
              if (flag) continue;
            }
            for (int p = 0; p < symbols.size(); p++)
              if (tmp == symbols[p].name) {
                nowItem.push_back(p);
                tmp = "";
                break;
              }
          }
          items.push_back(nowItem);
          P.push_back(make_pair(nowSymbolId, items.size() - 1));
          state = 3;
          break;
        }
        case 6: {
          for (int i = 0; i < N.size(); i++)
            if (strcmp(symbols[N[i]].name.c_str(), s) == 0) {
              Start = N[i];
              break;
            }
          break;
        }
        default:
          break;
      }
    }
  }
}

bool Grammar::isTerminal(int id) {
  return symbols[id].type == Symbol::Terminal;
}

bool Grammar::isNonNullTerminal(int id) {
  return symbols[id].type == Symbol::Terminal && symbols[id].name != "ε";
}

bool Grammar::isNonterminal(int id) {
  return symbols[id].type == Symbol::NonTerminal;
}

bool Grammar::isInFirst(int id, vector<int> item) {
  vector<int> f;
  GetFirst(item, f);
  for (int i = 0; i < f.size(); i++)
    if (id == f[i]) return true;
  return false;
}

bool Grammar::isInFollow(int termId, int nontermId) {
  for (int i = 0; i < follow[nontermId].size(); i++)
    if (termId == follow[nontermId][i]) return true;
  return false;
}

void Grammar::Debug() {
  printf("Symbols:\n");
  for (int i = 0; i < symbols.size(); i++) {
    printf("\t id: %d %ld, %s %s", i, symbols[i].name.size(),
           symbols[i].name.c_str(),
           symbols[i].type == Symbol::Terminal ? "Terminal" : "Nonterminal");
    if (Start == i)
      printf(" START\n");
    else
      printf("\n");
  }
  printf("Productions:\n");
  for (int i = 0; i < P.size(); i++) {
    printf("\t id: %d, %s ->", i, symbols[P[i].first].name.c_str());
    for (int j = 0; j < items[P[i].second].size(); j++)
      printf("%s", symbols[items[P[i].second][j]].name.c_str());
    printf("\n");
  }
}

void Grammar::Print() {
  printf("Terminal character: ");
  for (int i = 0; i < T.size(); i++) {
    printf("%s ", symbols[T[i]].name.c_str());
  }
  printf("\nNonterminal character: ");
  for (int i = 0; i < N.size(); i++) {
    printf("%s ", symbols[N[i]].name.c_str());
  }
  printf("\nProduction:\n");
  for (int i = 0; i < P.size(); i++) {
    printf("%s -> ", symbols[P[i].first].name.c_str());
    for (int j = 0; j < items[P[i].second].size(); j++) {
      printf("%s", symbols[items[P[i].second][j]].name.c_str());
    }
    printf("\n");
  }
  printf("Start caracter: %s\n", symbols[Start].name.c_str());
}

void Grammar::CalcFirst() {
  if (isCalcFirst) return;
  for (int i = 0; i < symbols.size(); i++) first.push_back({});
  int flag;
  int vis[symbols.size()][symbols.size()];
  memset(vis, 0, sizeof(vis));
  while (1) {
    flag = 0;
    for (int i = 0; i < P.size(); i++) {  // i-th production
      int firstId = items[P[i].second][0], nowId = P[i].first;
      if (isTerminal(firstId)) {  // X->a... or X->ε
        if (vis[nowId][firstId]) continue;
        // printf("add %s to first{%s}\n", symbols[firstId].name.c_str(),
        //        symbols[nowId].name.c_str());
        first[nowId].push_back(firstId);
        vis[nowId][firstId] = 1;
        flag = 1;
      } else {  // X->Y0Y1...
        for (int j = 0; j <= items[i].size(); j++) {
          int flagEmpty = 1;
          for (int k = 0; k < j; k++)
            if (!isInFirst(EPSILON_ID, vector<int>(1, items[P[i].second][k]))) {
              flagEmpty = 0;
              break;
            }
          if (flagEmpty) {  // ε in first(Y0), first(Y1), ..., first(Yj-1)
            if (j == items[i].size()) {  // Y0, Y1, ..., Yn => ε
              if (isInFirst(EPSILON_ID, vector<int>(1, nowId))) continue;
              vis[nowId][EPSILON_ID] = 1;
              first[nowId].push_back(EPSILON_ID);
              flag = 1;
              // printf("add %s to first{%s}\n", symbols[firstId].name.c_str(),
              //        symbols[nowId].name.c_str());
              continue;
            }
            // add first{Yj} to first{X}
            for (int k = 0; k < first[items[P[i].second][j]].size(); k++) {
              firstId = first[items[P[i].second][j]][k];
              if (vis[nowId][firstId] || firstId == EPSILON_ID) continue;
              vis[nowId][firstId] = 1;
              first[nowId].push_back(firstId);
              // printf("add %s to first{%s}\n", symbols[firstId].name.c_str(),
              //        symbols[nowId].name.c_str());
              flag = 1;
            }
          }
        }
      }
    }
    if (!flag) break;
  }
  printf("First:\n");
  for (int i = 0; i < symbols.size(); i++)
    if (symbols[i].type == Symbol::NonTerminal) {
      printf("\t%s: ", symbols[i].name.c_str());
      sort(first[i].begin(), first[i].end());
      for (int k = 0; k < first[i].size(); k++) {
        printf("%s ", symbols[first[i][k]].name.c_str());
      }
      printf("\n");
    }
  isCalcFirst = true;
}

void Grammar::GetFirst(vector<int> item, vector<int> &f) {
  f.clear();
  if (!items.size()) return;
  int vis[symbols.size()];
  memset(vis, 0, sizeof(vis));
  for (int i = 0; i < item.size(); i++) {
    int nowId = item[i];
    // printf("%d\n", nowId);
    if (isTerminal(nowId)) {  // aY1Y2...
      if (!vis[nowId]) f.push_back(nowId);
      return;
    } else {  // Y1Y2...
      int epsilonFlag = 0, firId;
      for (int j = 0; j < first[nowId].size(); j++) {
        firId = first[nowId][j];
        if (firId == EPSILON_ID)
          epsilonFlag = 1;
        else {
          if (vis[firId]) continue;
          f.push_back(firId);
          vis[firId] = 1;
        }
      }
      if (!epsilonFlag) return;
    }
  }
  f.push_back(EPSILON_ID);
  return;
}

void Grammar::CalcFollow() {
  if (isCalcFollow) return;
  for (int i = 0; i < symbols.size(); i++) follow.push_back({});
  follow[Start].push_back(DOLLAR_ID);
  int flag, vis[symbols.size()][symbols.size()];
  memset(vis, 0, sizeof(vis));
  vector<int> prod, nowf;
  while (1) {
    flag = 0;

    for (int i = 0; i < P.size(); i++) {  // i-th production
      int nowId = P[i].first, termId;
      prod = items[P[i].second];
      for (int j = 0; j < prod.size(); j++) {
        termId = prod[j];
        if (!isNonterminal(termId)) continue;
        GetFirst(vector<int>(prod.begin() + j + 1, prod.end()), nowf);
        // printf("%s ", symbols[nowId].name.c_str());
        // for (auto x : vector<int>(prod.begin() + j + 1, prod.end()))
        //   printf("%d ", x);
        // printf("\n");
        // printf("%s ", symbols[termId].name.c_str());
        // for (auto x : nowf) printf("%d ", x);
        // printf("\n");

        // A -> αBβ: add nowf[k] to follow(termId)
        for (int k = 0; k < nowf.size(); k++) {
          if (nowf[k] == EPSILON_ID || vis[termId][nowf[k]]) continue;
          vis[termId][nowf[k]] = 1;
          // printf("add %s to follow %s\n", symbols[nowf[k]].name.c_str(),
          //        symbols[termId].name.c_str());
          follow[termId].push_back(nowf[k]);
          flag = 1;
        }

        // A ->αB or A->αBβ, β=>ε: add follow(nowId) to follow(termId)
        if (nowf.back() == EPSILON_ID || j == prod.size() - 1) {
          for (int k = 0; k < follow[nowId].size(); k++) {
            if (vis[termId][follow[nowId][k]]) continue;
            vis[termId][follow[nowId][k]] = 1;
            follow[termId].push_back(follow[nowId][k]);
            // printf("add %s to follow %s\n", symbols[nowf[k]].name.c_str(),
            //        symbols[termId].name.c_str());
            flag = 1;
          }
        }
      }
    }
    if (!flag) break;
  }
  printf("Follow:\n");
  for (int i = 0; i < symbols.size(); i++)
    if (symbols[i].type == Symbol::NonTerminal) {
      printf("\t%s: ", symbols[i].name.c_str());
      sort(follow[i].begin(), follow[i].end());
      for (int k = 0; k < follow[i].size(); k++) {
        printf("%s ", symbols[follow[i][k]].name.c_str());
      }
      printf("\n");
    }
  isCalcFollow = true;
}
