//
//  main.c
//  Server
//
//  Created by Juan Amari on 9/10/17.
//  Copyright © 2017 Juan Amari. All rights reserved.
//

#include "main.h"
#include <assert.h>

#define BACKLOG 10
#define MESSAGE_LENGTH 10

PAPP_ARGUMENTS getProgramArguments(char **arguments) {

  PAPP_ARGUMENTS pArguments = malloc(sizeof(APP_ARGUMENTS));
  pArguments->port    = arguments[1];

  return pArguments;
}

VOID displayUsage() {

  fprintf(stdout, "Usage: server [port]\n");
  fprintf(stdout, "EXAMPLES:\n");
  fprintf(stdout, "%-20s\n", "server 8080");
}

VOID check_errors(int ret) {
  if(ret != 0) {

    fprintf(stderr, "error: %s\n", gai_strerror(ret));
    exit(1);
  }
}

// https://codereview.stackexchange.com/questions/29198/random-string-generator-in-c
static char *rand_string(char *str, size_t size) {
  const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
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

PVOID handleRequest(void *sfd) {
  int i_sfd = *((int *)sfd);
  char *buffer = malloc(MESSAGE_LENGTH);
  char *msg = rand_string(buffer, MESSAGE_LENGTH);
  send(i_sfd, msg, strlen(msg), 0);
  close(i_sfd);
  return 0;
}

int main(int argc, char * argv[]) {

  int ret, sockfd, new_fd;
  struct sockaddr_storage their_addr;
  socklen_t addr_size;
  struct addrinfo     hints, *res;                      // template
  PAPP_ARGUMENTS      arguments;                  // arguments

  // arguments
  if (argc < 2) {

    displayUsage();
    return 0;
  } else {

    arguments = getProgramArguments(argv);
  }

  // prepare template server info struct
  memset(&hints, 0, sizeof(hints));
  hints.ai_family 	= AF_INET;
  hints.ai_socktype 	= SOCK_STREAM;

  //
  // get server information
  //
  printf("Starting server on port %s...\n", arguments->port);
  ret = getaddrinfo(NULL, arguments->port, &hints, &res);

  assert(ret == 0);

  if (ret != 0) {

    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
    return 1;
  }

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  ret = bind(sockfd, res->ai_addr, res->ai_addrlen);
  assert(ret == 0);

  ret = listen(sockfd, BACKLOG);
  assert(ret == 0);

  free(res);
  while(TRUE) {
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    printf("Accepted new connection\n");

    pthread_t      worker;
    pthread_attr_t workerAttrs;
    
    pthread_attr_init(&workerAttrs);
    pthread_attr_setdetachstate(&workerAttrs, PTHREAD_CREATE_DETACHED);
    pthread_attr_setschedpolicy(&workerAttrs, SCHED_FIFO);
    pthread_create(&worker, &workerAttrs, handleRequest, &new_fd);
  }
}
