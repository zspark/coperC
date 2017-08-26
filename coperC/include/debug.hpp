#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED
#include <windows.h>
#include <stdio.h>
#include <string>
#include <iostream>

HANDLE static h=GetStdHandle(STD_OUTPUT_HANDLE);

void static info(const string& s){
  SetConsoleTextAttribute(h,FOREGROUND_INTENSITY|FOREGROUND_GREEN);
  cout<<s<<endl;
}

void static info(const char*c){
  SetConsoleTextAttribute(h,FOREGROUND_INTENSITY|FOREGROUND_GREEN);
  cout<<c<<endl;
}
void static error(const string &s){
  SetConsoleTextAttribute(h,FOREGROUND_INTENSITY|FOREGROUND_RED);
  cout<<s<<endl;
}
void static warning(const string &s){
  SetConsoleTextAttribute(h,FOREGROUND_INTENSITY|FOREGROUND_BLUE);
  cout<<s<<endl;
}

#endif // DEBUG_H_INCLUDED
