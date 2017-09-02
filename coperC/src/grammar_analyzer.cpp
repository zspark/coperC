#include "grammar_analyzer.h"
#include "clTypeUtil.h"
#include "clPrinter.h"
#include "lexical_analyzer.h"
#include "coper.h"
#include "clRegexpUtil.h"
#include "clPrinter.h"

using namespace std;
using namespace cl;

clbool GrammarAnalyzer::Analyze(const vector<LexicalInfo>& in,vector<cluint>& out,clbool verbose){
  const LexicalInfo* info=nullptr;
  const cluint n=in.size();
  for(clint i=0;i<n;i++){
    info=&in[i];

    switch(info->type){
    case LexicalInfoType::KEY_WORD:
      break;
    case LexicalInfoType::NAME:
      break;
    case LexicalInfoType::REGEXP:
      break;
    default:
      break;
    }
    
  }
  return true;
}
