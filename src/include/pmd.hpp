#ifndef __PMD_HPP__
#define __PMD_HPP__

#include "core.hpp"
#include "pmdEDUMgr.hpp"

enum MDB_DB_STATUS
{
   MDB_DB_NORMAL = 0,
   MDB_DB_SHUTDOWN,
   MDB_DB_PANIC
} ;

#define MDB_IS_DB_NORMAL ( MDB_DB_NORMAL == pmdGetKRCB()->getDBStatus () )
#define MDB_IS_DB_DOWN   ( MDB_DB_SHUTDOWN == pmdGetKRCB()->getDBStatus () || \
                           MDB_DB_PANIC    == pmdGetKRCB()->getDBStatus () )
#define MDB_IS_DB_UP     ( !MDB_IS_DB_DOWN )

#define MDB_SHUTDOWN_DB  { pmdGetKRCB()->setDBStatus(MDB_DB_SHUTDOWN); }

class pmdOptions ;
class MDB_KRCB
{
private :
   // configured options
   char          _dataFilePath [ OSS_MAX_PATHSIZE + 1 ] ;
   char          _logFilePath  [ OSS_MAX_PATHSIZE + 1 ] ;
   int           _maxPool ;
   char          _svcName [ NI_MAXSERV + 1 ] ;
   MDB_DB_STATUS _dbStatus ;
private :
      pmdEDUMgr     _eduMgr ;
public :
   // constructor
   MDB_KRCB ()
   {
      _dbStatus = MDB_DB_NORMAL ;
      memset ( _dataFilePath, 0, sizeof(_dataFilePath) ) ;
      memset ( _logFilePath, 0, sizeof(_logFilePath) ) ;
      _maxPool = 0 ;
      memset ( _svcName, 0, sizeof(_svcName) ) ;
   }
   // destructor
   ~MDB_KRCB () {}

   // inline function
   // get edu mgr
   pmdEDUMgr *getEDUMgr ()
   {
       return &_eduMgr ;
   }

   // inline function
   // get database status
   inline MDB_DB_STATUS getDBStatus ()
   {
      return _dbStatus ;
   }
   // get data file path
   inline const char *getDataFilePath ()
   {
      return _dataFilePath ;
   }
   // get log file path
   inline const char *getLogFilePath ()
   {
      return _logFilePath ;
   }
   // get service name
   inline const char *getSvcName ()
   {
      return _svcName ;
   }
   // get max thread pool
   inline int getMaxPool ()
   {
      return _maxPool ;
   }
   // setup database status
   inline void setDBStatus ( MDB_DB_STATUS status )
   {
      _dbStatus = status ;
   }

   // set data file path
   void setDataFilePath ( const char *pPath )
   {
      strncpy ( _dataFilePath, pPath, sizeof(_dataFilePath) ) ;
   }

   // set log file path
   void setLogFilePath ( const char *pPath )
   {
      strncpy ( _logFilePath, pPath, sizeof(_logFilePath) ) ;
   }

   // set service name
   void setSvcName ( const char *pName )
   {
      strncpy ( _svcName, pName, sizeof(_svcName) ) ;
   }
   // set max pool
   void setMaxPool ( int maxPool )
   {
      _maxPool = maxPool ;
   }

   // setup from pmdOptions
   int init ( pmdOptions *options ) ;
} ;

extern MDB_KRCB pmd_krcb ;

inline MDB_KRCB *pmdGetKRCB()
{
   return &pmd_krcb ;
}



#endif
