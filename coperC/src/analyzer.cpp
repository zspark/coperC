#include "analyzer.h"
#include "clTypeUtil.h"
#include "clPrinter.h"
#include "coper.h"
#include "clRegexpUtil.h"
#include "clPrinter.h"

using namespace std;
using namespace cl;

#define PrintGrammarError(s,info) PrintError_(s,info,m_sRawStr,m_defaultPrintColor,m_highLightPrintColor);

GrammarAnalyzer::GrammarAnalyzer(cluint defaultColor,cluint highlightColor,clbool verbose)
  :m_defaultPrintColor(defaultColor),m_highLightPrintColor(highlightColor),m_bVerbosePrint(verbose){}

GrammarAnalyzer::~GrammarAnalyzer(){ }

clbool GrammarAnalyzer::Analyze(clstr rawStr){
  m_sRawStr=rawStr;

  if(AnalyzeLexical_())
    return AnalyzeGrammar_();
  else return false;
}
clbool GrammarAnalyzer::AnalyzeLexical_(){

  if(m_bVerbosePrint){
    Info("Lexical analysing start...");
  }

  ParseToLexicalInfo_();
  if(ValidateNames_()){
    if(m_bVerbosePrint){
      Info("Lexical analysing finished, It's correct.");
    }
    return true;
  }

  if(m_bVerbosePrint){
    Error("Lexical analysing failed, Executing terminated!");
  }
  return false;
}

clbool GrammarAnalyzer::AnalyzeGrammar_(){

  if(m_bVerbosePrint){
    Info("Gammar analysing start...");
  }

  const LexicalInfo* info=nullptr;
  const cluint n=m_vecInfos.size();
  for(clint i=0;i<n;i++){
    LexicalInfo* info=m_vecInfos[i];
    switch(info->type){
    case LexicalInfoType::KEY_WORD:
      if(!AnalyzeKeyword_(i)){
        return false;
      }
      break;
    case LexicalInfoType::COMMON_EXTENSION:
    case LexicalInfoType::NAME:
      if(!AnalyzeName_(i)){
        return false;
      }
      break;
    case LexicalInfoType::REGEXP:
      if(!AnalyzeRegexp_(i)){
        return false;
      }
      break;
    default:
      clchar tmp[255];
      sprintf_s(tmp,"UNKNOW ERROR! rawString=%s , startIndex=%d , len=%d",info->rawStr,info->startIndex,info->rawStrLen);
      Error(tmp);
      return false;
      break;
    }
  }

  if(m_bVerbosePrint){
    Info("Gammar analysing finished, It's correct.");
  }

  return true;
}

inline const LexicalInfo * GrammarAnalyzer::GetLexicalInfoAt_(clint index) const{
  if(index<0||index>=m_vecInfos.size())return nullptr;
 return m_vecInfos[index];
}

clbool GrammarAnalyzer::AnalyzeKeyword_(cluint currentOffset){
  const LexicalInfo* info=GetLexicalInfoAt_(currentOffset);
  F_DBG_ASSERT(info);
  if(info){
    const clstr context=info->fixedStr;
    if(strcmp(context.c_str(),MARK_LEFT_BRACKET)==0){
      return AnalyzeKeywordLeftBracket_(currentOffset);
    } else if(strcmp(context.c_str(),MARK_RIGHT_BRACKET)==0){
      return AnalyzeKeywordRightBracket_(currentOffset);
    } else if(strcmp(context.c_str(),MARK_DOUBLE_RIGHT_BRACKET)==0){
      return AnalyzeKeywordDoubleRightBracket_(currentOffset);
    } else if(strcmp(context.c_str(),MARK_SLASH)==0){
      return AnalyzeKeywordFolder_(currentOffset);
    } else if(strcmp(context.c_str(),MARK_V_LINE)==0){
      return AnalyzeKeywordVLine_(currentOffset);
    }
  }
  return false;
}

clbool GrammarAnalyzer::AnalyzeKeywordFolder_(cluint currentOffset){
  const LexicalInfo* info=GetLexicalInfoAt_(currentOffset);
  const LexicalInfo* preInfo=GetLexicalInfoAt_(currentOffset-1);
  if(m_uBracketCount>0){
    PrintGrammarError(R"("/" MUST appear before "<>":)",info);
    return false;
  }
  if(preInfo){
    if(preInfo->type==LexicalInfoType::NAME){
      return true;
    } else{
      PrintGrammarError(R"(there MUST be folder or file names before "/":)",preInfo);
      return false;
    }
  } else{
    PrintGrammarError(R"(there MUST be folder or file names before "/":)",preInfo);
    return false;
  }
}

