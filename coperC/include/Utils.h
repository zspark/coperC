/*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/
#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED
#include <string>
#include <vector>
#include "clRegexpUtil.h"

using namespace std;

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


#define IsStartedWithCommentMark(str) (cl::clRegexp::IsStartedWith(str,COMMET_MARK,false))
#define IsStartedWithRootMark(str) (cl::clRegexp::IsStartedWith(str,ROOT,false))
#define IsStartedWithTargetMark(str) (cl::clRegexp::IsStartedWith(str,TARGET,false))

char* read(char* fileURL);

bool isDirectoryNoFiles(string &fileURL);

void getDirectoryFiles(const string &directoryURL,const char *filespec,vector<string>* fileNames);

#endif // UTILS_H_INCLUDED
