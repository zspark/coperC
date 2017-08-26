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
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Status.h"
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <math.h>
#include "debug.hpp"

using namespace std;
#define R_OK 4 /* Test for read permission. */
#define W_OK 2 /* Test for write permission. */
#define X_OK 1 /* Test for execute permission. */
#define F_OK 0 /* Test for existence. */

class CBrowseDir{
  //��ų�ʼĿ¼�ľ���·������'\'��β
  char m_szInitDir[_MAX_PATH];


public:

  //���ó�ʼĿ¼Ϊdir���������false����ʾĿ¼������
  bool SetInitDir(const char *dirURL){
    //�Ȱ�dirת��Ϊ����·��
      //if (_fullpath(m_szInitDir,dir,_MAX_PATH) == NULL)
        //  return false;
    strcpy(m_szInitDir,dirURL);

    //�ж�Ŀ¼�Ƿ����
    if(_chdir(m_szInitDir)!=0){
      cout<<"Ŀ¼������:"<<m_szInitDir<<endl;
      return false;
    }

    return true;
  }

  //��ʼ������ʼĿ¼������Ŀ¼����filespecָ�����͵��ļ�
  //filespec����ʹ��ͨ��� * ?�����ܰ���·����
  //�������false����ʾ�������̱��û���ֹ
  bool BrowseDir(const char * filespec,vector<string>* fileNames){
    long hFile;
    _finddata_t fileinfo;
    if((hFile=_findfirst(filespec,&fileinfo))!=-1){
      do{
        //����ǲ���Ŀ¼
        //�������,����д���
        if(!(fileinfo.attrib & _A_SUBDIR)){
          fileNames->push_back(*new string(fileinfo.name));
        }
      } while(_findnext(hFile,&fileinfo)==0);
      _findclose(hFile);
    }
    return true;
  }

};


char* read(char *fileURL){
  ifstream file_r;
  file_r.open(fileURL,ios::in);
  const int MAX_CHAR=2048;
  char *c=new char[MAX_CHAR];
  if(file_r){
    file_r.get(c,MAX_CHAR,EOF);
  }
  file_r.close();
  return c;
}

void readTextFileToArray(char* configFileURL,vector<string>* ret){
  ifstream file_r(configFileURL,ios::in);
  if(file_r){
    while(!file_r.eof()){
      char c[_MAX_PATH];
      file_r.getline(c,_MAX_PATH,'\n');
      ret->push_back(*new string(c));

      if(file_r.fail()){
        file_r.clear(file_r.rdstate()&~ifstream::failbit);
      }
    }
  }
  file_r.close();
}

bool checkFileExistance(const string &fileURL){
  return !_access(fileURL.c_str(),R_OK);
}

bool checkDirectoryExistance(const string &dirURL){
  return !_access(dirURL.c_str(),F_OK);
}

/**
 *ͳ��fileURL��ָ�ı���Ŀ¼�Ƿ�û���ļ�����
 *����Ŀ¼���ڲ��㣻
 *��������Ϊ����·����
 */
bool isDirectoryNoFiles(string &fileURL){
  vector<string> fileNames;
  CBrowseDir cb;
  if(cb.SetInitDir(fileURL.c_str())){
    cb.BrowseDir("*.*",&fileNames);
    return fileNames.size()==0;
  }return true;

}

void getDirectoryFiles(const string &directoryURL,const char *filespec,vector<string>* fileNames){
  CBrowseDir cb;
  if(cb.SetInitDir(directoryURL.c_str())){
    cb.BrowseDir(filespec,fileNames);
  }
}

void copyFileTo(string& fileURL,string & folderURL){
  string cmd("copy "+fileURL+" "+folderURL);
  system(cmd.c_str());
}

void createDirectory(string &directoryURL){
  string cmd("md "+directoryURL);
  system(cmd.c_str());
}

bool isExtensionRight(const string& fileName,const string& ext){
  int index(fileName.find(ext));
  return index>=0&&index==fileName.length()-ext.length();
}

bool isStringAllDecNumber(const string& str){
  for(int i=0;i<str.size();i++){
    if((str.at(i)>'9')||(str.at(i)<'0'))
      return  false;
  }
  return true;
}

string fixToLength(int num,int l){
  if(numberLength(num)>l){
    cout<<"��ͼ��һ�����������ֵ���Ϊ���Ƚ϶̵��ַ�����������ֹ��"<<endl;
    return "";
  }

  char f[10];
  sprintf(f,"%%0%dd",l);

  char c[10];
  sprintf(c,f,num);

  string s(c);
  return s;

}

int numberLength(int a){
  int flag=0;
  while(a){
    a/=10;
    flag++;
  }
  return flag;
}

void split(const string& s,const string& delim,vector< string >* ret){
  size_t last=0;
  size_t index=s.find_first_of(delim,last);
  while(index!=string::npos){
    ret->push_back(s.substr(last,index-last));
    last=index+1;
    index=s.find_first_of(delim,last);
  }
  if(index-last>0){
    ret->push_back(s.substr(last,index-last));
  }
}

string& trim(string &s){
  if(s.empty()){
    return s;
  }

  s.erase(0,s.find_first_not_of(" "));
  s.erase(s.find_last_not_of(" ")+1);
  return s;
}

string&   replace_all(string&   str,const   string&   old_value,const   string&   new_value){
  string::size_type   pos(0);
  while(true){
    if((pos=str.find(old_value,pos))!=string::npos){
      str.replace(pos,old_value.length(),new_value);
      pos+=new_value.length();
    } else   break;
  }
  return   str;
}

void copyFile(const string & relativePath,const string & fileName){
  string fileURL(root+relativePath+fileName);
  string folderURL(target+relativePath+fileName);
  folderURL=folderURL.substr(0,folderURL.find_last_of("\\")+1);

  if(!checkDirectoryExistance(folderURL)){
    createDirectory(folderURL);
  }

  warning(fileURL);

  copyFileTo(fileURL,folderURL);
}








