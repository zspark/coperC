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
#include "parameter_parser.h"
#include "analyzer.h"
#include "assembling.h"
#include "interactive_controller.h"
#include "config_file_loader.h"
#include "folder_file_validation.h"
#include "file_handler.h"

using namespace std;
using namespace cl;

void Run(const clchar* filename){
  InteractiveController ic;
  ParameterParser pp;

  //----------------------------------------------------------------------------------------------------
  // STEP 1-1;
  vector<clstr> needParsingItems;
  ConfigFileLoader cfl(ConForeColor::GRAY,ConForeColor::RED);
  if(cfl.Load(filename,needParsingItems,&pp)){
    // load successfully.
    cfl.PrintInfo();
    if(pp.NeedRequest()) if(!ic.RequestAnalyzing())return;
  } else return;

  //----------------------------------------------------------------------------------------------------
  // STEP 1-2
  const cluint needParsingItemCount=needParsingItems.size();
  cluint avaliableItemCount=0;
  GrammarAnalyzer ga(ConForeColor::GRAY,ConForeColor::RED,pp.IsVerbose());
  Assembling ab(ConForeColor::GRAY,ConForeColor::RED,pp.IsVerbose());
  for(clint i=0;i<needParsingItemCount;i++){
    const clstr s=needParsingItems[i];
    if(pp.IsVerbose()){
      cons->NewLine();
      T(s,true);
    }

    if(ga.Analyze(s)){
      avaliableItemCount++;
      ab.Assemble(ga.GetLexicalInfoVec());
    }
    ga.CleanCache();
  }
  hsass* about=ab.GetAssembledHS();
  cons->NewLine();
  T("Config file analysing finished!",true);
  
#if _DEBUG
  about->Print();
#endif
  
  //----------------------------------------------------------------------------------------------------
  // STEP 2;
  if(pp.NeedRequest()) if(!ic.RequestValidation())return;
  vector<clstr> vecFileURL;
  FolderFileValidation ffv(pp.GetRootPath()
    ,ConForeColor::GRAY
    ,ConForeColor::RED
    ,pp.IsVerbose()
  );
  ffv.Validate(about,&vecFileURL);
  ffv.PrintInfo();
  ffv.CleanCache();

  //----------------------------------------------------------------------------------------------------
  // STEP 3;
  if(pp.IsCopy()){
    FileHandler::FHCopy(pp,vecFileURL);
    T("All files were copied to : "+pp.GetTargetPath(),true);
  }

  T("All Down!",true);
}

clint main(clint argc,clchar* argv[]){

  cons->SetTitle(VERSION);
  T("coper.exe URL : "+clstr(argv[0]),true);

#if _DEBUG
  const clstr configFileURL("./config.txt");
  Run(configFileURL.c_str());
#else

  if(argc==2){
    T("config file URL : "+clstr(argv[1]),true);
    Run(argv[1]);
  } else{
    I("请将配置文件拖入coperC.exe文件",true);
  }
#endif
  cl::clConsole::Release();
  return 0;
}
