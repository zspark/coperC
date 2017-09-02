#include <iostream>
#include <Windows.h>
#include <regex>
#include "cllib.h"

using namespace std;
using namespace cl;

#define F_PRINT(s,i) std::cout<<s<<" 检测位置："<<i<<std::endl
#define F_PRINT_ITEM(s) std::cout<<"该条目是："+s<<std::endl

void ParseConfigItem(string rawString);

#define C_NO_SUB_FOLDER "没有子目录名！"
#define C_ILLEGAL_PATH "不合法的子路径！"
#define C_NO_FOLDER_IN_GROUP "目录不能出现在分组中！"

char GetCharAt(clString s,clI index){
  const clUi n=s.length();
  if(index<0)return '@';
  else if(index>=n)return '@';
  else s[index];
}


bool IsWildcardChar(char c){
  return (c=='?'||c=='*'||c=='+'||c=='!');
}

bool ValidateConfigItem(string s){
#define PRINT_AND_BREAK(info) {F_PRINT(info,i);F_PRINT_ITEM(s);break;}
  char currentChar;
  int leftMidBracketCount=0;
  int commonCount=0;
  bool inBracketFlag=false;
  const int n=s.length();
  for(int i=0;i<n;i++){
    currentChar=s[i];

    switch(currentChar){
    case '/':
      if(GetCharAt(s,i-1)=='@') PRINT_AND_BREAK(C_NO_SUB_FOLDER)
      if(GetCharAt(s,i-1)=='/') PRINT_AND_BREAK(C_ILLEGAL_PATH)
      if(GetCharAt(s,i-1)=='!') PRINT_AND_BREAK(C_NO_SUB_FOLDER)
      if(inBracketFlag)PRINT_AND_BREAK(C_NO_FOLDER_IN_GROUP)
      break;
    case '.':
      if(nextChar=='/'){
        if(preChar!='.'){
          F_PRINT("不合法的子路径！",i);
          failFlag=true;
        }
      } else if(nextChar=='.'){
        if(preChar!='@'&&preChar!='/'){
          F_PRINT("不合法的子路径！",i);
          failFlag=true;
        }
      } else if(preChar==':'){
        F_PRINT("'.'不能出现在':'之后！",i);
        failFlag=true;
      } else if(preChar=='.'){
        F_PRINT("不合法的子路径！",i);
        failFlag=true;
        return false;
      } else if(IsWildcardChar(preChar)){
        if(nextChar=='@'){
          F_PRINT("文件没有明确扩展名！",i);
          failFlag=true;
        }
      }
      break;
    case '+':
    case '*':
      if(preChar=='@'){
        if(nextChar=='@'){
          F_PRINT("文件没有明确扩展名！",i);
          failFlag=true;
        }
      } else if(IsWildcardChar(preChar)){
        string t=string(&currentChar,1);
        string tmp="其他通配符不能与'"+t+"'连用，正则表达式无意义！";
        F_PRINT(tmp,i);
        failFlag=true;
      }
      break;
    case '?':
      if(preChar=='@'){
        if(nextChar=='@'){
          F_PRINT("文件没有明确扩展名！",i);
          failFlag=true;
        }
      } else if(IsWildcardChar(preChar)&&preChar!='?'){
        F_PRINT("只有'?'才能与'?'连用，否则正则表达式无意义！",i);
        failFlag=true;
      }
      break;
    case '!':
      if(preChar=='@'){
        if(nextChar=='@'){
          F_PRINT("文件没有明确扩展名！",i);
          failFlag=true;
        }
      } else if(preChar=='/'){
        if(nextChar!='.'){
          F_PRINT("文件没有明确扩展名！",i);
          failFlag=true;
        }
      }
      break;
    case '[':
      if(++leftMidBracketCount>1){
        F_PRINT("分组不能嵌套!",i);
        F_PRINT_ITEM(s);
        return false;
      } else{
        if(i!=0){
          if(s[i-1]=='/'){
            inBracketFlag=true;
          } else{
            F_PRINT("分组前面必需是'/'",i);
            F_PRINT_ITEM(s);
            return false;
          }
        } else inBracketFlag=true;
      }
      break;
    case ']':
      if(inBracketFlag){
        inBracketFlag=false;
      } else{
        F_PRINT("分组括号不匹配",i);
        F_PRINT_ITEM(s);
        return false;
      }
      F_PRINT("分组结束，本条目也应该结束！",i);
      F_PRINT_ITEM(s);
      return false;
      break;
    case ':':
      if(!inBracketFlag){
        F_PRINT("分组中才能出现':'",i);
        F_PRINT_ITEM(s);
        return false;
      } else{
        if(++commonCount>1){
          F_PRINT("分组中最多只能有一个通用扩展名！",i);
          F_PRINT_ITEM(s);
          return false;
        }
      }
      break;
    case '|':
      if(!inBracketFlag){
        F_PRINT("分组中才能出现'|'",i);
        F_PRINT_ITEM(s);
        return false;
      }
      break;
    default:
      break;
    }

    if(failFlag){
      F_PRINT_ITEM(s);
      return false;
    }
  }
  F_PRINT("OK..",-1);
  F_PRINT_ITEM(s);
#undef FAIL_AND_BREAK
  return true;
}

