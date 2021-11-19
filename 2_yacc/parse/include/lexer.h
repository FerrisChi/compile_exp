#pragma once
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "gram.h"

const int HALFBUFSIZE = 1024;
const int OPTABLESIZE = 7;
const int UNARYOPSIZE = 25;
const int BINARYOPSIZE = 20;
const int KEYWORDSIZE = 44;
const int MAXIDLENGTH = 1024;

class Lexer {
  typedef vector<int> Item;

 private:
  FILE *infp;
  char *lsBufp, *leBufp, *rsBufp, *reBufp, *beginp, *forwardp;
  char buffer[HALFBUFSIZE * 2 + 2], nowC;
  int state, crossFlag;
  int numInt, numK, numE, numF;
  int charcCnt, charInLine, lineCnt, keywordCnt, idCnt, numCnt, opCnt, charCnt,
      strCnt, wordCnt, errorCnt;
  double numDoub;

 private:
  // int unaryOpNxtState[UNARYOPSIZE] = {8,  8,  8,  8,  8,  18, 18, 3,  9,
  //                                     9,  9,  9,  11, 15, 17, 10, 10, 10,
  //                                     10, 10, 10, 10, 10, 10, 10};
  // char opTable[OPTABLESIZE][5] = {
  //     "*",  "%",  "!",  "^",  "=",  "+",   "-",   ".",   "<",  ">",  "|",
  //     "&",
  //     "/",  "\'", "\"", "#",  "(",  ")",   "[",   "]",   "{",  "}",  ";",
  //     ",",
  //     "~",  "++", "--", "&&", "||", "<<",  ">>",  "==",  "<=", "!=", ">=",
  //     "+=",
  //     "-=", "*=", "/=", "%=", "^=", "&&=", "||=", "<<=", ">>="};
  char opTable[OPTABLESIZE][5] = {"+", "-", "*", "/", "(", ")", "."};
  char unaryOpNxtState[UNARYOPSIZE] = {8, 8, 8, 8, 8, 8, 3};

 private:
  void GetHalfBuffer(char *buffp);
  void GetNextChar();
  char *GetString();
  void insertId();
  void insertConstant();
  void insertOp();
  void retreatPtr();
  char *backPtr(int x);
  bool isLetter();
  bool isDigit();
  bool isWhiteSpace();
  int isUnaryOperator();
  void ThrowError(string str);

 public:
  Item outputItem;
  Grammar gram;
  Lexer(){};
  Lexer(Grammar gm);
  Item Analysis(const char *pfile);
  void Init(const char *infp);
  ~Lexer();
};