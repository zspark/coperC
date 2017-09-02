#pragma once

#include "clTypes.h"

#define ROOT "root"
#define TARGET "target"
#define COMMET_MARK "#"
#define EXT_ALL "*"
#define BACK_SLASH "\\"
#define MARK_BIG_BRACKET "{"
#define MARK_DOT "."
#define MARK_COLON ":"
#define MARK_V_LINE "|"
#define MARK_WAVE "~"
#define WINDOWS_ALLOWED_NAME R"(^[^\\/\*:\?"<>\|]+$)"

clbool IsAnyKeyword(const clstr& str);
clbool IsFolderKeyword(const clstr& str);
clbool IsVLineKeyword(const clstr& str);
clbool IsCommonExtensionKeyword(const clstr& str);
clbool IsRightBracketKeyword(const clstr& str);
clbool IsStarOnly(const clstr& str);
clbool IsLeftBracketKeyword(const clstr& str);
clbool checkNameGrammar(const clstr& str);

clbool IsNameWithoutDotLegal(const clstr& str);
clbool IsCoperAllowedName(const clstr& str);