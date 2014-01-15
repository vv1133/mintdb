#ifndef __DMSRECORD_HPP__
#define __DMSRECORD_HPP__

typedef unsigned int PAGEID ;
typedef unsigned int SLOTID ;
// each record is represented by RID, which can be broken into page id and slot id
struct dmsRecordID
{
   PAGEID _pageID ;
   SLOTID _slotID ;
} ;

#endif