clbool GrammarAnalyzer::AnalyzeKeywordVLine_(cluint currentOffset){
  const LexicalInfo* info=GetLexicalInfoAt_(currentOffset);
  const LexicalInfo* preInfo=GetLexicalInfoAt_(currentOffset-1);
  if(m_uBracketCount==0 || m_uBracketCount>1){
    PrintGrammarError(R"("|" MUST appear before "<>":)",preInfo);
    return false;
  } else{
    if(preInfo){
      if(preInfo->type==LexicalInfoType::NAME){
        if(IsStarOnly(preInfo->fixedStr)){
          PrintGrammarError(R"(组中不能使用通配符！:)",preInfo);
          return false;
        }else return true;
      } else{
        PrintGrammarError(R"(there MUST be folder or file names before "|":)",preInfo);
        return false;
      }
    } else{
      PrintGrammarError(R"("|" MUST NOT appear at the beginning!:)",preInfo);
      return false;
    }
  }
}

clbool GrammarAnalyzer::AnalyzeKeywordLeftBracket_(cluint currentOffset){
  const LexicalInfo* info=GetLexicalInfoAt_(currentOffset);
  const LexicalInfo* preInfo=GetLexicalInfoAt_(currentOffset-1);
  if(m_uBracketCount==0){
    m_uBracketCount++;
    if(preInfo){
      if(IsFolderKeyword(preInfo->fixedStr)){
        return true;
      } else{
        PrintGrammarError(R"(there MUST BE "/" before "<",unless "<" starts at the beginning.:)",preInfo);
        return false;
      }
    } else{
      return true;
    }
  } else if(m_uBracketCount==1){
    PrintGrammarError(R"(group can NOT nested!:)",preInfo);
    return false;
  } else{
    PrintGrammarError(R"(group can only has one at most!:)",preInfo);
    return false;
  }
}

clbool GrammarAnalyzer::AnalyzeKeywordRightBracket_(cluint currentOffset){
  const LexicalInfo* info=GetLexicalInfoAt_(currentOffset);
  const LexicalInfo* preInfo=GetLexicalInfoAt_(currentOffset-1);
  if(m_uBracketCount==0){
    PrintGrammarError(R"(">" MUST NOT appear before "<":)",info);
    return false;
  } else if(m_uBracketCount==1){
    m_uBracketCount++;
    if(preInfo){
      if(preInfo->type==LexicalInfoType::NAME){
        if(IsStarOnly(preInfo->fixedStr)){
          PrintGrammarError(R"(组中不能使用通配符！:)",info);
          return false;
        }else return true;
      } else{
        PrintGrammarError(R"(there MUST be folder or file names before ">":)",info);
        return false;
      }
    } else{
      PrintGrammarError(R"(">" MUST NOT appear at the beginning!:)",info);
      return false;
    }
  } else if(m_uBracketCount>1){
    PrintGrammarError(R"(group can only has one at most!:)",info);
    return false;
  }
}

clbool GrammarAnalyzer::AnalyzeKeywordDoubleRightBracket_(cluint currentOffset){
  const LexicalInfo* info=GetLexicalInfoAt_(currentOffset);
  const LexicalInfo* preInfo=GetLexicalInfoAt_(currentOffset-1);
  const LexicalInfo* nextInfo=GetLexicalInfoAt_(currentOffset+1);
  if(preInfo){
    if(IsFolderKeyword(preInfo->fixedStr)){
      if(m_uBracketCount==0){
        if(nextInfo && nextInfo->type==LexicalInfoType::REGEXP) return true;
        else{
          PrintGrammarError(R"(">>" MUST ended with regular expresion:)",info);
          return false;
        }
      } else{
        PrintGrammarError(R"(">>" MUST appear without "<>"!:)",info);
        return false;
      }
    } else{
      PrintGrammarError(R"(there MUST BE "/" before ">>" while it's not at the beginning!:)",info);
      return false;
    }
  } else{
    return true;
  }
}

clbool GrammarAnalyzer::AnalyzeName_(cluint currentOffset){
  return true;
}

