#include "hcr.h"

/* remove all comments and check parenthesis integrity */
void CChecker(FILE *fin) {
  int brack, brace, quote, paren, c, state;
  brace = brack = quote = paren = c = state = 0;
  while ((c = fgetc(fin)) != EOF) {
    // finite state machine

    switch (state) {
      // inside block comment
    case CCK_BLOCKCMT:
      if (c == '*') {
        state = CCK_STAR;
      }
      break;

      // inside line comment
    case CCK_COMMENT:
      if (c == '\n') {
        state = CCK_FILE;
      }
      break;

      // read one slash
    case CCK_SLASH:
      if (c == '*') {
        state = CCK_BLOCKCMT;
      } else if (c == '/') {
        state = CCK_COMMENT;
      } else {
        state = CCK_FILE;
        putc('/', stdout);
        putc(c, stdout);
      }
      break;

      // read one star in block comment
    case CCK_STAR:
      if (c == '/') {
        state = CCK_FILE;
      } else
        state = CCK_BLOCKCMT;
      break;

      // backslash
    case CCK_BACKSLASH:
      putc(c, stdout);
      state = CCK_FILE;
      break;

    case CCK_FILE:
      if (c == '/') {
        state = CCK_SLASH;
      } else if (c == '\\') {
        state = CCK_BACKSLASH;
        putc(c, stdout);
      } else {
        putc(c, stdout);
        // checker
        switch (c) {
        case '{':
          brace++;
          break;
        case '}':
          brace--;
          break;
        case '(':
          paren++;
          break;
        case ')':
          paren--;
          break;
        case '[':
          brack++;
          break;
        case ']':
          brack--;
          break;
        case '\"':
          if (quote) {
            quote--;
          } else
            quote++;
          break;
        default:
          break;
        }
      }
      break;

    default:
      break;
    }
  }
  putc('\n', stdout);
  if (paren | brace | brack | quote) {
    unix_panic("wrong grammar!\n");
  }
}
