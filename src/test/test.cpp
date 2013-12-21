#include <string.h>
#include "ossPrimitiveFileOp.hpp"

int main ( int argc, char **argv )
{
	const char *str = "hello world\n";
	ossPrimitiveFileOp *file = new ossPrimitiveFileOp();
	file->Open("hello.txt", OSS_PRIMITIVE_FILE_OP_OPEN_ALWAYS);
	file->Write(str, strlen(str));
	file->Close();

	return 0;
}
