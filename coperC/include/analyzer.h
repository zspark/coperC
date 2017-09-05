#pragma once

#include <vector>
#include "cllib.h"

namespace LexicalInfoType{
enum infotype{
  KEY_WORD=0,
  NAME,
  REGEXP,
  COMMON_EXTENSION,
};
}

struct LexicalInfo{
  LexicalInfo(clstr s,cluint index,LexicalInfoType::infotype t)
    :rawStr(s),startIndex(index),rawStrLen(s.size()),type(t){
    fixedStr=s;
  }
  LexicalInfo(const LexicalInfo& info)=delete;
  LexicalInfo& operator=(const LexicalInfo& info)=delete;

  const clstr rawStr;
  const cluint startIndex;
  const cluint rawStrLen;
  const LexicalInfoType::infotype type;
  clstr fixedStr;
};

class GrammarAnalyzer final{
public:
  GrammarAnalyzer(const GrammarAnalyzer&)=delete;
  GrammarAnalyzer(const GrammarAnalyzer*)=delete;
  GrammarAnalyzer(cluint defaultColor,cluint highlightColor,clbool verbose);
  ~GrammarAnalyzer();
  clbool Analyze(clstr rawStr);
  void CleanCache();
  const std::vector<LexicalInfo*>& GetLexicalInfoVec()const{ return m_vecInfos; }

private:
  //----------------------------------------------------------------------------------------------------
  // common;
  void PrintError_(clstr s,const LexicalInfo* info,clstr rawStr,cluint c1,cluint c2);
  //----------------------------------------------------------------------------------------------------
  // Lexical analyzer
  clbool AnalyzeLexical_();
  void ParseToLexicalInfo_();
  clbool ValidateNames_();
  inline void PrintFixedName_(clstr s,LexicalInfo* info);
  //----------------------------------------------------------------------------------------------------
  // Grammar analyzer
  clbool AnalyzeGrammar_();
  clbool AnalyzeKeyword_(cluint currentOffset);
  clbool AnalyzeKeywordFolder_(cluint currentOffset);
  clbool AnalyzeKeywordVLine_(cluint currentOffset);
  clbool AnalyzeKeywordLeftBracket_(cluint currentOffset);
  clbool AnalyzeKeywordRightBracket_(cluint currentOffset);
  clbool AnalyzeKeywordDoubleRightBracket_(cluint currentOffset);

  clbool AnalyzeName_(cluint currentOffset);
  clbool AnalyzeRegexp_(cluint currentOffset);
  inline const LexicalInfo* GetLexicalInfoAt_(clint index)const;
  //----------------------------------------------------------------------------------------------------

private:
  clbool m_bVerbosePrint;
  clstr m_sRawStr;
  std::vector<LexicalInfo*> m_vecInfos;
  cluint m_uBracketCount=0;

  const cluint m_defaultPrintColor;
  const cluint m_highLightPrintColor;
};