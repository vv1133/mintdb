#ifndef __CORE_HPP__
#define __CORE_HPP__

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/time.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <syscall.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <sys/mman.h>

#include <string>
#include <map>
#include <set>
#include <vector>
#include <iostream>

#define OSS_MAX_PATHSIZE  PATH_MAX
#define OSS_FILE_SEP_STR  "/"
#define OSS_FILE_SEP_CHAR *((const char*)OSS_FILE_SEP_STR)[0]
#define OSS_NEWLINE       "\n"
// error code list
#define MDB_OK                                           0
#define MDB_IO                                          -1
#define MDB_INVALIDARG                                  -2
#define MDB_PERM                                        -3
#define MDB_OOM                                         -4
#define MDB_SYS                                         -5
#define MDB_PMD_HELP_ONLY                               -6
#define MDB_PMD_FORCE_SYSTEM_EDU                        -7
#define MDB_TIMEOUT                                     -8
#define MDB_QUIESCED                                    -9
#define MDB_EDU_INVAL_STATUS                            -10
#define MDB_NETWORK                                     -11
#define MDB_NETWORK_CLOSE                               -12
#define MDB_APP_FORCED                                  -13
#define MDB_IXM_ID_EXIST                                -14
#define MDB_HEADER_INVALID                              -15
#define MDB_IXM_ID_NOT_EXIST                            -16
#define MDB_NO_ID                                       -17 
#endif
