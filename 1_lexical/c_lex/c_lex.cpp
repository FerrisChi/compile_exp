#include <bits/stdc++.h>
using namespace std;
const int HALFBUFSIZE = 1024;
const int OPTABLESIZE = 45;
const int UNARYOPSIZE = 25;
const int BINARYOPSIZE = 20;
const int KEYWORDSIZE = 44;
const int MAXIDLENGTH = 1024;

FILE *infp, *outfp;
char *inputPath, *outputPath, *lsBufp, *leBufp, *rsBufp, *reBufp, *beginp,
    *forwardp;
char buffer[HALFBUFSIZE * 2 + 2], nowC;
int charcCnt, charInLine, lineCnt, keywordCnt, idCnt, numCnt, opCnt, charCnt,
    strCnt, wordCnt, errorCnt;
int state, crossFlag;
int numInt, numK, numE, numF;
double numDoub;
// symbol table
vector<string> identifier, constStr;
vector<double> constDouble;
vector<int> constInt;
vector<long long> constLong;
vector<char> constChar;

int unaryOpNxtState[UNARYOPSIZE] = {8,  8,  8,  8,  8,  18, 18, 3,  9,
                                    9,  9,  9,  11, 15, 17, 10, 10, 10,
                                    10, 10, 10, 10, 10, 10, 10};
char opTable[OPTABLESIZE][5] = {
    "*",  "%",  "!",  "^",  "=",  "+",   "-",   ".",   "<",  ">",  "|",  "&",
    "/",  "\'", "\"", "#",  "(",  ")",   "[",   "]",   "{",  "}",  ";",  ",",
    "~",  "++", "--", "&&", "||", "<<",  ">>",  "==",  "<=", "!=", ">=", "+=",
    "-=", "*=", "/=", "%=", "^=", "&&=", "||=", "<<=", ">>="};

// Standard C keywords
char keywords[KEYWORDSIZE][15] = {
    "auto",       "break",     "case",           "char",
    "const",      "continue",  "default",        "do",
    "double",     "else",      "enum",           "extern",
    "float",      "for",       "goto",           "if",
    "inline",     "int",       "long",           "register",
    "restrict",   "return",    "short",          "signed",
    "sizeof",     "static",    "struct",         "switch",
    "typedef",    "union",     "unsigned",       "void",
    "volatile",   "while",     "_Alignas",       "_Alignof",
    "_Atomic",    "_Bool",     "_Complex",       "_Generic",
    "_Imaginary", "_Noreturn", "_Static_assert", "_Thread_local"};

int argPos(char *str, int argc, char **argv) {
  for (int i = 0; i < argc; ++i) {
    if (!strcmp(str, argv[i])) {
      if (i == argc - 1) {
        printf("Argument missing for %s\n", str);
        exit(1);
      }
      return i;
    }
  }
  return -1;
}

void getArg(int argc, char **argv) {
  int a;
  if ((a = argPos(const_cast<char *>("-input"), argc, argv)) > 0) {
    inputPath = argv[a + 1];
  } else {
    inputPath = const_cast<char *>("test.c");
  }
  if ((a = argPos(const_cast<char *>("-output"), argc, argv)) > 0) {
    outputPath = argv[a + 1];
  } else {
    outputPath = const_cast<char *>("result.txt");
  }
}

// half buffer end with EOF
// valid space: HALFBUFSIZE - 1
void getHalfBuffer(char *buffp) {
  int cnt = fread(buffp, 1, HALFBUFSIZE - 1, infp);
  buffp[cnt] = EOF;
}

void init() {
  infp = fopen(inputPath, "r");
  outfp = fopen(outputPath, "w");
  lsBufp = buffer, leBufp = buffer + HALFBUFSIZE - 1,
  rsBufp = buffer + HALFBUFSIZE, reBufp = buffer + HALFBUFSIZE * 2 - 1;
  forwardp = lsBufp;
  *leBufp = *reBufp = EOF;
  printf("Input file: %s\nOutput file: %s\n", inputPath, outputPath);
  getHalfBuffer(lsBufp);
  fprintf(outfp, "Line 0:\n");
}

