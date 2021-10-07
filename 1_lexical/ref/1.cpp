#include <stdio.h>
#include <string.h>
#define MAX 22     /*分析}表的最大容量*/
#define RES_MAX 10 /*关键字的最大长度*/
#define MAXBUF 255 /*缓冲区的大小*/

char ch = ' '; /*存放读入当前的输入字符*/
int Line_NO;   /*纪录行号*/

struct keywords /*关键字*/
{
  char lexptr[MAXBUF];
  int token;
};
struct keywords symtable[MAX];
char str[MAX][10] = {"int",   "char",   "float",   "main",  "double", "case",
                     "for",   "if",     "auto",    "else",  "do",     "while",
                     "void",  "static", "return",  "break", "struct", "const",
                     "union", "switch", "typedef", "enum"};

/*对关键字表进行初始化，div,mod,and,or也作为关键字处理*/
/*最小的token是program：3，最大的token是or：24*/
void init() {
  int j;
  for (j = 0; j < MAX; j++) {
    strcpy(symtable[j].lexptr, str[j]);
    symtable[j].token = j + 3;
  }
}

/***************对关键字进行搜索**************/
int Iskeyword(char *is_res) {
  int i;
  for (i = 0; i < MAX; i++) {
    if ((strcmp(symtable[i].lexptr, is_res)) == 0) break;
  }
  if (i < MAX)
    return symtable[i].token;
  else
    return 0;
}

/*****************判断是否为字母*****************/
int IsLetter(char c) {
  if (((c <= 'z') && (c >= 'a')) || ((c <= 'Z') && (c >= 'A')))
    return 1;
  else
    return 0;
}

/*************判断是否为数字**************/
int IsDigit(char c) {
  if (c >= '0' && c <= '9')
    return 1;
  else
    return 0;
}

