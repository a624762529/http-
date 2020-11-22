#include "spellhtml.h"

SpellHtml::SpellHtml()
{

}

string SpellHtml::getTitle(string path)
{
    string head_title=string("<html><head><title>目录名: ")+path+
            "</title></head>\n";
    string body_title=string("<body><h1>当前目录: ")+path+
            "</h1><table>\n";
    return head_title+body_title;
}

string SpellHtml::getDirInfo(string path)
{
    string ret=string("<h1> <a href=")+path
              +("> ")+path+"</a> </h1>\n";
    return ret;
}

string SpellHtml::getDirSendInfo(string dir_path)
{
    if(dir_path.back()=='/')
        dir_path.pop_back();
    string ret;
    DIR* dir = opendir(dir_path.c_str());
    if(dir == NULL)
    {
        perror("opendir error");
        exit(1);
    }

    struct dirent* ptr = NULL;
    while( (ptr = readdir(dir)) != NULL )
    {
        char* name = ptr->d_name;
        if(strcmp(name,".")==0||strcmp(name,"..")==0)
            continue;
        string complete_path=dir_path+ "/" +name;
        struct stat st;

        stat(complete_path.c_str(),&st);
        if(S_ISREG(st.st_mode))
        {
            ret+=this->getDirInfo(complete_path);
        }
        else if(S_ISDIR(st.st_mode))
        {
            ret+=this->getDirInfo(complete_path);
        }

    }
    closedir(dir);
    return ret;
}
