#pragma once
#include <vector>
#include "cllib.h"

struct AssembleInfo;

class FoldFileLoader final{
public:
  FoldFileLoader(clstr rootPath,cluint defaultColor,cluint highlightColor,clbool verbose);
  ~FoldFileLoader();

  void Validate(const std::vector<AssembleInfo*>& in,std::vector<clstr>* out);

private:
  clbool HandleLastFileName_(clint index);
  clbool HandleRegexp_(clint index);

private:
  std::vector<AssembleInfo*> m_vecInfos;
  std::vector<clstr>* m_pVecOut=nullptr;
  clstr m_commonExtension="";
  clstr m_sRelativePath="";

  const cluint m_defaultPrintColor;
  const cluint m_highLightPrintColor;
  const clbool m_bVerbosePrint;
  const clstr m_sRootPath;
};
