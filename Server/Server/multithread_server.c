//
//  multithread_server.c
//  Server
//
//  Created by Juan Amari on 22/10/2017.
//  Copyright © 2017 Juan Amari. All rights reserved.
//

#include <stdio.h>
#include "multithread_server.h"
#include "request_handlers.h"

#define BACKLOG 10

void multithread_server(PAPP_ARGUMENTS arguments, struct addrinfo hints, struct addrinfo *serverInfo) {
  int ret, sockfd, new_fd;
  struct sockaddr_storage their_addr;
  socklen_t addr_size;

  sockfd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

  ret = bind(sockfd, serverInfo->ai_addr, serverInfo->ai_addrlen);

  ret = listen(sockfd, BACKLOG);

  free(serverInfo);
  while(TRUE) {
    new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    printf("Accepted new connection\n");

    pthread_t      worker;
    pthread_attr_t workerAttrs;

    pthread_attr_init(&workerAttrs);
    pthread_attr_setdetachstate(&workerAttrs, PTHREAD_CREATE_DETACHED);
    pthread_attr_setschedpolicy(&workerAttrs, SCHED_FIFO);
    pthread_create(&worker, &workerAttrs, handleRequestWithRandomResponse, &new_fd);
  }
}