/***************分析程序**************/
void analyse(FILE *fpin, FILE *fpout) {
  /* 输入缓冲区，存放一个单词符号 */
  char arr[MAXBUF];
  int j = 0;
  while ((ch = fgetc(fpin)) != EOF) {
    /*碰到空格、tab则跳过*/
    if (ch == ' ' || ch == '\t') {
    } else if (ch == '\n') {
      Line_NO++;
    }
    /*********************字符串的处理*************************/
    else if (IsLetter(ch)) {
      while (IsLetter(ch) | IsDigit(ch) | ch == '_') {
        if ((ch <= 'Z') && (ch >= 'A')) ch = ch + 32; /*忽略大小写*/
        arr[j] = ch;
        j++;
        ch = fgetc(fpin);
      }
      /*输入指针回退一个字符*/
      fseek(fpin, -1L, SEEK_CUR);
      arr[j] = '\0';
      j = 0;
      if (Iskeyword(arr)) { /*如果是关键字*/
        fprintf(fpout, "%s\t\t%d\t\t关键字\n", arr, Iskeyword(arr));
      } else
        fprintf(fpout, "%s\t\t%d\t\t标识符\n", arr, 1); /*普通标识符*/
      /*************************数字的处理****************************/
    } else if (IsDigit(ch)) {
      int s = 0;
      while (IsDigit(ch) | IsLetter(ch)) {
        if (IsLetter(ch)) {
          arr[j] = ch;
          j++;
          ch = fgetc(fpin);
          s = 1;
        } else if (IsDigit(ch)) {
          arr[j] = ch;
          j++;
          ch = fgetc(fpin);
        }
      }
      fseek(fpin, -1L, SEEK_CUR);
      arr[j] = '\0';
      j = 0;
      if (s == 0)
        fprintf(fpout, "%s\t\t%d\t\t无符号整数\n", arr, 2);
      else if (s == 1)
        fprintf(fpout, "%s\t\t%d\t\t错误\n", arr, 3);
    } else
      switch (ch) {
        case '+':
          fprintf(fpout, "%s\t\t%d\t\t运算符\n", "+", 41);
          break;
        case '-':
          fprintf(fpout, "%s\t\t%d\t\t运算符\n", "-", 42);
          break;
        case '*':
          fprintf(fpout, "%s\t\t%d\t\t运算符\n", "*", 43);
          break;
        case '(':
          fprintf(fpout, "%s\t\t%d\t\t分界符\n", "(", 25);
          break;
        case ')':
          fprintf(fpout, "%s\t\t%d\t\t分界符\n", ")", 26);
          break;
        case '[':
          fprintf(fpout, "%s\t\t%d\t\t分界符\n", "[", 27);
          break;
        case ']':
          fprintf(fpout, "%s\t\t%d\t\t分界符\n", "]", 28);
          break;
        case ';':
          fprintf(fpout, "%s\t\t%d\t\t分界符\n", ";", 29);
          break;
        case '=':
          fprintf(fpout, "%s\t\t%d\t\t运算符\n", "=", 45);
          break;
        case '.':
          fprintf(fpout, "%s\t\t%d\t\t分界符\n", ".", 30);
          break;
        case ',':
          fprintf(fpout, "%s\t\t%d\t\t分界符\n", ",", 31);
          break;
        case ':':
          fprintf(fpout, "%s\t\t%d\t\t分界符\n", ":", 32);
          break;
        case '{':
          fprintf(fpout, "%s\t\t%d\t\t分界符\n", "{", 33);
          break;
        case '}':
          fprintf(fpout, "%s\t\t%d\t\t分界符\n", "}", 34);
          break;
        case '>': {
          ch = fgetc(fpin);
          if (ch == '=')
            fprintf(fpout, "%s\t\t%d\t\t运算符\n", ">=", 46);
          else {
            fprintf(fpout, "%s\t\t%d\t\t运算符\n", ">", 47);
            fseek(fpin, -1L, SEEK_CUR);
          }
        } break;

        case '<': {
          ch = fgetc(fpin);
          if (ch == '=')
            fprintf(fpout, "%s\t\t%d\t\t运算符\n", "<=", 48);
          else if (ch == '>')
            fprintf(fpout, "%s\t\t%d\n", "<>", 50);
          else {
            fprintf(fpout, "%s\t\t%d\t\t运算符\n", "<", 49);
            fseek(fpin, -1L, SEEK_CUR);
          }
        } break;
          /***************出现在/
           * /之间的全部作为注释部分处理*******************/
        case '/': {
          ch = fgetc(fpin);
          if (ch == '*') {
            while (ch != '/' && ch != EOF) {
              ch = fgetc(fpin);
            }
            if (ch == EOF) fprintf(fpout, "缺少一个'/'");
          } else {
            fprintf(fpout, "%s\t\t%d\t\t运算符\n", "/", 44);
            fseek(fpin, -1L, SEEK_CUR);
          }
        } break;
          /***************非法字符*******************/
        default:
          fprintf(fpout, "在第%d行无法识别的字符\t%c\n", Line_NO, ch);
      }
  }
}
/**********主程序中完成对输入输出文件的读写***********/
int main() {
  char in_fn[25], out_fn[25];
  FILE *fpin, *fpout;
  printf("<<<<<<<<<<<<<<<<<<WELCOME>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
  printf("..............词法分析程序实验......................\n");
  printf("\n");
  printf(".....目前共有词法程序范例：   1个   \n");
  printf(".....程序范例1：data.txt \n");
  printf(".....输入一个已有的程序:\n");
  scanf("%s", in_fn);
  printf(".....输入你想要保存分析的目标文件名:\n");
  scanf("%s", out_fn);
  fpin = fopen(in_fn, "r");
  fpout = fopen(out_fn, "w");
  init();
  analyse(fpin, fpout);
  fclose(fpin);
  fclose(fpout);
  printf(".....程序已分析完成分析并保存至目标文件\n");
  printf("........<谢谢使用>......\n");
  return 0;
}
