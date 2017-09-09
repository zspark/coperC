#include <fstream>
#include "config_file_loader.h"
#include "clRegexpUtil.h"
#include "clPrinter.h"
#include "parameter_parser.h"

using namespace std;
using namespace cl;

ConfigFileLoader::ConfigFileLoader(cluint defaultColor,cluint highlightColor)
  :m_defaultPrintColor(defaultColor),m_highLightPrintColor(highlightColor){}

ConfigFileLoader::~ConfigFileLoader(){}

clbool ConfigFileLoader::Load( clstr fileURL,std::vector<clstr>& out ,ParameterParser * pp){
  m_uParameterLineCount=0;
  m_sConfigFileURL=fileURL;

  ifstream file_r(fileURL,ios::in);
  if(file_r){
    while(!file_r.eof()){
      char c[_MAX_PATH];
      file_r.getline(c,_MAX_PATH,'\n');
      if(clstr(c).empty()){
        m_uTotalBlankItemCount++;
        continue;
      }
      if(clRegexp::IsStartedWith(c,R"(#)",true)){
        m_uTotalCommentItemCount++;
        continue;
      }

      m_uTotalAvaliableItemCount++;
      const clstr itemStr=string(c);

      if(clRegexp::IsStartedWith(itemStr,"parameter",true)){
        if(m_uParameterLineCount++>1){
          Error("parameter item should NOT exist more than one!");
          return false;
        }
        m_sParameterLine=itemStr;
        if(!pp->Parse(itemStr)){
          return false;
        }
      }else
        out.push_back(itemStr);

      if(file_r.fail()){
        file_r.clear(file_r.rdstate()&~ifstream::failbit);
      }
    }

  }
  file_r.close();
  if(m_uParameterLineCount==0){
    //Error("There is no parameter item exists, please check your config file.");
    return false;
  } else if(m_uParameterLineCount>1){
    Error("There are more than one parameter items exist, please check your config file.");
    return false;
  }
  return true;
}

void ConfigFileLoader::PrintInfo(){
#define print(s) Unimportant(s,true,false)
  print("|=================================================");
  print("| CONIFG FILE INFOMATIONS:");
  print("|-------------------------------------------------");
  print("| Config file URL : "+m_sConfigFileURL);
  print("| Total blank lines: "+clTypeUtil::NumberToString(m_uTotalBlankItemCount-1));
  print("| Total comment lines: "+clTypeUtil::NumberToString(m_uTotalCommentItemCount));
  print("| Total avaliable lines: "+clTypeUtil::NumberToString(m_uTotalAvaliableItemCount));
  print("| Parameter line: "+m_sParameterLine);
  print("|=================================================");
#undef print
}
