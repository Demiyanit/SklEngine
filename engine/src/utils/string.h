#ifndef _SKL_STRING_H_
#define _SKL_STRING_H_

#include <defines.h>

typedef struct _string {
    char* str_data;
    u64 length;
} string;

typedef struct _wstring {
    u16* wstr_data;
    u64 length;
} wstring;

#pragma region str_utils

void string_str_to_wstr(wstring* dest, const string* src);
void string_wstr_to_str(string* dest, const wstring* src);

u64 string_strlen(const char* str);
u64 string_wstrlen(const wchar* value);

#pragma endregion str_utils

#pragma region string
string string_create();
string string_create_ccp(const char* value);
string string_create_str(string value);
void string_destroy(string* str);
void string_set_ccp(string* str, const char* value);
void string_set_str(string* str, string value);
void string_add_ccp_ccp(string* out_str, const char* str1, const char* str2);
void string_add_str_ccp(string* out_str, string str1, const char* str2);
void string_add_ccp_str(string* out_str, const char* str1, string str2);
void string_add_str_str(string* out_str, string str1, string str2);
void string_add_str_ch(string* out_str, string str1, char ch);
void string_add_ch_str(string* out_str, char ch, string str2);
void string_add_ch_ch(string* out_str, char ch1, char ch2);
void string_add_ccp_ch(string* out_str, const char* str1, char ch);
void string_add_ch_ccp(string* out_str, char ch, const char* str2);

const char* string_c_str(string str);
b8 string_equals_str_str(string str1, string str2);
b8 string_equals_str_ccp(string str1, const char* str2);
b8 string_equals_ccp_ccp(const char* str1, const char* str2);
#pragma endregion string

#pragma region wstring
wstring wstring_create();
wstring wstring_create_wcp(const wchar* value);
wstring wstring_create_wstr(wstring value);
void wstring_destroy(wstring* str);
void wstring_set_wcp(wstring* str, const wchar* value);
void wstring_set_wstr(wstring* str, wstring value);
void wstring_add_wcp_wcp(wstring* out_str, const wchar* str1, const wchar* str2);
void wstring_add_wstr_wcp(wstring* out_str, wstring str1, const wchar* str2);
void wstring_add_wcp_wstr(wstring* out_str, const wchar* str1, wstring str2);
void wstring_add_wstr_wstr(wstring* out_str, wstring str1, wstring str2);
const wchar* wstring_wc_str(wstring str);
b8 wstring_equals_wstr_wstr(wstring str1, wstring str2);
b8 wstring_equals_wstr_wcp(wstring str1, const wchar* str2);
b8 wstring_equals_wcp_wcp(const wchar* str1, const wchar* str2);
#pragma endregion wstring

#endif//_SKL_STRING_H_