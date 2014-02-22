#ifndef __SNAPSHOT_HPP__
#define __SNAPSHOT_HPP__

#include <time.h>
#include "ossLatch.hpp"

class MonAppCB
{
public:
	MonAppCB();
	~MonAppCB();
	void setInsertTimes(long long insertTimes);
	long long getInsertTimes () const;
	void increaseInsertTimes();
	void setDelTimes(long long delTimes);
	long long getDelTimes() const;
	void increaseDelTimes();
	void setQueryTimes(long long queryTimes);
	long long getQueryTimes() const;
	void increaseQueryTimes();
	long long getServerRuntime();
	void setClientCounter(long long queryTimes);
	long long getClientCounter() const;
	void increaseClientCounter();
	void decreaseClientCounter();
private:
	long long _insertTimes;
	long long _delTimes;
	long long _queryTimes;
	long long _clientCounter;
	struct timeval _start;
	ossXLatch _mutex;
};

#endif
