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
#include <vector>
#include "coper.h"
#include "clTypeUtil.h"
#include "parameter_parser.h"
#include "analyzer.h"
#include "assembling.h"
#include "interactive_controller.h"
#include "config_file_loader.h"
#include "folder_file_validation.h"

using namespace std;
using namespace cl;

void Run(const clchar* filename){
  InteractiveController ic;
  ParameterParser pp;

  //----------------------------------------------------------------------------------------------------
  // STEP 1-1;
  vector<clstr> needParsingItems;
  ConfigFileLoader cfl(ConsoleForeground::GRAY,ConsoleForeground::RED);
  if(cfl.Load(filename,needParsingItems,&pp)){
    // load successfully.
    cfl.PrintInfo();
    if(pp.NeedRequest()) if(!ic.RequestAnalyzing())return;
  } else return;

  //----------------------------------------------------------------------------------------------------
  // STEP 1-2
  const cluint needParsingItemCount=needParsingItems.size();
  cluint avaliableItemCount=0;
  GrammarAnalyzer ga(ConsoleForeground::GRAY,ConsoleForeground::RED,pp.IsVerbose());
  Assembling ab(ConsoleForeground::GRAY,ConsoleForeground::RED,pp.IsVerbose());
  for(clint i=0;i<needParsingItemCount;i++){
    const clstr s=needParsingItems[i];
    if(pp.IsVerbose()){
      NewLine();
      Unimportant(s,true,false);
    }

    if(ga.Analyze(s)){
      avaliableItemCount++;
      ab.Assemble(ga.GetLexicalInfoVec());
    }
    ga.CleanCache();
  }
  const vector<AssembleInfo*> about=ab.GetAssembledInfos();
  NewLine();
  Unimportant("Config file analysing finished!");
  Unimportant("Assembled simple item count : "+clTypeUtil::NumberToString(about.size()));
  
  //----------------------------------------------------------------------------------------------------
  // STEP 2;
  if(pp.NeedRequest()) if(!ic.RequestValidation())return;
  vector<clstr> vecFileURL;
  FolderFileValidation ffv(pp.GetRootPath()
    ,ConsoleForeground::GRAY
    ,ConsoleForeground::RED
    ,pp.IsVerbose()
  );
  ffv.Validate(about,&vecFileURL);
  ffv.PrintInfo();
  ffv.CleanCache();

  //----------------------------------------------------------------------------------------------------
  // STEP 3;
  Info("OK");
}

clint main(clint argc,clchar* argv[]){
  //char * c_title="Coper version:1.0.1";
  //SetConsoleTitle(c_title);


  Unimportant("Hello Coper! ",false,false);
  Unimportant(VERSION,true,false);
  Unimportant("coper.exe URL : "+clstr(argv[0]),true,false);

#if 1
  const clstr configFileURL("./config.txt");
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
