#pragma once
#include"head.h"
void trit_source(char *p,char *buf1,char *path,char *proth);
void remove_rn(char *buf,int len);
void send_dir(int cfd, const char* dirname);
void send_respond_head(int cfd, int no, const char* desp, const char* type, long len);
void send_file(int cfd, const char* filename);
int get_line(int sock, char *buf, int size);
int hexit(char c);
const char *get_file_type(const char *name);
void decode_str(char *to, char *from);
void encode_str(char* to, int tosize, const char* from);