void getNextChar() {
  nowC = *forwardp++;
  charcCnt++, charInLine++;
  if (*forwardp == EOF) {
    if (forwardp == leBufp) {
      if (!crossFlag) getHalfBuffer(rsBufp);
      forwardp = rsBufp;
      crossFlag = 0;
    } else if (forwardp == reBufp) {
      if (!crossFlag) getHalfBuffer(lsBufp);
      forwardp = lsBufp;
      crossFlag = 0;
    }
  }
}

void retreatPtr() {
  if (forwardp == lsBufp) {
    forwardp = reBufp - 1;
    crossFlag = 1;
  } else if (forwardp == rsBufp) {
    forwardp = leBufp - 1;
    crossFlag = 1;
  } else {
    forwardp--;
  }
}

// find pointer back x position from *forwardp
char *backPtr(int x) {
  char *p = forwardp;
  for (int i = 1; i <= x; i++) {
    if (p == lsBufp)
      p = reBufp - 1;
    else if (forwardp == rsBufp)
      p = leBufp - 1;
    else
      p = p - 1;
  }
  return p;
}

bool isLetter() {
  return (nowC >= 'a' && nowC <= 'z') || (nowC >= 'A' && nowC <= 'Z') ||
         (nowC == '_');
}

bool isDigit() { return nowC >= '0' && nowC <= '9'; }

bool isWhiteSpace() {
  if (nowC == ' ' || nowC == '\t' || nowC == '\n' || nowC == '\r') return true;
  return false;
}

// return -1 if nowC isn't a operator, otherwise return index of nowC
int isUnaryOperator() {
  for (int i = 0; i < UNARYOPSIZE; i++)
    if (nowC == opTable[i][0]) return unaryOpNxtState[i];
  return -1;
}

// throw error retreat forwardptr and turn to state 0
void throwError(string str) {
  errorCnt++;
  retreatPtr();
  fprintf(outfp, "An error occurred in Line %d:%d: ", lineCnt, charInLine);
  fprintf(outfp, " %s\n", str.c_str());
  state = 0;
}

// return the string between beginp -> forwardp-1
char *getString() {
  char *str = static_cast<char *>(malloc(sizeof(char) * MAXIDLENGTH));
  int len;
  if ((beginp < forwardp && forwardp < leBufp) ||
      (beginp < forwardp && beginp > leBufp)) {
    strncpy(str, beginp, forwardp - beginp);
    len = forwardp - beginp;
  } else if (beginp < leBufp && forwardp >= rsBufp) {
    char tmp[MAXIDLENGTH];
    strncpy(str, beginp, leBufp - beginp);
    len = leBufp - beginp;
    if (forwardp != rsBufp) {
      strncpy(tmp, rsBufp, forwardp - rsBufp);
      strcat(str, tmp);
      len += forwardp - rsBufp;
    }
  } else if (beginp >= rsBufp && forwardp < leBufp) {
    char tmp[MAXIDLENGTH];
    strncpy(str, beginp, reBufp - beginp);
    len = reBufp - beginp;
    if (forwardp != lsBufp) {
      strncpy(tmp, lsBufp, forwardp - lsBufp);
      strcat(str, tmp);
      len += forwardp - lsBufp;
    }
  } else
    throwError("Identifier too long. (Max size: 1024)");
  str[len] = '\0';
  return str;
}

// insert identifier into table
// *beginp  -> *forwardp
void insertId() {
  char *idf = getString();
  int flag = 0;
  for (int i = 0; i < KEYWORDSIZE; i++)
    if (strcmp(idf, keywords[i]) == 0) {  // find keywords
      flag = 1;
      fprintf(outfp, "%s keyword %s\n", idf, idf);
      keywordCnt++;
      break;
    }
  std::string tmp = idf;
  for (int i = 0; i < identifier.size(); i++)
    if (identifier[i] == tmp) {
      flag = 1;
      idCnt++;
      fprintf(outfp, "%s id %d\n", idf, i);
      break;
    }
  if (!flag) {
    idCnt++;
    identifier.push_back(tmp);
    fprintf(outfp, "%s id %d\n", idf, identifier.size() - 1);
  }
  free(idf);
  state = 0;
}

