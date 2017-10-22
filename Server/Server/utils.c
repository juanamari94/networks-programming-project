//
//  utils.c
//  Server
//
//  Created by Juan Amari on 22/10/2017.
//  Copyright © 2017 Juan Amari. All rights reserved.
//

#include "utils.h"

VOID check_errors(int ret) {
  if(ret != 0) {

    fprintf(stderr, "error: %s\n", gai_strerror(ret));
    exit(1);
  }
}

// https://codereview.stackexchange.com/questions/29198/random-string-generator-in-c
char *rand_string(char *str, size_t size) {
  const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK.";
  if (size) {
    --size;
    for (size_t n = 0; n < size; n++) {
      int key = rand() % (int) (sizeof charset - 1);
      str[n] = charset[key];
    }
    str[size] = '\0';
  }
  return str;
}
