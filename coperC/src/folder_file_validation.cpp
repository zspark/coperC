#include "folder_file_validation.h"
#include "clFolderAndFile.h"
#include "clPrinter.h"
#include "coper.h"
#include "assembling.h"

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

void FoldFileLoader::Validate(const vector<AssembleInfo*>& in,vector<clstr>* out){
  m_sRelativePath="";
  m_vecInfos.clear();
  m_vecInfos=in;
  m_pVecOut=out;

  const AssembleInfo* info=nullptr;
  const cluint N=in.size();
  for(cluint i=0;i<N;i++){
    info=in[i];
    switch(info->fileNameType){
    case AssembleInfoNameType::CONCRETE:
      m_pVecOut->push_back(info->relativePath+info->fileName);
      break;
    case AssembleInfoNameType::REGEXP:
      HandleRegexp_();
      break;
    case AssembleInfoNameType::WILDCARD:
      HandleLastFileName_();
      break;
    default:
      break;
    }
  }
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
