#pragma once
#include <vector>
#include "coper.h"
#include "clHierarchicalStructure.h"


class FolderFileValidation final{
public:
  FolderFileValidation(clstr rootPath,cluint defaultColor,cluint highlightColor,clbool verbose);
  ~FolderFileValidation();

  void Validate(hsass* in,std::vector<clstr>* out);
  void PrintInfo();
  void CleanCache();

private:
  clbool HandleWildcardName_();
  clbool HandleRegexp_();
  clbool HandleConcrete_();
  clbool HandleFolder_();

  hsnode* FindAncestorNextSibling_(hsnode* node);
  void RedirectToParentPath_();

private:
  const hsass* m_vecInfos=nullptr;
  std::vector<clstr>* m_pVecOut=nullptr;
  hsnode* m_currentNode=nullptr;
  clstr m_sRelativePath="";
  cluint m_uNotExistedFileCount=0;
  cluint m_uMissedFoldersCount=0;

  const cluint m_defaultPrintColor;
  const cluint m_highLightPrintColor;
  const clbool m_bVerbosePrint;
  const clstr m_sRootPath;
};