int main(){
  cout<<"Hello World"<<endl;

  vector<string> items;
  //items.push_back("/");
  //items.push_back("a//");
  //items.push_back("*/");
  //items.push_back("?/");
  //items.push_back("./");
  //items.push_back("a/");

  //items.push_back("../");
  //items.push_back("./");
  //items.push_back("a./");
  //items.push_back("..a");
  //items.push_back(".a");
  //items.push_back("*.");
  //items.push_back("?.");
  //items.push_back("?.*");

  items.push_back("?*");
  items.push_back("+*");
  items.push_back("++");
  items.push_back("??");
  items.push_back("+?");
  items.push_back("+");
  items.push_back("?.");
  items.push_back("?.+");
  items.push_back("?.!");
  items.push_back("?*?.!");
  items.push_back("*?.!");
  items.push_back("a/b/!a");
  items.push_back("a/b/.!");

  /*
  items.push_back("a/b/[a.txt|b.png|b.png|xiaoshuo*.txt|c|d:j*]");
  items.push_back("/[a.txt]");
  items.push_back("a/b/[c/|a.txt]");
  items.push_back("a/b/[c/a.txt]");
  items.push_back("a/b/[c|a|d|e:jpg]");
  items.push_back("a/b/[c|a|d|e:jpg:png]");
  items.push_back("a/b/[c|a|d|e:jg*]");
  items.push_back("a/b/[c|a|d|e]");
  items.push_back("a/b/][c|a|d|e]");
  items.push_back("a/b/[][c|a|d|e]");
  items.push_back("a/b/[[c/]|a|d|e]");
  items.push_back("a/b/|[[c/]|a|d|e]");
  items.push_back("a/b:txt|[[c/]|a|d|e]");
  items.push_back("../a/b:txt|[[c/]|a|d|e]");
  items.push_back(".");
  items.push_back(":");
  items.push_back("|");
  items.push_back("?");
  items.push_back("*");
  items.push_back("*.*");
  items.push_back(".*");
  */

  for(int i=0;i<items.size();i++){
    string s=items[i];
    if(ValidateConfigItem(s)){
      //ParseConfigItem(s);
      //cout<<"OK.."<<endl;
    }
    cout<<endl;
  }
  system("pause");
  return 0;
}

void ParseNoneGroup(string rawString){
  cout<<"NG:"+rawString<<endl;

}
void ParseGroup(string rawString){
  cout<<"G:"+rawString<<endl;



  //handle':'
  string reg=R"(\[(.*):((\w|\*)*)\]$)";
  regex e(reg);

  smatch sm;
  if(regex_match(rawString,sm,e)){
    cout<<"extension is:"<<sm[2]<<endl;
  }
  //handle'|'

}

void ParseConfigItem(string rawString){
  const string reg=R"((.*)(\[.*\])$)";
  regex e(reg);

  smatch sm;
  if(regex_match(rawString,sm,e)){
    ParseNoneGroup(sm[1]);
    ParseGroup(sm[2]);
  }

}