#pragma once

#include "cllib.h"
#include "clPrinter.h"
#include "clRegexpUtil.h"

#define VERSION "V0.4"

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

clbool IsFolderKeyword(const clstr& str);
clbool IsVLineKeyword(const clstr& str);
clbool IsStarOnly(const clstr& str);
clbool IsLeftBracketKeyword(const clstr& str);
clbool IsRightBracketKeyword(const clstr& str);
clbool IsDoubleRightBracketKeyword(const clstr& str);


clbool IsNameWithoutDotLegal(const clstr& str);
clbool IsCoperAllowedName(const clstr& str);