#include "core.hpp"
#include "command.hpp"
#include "commandFactory.hpp"

COMMAND_BEGIN
COMMAND_ADD(COMMAND_CONNECT,ConnectCommand)
COMMAND_ADD(COMMAND_QUIT, QuitCommand)
COMMAND_ADD(COMMAND_HELP, HelpCommand)
COMMAND_END

extern int gQuit;

int ICommand::execute(ossSocket & sock, std::vector<std::string> & argVec)
{
   return MDB_OK;
}
int ICommand::getError(int code)
{
  switch(code)
   {
      case MDB_OK:
         break;
      case MDB_IO:
         std::cout << "io error is occurred" << std::endl;
         break;
      case MDB_INVALIDARG:
         std::cout << "invalid argument" << std::endl;
         break;
      case MDB_PERM:
         std::cout << "mdb_perm" << std::endl;
         break;
      case MDB_OOM:
         std::cout << "mdb_oom" << std::endl;
         break;
      case MDB_SYS:
         std::cout << "system error is occurred." << std::endl;
         break;
      case MDB_QUIESCED:
         std::cout << "MDB_QUIESCED" << std::endl;
         break;
      case MDB_NETWORK_CLOSE:
         std::cout << "net work is closed." << std::endl;
         break;
      case MDB_HEADER_INVALID:
         std::cout << "record header is not right." << std::endl;
         break;
      case MDB_IXM_ID_EXIST:
         std::cout << "record key is exist." << std::endl;
         break;
      case MDB_IXM_ID_NOT_EXIST:
         std::cout << "record is not exist" << std::endl;
         break;
      case MDB_NO_ID:
         std::cout << "_id is needed" << std::endl;
         break;
      case MDB_QUERY_INVALID_ARGUMENT:
         std::cout << "invalid query argument" << std::endl;
         break;
      case MDB_INSERT_INVALID_ARGUMENT:
         std::cout <<  "invalid insert argument" << std::endl;
         break;
      case MDB_DELETE_INVALID_ARGUMENT:
         std::cout << "invalid delete argument" << std::endl;
         break;
      case MDB_INVALID_RECORD:
         std::cout << "invalid record string" << std::endl;
         break;
      case MDB_SOCK_NOT_CONNECT:
         std::cout << "sock connection does not exist" << std::endl;
         break;
      case MDB_SOCK_REMOTE_CLOSED:
         std::cout << "remote sock connection is closed" << std::endl;
         break;
      case MDB_MSG_BUILD_FAILED:
         std::cout << "msg build failed" << std::endl;
         break;
      case MDB_SOCK_SEND_FAILD:
         std::cout << "sock send msg faild" << std::endl;
         break;
      case MDB_SOCK_INIT_FAILED:
         std::cout << "sock init failed" << std::endl;
         break;
      case MDB_SOCK_CONNECT_FAILED:
         std::cout << "sock connect remote server failed" << std::endl;
         break;
      default :
         break;
   }
   return code;
}
int ICommand::recvReply(ossSocket & sock)
{
   // define message data length.
   int length = 0;
   int ret = MDB_OK;
   // fill receive buffer with 0.
   memset(_recvBuf, 0, RECV_BUF_SIZE);
   if( !sock.isConnected() )
   {
      return getError(MDB_SOCK_NOT_CONNECT);
   }
   while(1)
   {
      // receive data from the server.first receive the length of the data.
      ret = sock.recv(_recvBuf, sizeof(int));
      if( MDB_TIMEOUT == ret )
      {
         continue;
      }
      else if( MDB_NETWORK_CLOSE == ret )
      {
         return getError(MDB_SOCK_REMOTE_CLOSED);
      }
      else
      {
         break;
      }
   }
   // get the value of length.
   length = *(int*)_recvBuf;
   // judge the length is valid or not.
   if(length > RECV_BUF_SIZE)
   {
      return getError(MDB_RECV_DATA_LENGTH_ERROR);
   }

   // receive data from the server.second receive the last data.
   while(1)
   {
      ret = sock.recv(&_recvBuf[sizeof(int)],length-sizeof(int));
      if(ret == MDB_TIMEOUT)
      {
         continue;
      }
      else if(MDB_NETWORK_CLOSE == ret)
      {
         return getError(MDB_SOCK_REMOTE_CLOSED);
      }
      else
      {
         break;
      }
   }
   return ret;
}

