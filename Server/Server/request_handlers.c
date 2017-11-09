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

VOID handleRequestWithRandomResponse(void *sfd) {

  int i_sfd = *((int *)sfd);
  char *buffer = malloc(MESSAGE_LENGTH);
  char *msg = rand_string(buffer, MESSAGE_LENGTH);

  PCHAR http_response_header_placeholder = "HTTP/1.1 OK\r\nServer: SERVER\r\nConnection: Close\r\nContent-Type: text/html; charset=iso-8859-1\r\nContent-Length: %lu\r\n\r\n";
  PCHAR http_response_body_placeholder = "<html><body><h1>%s</h1></body></html>";

  ULONG message_length = strlen(msg);
  ULONG header_length = strlen(http_response_header_placeholder);
  ULONG body_length = strlen(http_response_body_placeholder);

  PCHAR http_response_body = malloc(body_length + message_length);
  sprintf(http_response_body, http_response_body_placeholder, msg);

  PCHAR http_response_header = malloc(header_length + get_long_size(strlen(http_response_body)) + 8);
  sprintf(http_response_header, http_response_header_placeholder, strlen(http_response_body));

  send(i_sfd, http_response_header, strlen(http_response_header), 0);
  send(i_sfd, http_response_body, strlen(http_response_body), 0);
  close(i_sfd);
}
