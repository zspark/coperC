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

clbool IsNameWithoutDotLegal(const clstr & str){
  if(IsStarOnly(str))return true;
  return clRegexp::Match(str,WINDOWS_ALLOWED_NAME,true);
}

clbool IsCoperAllowedName(const clstr& str){
  const auto index=str.find_last_of('.');
  if(index==string::npos){
    //file nameN;
    return IsNameWithoutDotLegal(str);
  } else{
    const clstr nameN=str.substr(0,index);
    const clstr extension=str.substr(index+1);
    if(IsNameWithoutDotLegal(nameN)&&
      IsNameWithoutDotLegal(extension))return true;
  }
}
