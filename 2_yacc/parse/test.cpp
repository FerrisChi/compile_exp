#include <bits/stdc++.h>
using namespace std;
void f(vector<int> v) {
  for (auto x : v) printf("%d ", x);
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
  vector<int> v({1, 2, 3, 4});
  f(v.begin()+2)
  return 0;
}