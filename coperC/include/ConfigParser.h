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
#ifndef CONFIGPARSER_H_INCLUDED
#define CONFIGPARSER_H_INCLUDED
#include <string>
#include "Status.h"
#include "ConfigFileItem.h"
#include <vector>

class ConfigParser{

    void parseItem(ConfigFileItem *item);
    void checkExistAndPush(const string &fileName,ConfigFileItem *item,const string &fixedURL);
    void parseContentWithBigBracket(ConfigFileItem *tem);
    void pushSameNameFilesIntoItem(string &fileNameN,ConfigFileItem *item);
    void pushDirectoryFilesIntoItem(ConfigFileItem *item,const string &ext,const string &fixedURL);
    void parseRange(const string& range,const string& ext,ConfigFileItem *item);

public:

    /**
     * 解析优先级：\,{},|,:,~.
     * @param configFile		配置文件的本地绝对路径；
     * @param itemArr
     *
     */
     void parse(vector<string>*,vector<ConfigFileItem>*);

};

#endif // CONFIGPARSER_H_INCLUDED
