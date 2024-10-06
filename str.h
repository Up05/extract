#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long u64;

#define max(a, b) ((a) * ((a) < (b)) + (b) * ((a) >= (b))) 

int str_eq(char* a, char* b) {
    for(int i = 0;; i ++) {
        if(a[i] == 0 && b[i] == 0) return 1;
        if(a[i] == 0 || b[i] == 0) return 0;

        if(a[i] != b[i]) return 0;
    }
    return 1;
}

int str_contains(char* a, char* b) {
    int len_a = strlen(a),
        len_b = strlen(b);

    if(len_a < len_b) return 0;
    for(int i = 0; len_a < len_b; i ++) {
        if(str_eq(a, b)) return 1;
        len_a --;
        a ++;
    }

    return 1;
}

int str_ends_with(char* str, char* suffix) {
    int len_a = strlen(str),
        len_b = strlen(suffix);
 
    if(!len_a || !len_b) return 0;
    if(len_a < len_b) return 0;

    return str_eq(str + len_a - len_b, suffix);

}

char* str_concat(int count, char* str, ...) {
    int len = 0;
    int cap = strlen(str) * 2;
    char* buf = malloc(cap);
    va_list list;
    int i = 0;
    va_start(list, str);
    do {
        // printf("[%d/%d]: %p\n", i + 1, count, str);
        int this_len = strlen(str);
        if(cap <= len + this_len) {
            cap += max(cap, this_len);
            buf = realloc(buf, cap + 1);
        }
        memmove(buf + len, str, this_len);
        len += this_len;
        buf[len] = 0;
    } while(++ i < count && (u64) (str = va_arg(list, char*)) > 1);
    va_end(list);
    return buf;
}
