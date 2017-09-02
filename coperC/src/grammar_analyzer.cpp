#include "grammar_analyzer.h"
#include "clTypeUtil.h"
#include "clPrinter.h"
#include "lexical_analyzer.h"
#include "coper.h"

using namespace std;
using namespace cl;

clbool GrammarAnalyzer::Analyze(const vector<LexicalInfo>& in,vector<cluint>& out,clbool verbose){
  const LexicalInfo* info=nullptr;
  const cluint n=in.size();
  for(clint i=0;i<n;i++){
    info=&in[i];
    if(IsAnyKeyword(in[i].rawStr)){
      // key words
      if(IsLeftBracketKeyword(info->rawStr)){

      }

    } else{
      //file or folder or common extension name;
      if(i<n-1){
        if(IsFolderKeyword(in[i+1].rawStr)){
          //folder;
          
        } else if(IsRightBracketKeyword(in[i+1].rawStr)){
          F_DBG_ASSERT(i>0)
            if(IsCommonExtensionKeyword(in[i-1].rawStr)){
              // common extension;
            }
        } else if(IsVLineKeyword(in[i+1].rawStr)){
          // file;
        } else if(IsCommonExtensionKeyword(in[i+1].rawStr)){
          // file;
        }
      } else{
        //file
      }
    }
  }
  return true;
}
