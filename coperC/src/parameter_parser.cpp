#include <vector>
#include "parameter_parser.h"
#include "cl_folder_file.h"

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
  // 强行加上空格，解决4个字符取值时候，无参数命令位于最后的复杂判断；
  str=" "+str+" ";
  
  clstr currentStr;
  cluint i=0;
  for(;i<str.size()-3;i++){
    currentStr=str.substr(i,4);
    if(strcmp(currentStr.c_str()," -v ")==0){
      m_operation_flag|=V_VERBOSE;
    } else if(strcmp(currentStr.c_str()," -c ")==0){
      m_operation_flag|=V_OPERATION_COPY;
    } else if(strcmp(currentStr.c_str()," -x ")==0){
      m_operation_flag|=V_OPERATION_CUT;
    } else if(strcmp(currentStr.c_str()," -y ")==0){
      m_operation_flag|=V_OPERATION_YES;
    } else if(strcmp(currentStr.c_str()," -Y ")==0){
      m_operation_flag|=V_OPERATION_YES_ALL;
    } else if(strcmp(currentStr.c_str()," -d ")==0){
      m_operation_flag|=V_OPERATION_DELETE;
    } else if(strcmp(currentStr.c_str()," -r ")==0){
      m_rootPath=GetStringBetweenQuote_(str,i+3);
      i+=3+m_rootPath.size();
      if(!clRegexp::IsEndedWith(m_rootPath,R"(/)"))m_rootPath+="/";
    }else if(strcmp(currentStr.c_str()," -t ")==0){
      m_targetPath=GetStringBetweenQuote_(str,i+3);
      i+=3+m_targetPath.size();
    }
  }

  clbool flag=true;
  cluint operation=m_operation_flag&0x000000FF;
  if(operation==V_OPERATION_COPY || operation==V_OPERATION_CUT || operation==V_OPERATION_DELETE)m_operation_flag|=V_INITED;
  else{
    E("-c,-d,-x can NOT exist more than one. and MUST exist only one.",1);
    flag=false;
  }

  // avidate path avaliablability.
  if(FolderAndFile::IsFolderExist(m_rootPath))m_operation_flag|=V_INITED;
  else{
    m_operation_flag&=~V_INITED;
    E("root path is NOT exist in your file system.",1);
    flag=false;
  }
  if(!flag){
    T(str,1);
  }
  return flag;
}

clbool ParameterParser::NeedRequest() const{
  return (m_operation_flag&0x0000FF00)<V_OPERATION_YES;
}

clbool ParameterParser::NeedRequestAll() const{
  return (m_operation_flag&0x0000FF00)<V_OPERATION_YES_ALL;
}
