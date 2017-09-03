#pragma once

#include <vector>
#include "cllib.h"

struct LexicalInfo;


class GrammarAnalyzer final{
public:
  GrammarAnalyzer();
  ~GrammarAnalyzer();
  clbool Analyze(clstr rawStr,const std::vector<LexicalInfo*>& in,clbool verbose);

private:
  clbool AnalyzeKeyword_(cluint currentOffset);
  clbool AnalyzeKeywordFolder_(cluint currentOffset);
  clbool AnalyzeKeywordVLine_(cluint currentOffset);
  clbool AnalyzeKeywordLeftBracket_(cluint currentOffset);
  clbool AnalyzeKeywordRightBracket_(cluint currentOffset);
  clbool AnalyzeKeywordDoubleRightBracket_(cluint currentOffset);

  clbool AnalyzeName_(cluint currentOffset);
  clbool AnalyzeRegexp_(cluint currentOffset);
  void ResetLocal_();
  inline const LexicalInfo* GetLexicalInfoAt_(clint index)const;

private:
  clbool m_bVerbosePrint;
  clstr m_sRawStr;
  std::vector<LexicalInfo*> m_vecInfos;
  cluint m_uBracketCount=0;
};