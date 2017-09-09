#pragma once
#include "cllib.h"

#define V_OPERATION_DELETE (1<<1)
#define V_OPERATION_CUT (1<<2)
#define V_OPERATION_COPY (1<<3)
#define V_OPERATION_YES (1<<8)
#define V_OPERATION_YES_ALL (1<<9)
#define V_VERBOSE (1<<24)
#define V_INITED (1<<31)

class ParameterParser{
public:
  ParameterParser();
  ~ParameterParser();
  clbool Parse(clstr str);

  clbool IsVerbose()const{ return m_operation_flag&V_VERBOSE; }
  clbool IsCopy()const{ return m_operation_flag&V_OPERATION_COPY; }
  clbool IsDelete()const{ return m_operation_flag&V_OPERATION_DELETE; }
  clbool IsCut()const{ return m_operation_flag&V_OPERATION_CUT; }
  clbool NeedRequest()const;
  clbool NeedRequestAll()const;
  clstr GetRootPath()const{ return m_rootPath; }
  clstr GetTargetPath()const{ return m_targetPath; }

private:
  clchar GetCharAt_(clstr str,clint index);
  clstr GetStringBetweenQuote_(clstr str,cluint startIndex);

private:
  clstr m_rootPath="";
  clstr m_targetPath="";
  cluint m_operation_flag=0U;
};
