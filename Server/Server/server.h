//
//  server.h
//  Server
//
//  Created by Juan Amari on 22/10/2017.
//  Copyright © 2017 Juan Amari. All rights reserved.
//

#include "main.h"
#include "request_handlers.h"

#ifndef server_h
#define server_h

void multithread_server(struct addrinfo hints, struct addrinfo *serverInfo);
void polling_server(struct addrinfo hints, struct addrinfo *serverInfo);

#endif /* server_h */
