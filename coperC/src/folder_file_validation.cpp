#include "folder_file_validation.h"
#include "cl_folder_file.h"
#include "assembling.h"

using namespace std;
using namespace cl;

FolderFileValidation::FolderFileValidation(clstr rootPath,cluint defaultColor
  ,cluint highlightColor,clbool verbose)
  :m_sRootPath(rootPath)
  ,m_defaultPrintColor(defaultColor)
  ,m_highLightPrintColor(highlightColor)
  ,m_bVerbosePrint(verbose)
  ,m_sRelativePath("")
{}

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
      ValidateConcreteFile_();
      if(m_currentNode->GetNextSiblingNode())
        m_currentNode=m_currentNode->GetNextSiblingNode();
      else{
        m_currentNode=FindAncestorNextSibling_(m_currentNode);
        RedirectToParentPath_();
      }
      break;
    case NodeType::FILE_WILDCARD:
      ValidateWildcardName_();
      if(m_currentNode->GetNextSiblingNode())
        m_currentNode=m_currentNode->GetNextSiblingNode();
      else{
        m_currentNode=FindAncestorNextSibling_(m_currentNode);
        RedirectToParentPath_();
      }
      break;
    case NodeType::FOLDER:
      flag=false;
      if(ValidateFolder_()){
        if(m_currentNode->GetFirstChildNode())
          m_currentNode=m_currentNode->GetFirstChildNode();
        else{
          W("no config file(s) to be handled in this folder : "+m_sRootPath+m_sRelativePath,1);
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
      ValidateRegexp_();
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
  T("|",false); cons->FillRestWith('=');
  T("| VALIDATION INFOMATIONS:",1);
  T("|",false); cons->FillRestWith('-');
  T("| Missed folders count : "+clTypeUtil::NumberToString(m_uMissedFoldersCount),1);
  T("| Missed files count : "+clTypeUtil::NumberToString(m_uNotExistedFileCount),1);
  T("| Existed files count : "+clTypeUtil::NumberToString(m_pVecOut->size()),1);
  T("|",false); cons->FillRestWith('=');
}

void FolderFileValidation::CleanCache(){
  m_uNotExistedFileCount=0;
  m_uMissedFoldersCount=0;
  m_pVecOut=nullptr;
}

clbool FolderFileValidation::ValidateWildcardName_(){
  FolderAndFile ff;
  cFFInfo* ffInfo=ff.Traverse(m_sRootPath+m_sRelativePath,FolderAndFile::V_FILE);
  if(ffInfo==nullptr){
    E("there is no file at your folder : "+m_sRootPath+m_sRelativePath,1);
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

clbool FolderFileValidation::ValidateRegexp_(){
  FolderAndFile ff;
  cFFInfo* ffInfo=ff.Traverse(m_sRootPath+m_sRelativePath,FolderAndFile::V_FILE);
  if(ffInfo==nullptr){
    E("there is no file at your folder : "+m_sRootPath+m_sRelativePath,1);
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

clbool FolderFileValidation::ValidateConcreteFile_(){
  const clstr fileURL=m_sRootPath+m_sRelativePath+m_currentNode->customObject.name;
  clbool b=FolderAndFile::IsFileExist(fileURL);
  if(b){
    m_pVecOut->push_back(m_sRelativePath+m_currentNode->customObject.name);
    return true;
  } else{
    E("there is no such file at your system : "+fileURL,1);
    m_uNotExistedFileCount++;
    return false;
  }
}

clbool FolderFileValidation::ValidateFolder_(){
  const clstr folderPath=m_sRootPath+m_sRelativePath+m_currentNode->customObject.name+"/";
  clbool b=FolderAndFile::IsFolderExist(folderPath);
  if(b){
    return true;
  } else{
    E("there is no such folder at your system : "+folderPath,1);
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
