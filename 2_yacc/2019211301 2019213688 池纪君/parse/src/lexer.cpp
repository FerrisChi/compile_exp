#include "lexer.h"

Lexer::Lexer(Grammar gm) : gram(gm) {}

Lexer::~Lexer() {}

// half buffer end with EOF
// valid space: HALFBUFSIZE - 1
void Lexer::GetHalfBuffer(char *buffp) {
  int cnt = fread(buffp, 1, HALFBUFSIZE - 1, infp);
  buffp[cnt] = EOF;
}

void Lexer::GetNextChar() {
  nowC = *forwardp++;
  //   charcCnt++, charInLine++;
  if (*forwardp == EOF) {
    if (forwardp == leBufp) {
      if (!crossFlag) GetHalfBuffer(rsBufp);
      forwardp = rsBufp;
      crossFlag = 0;
    } else if (forwardp == reBufp) {
      if (!crossFlag) GetHalfBuffer(lsBufp);
      forwardp = lsBufp;
      crossFlag = 0;
    }
  }
}

void Lexer::Init(const char *pfile) {
  infp = fopen(pfile, "r");
  lsBufp = buffer, leBufp = buffer + HALFBUFSIZE - 1,
  rsBufp = buffer + HALFBUFSIZE, reBufp = buffer + HALFBUFSIZE * 2 - 1;
  forwardp = lsBufp;
  *leBufp = *reBufp = EOF;
  printf("Input file: %s\n", pfile);
  GetHalfBuffer(lsBufp);
  state = 0;
}

void Lexer::retreatPtr() {
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
char *Lexer::backPtr(int x) {
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

bool Lexer::isLetter() {
  return (nowC >= 'a' && nowC <= 'z') || (nowC >= 'A' && nowC <= 'Z') ||
         (nowC == '_');
}

bool Lexer::isDigit() { return nowC >= '0' && nowC <= '9'; }

bool Lexer::isWhiteSpace() {
  if (nowC == ' ' || nowC == '\t' || nowC == '\n' || nowC == '\r') return true;
  return false;
}

// return -1 if nowC isn't a operator, otherwise return index of nowC
int Lexer::isUnaryOperator() {
  for (int i = 0; i < UNARYOPSIZE; i++)
    if (nowC == opTable[i][0]) return 8;
  return -1;
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

char *Lexer::GetString() {
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
    ThrowError("Identifier too long. (Max size: 1024)");
  str[len] = '\0';
  return str;
}

// insert identifier into table
// *beginp  -> *forwardp
void Lexer::insertId() {
  // printf("get id\n");
  for (auto x : gram.T) {
    if (gram.symbols[x].name == "id") {
      outputItem.push_back(x);
      break;
    }
  }
  state = 0;
}

// insert identifier into table and turn to state 0
void Lexer::insertConstant() {
  // printf("get constant\n");
  for (auto x : gram.T) {
    if (gram.symbols[x].name == "num") {
      outputItem.push_back(x);
      break;
    }
  }
  state = 0;
}

// insert operator(delimiter) into table and turn to state 0
void Lexer::insertOp() {
  char *op = GetString();
  for (int i = 0; i < OPTABLESIZE; i++)
    if (strcmp(op, opTable[i]) == 0) {
      for (auto x : gram.T)
        if (string(op) == gram.symbols[x].name) {
          outputItem.push_back(x);
          break;
        }
      break;
    }
  free(op);
  state = 0;
}

// throw error retreat forwardptr and turn to state 0
void Lexer::ThrowError(string str) {
  printf("Error\n");
  retreatPtr();
  printf("An error occurred in Line %d:%d: ", lineCnt, charInLine);
  printf(" %s\n", str.c_str());
  state = 0;
}

vector<int> Lexer::Analysis(const char *pfile = "data/expression.in") {
  Init(pfile);
  int flag;
  do {
    GetNextChar();
    // if (nowC == EOF) {  // touch EOF!
    //   printf("%d %d %d %d %d %d\n", beginp, forwardp, state, buffer,
    //   leBufp,
    //          reBufp);
    // }
    // printf("state:%d\n", state);
    switch (state) {
      case 0:
        if (isWhiteSpace()) {
          if (nowC == '\n' || nowC == '\r') {
            printf("\nLine %d:\n", ++lineCnt);
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
        } else if ((flag = isUnaryOperator()) != -1) {
          state = flag;
        } else if (nowC != EOF) {
          ThrowError("Unkown sign.");
          GetNextChar();  // need go forward
        }
        break;

      case 1:
        if (isWhiteSpace() || (!isLetter() && !isDigit())) {
          retreatPtr();
          insertId();
        }
        break;

      // numbers
      case 2:
        if (isDigit()) {
          numInt = numInt * 10 + nowC - '0';
        } else if (nowC == '.') {
          state = 4;
        } else if (nowC == 'E' || nowC == 'e') {
          state = 5;
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
          ThrowError("Illegal number.");
        break;
      case 7:
        if (isDigit()) {
          numE = numE * 10 + nowC - '0';
        } else {
          retreatPtr();
          insertConstant();
        }
        break;

      case 8:  // unary op
        retreatPtr();
        insertOp();
        break;
      default:
        break;
    }
  } while (nowC != EOF);
  //   wordCnt = keywordCnt + idCnt + numCnt + opCnt + strCnt + charCnt;
  //   printf(
  //       "\nTotal:\n\t%d line\n\t%d tokens\n\t%d keywords\n\t%d "
  //       "identifiers\n\t%d "
  //       "constants\n\t%d operator\n\t%d string\n\t%d char\nTotal error: %d",
  //       lineCnt, wordCnt, keywordCnt, idCnt, numCnt, opCnt, strCnt, charCnt,
  //       errorCnt);
  return outputItem;
}