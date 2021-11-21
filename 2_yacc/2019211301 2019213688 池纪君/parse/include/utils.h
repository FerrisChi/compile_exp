#pragma once
#include <cstdio>
#include <iostream>

inline void PrintBlank(int cnt) {
  for (int i = 0; i < cnt; i++) printf(" ");
}

inline void PrintTab(int cnt) {
  for (int i = 0; i < cnt; i++) printf("\t");
}

template <typename T1, typename T2>
void PrintPrV(vector<pair<T1, T2>> v, const char *c = "\n") {
  std::cout << "{";
  for (auto pr : v) {
    std::cout << "<" << pr.first << ", " << pr.second << "> ";
  }
  printf("}%s", c);
}