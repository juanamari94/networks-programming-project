//
//  main.c
//  Server
//
//  Created by Juan Amari on 9/10/17.
//  Copyright © 2017 Juan Amari. All rights reserved.
//

#include "main.h"
#include <assert.h>
#include "utils.h"
#include "server.h"

PAPP_ARGUMENTS getProgramArguments(char **arguments) {

  PAPP_ARGUMENTS pArguments = malloc(sizeof(APP_ARGUMENTS));
  pArguments->port    = arguments[1];
  pArguments->serverType = atoi(arguments[2]);

  return pArguments;
}

VOID displayUsage() {

  fprintf(stdout, "Usage: server [port] [server type]\n");
  fprintf(stdout, "server type must be 1 or 2\n");
  fprintf(stdout, "EXAMPLES:\n");
  fprintf(stdout, "%-20s\n", "server 8080 1");
}

VOID start_server(VOID (*server_handler)(struct addrinfo, struct addrinfo *), PAPP_ARGUMENTS arguments) {

  struct addrinfo hints, *serverInfo;
  int ret;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family   = AF_INET;
  hints.ai_socktype   = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  //
  // get server information
  //
  printf("Starting server on port %s...\n", arguments->port);
  ret = getaddrinfo(NULL, arguments->port, &hints, &serverInfo);

  check_errors(ret);
  server_handler(hints, serverInfo);
};

int main(int argc, char * argv[]) {

  PAPP_ARGUMENTS      arguments;                  // arguments

  // arguments
  if (argc < 2) {

    displayUsage();
    return 0;
  } else {

    arguments = getProgramArguments(argv);
  }

  switch (arguments->serverType) {
    case 1:
      printf("Started a multithread server\n");
      start_server(multithread_server, arguments);
      break;

    case 2:
      printf("Started a polling server\n");
      start_server(polling_server, arguments);
      break;

    default:
      displayUsage();
      exit(0);
      break;
  }
  return 0;
}
