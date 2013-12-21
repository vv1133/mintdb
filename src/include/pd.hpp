#ifndef __PD_HPP__
#define __PD_HPP__

#include "core.hpp"

#define PD_LOG_STRINGMAX 4096

#define PD_LOG(level,fmt,...)                                         \
   do {                                                               \
      if ( _curPDLevel >= level )                                     \
      {                                                               \
         pdLog ( level, __func__, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
      }                                                               \
   } while ( 0 )                                                      \

#define PD_CHECK(cond,retCode,gotoLabel,level,fmt,...)                \
   do {                                                               \
      if ( !(cond) )                                                  \
      {                                                               \
         rc = (retCode) ;                                             \
         PD_LOG  ( (level), fmt, ##__VA_ARGS__) ;                     \
         goto gotoLabel ;                                             \
      }                                                               \
   } while ( 0 )                                                      \

#define PD_RC_CHECK(rc,level,fmt,...)                                 \
   do {                                                               \
      PD_CHECK ( (MDB_OK==(rc)), (rc), error, (level),                \
                 fmt, ##__VA_ARGS__) ;                                \
   } while ( 0 )                                                      \

#define MDB_VALIDATE_GOTOERROR(cond,ret,str)                          \
   {if(!(cond)) { pdLog(PDERROR, __func__, __FILE__, __LINE__, str) ; \
    rc = ret; goto error ; }}

#define MDB_ASSERT(cond,str)  {if(cond){}}
#define MDB_CHECK(cond,str)   {if(cond){}}

enum PDLEVEL
{
   PDSEVERE = 0,
   PDERROR,
   PDEVENT,
   PDWARNING,
   PDINFO,
   PDDEBUG
} ;

extern PDLEVEL _curPDLevel ;
const char *getPDLevelDesp ( PDLEVEL level ) ;

#define PD_DFT_DIAGLEVEL PDWARNING
void pdLog ( PDLEVEL level, const char *func, const char *file,
             unsigned int line, const char *format, ... ) ;
void pdLog ( PDLEVEL level, const char *func, const char *file,
             unsigned int line, std::string message ) ;
#endif
