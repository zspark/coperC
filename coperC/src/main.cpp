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
#include "coper.h"
#include "parameter_parser.h"
#include "lexical_analyzer.h"
#include "grammar_analyzer.h"
#include "interactive_controller.h"

using namespace std;
using namespace cl;

void ReadConfigFile(const clchar* configFileURL,vector<clstr>& out){
  ifstream file_r(configFileURL,ios::in);
  if(file_r){
    while(!file_r.eof()){
      char c[_MAX_PATH];
      file_r.getline(c,_MAX_PATH,'\n');
      if(clstr(c).empty())continue;
      if(clRegexp::IsStartedWith(c,R"(#)",true))continue;
      out.push_back(c);
      if(file_r.fail()){
        file_r.clear(file_r.rdstate()&~ifstream::failbit);
      }
    }
  }
  file_r.close();
}

void Run(const clchar* filename){
  vector<clstr> out;
  ReadConfigFile(filename,out);
  Unimportant("Avaliable items of config file : "+clTypeUtil::NumberToString(out.size()),true,false);

  ParameterParser pp;
  for(cluint i=0;i<out.size();i++){
    if(clRegexp::IsStartedWith(out[i],"parameter",true)){
      Unimportant(out[i],true,false);
      if(!pp.Parse(out[i])){
        Error("Config file parsing failed!");
        return;
      }
      out.erase(out.begin()+i);
      break;
    }
  }

  Unimportant("-----------------------------------------",true,false);
  InteractiveController ic;
  if(pp.NeedRequest()) if(!ic.RequestAnalyze())return;

  LexicalAnalyzer la(ConsoleForeground::GRAY,ConsoleForeground::RED);
  GrammarAnalyzer ga;
  const cluint n=out.size();
  for(clint i=0;i<n;i++){
    clstr s=out[i];
    if(la.Analyze(s,pp.IsVerbose())){
      /*
      vector<cluint> out;
      if(ga.Analyze(info,out,true)){
      Info("All Passed!");
      } else{
      HighLightText(s,ConsoleForeground::WHITE,out,ConsoleForeground::RED);
      }
      */
    }
  }
  /*
  vector<ConfigFileItem> cfis;
  ConfigParser cp;
  cp.parse(&cc,&cfis);
  FileCoper fc;
  fc.copyStart(&cfis);
*/
}

clint main(clint argc,clchar* argv[]){
  //char * c_title="Coper version:1.0.1";
  //SetConsoleTitle(c_title);


  Unimportant("Hello Coper! ",false,false);
  Unimportant(VERSION,true,false);
  Unimportant("coper.exe URL : "+clstr(argv[0]),true,false);

#if 1
  const clstr configFileURL("./config.txt");
  Unimportant("config file URL : "+configFileURL,true,false);
  Run(configFileURL.c_str());
#else
  appURL=argv[0];

  if(argc==2){
    Unimportant("config file URL : "+argv[1],true,false);
    Run(argv[1]);
  } else{
    Error("请将配置文件拖入coperC.exe文件");
    //configFileURL="config.txt";
  }
#endif
  system("pause");
  return 0;
}
