//
//  server.c
//  Server
//
//  Created by Juan Amari on 22/10/2017.
//  Copyright © 2017 Juan Amari. All rights reserved.
//

#include "server.h"

#define BACKLOG 10
#define CHUNK_SIZE 1024

BOOL is_favicon_request(PCHAR request);

PCHAR handle_req_recv(INT cx_socket, INT flags) {

  PCHAR chunk = NULL;

  chunk = malloc(CHUNK_SIZE);
  memset(chunk, 0, CHUNK_SIZE);

  ssize_t rxBytes = 0;

  rxBytes = recv(cx_socket, chunk, CHUNK_SIZE, 0);

  if (rxBytes == -1) {

    printf("recv error: ");
    perror("client: connect");
  }

  if(rxBytes == 0) {

    printf("\nConnection closed by peer. \n");
  }

  printf("%s", chunk);
  printf("\n\n-------------------------\n");

  return chunk;
}

void multithread_server(struct addrinfo hints, struct addrinfo *serverInfo) {

  int ret, sockfd;
  struct sockaddr_storage their_addr;
  socklen_t addr_size;

  sockfd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

  int reuse = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");

  ret = bind(sockfd, serverInfo->ai_addr, serverInfo->ai_addrlen);

  ret = listen(sockfd, BACKLOG);

  free(serverInfo);
  while(TRUE) {

    int new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    printf("Accepted new connection: %d\n", new_fd);
    PCHAR request = handle_req_recv(new_fd, 0);

    if (is_favicon_request(request)) {

      close(new_fd);
    } else {

      pthread_t      worker;
      pthread_attr_t workerAttrs;

      pthread_attr_init(&workerAttrs);
      pthread_attr_setdetachstate(&workerAttrs, PTHREAD_CREATE_DETACHED);
      pthread_attr_setschedpolicy(&workerAttrs, SCHED_FIFO);
      pthread_create(&worker, &workerAttrs, (PVOID) handle_request_with_image, &new_fd);
    }
  }
}

void threadpool_server(struct addrinfo hints, struct addrinfo *serverInfo) {

  int ret, sockfd;
  struct sockaddr_storage their_addr;
  socklen_t addr_size;

  sockfd = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

  int reuse = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");

  ret = bind(sockfd, serverInfo->ai_addr, serverInfo->ai_addrlen);

  ret = listen(sockfd, BACKLOG);

  free(serverInfo);
  threadpool_t *pool = threadpool_create(10, 20, 0);

  while(TRUE) {

    int new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

    if (new_fd == -1) {
      perror("Accept returned -1");
      continue;
    }

    printf("Accepted new connection: %d\n", new_fd);
    PCHAR request = handle_req_recv(new_fd, 0);

    if (is_favicon_request(request)) {

      close(new_fd);
    } else {

      threadpool_add(pool, handle_request_with_image, &new_fd, 0);
    }
  }
}

void polling_server(struct addrinfo hints, struct addrinfo *serverInfo) {

  int socketListen = socket(serverInfo->ai_family,
                            serverInfo->ai_socktype,
                            serverInfo->ai_protocol);

  int yes=1;
  setsockopt(socketListen, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

  // get socket options
  unsigned int bufferSize = 0;
  unsigned int bufferSizeLenght = sizeof(bufferSize);

  getsockopt(socketListen, SOL_SOCKET, SO_RCVBUF, &bufferSize, &bufferSizeLenght);
  printf("recv buffer size: %u\n", bufferSize);

  getsockopt(socketListen, SOL_SOCKET, SO_SNDBUF, &bufferSize, &bufferSizeLenght);
  printf("send buffer size: %u\n", bufferSize);

  int reuse = 1;
  if (setsockopt(socketListen, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
    perror("setsockopt(SO_REUSEADDR) failed");

  bind(socketListen, serverInfo->ai_addr, serverInfo->ai_addrlen);
  listen(socketListen, BACKLOG);

  struct timeval tv;

  // A file descriptor is considered ready for reading if a read call will not block.
  // This usually includes the read offset being at the end of the file or there is
  // an error to report. A server socket is considered ready for reading if there is
  // a pending connection which can be accepted with accept
  fd_set readfds;
  fd_set tempreadfds;

  int fdmax = socketListen;

  // specifies the minimum interval that select() should block
  // waiting for a file descriptor to become ready.
  // If both fields of the timeval structure are zero, then select() returns immediately.
  // If timeout is NULL (no timeout), select() can block indefinitely.
  tv.tv_sec = 2;
  tv.tv_usec = 500000;


  FD_ZERO(&readfds);
  FD_SET(socketListen, &readfds);

  while(TRUE) {

    tempreadfds = readfds;

    int ret = select(fdmax + 1, &tempreadfds, NULL, NULL, &tv);

    printf("select return: %d.\n", ret);

    if(ret == -1) {

      perror("select");
    }


    for(int i = 0; i <= fdmax; i++) {

      if(FD_ISSET(i, &tempreadfds)) {

        printf("%d OK ", i);
        if(i == socketListen) {

          struct sockaddr sockaddClient;
          socklen_t sockaddClientLength;

          int socketNewCx = accept(socketListen, &sockaddClient, &sockaddClientLength);
          PCHAR request = handle_req_recv(socketNewCx, 0);

          if (is_favicon_request(request)) {

            close(socketNewCx);
          } else {

            handle_request_with_image((void *) &socketNewCx);
          }
        }
      } else {
        
        printf("%d, ", i);
      }
    }

    printf("\n");
  }
}

BOOL is_favicon_request(PCHAR request) {

  PCHAR favicon = "favicon";
  return (strstr(request, favicon) != NULL);
}
