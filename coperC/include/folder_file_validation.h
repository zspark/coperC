#pragma once
#include <vector>
#include "cllib.h"

struct AssembleInfo;

class FolderFileValidation final{
public:
  FolderFileValidation(clstr rootPath,cluint defaultColor,cluint highlightColor,clbool verbose);
  ~FolderFileValidation();

  void Validate(const std::vector<AssembleInfo*>& in,std::vector<clstr>* out);
  void PrintInfo();
  void CleanCache();

private:
  clbool HandleLastFileName_();
  clbool HandleRegexp_();
  clbool HandleConcrete_();

private:
  std::vector<AssembleInfo*> m_vecInfos;
  std::vector<clstr>* m_pVecOut=nullptr;
  clint m_iCurrentIndex=-1;
  cluint m_uNotExistedFileCount=0;
  cluint m_uMissedFoldersCount=0;

  const cluint m_defaultPrintColor;
  const cluint m_highLightPrintColor;
  const clbool m_bVerbosePrint;
  const clstr m_sRootPath;
};
