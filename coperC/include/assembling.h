#pragma once
#include <vector>
#include "cllib.h"

struct LexicalInfo;


namespace AssembleInfoNameType{
enum type{
  CONCRETE=0,
  WILDCARD,
  REGEXP,
};
}

struct AssembleInfo{
  AssembleInfo(clstr path,AssembleInfoNameType::type type,clstr name)
    :relativePath(path),fileNameType(type),fileName(name){}
  AssembleInfo(const AssembleInfo&)=delete;
  AssembleInfo(const AssembleInfo*)=delete;
  AssembleInfo& operator=(const AssembleInfo&)=delete;
  const clstr relativePath;
  const AssembleInfoNameType::type fileNameType;
  const clstr fileName;
};

class Assembling final{
public:
  Assembling(cluint defaultColor,cluint highlightColor,clbool verbose);
  ~Assembling();
  void Assemble(const std::vector<LexicalInfo*>& in);
  const std::vector<AssembleInfo*>& GetAssembledInfos()const{ return m_vecOut; }

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
  std::vector<LexicalInfo*> m_vecInfos;
  std::vector<AssembleInfo*> m_vecOut;
  clstr m_commonExtension="";
  clstr m_sRelativePath="";

  const cluint m_defaultPrintColor;
  const cluint m_highLightPrintColor;
  const clbool m_bVerbosePrint;
};