#pragma once
#include <vector>
#include "cllib.h"

struct LexicalInfo;

class FoldFileLoader final{
public:
  FoldFileLoader(clstr rootPath,cluint defaultColor,cluint highlightColor,clbool verbose);
  ~FoldFileLoader();
  void Load(const std::vector<LexicalInfo*>& in,std::vector<clstr>* out);

private:
  clbool FindCommonExtension_();

  /**
  * index 是左括号的索引
  * 返回的是右括号LexicalInfo的索引
  */
  cluint HandleBracket_(clint index);
  clbool HandleSlash_(clint index);
  clbool HandleLastFileName_(clint index);
  clbool HandleRegexp_(clint index);

private:
  const cluint m_defaultPrintColor;
  const cluint m_highLightPrintColor;
  const clbool m_bVerbosePrint;
  const clstr m_sRootPath;
  std::vector<LexicalInfo*> m_vecInfos;
  std::vector<clstr>* m_pVecOut=nullptr;
  clstr m_commonExtension="";
  clstr m_sRelativePath="";
};
