//
//  utils.h
//  Server
//
//  Created by Juan Amari on 22/10/2017.
//  Copyright © 2017 Juan Amari. All rights reserved.
//

#include <stdio.h>
#include "main.h"

#ifndef utils_h
#define utils_h

ULONG get_long_size(ULONG number);
VOID check_errors(int ret);
char *rand_string(char *str, size_t size);

#endif /* utils_h */
