#include "assembling.h"
#include "coper.h"
#include "analyzer.h"

using namespace std;
using namespace cl;

Assembling::Assembling(cluint defaultColor
  ,cluint highlightColor,clbool verbose)
  :m_defaultPrintColor(defaultColor)
  ,m_highLightPrintColor(highlightColor)
  ,m_bVerbosePrint(verbose){}

Assembling::~Assembling(){}

void Assembling::Assemble(const vector<LexicalInfo*>& in){
  m_tmpNode=nullptr;
  m_vecInfos.clear();
  m_vecInfos=in;

  const LexicalInfo* info=nullptr;
  const cluint N=in.size();
  for(cluint i=0;i<N;i++){
    info=in[i];
    if(info->type==LexicalInfoType::KEY_WORD){
      if(strcmp(info->fixedStr.c_str(),MARK_SLASH)==0){
        HandleSlash_(i);
      } else if(strcmp(info->fixedStr.c_str(),MARK_V_LINE)==0){
        F_DBG_ASSERT(false);
      } else if(strcmp(info->fixedStr.c_str(),MARK_LEFT_BRACKET)==0){
        i=HandleBracket_(i);
      } else if(strcmp(info->fixedStr.c_str(),MARK_RIGHT_BRACKET)==0){
        F_DBG_ASSERT(false);
      } else if(strcmp(info->fixedStr.c_str(),MARK_DOUBLE_RIGHT_BRACKET)==0){
        HandleRegexp_(i);
      }
    } else if(info->type==LexicalInfoType::NAME){
      if(i==N-1){
        // last appears must be file name;
        HandleLastFileName_(i);
      }
    }
  }
}

cluint Assembling::HandleBracket_(clint index){
  clstr m_commonExtension="";
  for(auto i:m_vecInfos){
    if(i->type==LexicalInfoType::COMMON_EXTENSION){
      m_commonExtension=i->fixedStr;
      break;
    }
  }
  const LexicalInfo* info=nullptr;
  cluint offset=index;
  for(;;offset++){
    info=m_vecInfos[offset];
    const clstr fileName=info->fixedStr;

    if(IsRightBracketKeyword(fileName)){
      break;
    } else{
      if(info->type==LexicalInfoType::NAME){
        if(!Aim_(fileName)){
          hsnode* node=m_hs.CreateNode();
          node->customObject.name=fileName+m_commonExtension;
          node->customObject.type=NodeType::FILE;
          m_hs.InsertNode(m_tmpNode,node,cl::hs::clHSNodeRelation::R_LAST_CHILD);
        }
      }
    }
  }
  return offset;
}

inline clbool Assembling::HandleSlash_(clint index){
  const LexicalInfo* preInfo=m_vecInfos[index-1];
  if(!Aim_(preInfo->fixedStr)){
    // 是目录，又没有瞄准，需要创建目录
    hsnode* node=m_hs.CreateNode();
    node->customObject.name=preInfo->fixedStr;
    node->customObject.type=NodeType::FOLDER;
    m_hs.InsertNode(m_tmpNode,node,cl::hs::clHSNodeRelation::R_LAST_CHILD);

    m_tmpNode=node;
  }
  return true;
}

clbool Assembling::HandleLastFileName_(clint pos){
  const LexicalInfo* info=m_vecInfos[pos];
  clstr name=info->fixedStr;
  if(!Aim_(name)){
    // 是文件名，又没有瞄准，需要创建结点
    hsnode* node=m_hs.CreateNode();
    m_hs.InsertNode(m_tmpNode,node,cl::hs::clHSNodeRelation::R_LAST_CHILD);
    node->customObject.name=name;

    if(IsWildCardName(name)){
      node->customObject.type=NodeType::FILE_WILDCARD;
    } else
      node->customObject.type=NodeType::FILE;
  }
  return true;
}

clbool Assembling::HandleRegexp_(clint index){
  const LexicalInfo* info=m_vecInfos[index+1];
  F_DBG_ASSERT(info);
  if(!Aim_(info->fixedStr)){
    // 是目录，又没有瞄准，需要创建目录
    hsnode* node=m_hs.CreateNode();
    node->customObject.name=info->fixedStr;
    node->customObject.type=NodeType::REGEXP;
    m_hs.InsertNode(m_tmpNode,node,cl::hs::clHSNodeRelation::R_LAST_CHILD);
  }
  return true;
}

clbool Assembling::Aim_(clstr name){
  hsnode* tmp=m_hs.GetFirstChildNode(m_tmpNode);
  if(!tmp)return false;

  while(tmp){
    if(::strcmp(tmp->customObject.name.c_str(),name.c_str())==0){
      if(tmp->customObject.type==NodeType::FOLDER)
        m_tmpNode=tmp;
      return true;
    }
    tmp=tmp->GetNextSiblingNode();
  }
  return false;
}