clbool GrammarAnalyzer::AnalyzeRegexp_(cluint currentOffset){
  const LexicalInfo* info=GetLexicalInfoAt_(currentOffset);
  F_DBG_ASSERT(info);
  if(info){
    const LexicalInfo* preInfo=GetLexicalInfoAt_(currentOffset-1);
    if(preInfo){
      if(IsDoubleRightBracketKeyword(preInfo->fixedStr)){
        return true;
      } else{
        PrintGrammarError(R"(there MUST BE ">>" before regular expression!:)",info);
        return false;
      }
    } else{
      PrintGrammarError(R"(regular expression MUST NOT at the beginning!:)",info);
      return false;
    }
  }
}

void GrammarAnalyzer::ParseToLexicalInfo_(){
  clchar currentChar;
  clchar next,pre;
  const cluint n=m_sRawStr.length();
  for(clint i=0;i<n;){
    currentChar=m_sRawStr[i];
    switch(currentChar){
    case C_MARK_LEFT_BRACKET:
    case C_MARK_V_LINE:
    case C_MARK_SLASH:
      m_vecInfos.push_back(new LexicalInfo(clstr(&currentChar,1),i,LexicalInfoType::KEY_WORD));
      i+=1;
      break;
    case C_MARK_RIGHT_BRACKET:
      next=m_sRawStr[i+1];
      if(next && next==C_MARK_RIGHT_BRACKET){
        m_vecInfos.push_back(new LexicalInfo(MARK_DOUBLE_RIGHT_BRACKET,i,LexicalInfoType::KEY_WORD));
        i+=2;
        // the following are all regexp context;
        clstr subS=m_sRawStr.substr(i);
        m_vecInfos.push_back(new LexicalInfo(subS,i,LexicalInfoType::REGEXP));
        return;
      } else{
        m_vecInfos.push_back(new LexicalInfo(clstr(&currentChar,1),i,LexicalInfoType::KEY_WORD));
        i+=1;
      }
      break;
    default:
      clstr subS=m_sRawStr.substr(i);
      vector<clstr> out;
      clRegexp::GetFirstMatch(subS,R"([^<>\|/]+)",out,true);
      subS=out[0];
      if(i>0){
        pre=m_sRawStr[i-1];
        if(pre){
          if(pre==C_MARK_RIGHT_BRACKET){
            m_vecInfos.push_back(new LexicalInfo(subS,i,LexicalInfoType::COMMON_EXTENSION));
          } else{
            m_vecInfos.push_back(new LexicalInfo(subS,i,LexicalInfoType::NAME));
          }
        } else{
          m_vecInfos.push_back(new LexicalInfo(subS,i,LexicalInfoType::NAME));
        }
      }else
        m_vecInfos.push_back(new LexicalInfo(subS,i,LexicalInfoType::NAME));
      i+=subS.size();
      break;
    }
  }
}

clbool GrammarAnalyzer::ValidateNames_(){
  LexicalInfo* currentLex=nullptr;
  const cluint n=m_vecInfos.size();
  cluint i=0;
  for(;i<n;i++){
    currentLex=m_vecInfos[i];
    if(currentLex->type==LexicalInfoType::NAME
      || currentLex->type==LexicalInfoType::COMMON_EXTENSION ){
      // normal string;
      if(IsCoperAllowedName(currentLex->rawStr)){
        clstr tmp;
        const LexicalInfo* info=GetLexicalInfoAt_(i-1);
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
        PrintError_("name is illegal:",currentLex,m_sRawStr,
          m_defaultPrintColor,m_highLightPrintColor);
        return false;
      }
    }
  }
  return true;
}

void GrammarAnalyzer::CleanCache(){
  m_uBracketCount=0;
  m_sRawStr="";
  for(LexicalInfo* l: m_vecInfos) delete l;
  m_vecInfos.clear();
}

void GrammarAnalyzer::PrintError_(clstr s,const LexicalInfo* info,clstr rawStr,cluint c1,cluint c2){
  Error(s,false);
  Text("\"",c1,false);
  vector<cluint> pos;
  pos.push_back(info->startIndex);
  pos.push_back(info->rawStrLen);
  HighLightText(rawStr,c1,pos,c2,false);
  Text("\"",c1,true);
}

inline void GrammarAnalyzer::PrintFixedName_(clstr s,LexicalInfo* info){
  if(m_bVerbosePrint){
    Warning(s,false);
    Warning("\""+info->rawStr+"\"  ->  \""+info->fixedStr+"\"",true,false);
  }
}
