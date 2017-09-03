#include "lexical_analyzer.h"
#include "clRegexpUtil.h"
#include "clTypeUtil.h"
#include "clPrinter.h"
#include "coper.h"

using namespace std;
using namespace cl;


void PrintError(clstr s,const LexicalInfo* info,clstr rawStr,cluint c1,cluint c2){
  Error(s,false);
  Text("\"",c1,false);
  vector<cluint> pos;
  pos.push_back(info->startIndex);
  pos.push_back(info->rawStrLen);
  HighLightText(rawStr,c1,pos,c2,false);
  Text("\"",c1,true);
}

LexicalAnalyzer::LexicalAnalyzer(cluint defaultColor,cluint highlightColor)
  :m_defaultPrintColor(defaultColor),m_highLightPrintColor(highlightColor){}

LexicalAnalyzer::~LexicalAnalyzer(){
  ResetLocal_();
}

clbool LexicalAnalyzer::Analyze(clstr str,clbool verbose){
  ResetLocal_();
  m_rawString=str;
  m_verbosePrint=verbose;

  if(m_verbosePrint){
    Info("Lexical analysing start...");
  }
  ParseToLexicalInfo_();
  if(ValidateNames_()){
    if(m_verbosePrint){
      Info("Lexical analysing finished, It's correct.");
    }
    return true;
  }

  if(m_verbosePrint){
    Error("Lexical analysing failed, Executing terminated!");
  }
  return false;
}

void LexicalAnalyzer::ParseToLexicalInfo_(){
  clchar currentChar;
  clchar next;
  const cluint n=m_rawString.length();
  for(clint i=0;i<n;){
    currentChar=m_rawString[i];
    switch(currentChar){
    case C_MARK_LEFT_BRACKET:
    case C_MARK_V_LINE:
    case C_MARK_SLASH:
      m_vecInfos.push_back(new LexicalInfo(clstr(&currentChar,1),i,LexicalInfoType::KEY_WORD));
      i+=1;
      break;
    case C_MARK_RIGHT_BRACKET:
      next=m_rawString[i+1];
      if(next && next==C_MARK_RIGHT_BRACKET){
        m_vecInfos.push_back(new LexicalInfo(MARK_DOUBLE_RIGHT_BRACKET,i,LexicalInfoType::KEY_WORD));
        i+=2;
        // the following are all regexp context;
        clstr subS=m_rawString.substr(i);
        m_vecInfos.push_back(new LexicalInfo(subS,i,LexicalInfoType::REGEXP));
        return;
      } else{
        m_vecInfos.push_back(new LexicalInfo(clstr(&currentChar,1),i,LexicalInfoType::KEY_WORD));
        i+=1;
      }
      break;
    default:
      clstr subS=m_rawString.substr(i);
      vector<clstr> out;
      clRegexp::GetFirstMatch(subS,R"([^<>\|/]+)",out,true);
      subS=out[0];
      m_vecInfos.push_back(new LexicalInfo(subS,i,LexicalInfoType::NAME));
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
    currentLex=m_vecInfos[i];
    if(currentLex->type==LexicalInfoType::NAME){
      // normal string;
      if(IsCoperAllowedName(currentLex->rawStr)){
        clstr tmp;
        const LexicalInfo* info=GetLexicalInfo_(i-1);
        if(info && IsRightBracketKeyword(info->rawStr)){
          // common extension name;
          tmp=clTypeUtil::StringTrimLeft(currentLex->rawStr);
          if(!clRegexp::IsStartedWith(tmp,R"(.)")){
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
        PrintError("name is illegal:",currentLex,m_rawString,
          m_defaultPrintColor,m_highLightPrintColor);
        return false;
      }
    }
  }
  return true;
}

void LexicalAnalyzer::ResetLocal_(){
  m_rawString="";
  m_verbosePrint=false;
  for(LexicalInfo* l: m_vecInfos) delete l;
  m_vecInfos.clear();
}

LexicalInfo* LexicalAnalyzer::GetLexicalInfo_(clint index){
  if(index>=m_vecInfos.size()||index<0)return nullptr;
  return m_vecInfos[index];
}

inline void LexicalAnalyzer::PrintFixedName_(clstr s,LexicalInfo* info){
  if(m_verbosePrint){
    Warning(s,false);
    Warning("\""+info->rawStr+"\"  ->  \""+info->fixedStr+"\"",true,false);
  }
}