double qkpow(double x, int k) {
  double ans = 1;
  while (k) {
    if (k & 1) ans *= x;
    k >>= 1;
    x *= x;
  }
  return ans;
}

// insert identifier into table and turn to state 0
void insertConstant() {
  double tmp = (numInt + numDoub) * qkpow(10, numE * numF);
  int flag = 0;
  numCnt++;
  state = 0;
  if (numK) {
    for (int i = 0; i < constDouble.size(); i++)
      if (constDouble[i] == tmp) {
        flag = 1;
        fprintf(outfp, "%lf double %d\n", tmp, i);
        break;
      }
    if (!flag) {
      constDouble.push_back(tmp);
      fprintf(outfp, "%lf double %d\n", tmp, constDouble.size() - 1);
    }
  } else if (tmp > INT_MAX) {  // long long
    for (int i = 0; i < constLong.size(); i++)
      if (constLong[i] == tmp) {
        flag = 1;
        fprintf(outfp, "%lld long long %d\n", (long long)tmp, i);
        break;
      }
    if (!flag) {
      constLong.push_back(tmp);
      fprintf(outfp, "%lld long long %d\n", (long long)tmp,
              constLong.size() - 1);
    }
  } else {  // int
    for (int i = 0; i < constInt.size(); i++)
      if (constInt[i] == tmp) {
        flag = 1;
        fprintf(outfp, "%d int %d\n", (int)tmp, i);
        break;
      }
    if (!flag) {
      constInt.push_back(tmp);
      fprintf(outfp, "%d int %d\n", (int)tmp, constInt.size() - 1);
    }
  }
}

// insert operator(delimiter) into table and turn to state 0
void insertOp() {
  char *op = getString();
  for (int i = 0; i < OPTABLESIZE; i++)
    if (strcmp(op, opTable[i]) == 0) {
      fprintf(outfp, "%s op %s\n", op, op);
      break;
    }
  free(op);
  opCnt++;
  state = 0;
}

// insert string into table and turn to state 0
void insertStr() {
  char *str = getString();
  std::string tmpstr = str;
  int flag = 0;
  for (int i = 0; i < constStr.size(); i++)
    if (constStr[i] == tmpstr) {
      fprintf(outfp, "%s const_char[%d] %d\n", str, (int)strlen(str), i);
      flag = 1;
      break;
    }
  if (!flag) {
    constStr.push_back(tmpstr);
    fprintf(outfp, "%s const_char[%d] %d\n", str, (int)strlen(str),
            constStr.size() - 1);
  }
  free(str);
  strCnt++;
  state = 0;
}

// insert char into char table and turn to state 0
void insertChar() {
  char ch = *(backPtr(1));
  int flag = 0;
  for (int i = 0; i < constChar.size(); i++)
    if (constChar[i] == ch) {
      fprintf(outfp, "%c Char %d\n", ch, i);
      flag = 1;
      break;
    }
  if (!flag) {
    constChar.push_back(ch);
    fprintf(outfp, "%c Char %d\n", ch, constChar.size() - 1);
  }
  charCnt++;
  state = 0;
}

