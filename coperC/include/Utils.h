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

using namespace std;
int _stat(const char* path,struct _stat* buffer);
//////////////////////////file/////////////////////////////
char* read(char* fileURL);
void readTextFileToArray(char* fileURL,vector<string>* ret);
bool checkFileExistance(const string &fileURL);
bool checkDirectoryExistance(const string &dirURL);
void copyFileTo(string& fileURL,string & folderURL);
bool isDirectoryNoFiles(string &fileURL);

void getDirectoryFiles(const string &directoryURL,const char *filespec,vector<string>* fileNames);

void createDirectory(string &directoryURL);

bool isExtensionRight(const string& fileName,const string& ext);

int numberLength(int a);

bool isStringAllDecNumber(const string &str);

string fixToLength(int num,int l);

void split(const string& s,const string& delim,vector<string>* ret);

string& trim(string &s);
string&   replace_all(string&   str,const   string&   old_value,const   string&   new_value);

void copyFile(const string & relativePath,const string & fileName);

#endif // UTILS_H_INCLUDED
