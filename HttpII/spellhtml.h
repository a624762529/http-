#ifndef SPELLHTML_H
#define SPELLHTML_H

#include"head.h"
#include<iostream>
using namespace std;

class SpellHtml
{
public:
    SpellHtml();
public:
    string getTitle(string info);
    string getDirInfo(string path);
    string getDirSendInfo(string dir_path);
};

#endif // SPELLHTML_H
