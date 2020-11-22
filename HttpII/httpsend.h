#ifndef HTTPII_H
#define HTTPII_H

#include<string>
#include<cstring>

using namespace std;


//GET / HTTP/1.1
//Content-Type: text/plain; charset=iso-8859-1
//Content-Length: 32

class httpSend
{
public:
    httpSend();
    void setInfo(int http_code,string desp,
                 string source,string http_edition,
                 string type,  int    info_len
                 );
    string getHttpHead();
public:
    int    m_http_code;        //http的状态 250
    string m_desp;             //http方法
    string m_source;           //request资源路径
    string m_http_edition;     //http的版本
    string m_type;             //反馈回去的文件类型
    int    m_info_len;         //信息的长度
private:
};

#endif // HTTPII_H