int ICommand::sendOrder(ossSocket & sock, OnMsgBuild onMsgBuild)
{
   int ret = MDB_OK;
   bson::BSONObj bsonData;
   try {
      bsonData = bson::fromjson(_jsonString);
   } catch( std::exception & e) {
      return getError(MDB_INVALID_RECORD);
   }
   memset(_sendBuf,0, SEND_BUF_SIZE);
   int size = SEND_BUF_SIZE;
   char * pSendBuf = _sendBuf;
   ret = onMsgBuild(&pSendBuf, &size, bsonData);
   if(ret)
   {
      return getError(MDB_MSG_BUILD_FAILED);
   }
   ret = sock.send( pSendBuf, *(int*)pSendBuf );
   if(ret)
   {
      return getError(MDB_SOCK_SEND_FAILD);
   }
   return ret;
}

int ICommand::sendOrder(ossSocket & sock, int opCode)
{
   int ret = MDB_OK;
   memset(_sendBuf, 0, SEND_BUF_SIZE);
   char * pSendBuf = _sendBuf;
   const char *pStr = "hello world" ;
   *(int*)pSendBuf=strlen(pStr)+1 + sizeof(int) ;
   memcpy ( &pSendBuf[4], pStr, strlen(pStr)+1 ) ;
   /*MsgHeader *header = (MsgHeader*)pSendBuf;
   header->messageLen = sizeof(MsgHeader);
   header->opCode = opCode;*/
   ret = sock.send(pSendBuf, *(int*)pSendBuf);
   return ret;
}

/******************************ConnectCommand****************************************/
int ConnectCommand::execute(ossSocket & sock, std::vector<std::string> & argVec)
{
   int rc = MDB_OK;
   _address = argVec[0];
   _port = atoi(argVec[1].c_str());
   sock.close();
   sock.setAddress(_address.c_str(), _port);
   rc = sock.initSocket();
   if ( rc )
   {
      printf ( "Failed to init socket, rc = %d", rc ) ;
      goto error ;
   }
   rc = sock.connect();
   if ( rc )
   {
      printf ( "Failed to connect, rc = %d", rc ) ;
      goto error ;
   }
   sock.disableNagle();
done :
   return rc ;
error :
   goto done ;
}

/******************************QuitCommand**********************************************/
int QuitCommand::handleReply()
{
   int ret = MDB_OK;
   //gQuit = 1;
   return ret;
}

int QuitCommand::execute(ossSocket & sock, std::vector<std::string> & argVec)
{
   int ret = MDB_OK;
   if( !sock.isConnected() )
   {
      return getError(MDB_SOCK_NOT_CONNECT);
   }
   ret = sendOrder( sock, 0 );
   //sock.close();
   ret = handleReply();
   return ret;
}
/******************************HelpCommand**********************************************/
int HelpCommand::execute(ossSocket & sock, std::vector<std::string> & argVec)
{
   int ret = MDB_OK;
   printf("List of classes of commands:\n\n");
   printf("%s [server] [port]-- connecting mintdb server\n", COMMAND_CONNECT);
   printf("%s -- sending a insert command to mintdb server\n", COMMAND_INSERT);
   printf("%s -- sending a query command to mintdb server\n", COMMAND_QUERY);
   printf("%s -- sending a delete command to mintdb server\n", COMMAND_DELETE);
   printf("%s [number]-- sending a test command to mintdb server\n", COMMAND_TEST);
   printf("%s -- providing current number of record inserting\n", COMMAND_SNAPSHOT);
   printf("%s -- quitting command\n\n", COMMAND_QUIT);
   printf("Type \"help\" command for help\n");
   return ret;
}


