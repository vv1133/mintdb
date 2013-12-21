AUTOMAKE_OPTIONS=foreign
bin_PROGRAMS=mintdb mdb
mintdb_SOURCES=\
	pmd/pmdMain.cpp pmd/pmdTcpListener.cpp\
	oss/ossSocket.cpp oss/ossPrimitiveFileOp.cpp\
	bson/src/bsonobj.cpp bson/src/util/json.cpp bson/src/oid.cpp \
	bson/src/lib/base64.cpp bson/src/lib/md5.c bson/src/lib/nonce.cpp\
	pd/pd.cpp

mdb_SOURCES=\
	client/mdb.cpp client/command.cpp client/commandFactory.cpp \
	oss/ossSocket.cpp oss/ossPrimitiveFileOp.cpp\
	bson/src/bsonobj.cpp bson/src/util/json.cpp bson/src/oid.cpp \
	bson/src/lib/base64.cpp bson/src/lib/md5.c bson/src/lib/nonce.cpp\
	pd/pd.cpp

mintdb_CXXFLAGS=-I../boost -Ibson/src -Iinclude -D_FILE_OFFSET_BITS=64 -ggdb -Wall -O0
mintdb_LDADD=-lpthread -lm -lboost_system -lboost_thread -lboost_program_options -lrt
mintdb_LDFLAGS=-fPIC -rdynamic -L../boost/stage/lib -pthread

mdb_CXXFLAGS=-I../boost -Ibson/src -Iinclude -D_FILE_OFFSET_BITS=64 -ggdb -Wall -O0
mdb_LDADD=-lm -lboost_system -lboost_thread -lrt
mdb_LDFLAGS=-fPIC -rdynamic -L../boost/stage/lib
