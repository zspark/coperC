#include <iostream>
#include <Windows.h>
#include <regex>
#include "cllib.h"

using namespace std;
using namespace cl;

#define F_PRINT(s,i) std::cout<<s<<" ���λ�ã�"<<i<<std::endl
#define F_PRINT_ITEM(s) std::cout<<"����Ŀ�ǣ�"+s<<std::endl

void ParseConfigItem(string rawString);

#define C_NO_SUB_FOLDER "û����Ŀ¼����"
#define C_ILLEGAL_PATH "���Ϸ�����·����"
#define C_NO_FOLDER_IN_GROUP "Ŀ¼���ܳ����ڷ����У�"

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
          F_PRINT("���Ϸ�����·����",i);
          failFlag=true;
        }
      } else if(nextChar=='.'){
        if(preChar!='@'&&preChar!='/'){
          F_PRINT("���Ϸ�����·����",i);
          failFlag=true;
        }
      } else if(preChar==':'){
        F_PRINT("'.'���ܳ�����':'֮��",i);
        failFlag=true;
      } else if(preChar=='.'){
        F_PRINT("���Ϸ�����·����",i);
        failFlag=true;
        return false;
      } else if(IsWildcardChar(preChar)){
        if(nextChar=='@'){
          F_PRINT("�ļ�û����ȷ��չ����",i);
          failFlag=true;
        }
      }
      break;
    case '+':
    case '*':
      if(preChar=='@'){
        if(nextChar=='@'){
          F_PRINT("�ļ�û����ȷ��չ����",i);
          failFlag=true;
        }
      } else if(IsWildcardChar(preChar)){
        string t=string(&currentChar,1);
        string tmp="����ͨ���������'"+t+"'���ã�������ʽ�����壡";
        F_PRINT(tmp,i);
        failFlag=true;
      }
      break;
    case '?':
      if(preChar=='@'){
        if(nextChar=='@'){
          F_PRINT("�ļ�û����ȷ��չ����",i);
          failFlag=true;
        }
      } else if(IsWildcardChar(preChar)&&preChar!='?'){
        F_PRINT("ֻ��'?'������'?'���ã�����������ʽ�����壡",i);
        failFlag=true;
      }
      break;
    case '!':
      if(preChar=='@'){
        if(nextChar=='@'){
          F_PRINT("�ļ�û����ȷ��չ����",i);
          failFlag=true;
        }
      } else if(preChar=='/'){
        if(nextChar!='.'){
          F_PRINT("�ļ�û����ȷ��չ����",i);
          failFlag=true;
        }
      }
      break;
    case '[':
      if(++leftMidBracketCount>1){
        F_PRINT("���鲻��Ƕ��!",i);
        F_PRINT_ITEM(s);
        return false;
      } else{
        if(i!=0){
          if(s[i-1]=='/'){
            inBracketFlag=true;
          } else{
            F_PRINT("����ǰ�������'/'",i);
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
        F_PRINT("�������Ų�ƥ��",i);
        F_PRINT_ITEM(s);
        return false;
      }
      F_PRINT("�������������ĿҲӦ�ý�����",i);
      F_PRINT_ITEM(s);
      return false;
      break;
    case ':':
      if(!inBracketFlag){
        F_PRINT("�����в��ܳ���':'",i);
        F_PRINT_ITEM(s);
        return false;
      } else{
        if(++commonCount>1){
          F_PRINT("���������ֻ����һ��ͨ����չ����",i);
          F_PRINT_ITEM(s);
          return false;
        }
      }
      break;
    case '|':
      if(!inBracketFlag){
        F_PRINT("�����в��ܳ���'|'",i);
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