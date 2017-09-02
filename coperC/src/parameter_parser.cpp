#include <vector>
#include "coper.h"
#include "parameter_parser.h"
#include "clFolderAndFile.h"
#include "clPrinter.h"

using namespace std;
using namespace cl;

ParameterParser::ParameterParser(){}

ParameterParser::~ParameterParser(){}

clchar ParameterParser::GetCharAt_(clstr str,clint index){
  const cluint strLen=str.size();
  if(index>=strLen)return NULL;
  if(index<0)return NULL;
  return str[index];
}

clstr ParameterParser::GetStringBetweenQuote_(clstr str,cluint startIndex){
  const cluint len=str.size();
  if(startIndex>len)return "";
  clbool isInRange=false;
  cluint rangeStartIndex,rangeEndIndex;
  for(cluint i=startIndex;i<len;i++){
    clchar c=str[i];
    if(c=='"'){
      if(isInRange){
        rangeEndIndex=i;
        return str.substr(rangeStartIndex,rangeEndIndex-rangeStartIndex);
      } else{
        rangeStartIndex=i+1;
        isInRange=true;
      }
    }
  }
  return "";
}

clbool ParameterParser::Parse(clstr str){
  const clstr p=MARK_PARAMETER;
  str=str.substr(p.size());
  // 强行加上空格，解决3个字符取值时候，无参数命令位于最后的复杂判断；
  str+=" ";
  
  clstr currentStr;
  cluint i=0;
  for(;i<str.size()-2;i++){
    currentStr=str.substr(i,3);
    if(strcmp(currentStr.c_str(),"-v ")==0){
      m_operation_flag|=V_VERBOSE;
    } else if(strcmp(currentStr.c_str(),"-c ")==0){
      m_operation_flag|=V_OPERATION_COPY;
    } else if(strcmp(currentStr.c_str(),"-x ")==0){
      m_operation_flag|=V_OPERATION_CUT;
    } else if(strcmp(currentStr.c_str(),"-d ")==0){
      m_operation_flag|=V_OPERATION_DELETE;
    } else if(strcmp(currentStr.c_str(),"-r ")==0){
      m_rootPath=GetStringBetweenQuote_(str,i+3);
      i+=3+m_rootPath.size();
    }else if(strcmp(currentStr.c_str(),"-t ")==0){
      m_targetPath=GetStringBetweenQuote_(str,i+3);
      i+=3+m_targetPath.size();
    }
  }

  cluint operation=m_operation_flag&0x000000FF;
  if(0u<operation && operation<=0x00000008)m_operation_flag|=V_INITED;
  else{
    Error("-c,-d,-x can NOT exist more than one. and MUST exist only one.");
    return false;
  }

  // avidate path avaliablability.
  if(FolderAndFile::IsFolderExist(m_rootPath))m_operation_flag|=V_INITED;
  else{
    m_operation_flag&=~V_INITED;
    Error("root path is NOT correct in your file system.");
    return false;
  }
  return true;
}
