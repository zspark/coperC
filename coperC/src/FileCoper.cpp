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
#include "FileCoper.h"
#include <string>
#include "debug.hpp"
#include "Status.h"
#include "ConfigFileItem.h"
#include "Utils.h"
#include <vector>

using namespace std;

void FileCoper::copyStart(vector<ConfigFileItem>* itemArray){
    checkStatus(itemArray);

    warning("��ʼ����...");

    for(vector<ConfigFileItem>::iterator it =itemArray->begin();it!=itemArray->end();it++){
        it->forEachExe(copyFile);
    }
}

void FileCoper::checkStatus(vector<ConfigFileItem>* itemArray){
    if(itemArray->size()==0)error("���������û������!");
    if(root=="" || target=="")error("��Ҫ·����Ϣ��δ���ã��������ñ����䣬���飡");
}

