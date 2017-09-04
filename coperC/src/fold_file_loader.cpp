#include "fold_file_loader.h"
#include "clPrinter.h"
#include "coper.h"
#include "lexical_analyzer.h"
#include "clFolderAndFile.h"

using namespace std;
using namespace cl;

FoldFileLoader::FoldFileLoader(clstr rootPath,cluint defaultColor
  ,cluint highlightColor,clbool verbose)
  :m_sRootPath(rootPath)
  ,m_defaultPrintColor(defaultColor)
  ,m_highLightPrintColor(highlightColor)
  ,m_bVerbosePrint(verbose) {}

FoldFileLoader::~FoldFileLoader(){
  m_vecInfos.clear();
  m_pVecOut=nullptr;
}

void FoldFileLoader::Load(const vector<LexicalInfo*>& in,vector<clstr>* out){
  m_sRelativePath="";
  m_vecInfos.clear();
  m_vecInfos=in;
  m_pVecOut=out;

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

clbool FoldFileLoader::FindCommonExtension_(){
  for(auto i:m_vecInfos){
    if(i->type==LexicalInfoType::COMMON_EXTENSION){
      m_commonExtension=i->fixedStr;
      return true;
    }
  }
  m_commonExtension="";
  return false;
}

cluint FoldFileLoader::HandleBracket_(clint index){
  FindCommonExtension_();
  const LexicalInfo* info=nullptr;
  cluint offset=index;
  for(offset;;offset++){
    info=m_vecInfos[offset];
    if(info->type==LexicalInfoType::NAME){
      clstr fileName=info->fixedStr;
      m_pVecOut->push_back(m_sRelativePath+fileName+m_commonExtension);
    } else if(IsRightBracketKeyword(info->fixedStr)){
      break;
    }
  }
  return offset;
}

clbool FoldFileLoader::HandleSlash_(clint index){
  const LexicalInfo* preInfo=m_vecInfos[index-1];
  clstr str=preInfo->fixedStr;
  if(IsStarOnly(str)){
    F_DBG_ASSERT(false);
  } else{
    m_sRelativePath+=str+"/";
  }
  return true;
}

clbool FoldFileLoader::HandleLastFileName_(clint pos){
  clbool hasStarFlag=false;
  clstr nameN;
  clstr extension="";
  const LexicalInfo* info=m_vecInfos[pos];
  clstr str=info->fixedStr;
  const auto index=str.find_last_of('.');
  if(index!=string::npos){
    nameN=str.substr(0,index);
    extension=str.substr(index+1);
    if(IsStarOnly(nameN)||IsStarOnly(extension)){
      hasStarFlag=true;
    } else{
      // concrete file name;
      m_pVecOut->push_back(m_sRelativePath+str);
    }
  } else{
    if(IsStarOnly(str)){
      nameN=str;
      hasStarFlag=true;
    } else m_pVecOut->push_back(m_sRelativePath+str);
  }



  if(hasStarFlag){
    const clstr path=m_sRootPath+m_sRelativePath;
    FolderAndFile ff;
    cFFInfo* ffInfo=ff.Traverse(path,FolderAndFile::V_FILE);
    if(ffInfo==nullptr){
      Error("there is no such path at your system : "+path);
      return false;
    } else{
      while(ffInfo){
        if(IsStarOnly(nameN)){
          if(IsStarOnly(extension)){
            m_pVecOut->push_back(m_sRelativePath+ffInfo->nameE);
          } else{
            if(strcmp(ffInfo->extension.c_str(),extension.c_str())==0){
              m_pVecOut->push_back(m_sRelativePath+ffInfo->nameE);
            }
          }
        } else{
          if(IsStarOnly(extension)){
            if(strcmp(ffInfo->nameN.c_str(),nameN.c_str())==0){
              m_pVecOut->push_back(m_sRelativePath+ffInfo->nameE);
            }
          } else{
            F_DBG_ASSERT(false);
          }
        }

        ffInfo=ffInfo->next;
      }
    }
    return true;
  }
}

clbool FoldFileLoader::HandleRegexp_(clint index){
  const LexicalInfo* nextInfo=m_vecInfos[index+1];
  F_DBG_ASSERT(nextInfo);
  const clstr regexpStr=nextInfo->rawStr;
  const clstr path=m_sRootPath+m_sRelativePath;
  FolderAndFile ff;
  cFFInfo* ffInfo=ff.Traverse(path,FolderAndFile::V_FILE);
  if(ffInfo==nullptr){
    Error("there is no such path at your system : "+path);
    return false;
  } else{
    while(ffInfo){
      if(clRegexp::Match(ffInfo->nameE,regexpStr,true)){
        m_pVecOut->push_back(m_sRelativePath+ffInfo->nameE);
      }
      ffInfo=ffInfo->next;
    }
    return true;
  }
}
