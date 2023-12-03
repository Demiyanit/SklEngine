#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <utils/string.h>

u64 string_strlen(const char* str) {
    const char* s;
    for (s = str; *s; ++s);
    return (u64)((const char*)s - str);
}

u64 wstring_wstrlen(const wchar* value) {
    const wchar* s;
    for (s = value; *s; ++s);
    return (u64)((const char*)s - (u64)value);
}

#pragma region str_utils

void string_str_to_wstr(wstring* dest, const string* src) {
    const u64 total_length = src->length + 1;
    free(dest->wstr_data);
    dest->wstr_data = malloc(total_length * sizeof(wchar));
    dest->length = total_length - 1;
    mbstowcs(dest->wstr_data, src->str_data, total_length);
}

void string_wstr_to_str(string* dest, const wstring* src) {
    const u64 total_length = src->length + 1;
    free(dest->str_data);
    dest->length = total_length - 1;
    dest->str_data = malloc(total_length);
    wcstombs(dest->str_data, src->wstr_data, total_length);
}

#pragma endregion str_utils

// string functions
#pragma region string

string string_create() {
    string ret =  {0};
    ret.length = 0;
    ret.str_data = malloc(1);
    ret.str_data[ret.length] = '\0';
    return ret;
}

string string_create_ccp(const char* value) {
    string ret =  {0};
    ret.length = string_strlen(value);
    ret.str_data = malloc(ret.length + 1);
    memcpy(ret.str_data, value, ret.length);
    ret.str_data[ret.length] = '\0';
    return ret;
}

string string_create_str(string value) {
    string ret = {0};
    ret.length = value.length;
    ret.str_data = malloc(value.length + 1);
    memcpy(ret.str_data, value.str_data, ret.length + 1); // ret.length + 1 = '\0'
    return ret;
}

void string_destroy(string* str) {
    free(str->str_data);
    str->length = 0;
    str->str_data = 0;
}

void string_set_ccp(string* str, const char* value) {
    string_destroy(str);
    *str = string_create_ccp(value);
}

void string_set_str(string* str, string value) {
    string_destroy(str);
    *str = string_create_str(value);
}

void string_add_ccp_ccp(string* out_str, const char* str1, const char* str2) {
    string_destroy(out_str);
    u64 length1 = string_strlen(str1);
    u64 length2 = string_strlen(str2);

    out_str->str_data = malloc(length1 + length2 + 1);
    out_str->length = length1 + length2;
    char* ptr = out_str->str_data;
    memcpy(ptr, str1, length1);
    ptr+= length1;
    memcpy(ptr, str2, length2);
    out_str->str_data[length1 + length2] = '\0';
}

void string_add_str_ccp(string* out_str, string str1, const char* str2) {
    string local_str1 = string_create_str(str1);
    string_destroy(out_str);
    u64 length2 = string_strlen(str2);

    out_str->str_data = malloc(local_str1.length + length2 + 1);
    out_str->length = local_str1.length + length2;
    char* ptr = out_str->str_data;
    memcpy(ptr, local_str1.str_data, local_str1.length);
    ptr+= local_str1.length;
    memcpy(ptr, str2, length2);
    out_str->str_data[local_str1.length + length2] = '\0';
    string_destroy(&local_str1);
}

void string_add_ccp_str(string* out_str, const char* str1, string str2) {
    string local_str2 = string_create_str(str2);
    string_destroy(out_str);
    u64 length1 = string_strlen(str1);

    out_str->str_data = malloc(local_str2.length + length1 + 1);
    out_str->length = local_str2.length + length1;
    char* ptr = out_str->str_data;
    memcpy(ptr, str1, length1);
    ptr+= length1;
    memcpy(ptr, local_str2.str_data, local_str2.length);
    out_str->str_data[local_str2.length + length1] = '\0';
    string_destroy(&local_str2);
}

