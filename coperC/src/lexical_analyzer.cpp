#include "lexical_analyzer.h"
#include "clRegexpUtil.h"
#include "clTypeUtil.h"
#include "clPrinter.h"
#include "coper.h"

using namespace std;
using namespace cl;

LexicalAnalyzer::LexicalAnalyzer(cluint defaultColor,cluint highlightColor)
  :m_defaultPrintColor(defaultColor),m_highLightPrintColor(highlightColor){}

LexicalAnalyzer::~LexicalAnalyzer(){
  m_vecInfos.clear();
}

clbool LexicalAnalyzer::Analyze(clstr str,clbool verbose){
  ResetLocal_();
  m_rawString=str;
  m_verbosePrint=verbose;

  if(m_verbosePrint){
    NewLine();
    Info("Lexical analysing start...");
    Info("Config string is:\""+str+"\"");
  }
  ParseToLexicalInfo_();
  if(ValidateNames_()){
    if(m_verbosePrint){
      Info("Lexical analysing finished, It's correct.");
    }
    return true;
  }

  /*
  if(m_verbosePrint){
    LexicalInfo info(R"(/\:*?"<>|)",0);
    PrintErrorName_("Name should not contain:",&info,info.rawStr,1);
  }
  */
  Error("Lexical analysing failed, Executing terminated!");
  return false;
}

void LexicalAnalyzer::ParseToLexicalInfo_(){
  clchar currentChar;
  const cluint n=m_rawString.length();
  for(clint i=0;i<n;){
    currentChar=m_rawString[i];
    switch(currentChar){
    case '<':
    case '>':
    case '|':
    case '/':
    case '"':
      m_vecInfos.emplace_back(clstr(&currentChar,1),i,true);
      i+=1;
      break;
    default:
      clstr subS=m_rawString.substr(i);
      vector<clstr> out;
      clRegexp::GetFirstMatch(subS,R"([^<>\|/"]+)",out,true);
      subS=out[0];
      m_vecInfos.emplace_back(subS,i,false);
      i+=subS.size();
      break;
    }
  }
}

clbool LexicalAnalyzer::ValidateNames_(){
  LexicalInfo* currentLex=nullptr;
  const cluint n=m_vecInfos.size();
  cluint i=0;
  for(;i<n;i++){
    currentLex=&m_vecInfos[i];
    if(!currentLex->isKeyword){
      // normal string;
      if(IsCoperAllowedName(currentLex->rawStr)){
        clstr tmp;
        const LexicalInfo* info=GetLexicalInfo(i-1);
        if(info && IsRightBracketKeyword(info->rawStr)){
          // common extension name;
          tmp=clTypeUtil::StringTrimLeft(currentLex->rawStr);
          if(!clRegexp::IsStartedWith(tmp,R"(^\.)")){
            tmp="."+tmp;
          }
        } else{
          tmp=clTypeUtil::StringTrim(currentLex->rawStr);
        }
        if(tmp.length()!=currentLex->rawStr.length()){
          currentLex->fixedStr=tmp;
          PrintFixedName_("fixed name:",currentLex);
        }
      } else{
        PrintErrorName_("name is illegal:",currentLex,m_rawString);
        return false;
      }
    }
  }
  return true;
}

void LexicalAnalyzer::ResetLocal_(){
  m_rawString="";
  m_verbosePrint=false;
  m_vecInfos.clear();
}

LexicalInfo* LexicalAnalyzer::GetLexicalInfo(clint index){
  if(index>=m_vecInfos.size()||index<0)return nullptr;
  return &m_vecInfos[index];
}

inline void LexicalAnalyzer::PrintFixedName_(clstr s,LexicalInfo* info){
  if(m_verbosePrint){
    Warning(s,false);
    Warning("\""+info->rawStr+"\"  ->  \""+info->fixedStr+"\"",true,false);
  }
}

inline void LexicalAnalyzer::PrintErrorName_(clstr s,LexicalInfo* info,clstr str,cluint mode){
  if(mode==0) Error(s,false);
  else if(mode==1)Warning(s,false);
  Text("\"",m_defaultPrintColor,false);
  vector<cluint> pos;
  pos.push_back(info->startIndex);
  pos.push_back(info->rawStrLen);
  HighLightText(str,m_defaultPrintColor,pos,m_highLightPrintColor,false);
  Text("\"",m_defaultPrintColor,true);
}
