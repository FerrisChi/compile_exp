# compile_exp
 Making c\c++ compiler

## 1_lexical

A lexical analysis program aimed for C\C++ program. 

A C++ version is presented in `c_lex\` and a Lex version is presented in `lex\`.

Lex version is in this experiment is a modified version of *ANSI C grammar, Lex specification*. [click here.](http://www.quut.com/c/ANSI-C-grammar-l-1998.html)

Yaac is supposed to be accompanied with the matching lex file. That's the work to do at next stage. [click here](http://www.quut.com/c/ANSI-C-grammar-y-1998.html)



Reference:

c++ version: 

​	GitHub in ref\

​	[Microsoft Ignite](https://docs.microsoft.com/en-us/cpp/c-language/c-character-constants?view=msvc-160)

lex version:

​	[lex入门](https://www.bwangel.me/2019/12/15/flex/)

​	[lex网络教程](https://blog.csdn.net/wp1603710463/article/details/50365495)

​	[另一个示例](https://rtoax.blog.csdn.net/article/details/79947149?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-1.no_search_link&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7Edefault-1.no_search_link)



## parser

A syntactic parser of arithmetic expression with + - * / ( )

A C++ version with cmake in `parse\` and a Yacc version in `yacc\`

#### In C++ version:

* Recursive analysis is intended for example grammar.
* LL(1) analysis and SLR(1) is intended for arbitrary grammar.
* Not eliminate left-recursion yet. To avoid the endless loop, grammar must have no left-recursion in productions when doing LL(1) and recursive analysis.



Reference:

c++ version:

[消去左递归](https://blog.csdn.net/qq_40294512/article/details/89396595)

[c++ template](https://blog.csdn.net/lyn631579741/article/details/110730145)



yacc version:

*ANSI C grammar, Lex specification*. [ANSI yacc(2011版)](http://www.quut.com/c/ANSI-C-grammar-y-2011.html)

[Yacc介绍](https://blog.csdn.net/wp1603710463/article/details/50365640)
