#include"httpsend.h"

httpSend::httpSend()
{

}

string httpSend::getHttpHead()
{
    //http/1.1 状态码 辅助信息\r\n
    string first_info =string("http/1.1 ")+to_string(m_http_code)
            +" " + m_desp+ "\r\n";
    //Content-Type: text/plain; charset=iso-8859-1\r\n
    string second_info=string("Content-Type: ")+ (m_type)+"\r\n";
    //Content-Length: 32\r\n
    string thir_info="Content-Length: -1\r\n";

    return first_info+second_info+thir_info+"\r\n";
}


void httpSend::setInfo(int http_code,string desp,
             string source,string http_edition,
             string type,  int    info_len)
{
    m_http_code   =http_code;
    m_desp        =desp;
    m_source      =source;
    m_http_edition=http_edition;
    m_type        =type;
    m_info_len    =info_len;
}
