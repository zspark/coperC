#include "assembling.h"
#include "coper.h"
#include "lexical_analyzer.h"

using namespace std;

Assembling::Assembling(cluint defaultColor
  ,cluint highlightColor,clbool verbose)
  :m_defaultPrintColor(defaultColor)
  ,m_highLightPrintColor(highlightColor)
  ,m_bVerbosePrint(verbose){}

Assembling::~Assembling(){}

void Assembling::Assemble(const std::vector<LexicalInfo*>& in){
  m_sRelativePath="";
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
clbool Assembling::FindCommonExtension_(){
  for(auto i:m_vecInfos){
    if(i->type==LexicalInfoType::COMMON_EXTENSION){
      m_commonExtension=i->fixedStr;
      return true;
    }
  }
  m_commonExtension="";
  return false;
}

cluint Assembling::HandleBracket_(clint index){
  FindCommonExtension_();
  const LexicalInfo* info=nullptr;
  cluint offset=index;
  for(offset;;offset++){
    info=m_vecInfos[offset];
    const clstr fileName=info->fixedStr;
    if(info->type==LexicalInfoType::NAME){
      m_vecOut.push_back(new AssembleInfo(m_sRelativePath,true,fileName+m_commonExtension));
    } else if(IsRightBracketKeyword(fileName)){
      break;
    }
  }
  return offset;
}

clbool Assembling::HandleSlash_(clint index){
  const LexicalInfo* preInfo=m_vecInfos[index-1];
  clstr str=preInfo->fixedStr;
  if(IsStarOnly(str)){
    F_DBG_ASSERT(false);
  } else{
    m_sRelativePath+=str+"/";
  }
  return true;
}

clbool Assembling::HandleLastFileName_(clint pos){
  const LexicalInfo* info=m_vecInfos[pos];
  clstr str=info->fixedStr;
  const auto index=str.find_last_of('.');
  if(index!=string::npos){
    const clstr nameN=str.substr(0,index);
    const clstr extension=str.substr(index+1);
    if(IsStarOnly(nameN)||IsStarOnly(extension)){
      m_vecOut.push_back(new AssembleInfo(m_sRelativePath,false,str));
    } else{
      // concrete file name;
      m_vecOut.push_back(new AssembleInfo(m_sRelativePath,true,str));
    }
  } else{
    if(IsStarOnly(str)){
      m_vecOut.push_back(new AssembleInfo(m_sRelativePath,false,str));
    } else m_vecOut.push_back(new AssembleInfo(m_sRelativePath,true,str));
  }
  return false;
}

clbool Assembling::HandleRegexp_(clint index){
  const LexicalInfo* info=m_vecInfos[index+1];
  F_DBG_ASSERT(info);
  m_vecOut.push_back(new AssembleInfo(m_sRelativePath,false,info->fixedStr));
}