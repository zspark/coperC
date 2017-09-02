#pragma once

#include <vector>
#include "cllib.h"

struct LexicalInfo;

class GrammarAnalyzer final{
public:
  clbool Analyze(const std::vector<LexicalInfo>& in,std::vector<cluint>& out,clbool verbose);
};