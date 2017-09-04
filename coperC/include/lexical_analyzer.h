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

void PrintError(clstr s,const LexicalInfo* info,clstr rawStr,
  cluint c1,cluint c2);

class LexicalAnalyzer final{
public:
  LexicalAnalyzer(cluint defaultColor,cluint highlightColor);
  ~LexicalAnalyzer();
  clbool Analyze(clstr str,clbool verbose=false);
  const std::vector<LexicalInfo*>& GetLexicalInfoVec()const{ return m_vecInfos; }

private:
  void ResetLocal_();
  void ParseToLexicalInfo_();
  clbool ValidateNames_();
  inline void PrintFixedName_(clstr s,LexicalInfo* info);
  inline LexicalInfo* GetLexicalInfo_(clint index);

private:
  clstr m_rawString;
  clbool m_verbosePrint;
  std::vector<LexicalInfo*> m_vecInfos;

  const cluint m_defaultPrintColor;
  const cluint m_highLightPrintColor;
};
