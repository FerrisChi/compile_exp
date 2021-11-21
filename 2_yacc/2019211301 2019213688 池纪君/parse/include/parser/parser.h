#pragma once
#include <map>
#include <stack>
#include <unordered_set>
#include <vector>

#include "gram.h"
#include "utils.h"

class Parser {
 public:
  Grammar gram;
  Parser(Grammar gram) : gram(gram){};
};