#include <stdio.h>
#include <stdlib.h>
char *format_msg(char *str, int len) {
  char *res = (char *)malloc(256 * sizeof(char));
  int start = 0;
  int res_len = 0;
  for (int i = 0; i < len; i++) {
    if (str[i] != ' ') {
      if (str[i] >= 'a' && str[i] <= 'z')
        str[i] -= 32;
      res[0] = str[i];
      start = i + 1;
      res_len++;
      break;
    }
  }
  for (int i = start; i < len; i++) {
    if ((str[i - 1] == ' ') && str[i] != ' ') {
      if (str[i] >= 'a' && str[i] <= 'z')
        str[i] -= 32;
      res[res_len] = str[i];
      res_len++;
    } else if (str[i - 1] != ' ') {
      res[res_len] = str[i];
      res_len++;
    }
  }
  res[res_len - 1] = 0;
  if (res[res_len - 2] == ' ')
    res[res_len - 2] = 0;
  return res;
}