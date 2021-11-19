#include "parser/recparser.h"

bool RecurParser::Analysis(Item inp) {
  input = inp;
  pos = 0;
  fp();
  ProcE();
  printf("Accept!\n");
  return true;
}

void RecurParser::ProcF() {
  if (nowS == Symbol("(", Symbol::Terminal)) {
    fp();
    ProcE();
    if (nowS == Symbol(")", Symbol::Terminal))
      fp();
    else
      Error();
  } else if (nowS == Symbol("num", Symbol::Terminal))
    fp();
  else
    Error();
}

void RecurParser::ProcT() {
  ProcF();
  if (nowS == Symbol("*", Symbol::Terminal) ||
      nowS == Symbol("/", Symbol::Terminal)) {
    fp();
    ProcT();
  }
}

void RecurParser::ProcE() {
  ProcT();
  if (nowS == Symbol("+", Symbol::Terminal) ||
      nowS == Symbol("-", Symbol::Terminal)) {
    fp();
    ProcE();
  }
}