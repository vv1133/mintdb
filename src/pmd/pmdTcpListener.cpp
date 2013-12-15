#include "core.hpp"
#include "ossSocket.hpp"
#define PMD_TCPLISTENER_RETRY 5
#define OSS_MAX_SERVICENAME NI_MAXSERV


int pmdTcpListenerEntryPoint ()
{
   int rc = MDB_OK ;
   int port = 48127 ;
   ossSocket sock ( port ) ;
   rc = sock.initSocket () ;
   if ( rc )
   {
      printf ( "Failed to initialize socket, rc = %d", rc ) ;
      goto error ;
   }
   rc = sock.bind_listen () ;
   if ( rc )
   {
      printf ( "Failed to bind/listen socket, rc = %d", rc ) ;
      goto error ;
   }
   // master loop for tcp listener
   while ( true )
   {
      int s ;
      rc = sock.accept ( &s, NULL, NULL ) ;
      // if we don't get anything from period of time, let's loop
      if ( MDB_TIMEOUT == rc )
      {
         rc = MDB_OK ;
         continue ;
      }
      char buffer[1024] ;
      int size;
      ossSocket sock1 ( &s ) ;
      sock1.disableNagle () ;
      do
      {
         rc = sock1.recv ((char*)&size,4 ) ;
         if ( rc && rc != MDB_TIMEOUT )
         {
            printf ( "Failed to receive size, rc = %d", rc ) ;
            goto error ;
         }
      }
      while ( MDB_TIMEOUT == rc ) ;
      do
      {
         rc = sock1.recv ( &buffer[0], size-sizeof(int) ) ;
         if ( rc && rc != MDB_TIMEOUT )
         {
            printf ( "Failed to receive buffer, rc = %d", rc ) ;
            goto error ;
         }
      }
      while ( MDB_TIMEOUT == rc ) ;
      printf ( "%s\n", buffer ) ;
      sock1.close () ;
   }
done:
   return rc ;
error :
   switch ( rc )
   {
   case MDB_SYS :
      printf ( "System error occured" ) ;
      break ;
   default :
      printf ( "Internal error" ) ;
   }
   goto done ;
}
