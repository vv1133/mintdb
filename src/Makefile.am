AUTOMAKE_OPTIONS=foreign
bin_PROGRAMS=mintdb
mintdb_SOURCES=\
	pmd/pmdMain.cpp 
	bson/src/bsonobj.cpp bson/src/util/json.cpp bson/src/oid.cpp \
	bson/src/lib/base64.cpp bson/src/lib/md5.c bson/src/lib/nonce.cpp

mintdb_CXXFLAGS=-I../boost -Ibson/src -Iinclude -D_FILE_OFFSET_BITS=64 -ggdb -Wall -O0
mintdb_LDADD=-lpthread -lm -lboost_system -lboost_thread -lboost_program_options -lrt
mintdb_LDFLAGS=-fPIC -rdynamic -L../boost/stage/lib -pthread
