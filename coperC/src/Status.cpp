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
#include <string>
#include "Status.h"
using namespace std;

bool parsing=false;
bool coping=false;
int totalItemCount=0;
int totalResCount=0;
string root="";
string target="";

char* appURL;

string const EXT_ALL="*";
string const ROOT("root");
string const TARGET="target";
string const COMMET_MARK="#";
string const BACK_SLASH="\\";
string const MARK_BIG_BRACKET="{";
string const MARK_DOT(".");
string const MARK_COLON(":");
const string MARK_V_LINE("|");
string const MARK_WAVE("~");

