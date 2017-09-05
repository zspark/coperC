#include "folder_file_validation.h"
#include "clFolderAndFile.h"
#include "clPrinter.h"
#include "clTypeUtil.h"
#include "coper.h"
#include "assembling.h"

using namespace std;
using namespace cl;

FolderFileValidation::FolderFileValidation(clstr rootPath,cluint defaultColor
  ,cluint highlightColor,clbool verbose)
  :m_sRootPath(rootPath)
  ,m_defaultPrintColor(defaultColor)
  ,m_highLightPrintColor(highlightColor)
  ,m_bVerbosePrint(verbose) {}

FolderFileValidation::~FolderFileValidation(){
  m_pVecOut=nullptr;
}

void FolderFileValidation::Validate(const vector<AssembleInfo*>& in,vector<clstr>* out){
  m_vecInfos=in;
  m_pVecOut=out;

  m_iCurrentIndex=0;
  const AssembleInfo* info=nullptr;
  const cluint N=in.size();
  for(;m_iCurrentIndex<N;m_iCurrentIndex++){
    info=in[m_iCurrentIndex];
    switch(info->fileNameType){
    case AssembleInfoNameType::CONCRETE:
      HandleConcrete_();
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

void FolderFileValidation::PrintInfo(){
#define print(s) Unimportant(s,true,false)
  print("|=================================================");
  print("| VALIDATION INFOMATIONS:");
  print("|-------------------------------------------------");
  print("| Missed folders count : "+clTypeUtil::NumberToString(m_uMissedFoldersCount));
  print("| Missed files count : "+clTypeUtil::NumberToString(m_uNotExistedFileCount));
  print("| Existed files count : "+clTypeUtil::NumberToString(m_pVecOut->size()));
  print("|=================================================");
#undef print
}

void FolderFileValidation::CleanCache(){
  m_iCurrentIndex=0;
  m_uNotExistedFileCount=0;
  m_uMissedFoldersCount=0;
  m_pVecOut=nullptr;
  m_vecInfos.clear();
}

clbool FolderFileValidation::HandleLastFileName_(){
  clbool hasStarFlag=false;
  clstr nameN;
  clstr extension="";
  const AssembleInfo* info=m_vecInfos[m_iCurrentIndex];
  const clstr str=info->fileName;
  const auto index=str.find_last_of('.');
  if(index!=string::npos){
    nameN=str.substr(0,index);
    extension=str.substr(index+1);
    if(IsStarOnly(nameN)||IsStarOnly(extension)){
      hasStarFlag=true;
    } else{
      // concrete file name;
      m_pVecOut->push_back(info->relativePath+str);
    }
  } else{
    if(IsStarOnly(str)){
      nameN=str;
      hasStarFlag=true;
    } else m_pVecOut->push_back(info->relativePath+str);
  }



  if(hasStarFlag){
    const clstr path=m_sRootPath+info->relativePath;
    FolderAndFile ff;
    cFFInfo* ffInfo=ff.Traverse(path,FolderAndFile::V_FILE);
    if(ffInfo==nullptr){
      Error("there is no such path at your system : "+path);
      m_uMissedFoldersCount++;
      return false;
    } else{
      while(ffInfo){
        if(IsStarOnly(nameN)){
          if(IsStarOnly(extension)){
            m_pVecOut->push_back(info->relativePath+ffInfo->nameE);
          } else{
            if(strcmp(ffInfo->extension.c_str(),extension.c_str())==0){
              m_pVecOut->push_back(info->relativePath+ffInfo->nameE);
            }
          }
        } else{
          if(IsStarOnly(extension)){
            if(strcmp(ffInfo->nameN.c_str(),nameN.c_str())==0){
              m_pVecOut->push_back(info->relativePath+ffInfo->nameE);
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

clbool FolderFileValidation::HandleRegexp_(){
  const AssembleInfo* info=m_vecInfos[m_iCurrentIndex];
  const clstr regexpStr=info->fileName;
  const clstr path=m_sRootPath+info->relativePath;
  FolderAndFile ff;
  cFFInfo* ffInfo=ff.Traverse(path,FolderAndFile::V_FILE);
  if(ffInfo==nullptr){
    Error("there is no such path at your system : "+path);
    m_uMissedFoldersCount++;
    return false;
  } else{
    while(ffInfo){
      if(clRegexp::Match(ffInfo->nameE,regexpStr,true)){
        m_pVecOut->push_back(info->relativePath+ffInfo->nameE);
      }
      ffInfo=ffInfo->next;
    }
    return true;
  }
}

clbool FolderFileValidation::HandleConcrete_(){
  const AssembleInfo* info=m_vecInfos[m_iCurrentIndex];
  const clstr path=m_sRootPath+info->relativePath;
  clbool b=FolderAndFile::IsFolderExist(path);
  if(b){
    clbool b=FolderAndFile::IsFileExist(path+info->fileName);
    if(b){
      m_pVecOut->push_back(info->relativePath+info->fileName);
      return true;
    } else{
      Error("there is no such file at your system : "+path+info->fileName);
      m_uNotExistedFileCount++;
      return false;
    }
  } else{
    Error("there is no such path at your system : "+path);
    m_uMissedFoldersCount++;
    return false;
  }
}
