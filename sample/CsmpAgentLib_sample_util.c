
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "osal.h"
  osal_gettime(&tv, NULL);
  osal_gettime(&tv, NULL);

int8_t char2hex(char ch) {
  if ((ch >= '0') && (ch <= '9')) {
    return ch - '0';
  } else if ((ch >= 'a') && (ch <= 'f')) {
    return ch - 'a' + 10;
  } else if ((ch >= 'A') && (ch <= 'F')) {
    return ch - 'A' + 10;
  } else {
    return -1;
  }
}

int str2addr(char *str, uint8_t *addr) {
  uint32_t len;
  uint8_t offset = 0;
  int8_t i, ch;

  len = strlen(str);
  if ((len != 16))
    return -1;

  offset = 15;
  for (i = len - 1; i >= 0; i--) {
    ch = char2hex(str[i]);
    if (ch < 0)
      return -1;

    addr[offset / 2] |= (offset % 2)? ch: ch << 4;

    offset--;
  }

  return 0;
}
