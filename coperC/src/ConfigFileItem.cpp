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

#include "ConfigFileItem.h"
#include <string>
#include <vector>
using namespace std;

ConfigFileItem::ConfigFileItem(string &content):m_content(content){}
ConfigFileItem::ConfigFileItem():m_content(""){}

string ConfigFileItem::getContent()const{return m_content;}
string ConfigFileItem::getLastContentPart()const {return m_lastContentPart;}
void ConfigFileItem::setLastContentPart(string &value){m_lastContentPart=value;}
string ConfigFileItem::getRelativePath()const{return m_relativePath;}
void ConfigFileItem::setRelativePath(string &value){m_relativePath=value;}
void ConfigFileItem::push(string &file){m_fileArray.push_back(file);}

void ConfigFileItem::forEachExe(void (*fn)(const string& ,const string&)){
    for(vector<string>::iterator it=m_fileArray.begin();it!=m_fileArray.end();it++){
        (*fn)(m_relativePath,*it);
    }
}
