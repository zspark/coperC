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
#include "Utils.h"
#include "Status.h"
#include "ConfigFileItem.h"
#include "ConfigParser.h"
#include "FileCoper.h"
#include "debugger.h"


using namespace std;

void run(char* filename){

  vector<string> cc;
  readTextFileToArray(filename,&cc);

  cl::Info("配置文件行数："+cc.size());

  vector<ConfigFileItem> cfis;
  ConfigParser cp;
  cp.parse(&cc,&cfis);

  FileCoper fc;
  fc.copyStart(&cfis);
}

int main(int argc,char * argv[]){
  //char * c_title="Coper version:1.0.1";
  //SetConsoleTitle(c_title);


  cl::Info("Hello Coper!");
  cl::Info(argv[0]);
  cl::Info("-----------------------------------------");
  appURL=argv[0];

  if(argc==2){
    run(argv[1]);
  } else{
    cl::Error("请将配置文件拖入coperC.exe文件");
    //configFileURL="config.txt";
  }

  system("pause");
  return 0;
}