void string_add_str_str(string* out_str, string str1, string str2) {
    string local_str1 = string_create_str(str1);
    string local_str2 = string_create_str(str2);
    string_destroy(out_str);
    out_str->str_data = malloc(local_str1.length + local_str2.length + 1);
    out_str->length = local_str1.length + local_str2.length;
    u64 ptr = 0;
    for (u64 i = 0; i < local_str1.length; ++i) {
        out_str->str_data[ptr] = local_str1.str_data[i];
        ptr++;
    }
    for (u64 i = 0; i < local_str2.length; ++i) {
        out_str->str_data[ptr] = local_str2.str_data[i];
        ptr++;
    }
    out_str->str_data[local_str1.length + local_str2.length] = '\0';
    string_destroy(&local_str1);
    string_destroy(&local_str2);
}

void string_add_str_ch(string* out_str, string str1, char ch) {
    string local_str1 = string_create_str(str1);
    string_destroy(out_str);
    out_str->str_data = malloc(local_str1.length + 2);//char to add + \0 << null terminating character
    out_str->length = local_str1.length + 1;
    u64 ptr = 0;
    for (u64 i = 0; i < local_str1.length; ++i) {
        out_str->str_data[ptr] = local_str1.str_data[i];
        ptr++;
    }
    out_str->str_data[ptr] = ch;
    out_str->str_data[out_str->length] = '\0';
    string_destroy(&local_str1);
}

void string_add_ch_str(string* out_str, char ch, string str2) {
    string local_str2 = string_create_str(str2);
    string_destroy(out_str);
    out_str->str_data = malloc(local_str2.length + 2);//char to add + \0 << null terminating character
    out_str->length = local_str2.length + 1;
    u64 ptr = 0;
    out_str->str_data[ptr] = ch;
    ptr++;
    for (u64 i = 0; i < local_str2.length; ++i) {
        out_str->str_data[ptr] = local_str2.str_data[i];
        ptr++;
    }
    out_str->str_data[out_str->length] = '\0';
    string_destroy(&local_str2);
}

void string_add_ch_ch(string* out_str, char ch1, char ch2) {
    string_destroy(out_str);
    out_str->str_data = malloc(3);
    out_str->length = 2;
    out_str->str_data[0] = ch1;
    out_str->str_data[1] = ch2;
    out_str->str_data[2] = '\0';
}

void string_add_ccp_ch(string* out_str, const char* str1, char ch) {
    string_destroy(out_str);
    u64 length = string_strlen(str1);
    out_str->str_data = malloc(length + 2);
    for (u64 i = 0; i < length; ++i) {
        out_str->str_data[i] = str1[1];
    }
    out_str->str_data[length] = ch;
    out_str->str_data[length + 1] = '\0';
}

void string_add_ch_ccp(string* out_str, char ch, const char* str2) {
    string_destroy(out_str);
    u64 length = string_strlen(str2);
    out_str->str_data = malloc(length + 2);
    out_str->str_data[0] = ch;
    for (u64 i = 1; i < length + 1; ++i) {
        out_str->str_data[i] = str2[1];
    }
    out_str->str_data[length + 1] = '\0';
}


const char* string_c_str(string str) {
    return (const char*)str.str_data;
}

b8 string_equals_str_str(string str1, string str2) {
    if(str1.length != str2.length) return false;
    for(u64 i = 0; i < str1.length; ++i) {
        if(str1.str_data[i] != str2.str_data[i]) return false;
    }
    return true;
}

b8 string_equals_str_ccp(string str1, const char* str2) {
    if(str1.length != string_strlen(str2)) return false;
    for(u64 i = 0; i < str1.length; ++i) {
        if(str1.str_data[i] != str2[i]) return false;
    }
    return true;
}

b8 string_equals_ccp_ccp(const char* str1, const char* str2) {
    u64 length = string_strlen(str1);
    if(length != string_strlen(str2)) return false;
    for(u64 i = 0; i < length; ++i) {
        if(str1[i] != str2[i]) return false;
    }
    return true;
}

#pragma endregion string


// wide string functions
#pragma region wstring

wstring wstring_create() {
    wstring ret =  {0};
    ret.length = 0;
    ret.wstr_data = malloc(1 * sizeof(wchar));
    ret.wstr_data[ret.length] = L'\0';
    return ret;
}

