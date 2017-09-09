#include "file_handler.h"
#include <vector>
#include <map>
#include "clFolderAndFile.h"
#include "parameter_parser.h"

namespace FileHandler{
using namespace std;


void FHCopy(const ParameterParser & pp,const vector<clstr>& vec){
  //create target folder;
  const clstr rootPath=pp.GetRootPath();
  const clstr targetPath=pp.GetTargetPath();
  cl::FolderAndFile::CreateFolder(targetPath);

  map<clstr,vector<clstr>> mapTmp;
  clstr path,name;
  for(auto& URL:vec){
    clint index=URL.find_last_of("/");
    if(index>=0){
      path=URL.substr(0,index+1);
      name=URL.substr(index+1);
    } else{
      path="";
      name=URL;
    }

    mapTmp[path].push_back(name);
  }

  for(auto& it:mapTmp){
    // create folders
    path=it.first;
    cl::FolderAndFile::CreateFolder(targetPath+path);

    //copy files
    for(auto& name:it.second){
      cl::FolderAndFile::CopyFileTo(rootPath+path+name,targetPath+path+name);
    }
  }



}

}
