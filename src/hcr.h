#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

/*
    Comment: When to use #define and when to use enum?
  
    It depends on what you need the value for. You (and everyone else so far) omitted the third alternative:

    (1) static const int var = 5;
    (2) #define var 5
    (3) enum { var = 5 };

    Ignoring issues about the choice of name, then:

    If you need to pass a pointer around, you must use (1).
    Since (2) is apparently an option, you don't need to pass pointers around.
    Both (1) and (3) have a symbol in the debugger's symbol table - that makes debugging easier. It is more likely that (2) will not have a symbol, leaving you wondering what it is.
    (1) cannot be used as a dimension for arrays at global scope; both (2) and (3) can.
    (1) cannot be used as a dimension for static arrays at function scope; both (2) and (3) can.
    Under C99, all of these can be used for local arrays. Technically, using (1) would imply the use of a VLA (variable-length array), though the dimension referenced by 'var' would of course be fixed at size 5.
    (1) cannot be used in places like switch statements; both (2) and (3) can.
    (1) cannot be used to initialize static variables; both (2) and (3) can.
    (2) can change code that you didn't want changed because it is used by the preprocessor; both (1) and (3) will not have unexpected side-effects like that.
    You can detect whether (2) has been set in the preprocessor; neither (1) nor (3) allows that.
    So, in most contexts, prefer the 'enum' over the alternatives. Otherwise, the first and last bullet points are likely to be the controlling factors — and you have to think harder if you need to satisfy both at once.

    If you were asking about C++, then you'd use option (1) — the static const — every time. 

    source: https://stackoverflow.com/questions/1674032/static-const-vs-define-vs-enum 
 */

// wc program
void wc(FILE *fin);

// c checker program
void CChecker(FILE *fin);

// error handling
void unix_panic(const char* msg);
void posix_panic(int code, const char* msg);
void panic(const char* msg);

// machine dependent type info
void tminfo();

// binary search
int binSearch(int x, int vec[], int sz);

// sorting algorithms
void shellsort(int v[], int n);

// array utilities
void swap(int v[], int i, int j);
void reverse(char s[]);

// colored output

// driver

// debugging
#define DEBUG fprintf(stderr, "line: %s\n", __LINE__);