wstring wstring_create_wcp(const wchar* value) {
    wstring ret =  {0};
    ret.length = wstring_wstrlen(value);
    ret.wstr_data = malloc((ret.length + 1) * sizeof(wchar));
    memcpy(ret.wstr_data, value, sizeof(wchar) * ret.length);
    ret.wstr_data[ret.length] = L'\0';
    return ret;
}

wstring wstring_create_wstr(wstring value) {
    wstring ret = {0};
    ret.length = value.length;
    ret.wstr_data = malloc((value.length + 1) * sizeof(wchar));
    memcpy(ret.wstr_data, value.wstr_data, (ret.length + 1) * sizeof(wchar)); // ret.length + 1 = '\0'
    return ret;
}

void wstring_destroy(wstring* str) {
    free(str->wstr_data);
    str->length = 0;
    str->wstr_data = 0;
}

void wstring_set_wcp(wstring* str, const wchar* value) {
    wstring_destroy(str);
    *str = wstring_create_wcp(value);
}

void wstring_set_wstr(wstring* str, wstring value) {
    wstring_destroy(str);
    *str = wstring_create_wstr(value);
}

void wstring_add_wcp_wcp(wstring* out_str, const wchar* str1, const wchar* str2) {
    wstring_destroy(out_str);
    u64 length1 = wstring_wstrlen(str1);
    u64 length2 = wstring_wstrlen(str2);

    out_str->wstr_data = malloc((length1 + length2 + 1) * sizeof(wchar));
    out_str->length = length1 + length2;
    wchar* ptr = out_str->wstr_data;
    memcpy(ptr, str1, sizeof(wchar) * length1);
    ptr+= length1;
    memcpy(ptr, str2, sizeof(wchar) * length2);
    out_str->wstr_data[length1 + length2] = L'\0';
}

void wstring_add_wstr_wcp(wstring* out_str, wstring str1, const wchar* str2) {
    wstring local_str1 = wstring_create_wstr(str1);
    wstring_destroy(out_str);
    u64 length2 = wstring_wstrlen(str2);

    out_str->wstr_data = malloc((local_str1.length + length2 + 1) * sizeof(wchar));
    out_str->length = local_str1.length + length2;
    wchar* ptr = out_str->wstr_data;
    memcpy(ptr, local_str1.wstr_data, sizeof(wchar) * local_str1.length);
    ptr+= local_str1.length;
    memcpy(ptr, str2, sizeof(wchar) * length2);
    out_str->wstr_data[local_str1.length + length2] = L'\0';
    wstring_destroy(&local_str1);
}

void wstring_add_wcp_wstr(wstring* out_str, const wchar* str1, wstring str2) {
    wstring local_str2 = wstring_create_wstr(str2);
    wstring_destroy(out_str);
    u64 length1 = wstring_wstrlen(str1);

    out_str->wstr_data = malloc((local_str2.length + length1 + 1) * sizeof(wchar));
    out_str->length = local_str2.length + length1;
    wchar* ptr = out_str->wstr_data;
    memcpy(ptr, str1, sizeof(wchar) * length1);
    ptr+= length1;
    memcpy(ptr, local_str2.wstr_data, sizeof(wchar) * local_str2.length);
    out_str->wstr_data[local_str2.length + length1] = L'\0';
    wstring_destroy(&local_str2);
}

void wstring_add_wstr_wstr(wstring* out_str, wstring str1, wstring str2) {
    wstring local_str1 = wstring_create_wstr(str1);
    wstring local_str2 = wstring_create_wstr(str2);
    wstring_destroy(out_str);
    out_str->wstr_data = malloc((local_str1.length + local_str2.length + 1) * sizeof(wchar));
    out_str->length = local_str1.length + local_str2.length;
    u64 ptr = 0;
    for (u64 i = 0; i < local_str1.length; ++i) {
        out_str->wstr_data[ptr] = local_str1.wstr_data[i];
        ptr++;
    }
    for (u64 i = 0; i < local_str2.length; ++i) {
        out_str->wstr_data[ptr] = local_str2.wstr_data[i];
        ptr++;
    }
    out_str->wstr_data[local_str1.length + local_str2.length] = L'\0';
    wstring_destroy(&local_str1);
    wstring_destroy(&local_str2);
}

