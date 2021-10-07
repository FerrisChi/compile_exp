#include<bits/stdc++.h>

using namespace std;
/*
easy lexical analyzer of zwx
*/
bool isNum(char chr){//判断数字
   return  chr>='0'&&chr<='9';
}

bool isLetter(char chr){//判断字符
    //由于变量名中可以出现下划线，所以这里对下划线也返回true,对于引入的文件，可能存在.，也加入判断
    return (chr>='a'&&chr<='z')||(chr>='A'&&chr<='Z')||(chr=='_')||(chr=='.');
}
//定界符 单个
char symbol[]={'#','<','>','(',')','[',']','{','}',',',';','+','-','*','/','%','!','\'','"','='};//20个 0~19 +[2]
string twoSymbol[]={"++","--","&&","||",">=","<=","==","!=",">>","<<",":="};//11个 0~10+[22]

int isSymbol(char chr){//判断是否是定界符
    for(int i=0;i<20;i++){
        if(chr==symbol[i])return i+2;
    }
    return -1;
}

int isTwoSymbol(string str){//判断是不是两位的符号
    for(int i=0;i<11;i++){
        if(str==twoSymbol[i])return i+22;
    }
    return -1;
}

//关键字 (保留字)reserved word，c语言中一共有32个
string keyword[]={"int","long","short","float","double","char","unsigned","signed","const","void","volatile","enum","struct","union","if","else","goto","switch","case","do","while","for","continue","break","return","default","typedef","auto","register","extern","static","sizeof","include"};
//32个 0~31+[27]

int isKeyword(string str){//判断关键字
    if(str.size()>2&&str[str.size()-1]=='h'&&str[str.size()-2]=='.')return 66;
    for(int i=0;i<33;i++){
        if(str==keyword[i])return i+33;
    }
    return -1;
}


const int len=255;
char word[len],temp[len];
string _input;
// 输入字符串 ，空格处理后，注释处理后
void hitSpace()//多余空格处理:只保留间隔用的空格
{
    int tot=0;
    memset(word,0,sizeof(word));
    bool pre=false; //?
    for(int i=0;i<_input.size();i++){
        if(_input[i]==' '&&pre){
            word[tot]=_input[i];
            pre=false;
        }
        else if(_input[i]=='\t'){
                continue;
        }
        else {
            word[tot]=_input[i];
            pre=true;
        }
        tot++;
    }

}

// 行注释处理
void delNotes(){
    memset(temp,0,sizeof(temp));
    for(int i=0;i<strlen(word);i++){
        if(word[i]=='/'&&word[i+1]=='/')break;
        temp[i]=word[i];
    }
}
void dToB(int ans){//十进制
    if(!ans)return ;
    dToB(ans/2);
    printf("%d",ans%2);
}

void init(){
    bool Bnotes=false;//多行注释
    string tempstr;
    char *str;
    int line=0;//当前行数
    while(getline(cin,_input)){//按行读入，对空格不敏感
        line++;
        hitSpace();//处理多余空格
        delNotes();//处理注释
        //printf("第%d行读进来的是：%s\n",line,temp);
        //printf("%d",line);
         str=strtok(temp," ");//分割字符串
         while(str!=NULL){
        //printf("读进来的：%s\n",str);
//            if(*(str)=='#'){
//                puts("<2,#>");
//                break;
//            }

            for(int i=0;i<strlen(str);){//遍历此行

                if(*(str+i)=='/'&&*(str+i+1)=='*'){//处理多行注释中的左注释
                        Bnotes=true;
                        break;
                }

                if(*(str+i)=='*'&&*(str+i+1)=='/'&&Bnotes){ //处理多行注释中的右注释
                    Bnotes=false;
                    i+=2;
                    break; // 不应该删除，
                }
                tempstr="";
                if(!Bnotes&&isLetter(*(str+i))){ //出现字符的时候，
                    while(isLetter(*(str+i))||isNum(*(str+i)))//提取字符串
                        tempstr+=*(str+(i++));
                    if(isKeyword(tempstr)==-1)
                        printf("< %d ,%s >\n",1,tempstr.c_str());
                    else if(tempstr[0]=='.')
                        printf("第%d行出现因为小数点作为开头定义变量的错误:%s\n",line,tempstr.c_str());
                    else
                        printf("< %d ,%s >\n",isKeyword(tempstr),tempstr.c_str());
                    continue;
                }
                if(!Bnotes&&isNum(*(str+i))){
                    while(isNum(*(str+i)))//提取数字串
                        tempstr+=*(str+(i++));
                        int slen=tempstr.size();
                        long long  ans=0;
                        for(int i=0;i<slen;i++){
                        ans=ans*10+(int)(tempstr[i]-'0');//ans一定在long long 范围内
                        }
                    printf("< %d ,binary:",0);
                    if(ans)
                        dToB(ans);
                    else
                        printf("0");
                    printf(" >\n");//数字的标识符

                    continue;
                }
                if(!Bnotes&&isSymbol(*(str+i))!=-1){//定界符和运算符
                    if(isSymbol(*(str+i+1))!=-1){
                        string pstr="";
                        pstr+=*(str+i);
                        pstr+=*(str+(i+1));
                        //cout<<pstr<<endl;
                        //printf("now：%s\n",*(str+i)+*(str+i+1));
                        if(isTwoSymbol(pstr)!=-1){
                            printf("< %d ,%s >\n",isTwoSymbol(pstr),pstr.c_str());
                            i++;
                        }
                        else
                            printf("< %d ,%c >\n",isSymbol(*(str+i)),*(str+i));
                    }
                    else {
                        printf("< %d ,%c >\n",isSymbol(*(str+i)),*(str+i));
                    }
                    i++;
                    continue;
                }
                if(*(str+i)!=' ')
                printf("第%d行出现错误：%c\n",line,*(str+i));

            }
            str=strtok(NULL," ");
         }

    }
}

int main (){
    freopen("input.cpp","r",stdin);//文件标准读入
    freopen("lexicalResult.txt","w",stdout);//文件标准输出
    init();
    printf("< 2 ,# >");
    return 0;
}
