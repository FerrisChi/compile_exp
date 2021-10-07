
#include<bits/stdc++.h>

using namespace std;
//grammer analyzer of zwx
//暂时没加出错行数
string sign[100]={"digit","letter","#","<",">","(",")","[","]","{","}",",",";","+","-","*","/","%","!","'","\"","=","++","--","&&","||",">=","<=","==","!=",">>","<<",":=","int","long","short","float","double","char","unsigned","signed","const","void","volatile","enum","struct","union","if","else","goto","switch","case","do","while","for","continue","break","return","default","typedef","auto","register","extern","static","sizeof","include"," ","kinds"};
stack<int>sta;
stack<int>atmsta;
int no=0;//行数
char str[20];
int atm;
vector<int> predit[200][100];//预测分析表
void init(){//预测分析表的读入
    FILE *fp=freopen("lexinit.txt","r",stdin);
    int x,y, n,t;
    string str;
    for(int k=0;k<51;k++)
    {
    scanf("%d %d %d",&x,&y,&n);
        for(int i=0;i<n;i++){
            cin>>str;
            if(str[0]>='A'&&str[0]<='Z')t=(str[0]-'A'+100);
            else if(str=="kinds")t=67;
            else if(str=="ε")t=-1;
            else {
                    for(t=0;t<70;t++){
                        if(sign[t]==str)break;
                    }
            }
            //cout<<t<<endl;
            predit[x][y].push_back(t);
        }
    }
//    x=114 ;y=54;
//    for(int i=0;i<predit[x][y].size();i++)
//    cout<<predit[x][y][i]<<" ";
  //  cout<<predit[123][22].size()<<endl;
    fclose(fp);
}
void print(int x ,int y){
        //print
        int len=0;//栈中元素长度，为了美化输出
        printf("%4d ",no);
        while(sta.size()){
            atmsta.push(sta.top());
            sta.pop();
        }
        len=0;
        while(atmsta.size()){
            if(atmsta.top()==-1)printf("$");
            else if(atmsta.top()>=100)printf("%c",(char)(atmsta.top()-100+'A'));
            else cout<<sign[atmsta.top()];
            len+=sign[atmsta.top()].size();
            sta.push(atmsta.top());
            atmsta.pop();
        }
        for(int i=len;i<20;i++)printf(" ");
//        if(atm==0)printf("digit");
//        else if(atm==1)printf("letter");
//        else if(atm==67||(atm>=33&&atm<=45))printf("kinds");
//        else
            printf("%s",str);
        for(int i=strlen(str);i<14;i++)printf(" ");
        if(x>=0&&y>=0){
            cout<<(char)(x-100+'A')<<"->";
            for(unsigned int i=0;i<predit[x][y].size();i++)
                {
                    int t =predit[x][y][i];
                    if(t<=67&&t>=0)cout<<sign[t];
                    else if(t<0)cout<<"ε";
                    else cout<<(char)(t-100+'A');
                }
        }
        printf("\n");
}
void analyzer(){
    //init
    sta.push(2);
    sta.push(100);

    printf("步骤  符号栈               当前输入的符号   动作\n");
    bool f=true;
    while(scanf("< %d ,%s >",&atm,str)){
    if(!f)break;
        getchar();
        //if(atm==2)break; 代码中有#的问题还没有解决
        //printf("%d %s\n",atm,str);
        print(-1,-1);no++;
        //action
        if(atm>=33&&atm<=45)atm=67;
        while(true){
        int now=sta.top();sta.pop();
        if(now<=67&&now!=2){//是一个终结符
            if(now==atm)break;
            else {
                printf("error!");
                f=false;
                break;
            }
        }
        else if(now==2){
            if(now==2&&atm==2&&sta.size()==0){
                printf("succeess!");
                break;
            }
            else {
                printf("error!");
                f=false;
                break;
            }
        }
        else {
        int ans;
        //cout<<now<<" "<<atm<<" ";
            if(predit[now][atm].size()){
                for(int i=predit[now][atm].size()-1;i>=0;i--)
                    {
                        ans=predit[now][atm][i];
                        //cout<<ans<<" ";
                        if(ans>=0&&ans<=200)sta.push(ans);
                    }
                //cout<<endl;
                print(now,atm);
                no++;
            }
            else {
                printf("error!");
                f=false;
                break;
            }
        }
        }
        if(atm==2)break;
    }
}

int main (){
    init();
    freopen("lexicalResult.txt","r",stdin);//文件标准读入
    freopen("result.txt","w",stdout);//文件标准输出
    analyzer();
    return 0;
}
