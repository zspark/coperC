#pragma once
#include <vector>
#include "coper.h"

struct LexicalInfo;

namespace NodeType{
enum type{
  FOLDER=0,
  FILE,
  FILE_WILDCARD,
  REGEXP,
};
}

struct NodeInfo{
  NodeType::type type;
  clstr name;
};

class Assembling final{
public:
  Assembling(cluint defaultColor,cluint highlightColor,clbool verbose);
  ~Assembling();
  void Assemble(const std::vector<LexicalInfo*>& in);
  cl::hs::clHS_T<NodeInfo>* GetAssembledHS(){ return &m_hs; }

private:
  /**
  * index 是左括号的索引
  * 返回的是右括号LexicalInfo的索引
  */
  cluint HandleBracket_(clint index);
  inline clbool HandleSlash_(clint index);
  clbool HandleLastFileName_(clint index);
  clbool HandleRegexp_(clint index);

  clbool Aim_(clstr name);

private:
  std::vector<LexicalInfo*> m_vecInfos;

  hsass m_hs;
  hsnode* m_tmpNode=nullptr;
  const cluint m_defaultPrintColor;
  const cluint m_highLightPrintColor;
  const clbool m_bVerbosePrint;
};