//
//  main.h
//  Client
//
//  Created by Juan Amari on 9/11/17.
//  Copyright © 2017 Juan Amari. All rights reserved.
//

#ifndef main_h
#define main_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef CONST
#define CONST               const
#endif

typedef unsigned long 		ULONG;
typedef 					ULONG *PULONG;
typedef unsigned short 		USHORT;
typedef 					USHORT *PUSHORT;
typedef unsigned char 		UCHAR;
typedef 					UCHAR *PUCHAR;
typedef char 				*PSZ;
typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT 				*PFLOAT;
typedef void				VOID, *PVOID;
typedef char				CHAR, *PCHAR;
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;
typedef short 				SHORT;
typedef long 				LONG;
typedef double 				LONGLONG;
typedef double 				ULONGLONG;
typedef BYTE				BOOLEAN;
typedef char				**PPCHAR;


typedef struct _appArguments {

  PCHAR port;

} APP_ARGUMENTS, *PAPP_ARGUMENTS;

#endif /* main_h */
