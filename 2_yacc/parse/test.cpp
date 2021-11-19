#include <bits/stdc++.h>
using namespace std;
void f(vector<int> v) {
  for (auto x : v) printf("%d ", x);
  printf("\n");
}

template <typename T1, typename T2>
void PrintStkPr(vector<pair<T1, T2>>& v) {
  for (auto pr : v) {
    std::cout << "<" << pr.first <<", "<< pr.second << "> ";
  }
  printf("\n");
}

int main() {
  // vector<int>v({1,2,3,4});
  // vector<int>v1(v.begin()+4, v.end());
  // for(auto x:v1){
  //     printf("%d\n", x);
  // }
  // printf("\n%d\n", v.back());
  // char s[10]="asdf";
  // printf("\n1111%c%s\n",  '\0',s);
  // char c = "ϵ";
  // int x = 2+(c!='\0');
  // vector<int> v({1, 0, 3, 4});
  // v.erase(v.begin()+1);
  // v.insert(v.begin()+2,0);
  // f(v);
  // string s("11");
  // char end[10] = "\0";
  // int l = s.length() + !(strcmp(end, "\0") == 0);
  // printf("%d", l);
  vector<pair<int,double>>v;
  v.push_back(make_pair(1,1.1));
  v.push_back(make_pair(1,2.1));
  v.push_back(make_pair(1,3.1));
  v.push_back(make_pair(1,4.1));
  v.push_back(make_pair(1,5.1));
  PrintStkPr(v);
  return 0;
}