void lexAnalysis() {
  int flag;
  do {
    getNextChar();
    // if (nowC == EOF) {  // touch EOF!
    //   printf("%d %d %d %d %d %d\n", beginp, forwardp, state, buffer, leBufp,
    //          reBufp);
    // }
    switch (state) {
      case 0:
        if (isWhiteSpace()) {
          if (nowC == '\n' || nowC == '\r') {
            fprintf(outfp, "\nLine %d:\n", ++lineCnt);
            charInLine = 0;
          }
          continue;
        }
        beginp = backPtr(1);
        if (isLetter()) {
          state = 1;
        } else if (isDigit()) {
          numInt = nowC - '0', numDoub = numK = numE = 0, numF = 1;
          state = 2;
        } else if (nowC == '.') {
          numInt = 0, numDoub = numK = numE = 0, numF = 1;
          state = 3;
        } else if ((flag = isUnaryOperator()) != -1) {
          state = flag;
        } else if (nowC != EOF) {
          throwError("Unkown sign.");
          getNextChar();  // need go forward
        }

        break;

      case 1:
        if (isWhiteSpace() || (!isLetter() && !isDigit())) {
          retreatPtr();
          insertId();
        }
        break;

      case 2:
        if (isDigit()) {
          numInt = numInt * 10 + nowC - '0';
        } else if (nowC == '.') {
          state = 4;
        } else if (nowC == 'E' || nowC == 'e') {
          state = 5;
        } else if (isLetter()) {
          throwError("Illegal number.");
        } else {
          retreatPtr();
          insertConstant();
        }
        break;

      case 3:
        if (isDigit()) {
          numDoub += qkpow(0.1, ++numK) * (nowC - '0');
          state = 4;
        } else {
          retreatPtr();
          insertOp();
        }
        break;

      case 4:
        if (isDigit()) {
          numDoub += qkpow(0.1, ++numK) * (nowC - '0');
        } else if (nowC == 'E' || nowC == 'e') {
          state = 5;
        } else {
          retreatPtr();
          insertConstant();
        }
        break;

      case 5:
        if (isDigit()) {
          numE = numE * 10 + nowC - '0';
          state = 7;
        } else if (nowC == '+' || nowC == '-') {
          if (nowC == '-') numF = -1;
          state = 6;
        } else {
          retreatPtr();
          insertConstant();
        }
        break;
      case 6:
        if (isDigit()) {
          numE = numE * 10 + nowC - '0';
          state = 7;
        } else
          throwError("Illegal number.");
        break;
      case 7:
        if (isDigit()) {
          numE = numE * 10 + nowC - '0';
        } else {
          retreatPtr();
          insertConstant();
        }
        break;

      case 8:
        if (nowC != '=') retreatPtr();
        insertOp();
        break;
      case 9:
        if (nowC == (*backPtr(2))) {
          state = 19;
        } else if (nowC == '=') {
          insertOp();
        } else {
          retreatPtr();
          insertOp();
        }
        break;
      case 10:
        retreatPtr();
        insertOp();
        break;
      case 11:
        switch (nowC) {
          case '/':
            state = 12;
            break;
          case '*':
            state = 13;
            break;
          case '=':
            insertOp();
            break;
          default:
            retreatPtr();
            insertOp();
            break;
        }
        break;
      case 12:
        if (nowC == '\n' || nowC == '\r') {
          fprintf(outfp, "\nLine %d:\n", ++lineCnt);
          charInLine = 0;
          state = 0;
        }
        break;
      case 13:
        if (nowC == '*') state = 14;
        break;
      case 14:
        if (nowC == '/')
          state = 0;
        else
          state = 13;
        break;
      case 15:
        state = 16;
        break;
      case 16:
        if (nowC == '\'')
          insertChar();
        else
          throwError("Too many characters entered.");
        break;
      case 17:
        if (nowC == '\"')
          insertStr();
        else if (nowC == '\n' || nowC == '\r')
          throwError("Lack of \"");
        break;
      case 18:
        if (nowC != (*backPtr(2)) && nowC != '=') retreatPtr();
        insertOp();
        break;
      case 19:
        if (nowC != '=') retreatPtr();
        insertOp();
        break;
      default:
        break;
    }
  } while (nowC != EOF);
  wordCnt = keywordCnt + idCnt + numCnt + opCnt + strCnt + charCnt;
  fprintf(outfp,
          "\nTotal:\n\t%d line\n\t%d tokens\n\t%d keywords\n\t%d "
          "identifiers\n\t%d "
          "constants\n\t%d operator\n\t%d string\n\t%d char\nTotal error: %d",
          lineCnt, wordCnt, keywordCnt, idCnt, numCnt, opCnt, strCnt, charCnt,
          errorCnt);
}

int main(int argc, char **argv) {
  getArg(argc, argv);
  init();
  lexAnalysis();
  fclose(infp);
  fclose(outfp);
  printf("Lexical analysis succeed. Result saved to %s", outputPath);
  return 0;
}