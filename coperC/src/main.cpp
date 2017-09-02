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

#include <windows.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "cllib.h"
#include "Utils.h"
#include "Status.h"
#include "ConfigFileItem.h"
#include "ConfigParser.h"
#include "FileCoper.h"
#include "clPrinter.h"
#include "clUtil.h"


using namespace std;
using namespace cl;

void readTextFileToArray(clCs configFileURL,vector<string>* ret){
  ifstream file_r(configFileURL,ios::in);
  if(file_r){
    while(!file_r.eof()){
      char c[_MAX_PATH];
      file_r.getline(c,_MAX_PATH,'\n');
      ret->push_back(c);

      if(file_r.fail()){
        file_r.clear(file_r.rdstate()&~ifstream::failbit);
      }
    }
  }
  file_r.close();
}

void run(clCs filename){

  vector<string> cc;
  readTextFileToArray(filename,&cc);

  Info("配置文件行数："+cl::NumberToString(cc.size()));

  vector<ConfigFileItem> cfis;
  ConfigParser cp;
  cp.parse(&cc,&cfis);

  /*
  FileCoper fc;
  fc.copyStart(&cfis);
*/
}

int main(int argc,clCs argv[]){
  //char * c_title="Coper version:1.0.1";
  //SetConsoleTitle(c_title);


  Info("Hello Coper!");
  Info(argv[0]);
  Info("-----------------------------------------");

#if(1)
  run("./config.txt");
#else
  appURL=argv[0];

  if(argc==2){
    run(argv[1]);
  } else{
    Error("请将配置文件拖入coperC.exe文件");
    //configFileURL="config.txt";
  }
#endif
  system("pause");
  return 0;
}
