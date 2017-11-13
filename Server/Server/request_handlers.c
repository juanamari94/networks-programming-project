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
#define CHUNK_SIZE 1024
#define CAT_PIC_PATH "/Users/juanamari/Developer/XCode/NetworksProgramming/Server/Server/cat.jpg"

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

VOID handle_request_with_image(void *sfd) {

  int i_sfd = *((int *)sfd);
  FILE *file = fopen(CAT_PIC_PATH, "r");
  size_t bytesRead = 0;

  if (file == NULL) {
    close(i_sfd);
    return;
  }

  PCHAR http_response_header_placeholder = "HTTP/1.1 OK\r\nServer: SERVER\r\nConnection: Close\r\nContent-Type: image/jpeg\r\nContent-Length: %d\r\n\r\n";
  PCHAR http_response_header;
  PCHAR buffer = malloc(CHUNK_SIZE);

  int res = fseek(file, 0, SEEK_END); // Set pointer at the end of the file.

  if (res != 0) { // Check for errors when seeking.
    close(i_sfd);
    return;
  }

  long file_size = ftell(file); // Get the end of file pointer.
  fseek(file, 0, SEEK_SET); // Set pointer back to the beggining.

  http_response_header = malloc(strlen(http_response_header_placeholder) + 10);
  sprintf(http_response_header, http_response_header_placeholder, file_size);
  send(i_sfd, http_response_header, strlen(http_response_header), 0);

  // read up to sizeof(buffer) bytes
  while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0)
  {
    // process bytesRead worth of data in buffer
    send(i_sfd, buffer, sizeof(buffer), 0);
  }

  fclose(file);
  close(i_sfd);
}
