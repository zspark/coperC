#include <Windows.h>
#include "cllib.h"
#include "clPrinter.h"
#include "lexical_analyzer.h"
#include "grammar_analyzer.h"

using namespace std;
using namespace cl;

struct GrammarAnalyzerInput{
  GrammarAnalyzerInput(vector<LexicalInfo> info){}
  vector<LexicalInfo> infos;
  cluint fromIndex;
  cluint endIndex;
};




clint main(){
  Info("Hello Coper!");

  vector<clstr> items;
  //items.push_back("sfsfsf/ as ffge-sfsf_sge+.zip/b*/<ab?c+|dddd|!|  aaa    .txt>png   ");
  //items.push_back("xxx.jxpeg");//o
  //items.push_back("bb/xxx.jxpeg");//o
  //items.push_back("bb/*.extension");//o
  //items.push_back("bb/aab3*.extension");//x
  //items.push_back("bb/this is file nameN.*");//o
  //items.push_back("bb/no extention file name");//o
  items.push_back(R"(>)");//Lo Gx
  items.push_back(R"(<>)");//Lo Go
  items.push_back(R"(aa/<ggg|a*>.pn)");//x
  items.push_back(R"(aa/<***|ss>.pn)");//x
  items.push_back(R"(<ss>pn)");//o
  items.push_back(R"(<ss>.    pn)");//o
  items.push_back(R"(<ss>    .pn)");//o
  items.push_back(R"(<ss>    .     pn)");//o
  items.push_back(R"(a.a/<*|ss>.pn)");//o
  items.push_back(R"(aa/<*|ss>.*)");//o
  items.push_back(R"(aa/<ggg|s>)");//o
  items.push_back(R"(aa<s>.png)");//Lo Gx
  items.push_back("aa/>>^[^\\\\\"/\\*:\\?<>\\|]+$");//o
  //items.push_back(R"(aa/<ss|sfsfsf|ggg.png|s>.    sss)");//o
  //items.push_back("/");
  //items.push_back("a//");
  //items.push_back("*/");
  //items.push_back("?/");
  //items.push_back("./");
  //items.push_back("a/");
  //items.push_back("../");
  //items.push_back("./");
  //items.push_back("a./");
  //items.push_back("..a");
  //items.push_back(".a");
  //items.push_back("*.");
  //items.push_back("\"?.");
  //items.push_back("?.*");
  //items.push_back("?*");
  //items.push_back("+*");
  //items.push_back("++");
  //items.push_back("??");
  //items.push_back("+?");
  //items.push_back("+");
  //items.push_back("?.");
  //items.push_back("?.+");
  //items.push_back("?.!");
  //items.push_back("?*?.!");
  //items.push_back("*?.!");
  //items.push_back("a/b/!a");
  //items.push_back("a/b/.!");

  /*
  items.push_back("a/b/<a.txt|b.png|b.png|xiaoshuo*.txt|c|d:j*>");
  items.push_back("/<a.txt>");
  items.push_back("a/b/<c/|a.txt>");
  items.push_back("a/b/<c/a.txt>");
  items.push_back("a/b/<c|a|d|e:jpg:png>");
  items.push_back("a/b/<c|a|d|e:jg*>");
  items.push_back("a/b/<c|a|d|e>");
  items.push_back("a/b/><c|a|d|e>");
  items.push_back("a/b/<><c|a|d|e>");
  items.push_back("a/b/<<c/>|a|d|e>");
  items.push_back("a/b/|<<c/>|a|d|e>");
  items.push_back("a/b:txt|<<c/>|a|d|e>");
  items.push_back("../a/b.txt|<<c/>|a|d|e>");
  items.push_back(".");
  items.push_back(":");
  items.push_back("|");
  items.push_back("?");
  items.push_back("*");
  items.push_back("*.*");
  items.push_back(".*");
  */

  LexicalAnalyzer la(ConsoleForeground::GRAY,ConsoleForeground::RED);
  GrammarAnalyzer ga;
  for(clint i=0;i<items.size();i++){
    clstr s=items[i];
    if(la.Analyze(s,true)){
      /*
      vector<cluint> out;
      if(ga.Analyze(info,out,true)){
        Info("All Passed!");
      } else{
        HighLightText(s,ConsoleForeground::WHITE,out,ConsoleForeground::RED);
      }
*/
    }
  }

  system("pause");
  return 0;
}