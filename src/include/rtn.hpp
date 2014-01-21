#ifndef __RTN_HPP__
#define __RTN_HPP__

#include "bson.h"
#include "dms.hpp"
// define the storage file name
#define RTN_FILE_NAME "data.1"

class rtn
{
private :
   dmsFile           *_dmsFile ;
public :
   rtn () ;
   ~rtn() ;
   int rtnInitialize () ;
   int rtnInsert ( bson::BSONObj &record ) ;
   int rtnFind ( bson::BSONObj &inRecord, bson::BSONObj &outRecord ) ;
   int rtnRemove ( bson::BSONObj &record ) ;
} ;

#endif
