#pragma once

#include "clTypes.h"

#define MARK_ROOT "root"
#define MARK_TARGET "target"
#define MARK_COMMET "#"
#define MARK_START "*"
#define MARK_SLASH "/"
#define MARK_LEFT_BRACKET "<"
#define MARK_RIGHT_BRACKET ">"
#define MARK_DOUBLE_RIGHT_BRACKET ">>"
#define MARK_DOT "."
#define MARK_V_LINE "|"
#define C_MARK_V_LINE '|'
#define C_MARK_SLASH '/'
#define C_MARK_LEFT_BRACKET '<'
#define C_MARK_RIGHT_BRACKET '>'
#define WINDOWS_ALLOWED_NAME R"(^[^\\/\*:\?"<>\|]+$)"

clbool IsFolderKeyword(const clstr& str);
clbool IsVLineKeyword(const clstr& str);
clbool IsStarOnly(const clstr& str);
clbool IsLeftBracketKeyword(const clstr& str);
clbool IsRightBracketKeyword(const clstr& str);


clbool IsNameWithoutDotLegal(const clstr& str);
clbool IsCoperAllowedName(const clstr& str);