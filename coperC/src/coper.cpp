#include <vector>
#include "coper.h"
#include "clRegexpUtil.h"

/*
window系统规定，目录，文件名不能含有以下字符：
/\:*?"<>|
*/
using namespace std;
using namespace cl;

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

clbool IsDoubleRightBracketKeyword(const clstr& str){
  return clRegexp::Match(str,R"(^>>$)",true);
}

clbool IsStarOnly(const clstr & str){
  return clRegexp::Match(str,R"(^\*$)",true);
}

clbool IsLeftBracketKeyword(const clstr& str){
  return clRegexp::Match(str,R"(^<$)",true);
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
    return false;
  }
}

clbool IsWildCardName(const clstr& str){
  return clRegexp::Match(str,R"((\*\.\*|^[^\\/\*:\?"<>\|]*\.\*|\*\.[^\\/\*:\?"<>\|]*$|\*))",true);
}


