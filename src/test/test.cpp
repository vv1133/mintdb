#include <string.h>
//#include "ossPrimitiveFileOp.hpp"
#include "ossMmapFile.hpp"

int main ( int argc, char **argv )
{
	const char *str = "hello world\n";
//	ossPrimitiveFileOp *file = new ossPrimitiveFileOp();
//	file->Open("hello.txt", OSS_PRIMITIVE_FILE_OP_OPEN_ALWAYS);
//	file->Write(str, strlen(str));
//	file->Close();
	
	void *pos;
	ossMmapFile *mapfile = new ossMmapFile();
	mapfile->open("hello.txt", OSS_PRIMITIVE_FILE_OP_OPEN_ALWAYS);
	mapfile->map(0, 4096, &pos);
	strcpy((char*)pos, str);
	mapfile->close();

	return 0;
}
