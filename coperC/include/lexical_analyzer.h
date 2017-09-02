#pragma once

#include <vector>
#include "cllib.h"

struct LexicalInfo{
  LexicalInfo(clstr s,cluint index)
    :rawStr(s),startIndex(index),rawStrLen(s.size()){
    fixedStr=s;
  }
  const clstr rawStr;
  const cluint startIndex;
  const cluint rawStrLen;
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
  clbool ParseToLexicalInfo_();
  clbool ValidateNames_();

  /**
  * folder MUST NOT contain '*'
  */
  clbool ValidateFolderName_(LexicalInfo* info);

  /**
  * file full name CAN contain '*'
  * but MUST has only one for either file name or extension name
  */
  clbool ValidateFileName_(LexicalInfo* info);

  /**
  * extension name CAN contain '*'
  * but MUST has only one
  */
  clbool ValidateExtensionName_(LexicalInfo* info);
  inline void PrintFixedName_(clstr s,LexicalInfo* info);
  inline void PrintErrorName_(clstr s,LexicalInfo* info,clstr str,cluint mode=0);
  inline LexicalInfo* GetLexicalInfo(clint index);

private:
  clstr m_rawString;
  clbool m_verbosePrint;
  cluint m_rawStringLen;
  std::vector<LexicalInfo> m_vecInfos;

  const cluint m_defaultPrintColor;
  const cluint m_highLightPrintColor;
};
