#pragma once

#include <vector>
#include "coper.h"


class ParameterParser;

class ConfigFileLoader{
public:
  ConfigFileLoader(cluint defaultColor,cluint highlightColor);
  ~ConfigFileLoader();

  clbool Load(clstr fileURL ,std::vector<clstr>& out ,ParameterParser* pp);
  void PrintInfo();

private:
  cluint m_uTotalCommentItemCount=0;
  cluint m_uTotalBlankItemCount=0;
  cluint m_uTotalAvaliableItemCount=0;
  clstr m_sParameterLine="";
  clstr m_sConfigFileURL="";

  cluint m_uParameterLineCount=0;
  const cluint m_defaultPrintColor;
  const cluint m_highLightPrintColor;
};
