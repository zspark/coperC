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
  m_rawStringLen=str.length();

  if(m_verbosePrint){
    NewLine();
    Info("Lexical analysing start...");
    Info("Config string is:\""+str+"\"");
  }
  if(ParseToLexicalInfo_()){
    if(ValidateNames_()){
      if(m_verbosePrint){
        Info("Lexical analysing finished, It's correct.");
      }
      return true;
    }
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

clbool LexicalAnalyzer::ParseToLexicalInfo_(){
  clchar currentChar;
  for(clint i=0;i<m_rawStringLen;){
    currentChar=m_rawString[i];
    switch(currentChar){
    case '/':
    case '<':
    case '>':
    case '|':
      m_vecInfos.emplace_back(clstr(&currentChar,1),i);
      i+=1;
      break;
    default:
      clstr subS=m_rawString.substr(i);
      vector<clstr> out;
      clRegexp::GetFirstMatch(subS,R"([^/<>|]+)",out,true);
      subS=out[0];
      m_vecInfos.emplace_back(subS,i);
      i+=subS.size();
      break;
    }
  }
  return true;
}

clbool LexicalAnalyzer::ValidateNames_(){
  LexicalInfo* currentLex=nullptr;
  const cluint n=m_vecInfos.size();
  cluint i=0;
  for(;i<n;i++){
    currentLex=&m_vecInfos[i];
    if(IsFolderKeyword(currentLex->rawStr)){
      LexicalInfo* tmp=GetLexicalInfo(i-1);
      if(tmp){
        if(!ValidateFolderName_(tmp))return false;
      }
    } else if(IsRightBracketKeyword(currentLex->rawStr)){
      LexicalInfo* tmp=GetLexicalInfo(i-1);
      if(tmp){
        if(!ValidateFileName_(tmp))return false;
      }
    } else if(IsVLineKeyword(currentLex->rawStr)){
      LexicalInfo* tmp=GetLexicalInfo(i-1);
      if(tmp){
        if(!ValidateFileName_(tmp))return false;
      }
    }
  }
  if(!IsAnyKeyword(currentLex->rawStr)){
    //only file name or extension name is here;
    LexicalInfo* tmp=GetLexicalInfo(i-2);
    if(tmp){
      if(IsFolderKeyword(tmp->rawStr)){
        if(!ValidateFileName_(currentLex))return false;
      } else{
        if(!ValidateExtensionName_(currentLex))return false;
      }
    } else{
      if(!ValidateFileName_(currentLex))return false;
    }
  }
  return true;
}

void LexicalAnalyzer::ResetLocal_(){
  m_rawString="";
  m_verbosePrint=false;
  m_rawStringLen=0;
  m_vecInfos.clear();
}

clbool LexicalAnalyzer::ValidateFolderName_(LexicalInfo* info){
  if(!IsPotentialFolderName(info->rawStr)){
    PrintErrorName_("directory name is illegal:",info,m_rawString);
    return false;
  }
  clstr tmp=clTypeUtil::StringTrim(info->rawStr);
  if(tmp.length()!=info->rawStr.length()){
    info->fixedStr=tmp;
    PrintFixedName_("fixed directory name:",info);
  }
  return true;
}

clbool LexicalAnalyzer::ValidateFileName_(LexicalInfo* info){
  if(!IsPotentialFileName(info->rawStr)){
    PrintErrorName_("file name is illegal:",info,m_rawString);
    return false;
  }
  clstr tmp=clTypeUtil::StringTrim(info->rawStr);
  if(tmp.length()!=info->rawStr.length()){
    info->fixedStr=tmp;
    PrintFixedName_("fixed file name:",info);
  }
  return true;
}

clbool LexicalAnalyzer::ValidateExtensionName_(LexicalInfo* info){
  if(!IsPotentialCommonExtensionName(info->rawStr)){
    PrintErrorName_("extension name is illegal:",info,m_rawString);
    return false;
  }
  clbool flag=false;
  if(!clRegexp::IsStartedWith(info->rawStr,R"(^\.)")){
    info->fixedStr="."+info->rawStr;
    flag=true;
  }
  clstr tmp=clTypeUtil::StringTrimRight(info->fixedStr);
  if(tmp.length()!=info->rawStr.length()){
    info->fixedStr=tmp;
  }
  if(flag)PrintFixedName_("fixed extension name:",info);
  return true;
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
