#include "grammar_analyzer.h"
#include "clTypeUtil.h"
#include "clPrinter.h"
#include "lexical_analyzer.h"
#include "coper.h"
#include "clRegexpUtil.h"
#include "clPrinter.h"

using namespace std;
using namespace cl;

GrammarAnalyzer::GrammarAnalyzer(){}

GrammarAnalyzer::~GrammarAnalyzer(){
  ResetLocal_();
}

clbool GrammarAnalyzer::Analyze(clstr rawStr,const vector<LexicalInfo*>& in,clbool verbose){
  ResetLocal_();
  m_sRawStr=rawStr;
  m_vecInfos=in;
  m_bVerbosePrint=verbose;

  if(m_bVerbosePrint){
    Info("Gammar analysing start...");
  }

  const LexicalInfo* info=nullptr;
  const cluint n=in.size();
  for(clint i=0;i<n;i++){
    LexicalInfo* info=in[i];
    switch(info->type){
    case LexicalInfoType::KEY_WORD:
      if(!AnalyzeKeyword_(i))return false;
      break;
    case LexicalInfoType::NAME:
      if(!AnalyzeName_(i))return false;
      break;
    case LexicalInfoType::REGEXP:
      if(!AnalyzeRegexp_(i))return false;
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
  const LexicalInfo* preInfo=GetLexicalInfoAt_(currentOffset-1);
  if(m_uBracketCount>0){
    Error(R"("/" MUST appear before "<>")");
    return false;
  }
  if(preInfo){
    if(preInfo->type==LexicalInfoType::NAME){
      return true;
    } else{
      Error(R"(there MUST be folder or file names before "/")");
      return false;
    }
  } else{
    Error(R"(there MUST be folder or file names before "/")");
    return false;
  }
}

clbool GrammarAnalyzer::AnalyzeKeywordVLine_(cluint currentOffset){
  const LexicalInfo* preInfo=GetLexicalInfoAt_(currentOffset-1);
  if(m_uBracketCount==0 || m_uBracketCount>1){
    Error(R"("|" MUST appear before "<>")");
    return false;
  } else{
    if(preInfo){
      if(preInfo->type==LexicalInfoType::NAME){
        return true;
      } else{
        Error(R"(there MUST be folder or file names before "|")");
        return false;
      }
    } else{
      Error(R"("|" MUST NOT appear at the beginning!)");
      return false;
    }
  }
}

clbool GrammarAnalyzer::AnalyzeKeywordLeftBracket_(cluint currentOffset){
  const LexicalInfo* preInfo=GetLexicalInfoAt_(currentOffset-1);
  if(m_uBracketCount==0){
    m_uBracketCount++;
    if(preInfo){
      if(IsFolderKeyword(preInfo->fixedStr)){
        return true;
      } else{
        Error(R"(there MUST BE "/" before "<",unless "<" starts at the beginning.)");
        return false;
      }
    } else{
      return true;
    }
  } else if(m_uBracketCount==1){
    Error(R"(group can NOT nested!)");
    return false;
  } else{
    Error(R"(group can only has one at most!)");
    return false;
  }
}

clbool GrammarAnalyzer::AnalyzeKeywordRightBracket_(cluint currentOffset){
  const LexicalInfo* preInfo=GetLexicalInfoAt_(currentOffset-1);
  if(m_uBracketCount==0){
    Error(R"(">" MUST NOT appear before "<")");
    return false;
  } else if(m_uBracketCount==1){
    m_uBracketCount++;
    if(preInfo){
      if(preInfo->type==LexicalInfoType::NAME){
        return true;
      } else{
        Error(R"(there MUST be folder or file names before ">")");
        return false;
      }
    } else{
      Error(R"(">" MUST NOT appear at the beginning!)");
      return false;
    }
  } else if(m_uBracketCount>1){
    Error(R"(group can only has one at most!)");
    return false;
  }
}

clbool GrammarAnalyzer::AnalyzeKeywordDoubleRightBracket_(cluint currentOffset){
  const LexicalInfo* preInfo=GetLexicalInfoAt_(currentOffset-1);
  if(preInfo){
    if(IsFolderKeyword(preInfo->fixedStr)){
      return true;
    } else{
      Error(R"(there MUST BE "/" before ">>" while it's not at the beginning!)");
      return false;
    }
  } else{
    return true;
  }
}

clbool GrammarAnalyzer::AnalyzeName_(cluint currentOffset){
  const LexicalInfo* preInfo=GetLexicalInfoAt_(currentOffset-1);
  return true;
}

clbool GrammarAnalyzer::AnalyzeRegexp_(cluint currentOffset){
  const LexicalInfo* info=GetLexicalInfoAt_(currentOffset);
  if(info){
    const LexicalInfo* preInfo=GetLexicalInfoAt_(currentOffset-1);
    if(preInfo){
      if(IsDoubleRightBracketKeyword(preInfo->fixedStr)){
        return true;
      } else{
        Error(R"(there MUST BE ">>" before regular expression!)");
        return false;
      }
    } else{
      Error(R"(regular expression MUST NOT at the beginning!)");
      return false;
    }
  }

}

void GrammarAnalyzer::ResetLocal_(){
  m_bVerbosePrint=false;
  m_uBracketCount=0;
  m_sRawStr="";
  m_vecInfos.clear();
}
