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
  ,m_bVerbosePrint(verbose) {
  m_sRelativePath="";
}

FolderFileValidation::~FolderFileValidation(){
  m_pVecOut=nullptr;
}

void FolderFileValidation::Validate(hsass* in,vector<clstr>* out){
  m_vecInfos=in;
  m_pVecOut=out;

  clbool flag=false;
  m_currentNode=in->GetFirstChildNode(nullptr);
  while(m_currentNode){
    switch(m_currentNode->customObject.type){
    case NodeType::FILE:
      HandleConcrete_();
      if(m_currentNode->GetNextSiblingNode())
        m_currentNode=m_currentNode->GetNextSiblingNode();
      else{
        m_currentNode=FindAncestorNextSibling_(m_currentNode);
        RedirectToParentPath_();
      }
      break;
    case NodeType::FILE_WILDCARD:
      HandleWildcardName_();
      if(m_currentNode->GetNextSiblingNode())
        m_currentNode=m_currentNode->GetNextSiblingNode();
      else{
        m_currentNode=FindAncestorNextSibling_(m_currentNode);
        RedirectToParentPath_();
      }
      break;
    case NodeType::FOLDER:
      flag=false;
      if(HandleFolder_()){
        if(m_currentNode->GetFirstChildNode())
          m_currentNode=m_currentNode->GetFirstChildNode();
        else{
          Warning("no config file(s) to be handled in this folder : "+m_sRootPath+m_sRelativePath);
          flag=true;
        }
      } else flag=true;

      if(flag){
        if(m_currentNode->GetNextSiblingNode())
          m_currentNode=m_currentNode->GetNextSiblingNode();
        else m_currentNode=FindAncestorNextSibling_(m_currentNode);
      }
      RedirectToParentPath_();
      break;
    case NodeType::REGEXP:
      HandleRegexp_();
      if(m_currentNode->GetNextSiblingNode())
        m_currentNode=m_currentNode->GetNextSiblingNode();
      else{
        m_currentNode=FindAncestorNextSibling_(m_currentNode);
        RedirectToParentPath_();
      }
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
  m_uNotExistedFileCount=0;
  m_uMissedFoldersCount=0;
  m_pVecOut=nullptr;
}

clbool FolderFileValidation::HandleWildcardName_(){
  FolderAndFile ff;
  cFFInfo* ffInfo=ff.Traverse(m_sRootPath+m_sRelativePath,FolderAndFile::V_FILE);
  if(ffInfo==nullptr){
    Error("there is no file at your folder : "+m_sRootPath+m_sRelativePath);
    return false;
  } else{
    clstr nameN;
    clstr extension="";
    const clstr str=m_currentNode->customObject.name;
    const auto index=str.find_last_of('.');
    if(index!=string::npos){
      nameN=str.substr(0,index);
      extension=str.substr(index+1);
    } else{
      nameN=str;
    }

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
    return true;
  }
}

clbool FolderFileValidation::HandleRegexp_(){
  FolderAndFile ff;
  cFFInfo* ffInfo=ff.Traverse(m_sRootPath+m_sRelativePath,FolderAndFile::V_FILE);
  if(ffInfo==nullptr){
    Error("there is no file at your folder : "+m_sRootPath+m_sRelativePath);
    m_uMissedFoldersCount++;
    return false;
  } else{
    const clstr regexpStr=m_currentNode->customObject.name;
    while(ffInfo){
      if(clRegexp::Match(ffInfo->nameE,regexpStr,true)){
        m_pVecOut->push_back(m_sRelativePath+ffInfo->nameE);
      }
      ffInfo=ffInfo->next;
    }
    return true;
  }
}

clbool FolderFileValidation::HandleConcrete_(){
  const clstr fileURL=m_sRootPath+m_sRelativePath+m_currentNode->customObject.name;
  clbool b=FolderAndFile::IsFileExist(fileURL);
  if(b){
    m_pVecOut->push_back(m_sRelativePath+m_currentNode->customObject.name);
    return true;
  } else{
    Error("there is no such file at your system : "+fileURL);
    m_uNotExistedFileCount++;
    return false;
  }
}

clbool FolderFileValidation::HandleFolder_(){
  const clstr folderPath=m_sRootPath+m_sRelativePath+m_currentNode->customObject.name+"/";
  clbool b=FolderAndFile::IsFolderExist(folderPath);
  if(b){
    return true;
  } else{
    Error("there is no such folder at your system : "+folderPath);
    return false;
  }
}

hsnode * FolderFileValidation::FindAncestorNextSibling_(hsnode * node){
  hsnode* parentNode=node->GetParentNode();
  while(parentNode){
    if(parentNode->GetNextSiblingNode()){
      return parentNode->GetNextSiblingNode();
    } else parentNode=parentNode->GetParentNode();
  }
  return nullptr;
}

void FolderFileValidation::RedirectToParentPath_(){
  if(!m_currentNode)return;
  m_sRelativePath="";
  hsnode* pnode=m_currentNode->GetParentNode();
  while(pnode){
    m_sRelativePath=pnode->customObject.name+"/"+m_sRelativePath;
    pnode=pnode->GetParentNode();
  }
}
