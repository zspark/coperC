#pragma once

#include "cl/cl_types.h"
#include "cl/cl_misc.h"
#include "cl/cl_regexp_util.h"
#include "cl/cl_hierarchical_structure.h"
#include "cl/cl_console.h"

#define VERSION "coper v0.6"

#define MARK_PARAMETER "parameter"
#define MARK_ROOT "root"
#define MARK_TARGET "target"
#define MARK_SHARP "#"
#define MARK_START "*"
#define MARK_SLASH "/"
#define MARK_LEFT_BRACKET "<"
#define MARK_RIGHT_BRACKET ">"
#define MARK_DOUBLE_RIGHT_BRACKET ">>"
#define MARK_DOT "."
#define MARK_V_LINE "|"
#define C_MARK_SHARP '#'
#define C_MARK_V_LINE '|'
#define C_MARK_SLASH '/'
#define C_MARK_LEFT_BRACKET '<'
#define C_MARK_RIGHT_BRACKET '>'
#define WINDOWS_ALLOWED_NAME R"(^[^\\/\*:\?"<>\|]*$)"

#define str_(s) #s
#define STR(s) str_(s)

#define cons cl::clConsole::GetSingalton()
#define W(s,b) cons->Warning(s,b);
#define I(s,b) cons->Info(s,b);
#define E(s,b) cons->Error(s,b);
#define T(s,b) cons->Text(s,b);
#define ASSERT(s) {if(!(s))throw "assertion failure";}

struct NodeInfo;

typedef cl::hs::clHS hsass;
typedef cl::hs::clHSNode hsnode;


clbool IsFolderKeyword(const clstr& str);
clbool IsVLineKeyword(const clstr& str);
clbool IsStarOnly(const clstr& str);
clbool IsLeftBracketKeyword(const clstr& str);
clbool IsRightBracketKeyword(const clstr& str);
clbool IsDoubleRightBracketKeyword(const clstr& str);


clbool IsNameWithoutDotLegal(const clstr& str);
clbool IsCoperAllowedName(const clstr& str);
clbool IsWildCardName(const clstr& str);
