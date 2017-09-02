#pragma once

#include <vector>
#include "cllib.h"

struct LexicalInfo{
  LexicalInfo(clstr s,cluint index,clbool keyword)
    :rawStr(s),startIndex(index),rawStrLen(s.size()),isKeyword(keyword){
    fixedStr=s;
  }
  const clstr rawStr;
  const cluint startIndex;
  const cluint rawStrLen;
  const clbool isKeyword;
  clstr fixedStr;
};

class LexicalAnalyzer final{
public:
  LexicalAnalyzer(cluint defaultColor,cluint highlightColor);
  ~LexicalAnalyzer();
  clbool Analyze(clstr str,clbool verbose=false);
  const std::vector<LexicalInfo>& GetLexicalInfoVec()const{ return m_vecInfos; }

private:
  void ResetLocal_();
  void ParseToLexicalInfo_();
  clbool ValidateNames_();
  inline void PrintFixedName_(clstr s,LexicalInfo* info);
  inline void PrintErrorName_(clstr s,LexicalInfo* info,clstr str,cluint mode=0);
  inline LexicalInfo* GetLexicalInfo(clint index);

private:
  clstr m_rawString;
  clbool m_verbosePrint;
  std::vector<LexicalInfo> m_vecInfos;

  const cluint m_defaultPrintColor;
  const cluint m_highLightPrintColor;
};
