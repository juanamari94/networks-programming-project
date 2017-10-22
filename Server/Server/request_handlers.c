//
//  request_handlers.c
//  Server
//
//  Created by Juan Amari on 22/10/2017.
//  Copyright © 2017 Juan Amari. All rights reserved.
//

#include "request_handlers.h"
#include "utils.h"
#include "main.h"

#define MESSAGE_LENGTH 10

PVOID handleRequestWithRandomResponse(void *sfd) {
  int i_sfd = *((int *)sfd);
  char *buffer = malloc(MESSAGE_LENGTH);
  char *msg = rand_string(buffer, MESSAGE_LENGTH);
  send(i_sfd, msg, strlen(msg), 0);
  close(i_sfd);
  return 0;
}
