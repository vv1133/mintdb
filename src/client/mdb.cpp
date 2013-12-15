#include <iostream>
#include <sstream>
#include "mdb.hpp"
#include "core.hpp"
#include "command.hpp"

const char    SPACE             =  ' ';
const char    TAB               =  '\t';
const char    BACK_SLANT        =  '\\';
const char    NEW_LINE          =   '\n';

int gQuit = 0;


void Mdb::start(void)
{
   std::cout << "Welcome to MintDB Shell!" << std::endl;
   std::cout << "mdb help for help, Ctrl+c or quit to exit" << std::endl;
   while(0==gQuit)
   {
      prompt();
   }
}
void Mdb::prompt(void)
{
   int ret = MDB_OK;
   ret = readInput("mdb", 0);

   if(ret)
   {
      return;
   }

   // Input string
   std::string textInput = _cmdBuffer;
   // Split the inputing sentence.
   std::vector<std::string> textVec;
   split(textInput, SPACE, textVec);
   int count = 0;
   std::string cmd = "";
   std::vector<std::string> optionVec;

   std::vector<std::string>::iterator iter = textVec.begin();
   // handle different command here.
   ICommand* pCmd = NULL;
   for(;iter != textVec.end(); ++iter)
   {
      std::string str = *iter;
      if(0 == count)
      {
         cmd = str;
         count++;
      }
      else
      {
         optionVec.push_back(str);
      }
   }
   pCmd = _cmdFactory.getCommandProcesser(cmd.c_str());
   if( NULL != pCmd )
   {
      pCmd->execute( _sock, optionVec);
   }
}

int Mdb::readInput(const char *pPrompt, int numIndent)
{
   memset(_cmdBuffer, 0, CMD_BUFFER_SIZE);
   // print tab
   for(int i=0; i<numIndent; i++)
   {
      std::cout << TAB;
   }
   // print "mdb> "
   std::cout << pPrompt << ">  ";
   // read a line from cmd
   readLine(_cmdBuffer,CMD_BUFFER_SIZE-1);
   int curBufLen = strlen(_cmdBuffer);
   // "\" means continue
   while(_cmdBuffer[curBufLen-1] == BACK_SLANT
      && (CMD_BUFFER_SIZE-curBufLen) >0)
   {
      for(int i=0; i<numIndent; i++)
      {
         std::cout << TAB;
      }
      std:: cout << "> ";
      readLine(&_cmdBuffer[curBufLen-1], CMD_BUFFER_SIZE - curBufLen);
   }
   curBufLen = strlen(_cmdBuffer);
   for( int i = 0; i < curBufLen; i++ )
   {
      if( _cmdBuffer[i] == TAB )
      {
         _cmdBuffer[i] = SPACE;
      }
   }
   return MDB_OK;
}

char *Mdb::readLine(char *p, int length)
{
   int len = 0;
   int ch;
   while((ch=getchar()) != NEW_LINE) {
      switch(ch)
      {
         case BACK_SLANT:
            break;
         default:
            p[len] = ch;
            len++;
      }
      continue;
   }
   len = strlen(p);
   p[len] = 0;
   return p;
}

void Mdb::split(const std::string &text, char delim, std::vector<std::string> &result)
{
   size_t strLen = text.length();
   size_t first = 0;
   size_t pos =0;
   for(first=0; first < strLen; first =pos+1)
   {
      pos = first;
      while(text[pos]!= delim && pos<strLen)
      {
         pos++;
      }
      std::string str = text.substr(first,pos-first);
      result.push_back(str);
   }
   return;
}

int main(int argc, char** argv)
{
   Mdb mdb;
   mdb.start();
   return 0;
}

