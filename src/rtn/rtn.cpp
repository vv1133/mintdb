#include "core.hpp"
#include "rtn.hpp"
#include "pd.hpp"
#include "pmd.hpp"

using namespace bson ;

rtn::rtn() :
_dmsFile(NULL)
{
}

rtn::~rtn()
{
   if ( _dmsFile )
   {
      delete _dmsFile ;
   }
}

int rtn::rtnInitialize ()
{
   int rc = MDB_OK ;
   _dmsFile = new(std::nothrow) dmsFile () ;
   if ( !_dmsFile )
   {
      rc = MDB_OOM ;
      PD_LOG ( PDERROR, "Failed to new dms file" ) ;
      goto error ;
   }
   // init dms
   rc = _dmsFile->initialize ( pmdGetKRCB()->getDataFilePath () ) ;
   if ( rc )
   {
      PD_LOG ( PDERROR, "Failed to call dms initialize, rc = %d", rc ) ;
      goto error ;
   }
done :
   return rc ;
error :
   goto done ;
}

int rtn::rtnInsert ( BSONObj &record )
{
   int rc = MDB_OK ;
   dmsRecordID recordID ;
   BSONObj outRecord ;
   // write data into file
   rc = _dmsFile->insert ( record, outRecord, recordID ) ;
   if ( rc )
   {
      PD_LOG ( PDERROR, "Failed to call dms insert, rc = %d", rc ) ;
      goto error ;
   }
done :
   return rc ;
error :
   goto done ;
}

int rtn::rtnFind ( BSONObj &inRecord, BSONObj &outRecord )
{
   return MDB_OK ;
}

int rtn::rtnRemove ( BSONObj &record )
{
   return MDB_OK ;
}
