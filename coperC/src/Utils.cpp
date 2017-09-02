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
#include <fstream>
#include <vector>
#include <string>
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <math.h>
#include "Utils.h"
#include "Status.h"
#include "clPrinter.h"

using namespace std;

class CBrowseDir{
  //存放初始目录的绝对路径，以'\'结尾
  char m_szInitDir[_MAX_PATH];


public:

  //设置初始目录为dir，如果返回false，表示目录不可用
  bool SetInitDir(const char *dirURL){
    //先把dir转换为绝对路径
      //if (_fullpath(m_szInitDir,dir,_MAX_PATH) == NULL)
        //  return false;
    strcpy(m_szInitDir,dirURL);

    //判断目录是否存在
    if(_chdir(m_szInitDir)!=0){
      cout<<"目录不存在:"<<m_szInitDir<<endl;
      return false;
    }

    return true;
  }

  //开始遍历初始目录及其子目录下由filespec指定类型的文件
  //filespec可以使用通配符 * ?，不能包含路径。
  //如果返回false，表示遍历过程被用户中止
  bool BrowseDir(const char * filespec,vector<string>* fileNames){
    long hFile;
    _finddata_t fileinfo;
    if((hFile=_findfirst(filespec,&fileinfo))!=-1){
      do{
        //检查是不是目录
        //如果不是,则进行处理
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

/**
 *统计fileURL所指的本地目录是否没有文件存在
 *有子目录存在不算；
 *参数必须为绝对路径；
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

