/*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include <iostream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include "debugger.h"
#include "ConfigParser.h"
#include "Utils.h"
#include "ConfigFileItem.h"
#include "Status.h"

using namespace std;
using namespace cl;

class Range{
public:
  string samePart="";
  int differentPartLength=-1;
  int fromNumber=0;
  int endNumber=0;
  string extension=EXT_ALL;

public:
  Range(const string& range,const string & ext){
    extension=ext;
    vector<string> arr;
    split(range,MARK_WAVE,&arr);
    string s0(arr[0]);
    string s1(arr[1]);

    int l(fmin(s0.length(),s1.length()));

    int dIndex=-1;
    string::iterator s0it(s0.begin());
    string::iterator s1it(s1.begin());
    for(int i=0;i<l;i++){
      if(*s0it!=*s1it){
        dIndex=i;
        break;
      } else{
        s0it++;s1it++;
      }
    }

    if(dIndex==-1){
      Error("无法识别的名字区间{"+range+":"+ext+"}");
      return;
    }
    samePart=s0.substr(0,dIndex);
    if(s0.length()==s1.length())differentPartLength=s0.length()-dIndex;

    if(isStringAllDecNumber(s0.substr(dIndex))&&isStringAllDecNumber(s1.substr(dIndex))){
      fromNumber=atoi(s0.substr(dIndex).data());
      endNumber=atoi(s1.substr(dIndex).data());
    } else{
      Error("名字后几位不是数字，无法知道如何步进！{"+range+":"+ext+"}");
    }
  }
};

void ConfigParser::parse(vector<string>* c,vector<ConfigFileItem>* itemArray){
  /*
  parsing=true;
  totalItemCount=0;
  totalResCount=0;
*/
  for(vector<string>::iterator it=c->begin();it!=c->end();it++){
    if(it->find_first_of(COMMET_MARK)==0||it->length()==0)continue;
    if(it->find(ROOT)==0){
      root=it->substr(it->find_first_of(MARK_COLON)+1);
      Info("Root URL is: \""+root);
      continue;
    } else if(it->find(TARGET)==0){
      target=it->substr(it->find_first_of(MARK_COLON)+1);
      Info("Target URL is: \""+target);
      continue;
    }

    int lastBackSlashIndex(it->find_last_of(BACK_SLASH,it->find_first_of(MARK_BIG_BRACKET)));
    ConfigFileItem *cfi=new ConfigFileItem(*it);
    string sub(it->substr(0,lastBackSlashIndex+1));
    cfi->setRelativePath(sub);
    sub=it->substr(lastBackSlashIndex+1,it->length()-lastBackSlashIndex);
    cfi->setLastContentPart(sub);
    // cout<<"========================log=========================="<<endl;
     //cout<<"lastBackSlashIndex:"<<lastBackSlashIndex<<endl;
   //  cout<<cfi->getRelativePath()<<" "<<cfi->getLastContentPart()<<endl;
    // cout<<"========================log_over=========================="<<endl;

    parseItem(cfi);
    itemArray->push_back(*cfi);

  }
}

void ConfigParser::parseItem(ConfigFileItem *item){
  string lastPart(item->getLastContentPart());
  Info("Line: "+item->getContent());
  if(lastPart==""){
    pushDirectoryFilesIntoItem(item,EXT_ALL,"");
  } else if(lastPart.find_first_of(MARK_BIG_BRACKET)==0){
    parseContentWithBigBracket(item);
  } else{
    if(lastPart.find_last_of(MARK_DOT)<lastPart.length()){
      checkExistAndPush(lastPart,item,"");
    } else{
      pushSameNameFilesIntoItem(lastPart,item);
    }
  }
}

void ConfigParser::checkExistAndPush(const string& fileNameE,ConfigFileItem* item,const string& fixedURL){
  string fileURL(root+item->getRelativePath()+fixedURL+fileNameE);
  //cout<<fileURL<<endl;
  if(checkFileExistance(fileURL)){
    string newName(fixedURL+fileNameE);
    //cout<<newName<<endl;
    item->push(newName);
  } else{
    Error("资源不存在:"+fileURL);
  }
}

void ConfigParser::parseContentWithBigBracket(ConfigFileItem* item){
  string s=item->getLastContentPart();
  string lastPart(s.substr(1,s.length()-2));
  vector<string> arr;
  split(lastPart,MARK_V_LINE,&arr);
  for(vector<string>::iterator it=arr.begin();it!=arr.end();it++){
    if(it->find(MARK_COLON)<it->length()){
      /**
       * 有统一的扩展名，有下面2种情况
       * 001~013:doc		001~013:\
       * :png
       * */
      vector<string> arrc;
      split(*it,MARK_COLON,&arrc);
      string* arrcElem=&arrc[0];
      if(arrcElem->find(MARK_WAVE)<arrcElem->length()){
        parseRange(*arrcElem,arrc[1],item);
      } else{
        pushDirectoryFilesIntoItem(item,arrc[1],"");
      }

    } else{
      /**
       * 没有统一的扩展名，有下面4种情况
       * 001~013
       * 015.txt
       * 001
       * aaa\
       * */
      if(it->find(MARK_WAVE)<it->length()){
        parseRange(*it,EXT_ALL,item);
      } else if(it->find(MARK_DOT)<it->length()){
        checkExistAndPush(*it,item,"");
      } else if(it->find_last_of(BACK_SLASH)==it->length()-1){
        pushDirectoryFilesIntoItem(item,"*",*it);
      } else{
        pushSameNameFilesIntoItem(*it,item);
      }
    }
  }
}

void ConfigParser::pushSameNameFilesIntoItem(string& fileNameN,ConfigFileItem* item){
  string s(root+item->getRelativePath());

  //cout<<"要检查的文件:"<<s<<endl;
  if(checkDirectoryExistance(s)){
    vector<string> files;
    string fileNameE(fileNameN+".*");
    getDirectoryFiles(s,fileNameE.c_str(),&files);
    vector<string>::iterator it;
    for(it=files.begin();it!=files.end();it++){
      item->push(*it);
    }
  } else{
    Error("目录不存在:"+s);
  }
}

void ConfigParser::pushDirectoryFilesIntoItem(ConfigFileItem* item,const string &ext,const string& fixedURL){
  string dirURL(root+item->getRelativePath()+fixedURL);
  if(!checkDirectoryExistance(dirURL)){
    Error("目录不存在:"+dirURL);
  } else{
    vector<string> files;
    string fileNameE("*."+ext);
    getDirectoryFiles(dirURL,fileNameE.c_str(),&files);
    vector<string>::iterator it;
    for(it=files.begin();it!=files.end();it++){
      string s(fixedURL+*it);
      item->push(s);
    }
  }
}

void ConfigParser::parseRange(const string& range,const string& ext,ConfigFileItem* item){
  Range r(range,ext);
  string fileNameN("");
  for(int i=r.fromNumber;i<=r.endNumber;i++){
    if(r.differentPartLength==-1){
      //自由长度；
      fileNameN=r.samePart;
    } else{
      //固定长度；
      fileNameN=r.samePart+fixToLength(i,r.differentPartLength);
    }

    if(r.extension==BACK_SLASH){
      pushDirectoryFilesIntoItem(item,EXT_ALL,fileNameN+BACK_SLASH);
    } else if(r.extension==EXT_ALL){
      pushSameNameFilesIntoItem(fileNameN,item);
    } else{
      string fileNameE(fileNameN+"."+r.extension);
      checkExistAndPush(fileNameE,item,"");
    }

  }
}

