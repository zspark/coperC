#include <vector>
#include "coper.h"
#include "clRegexpUtil.h"

/*
window系统规定，目录，文件名不能含有以下字符：
/\:*?"<>|
*/
using namespace std;
using namespace cl;

clbool IsAnyKeyword(const clstr& str){
  return clRegexp::Match(str,R"(^[/\[\]:|]$)",true);
}

clbool IsFolderKeyword(const clstr& str){
  return clRegexp::Match(str,R"(^/$)",true);
}

clbool IsVLineKeyword(const clstr& str){
  return clRegexp::Match(str,R"(^\|$)",true);
}

clbool IsCommonExtensionKeyword(const clstr& str){
  return clRegexp::Match(str,R"(^:$)",true);
}

clbool IsRightBracketKeyword(const clstr& str){
  return clRegexp::Match(str,R"(^>$)",true);
}

clbool IsStarOnly(const clstr & str){
  return clRegexp::Match(str,R"(^\*$)",true);
}

clbool IsLeftBracketKeyword(const clstr& str){
  return clRegexp::Match(str,R"(^<$)",true);
}

clbool IsFolderName(const clstr& str){
  return clRegexp::Match(str,R"(^[a-zA-Z\*\+\?\-_\. !]+$)",true);
}

clbool IsFileName(const clstr& str){
  return clRegexp::Match(str,R"(^[a-zA-Z\*\+\?\-_\. !]+$)",true);
}

clbool checkNameGrammar(const clstr & str){
  vector<cluint> pos;
  if(clRegexp::GetIndices(str,R"(\*)",pos)){
    if(pos.size()>1){
      for(cluint i=1;i<pos.size();i++){
        if(pos[i]-pos[i-1]==1){
          return false;
        }
      }
    }
  }
  return true;
}

clbool IsExtensionName(const clstr& str){
  return clRegexp::Match(str,R"(^[a-zA-Z\*\+\?\-_\. !]+$)",true);
}

clbool IsPotentialFolderName(const clstr & str){
  return clRegexp::Match(str,R"(^[^\\/\*:\?"<>|]+$)",true);
}

clbool IsPotentialFileName(const clstr & str){
  clbool result=true;
  const auto index=str.find_last_of('.');
  if(index!=string::npos){
    const clstr nameN=str.substr(0,index);
    result=IsPotentialCommonExtensionName(nameN);
    if(!result)return false;

    const clstr extension=str.substr(index+1);
    result=IsPotentialCommonExtensionName(extension);
    if(!result)return false;
  } else{
    //file nameN;
    result=IsPotentialCommonExtensionName(str);
    if(!result)return false;
  }
}

clbool IsPotentialCommonExtensionName(const clstr& str){
  cluint len=str.length();
  clbool result=true;
  if(len>1)
    result=clRegexp::Match(str,R"(^[^\\/\*:\?"<>|]+$)",true);
  else if(len==1){
    result=IsStarOnly(str);
  }
  return result;
}
