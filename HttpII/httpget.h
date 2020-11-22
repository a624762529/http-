#ifndef HTTP_H
#define HTTP_H
#include"head.h"
#include<iostream>
#include<sstream>
using namespace std;

/*
 *功能:对http进行基本的拆包
 *    仅仅对http的get方法进行拆包
*/

//GET / HTTP/1.1
//Content-Type: text/plain; charset=iso-8859-1
//Content-Length: 32


class HttpGet
{
public:
    HttpGet();
    void   InitHttpInfo(char *info,int len);
    string getSourceType(string file_type);
public:
    int    m_http_code;        //http的状态
    string m_method;           //http方法
    string m_source;           //request资源路径
    string m_http_edition;     //http的版本
    string m_type;             //反馈回去的文件类型
};


#endif // HTTP_H