void wstring_add_wstr_wch(wstring* out_wstr, wstring wstr1, wchar ch) {
    wstring local_wstr1 = wstring_create_wstr(wstr1);
    wstring_destroy(out_wstr);
    out_wstr->wstr_data = malloc((local_wstr1.length + 2) * 2);//char to add + \0 << null terminating character
    out_wstr->length = local_wstr1.length + 1;
    u64 ptr = 0;
    for (u64 i = 0; i < local_wstr1.length; ++i) {
        out_wstr->wstr_data[ptr] = local_wstr1.wstr_data[i];
        ptr++;
    }
    out_wstr->wstr_data[ptr] = ch;
    out_wstr->wstr_data[out_wstr->length] = L'\0';
    wstring_destroy(&local_wstr1);
}

void wstring_add_wch_wstr(wstring* out_wstr, wchar ch, wstring wstr2) {
    wstring local_wstr2 = wstring_create_wstr(wstr2);
    wstring_destroy(out_wstr);
    out_wstr->wstr_data = malloc((local_wstr2.length + 2) * 2);//char to add + \0 << null terminating character
    out_wstr->length = local_wstr2.length + 1;
    u64 ptr = 0;
    out_wstr->wstr_data[ptr] = ch;
    ptr++;
    for (u64 i = 0; i < local_wstr2.length; ++i) {
        out_wstr->wstr_data[ptr] = local_wstr2.wstr_data[i];
        ptr++;
    }
    out_wstr->wstr_data[out_wstr->length] = L'\0';
    wstring_destroy(&local_wstr2);
}

void wstring_add_wch_wch(wstring* out_wstr, wchar wch1, wchar wch2) {
    wstring_destroy(out_wstr);
    out_wstr->wstr_data = malloc(3 * 2);
    out_wstr->length = 2;
    out_wstr->wstr_data[0] = wch1;
    out_wstr->wstr_data[1] = wch2;
    out_wstr->wstr_data[2] = L'\0';
}

void wstring_add_wcp_wch(wstring* out_wstr, const wchar* wstr1, wchar wch) {
    wstring_destroy(out_wstr);
    u64 length = wstring_wstrlen(wstr1);
    out_wstr->wstr_data = malloc((length + 2) * 2);
    for (u64 i = 0; i < length; ++i) {
        out_wstr->wstr_data[i] = wstr1[1];
    }
    out_wstr->wstr_data[length] = wch;
    out_wstr->wstr_data[length + 1] = L'\0';
}

void string_add_wch_wcp(wstring* out_wstr, wchar wch, const wchar* wstr2) {
    wstring_destroy(out_wstr);
    u64 length = wstring_wstrlen(wstr2);
    out_wstr->wstr_data = malloc((length + 2) * 2);
    out_wstr->wstr_data[0] = wch;
    for (u64 i = 1; i < length + 1; ++i) {
        out_wstr->wstr_data[i] = wstr2[1];
    }
    out_wstr->wstr_data[length + 1] = L'\0';
}

const wchar* wstring_wc_str(wstring str) {
    return str.wstr_data;
}

b8 wstring_equals_wstr_wstr(wstring str1, wstring str2) {
    if(str1.length != str2.length) return false;
    for(u64 i = 0; i < str1.length; ++i) {
        if(str1.wstr_data[i] != str2.wstr_data[i]) return false;
    }
    return true;
}

b8 wstring_equals_wstr_wcp(wstring str1, const wchar* str2) {
    if(str1.length != wstring_wstrlen(str2)) return false;
    for(u64 i = 0; i < str1.length; ++i) {
        if(str1.wstr_data[i] != str2[i]) return false;
    }
    return true;
}

b8 wstring_equals_wcp_wcp(const wchar* str1, const wchar* str2) {
    u64 length = wstring_wstrlen(str1);
    if(length != wstring_wstrlen(str2)) return false;
    for(u64 i = 0; i < length; ++i) {
        if(str1[i] != str2[i]) return false;
    }
    return true;
}

#pragma endregion wstring
