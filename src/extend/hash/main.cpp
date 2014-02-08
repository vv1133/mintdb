#include <map>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdio.h>

using namespace std;

#define get16bits(d) (*((const unsigned short *) (d)))

unsigned int ossHash ( const char *data, int len )
{
   unsigned int hash = len, tmp ;
   int rem ;
   if ( len <= 0 || data == 0 ) return 0 ;
   rem = len&3 ;
   len >>= 2 ;
   for (; len > 0 ; --len )
   {
      hash += get16bits (data) ;
      tmp   = (get16bits (data+2) << 11) ^ hash;
      hash  = (hash<<16)^tmp ;
      data += 2*sizeof(unsigned short) ;
      hash += hash>>11 ;
   }
   switch ( rem )
   {
   case 3:
      hash += get16bits (data) ;
      hash ^= hash<<16 ;
      hash ^= ((char)data[sizeof (unsigned short)])<<18 ;
      hash += hash>>11 ;
      break ;
   case 2:
      hash += get16bits(data) ;
      hash ^= hash <<11 ;
      hash += hash >>17 ;
      break ;
   case 1:
      hash += (char)*data ;
      hash ^= hash<<10 ;
      hash += hash>>1 ;
   }
   hash ^= hash<<3 ;
   hash += hash>>5 ;
   hash ^= hash<<4 ;
   hash += hash>>17 ;
   hash ^= hash<<25 ;
   hash += hash>>6 ;
   return hash ;
}

typedef struct _Element {
	int id;
	const char *data;
}Element;

#define BUCKET_MAX_ELEMENT(num) ((num) * 2)

class BucketManager
{
	class Bucket
	{
	public:
		multimap<unsigned int, Element *> _bucketMap;
		~Bucket()
		{   
    		for ( multimap<unsigned int, Element*>::iterator it 
				= _bucketMap.begin(); it != _bucketMap.end(); ++it )
			{
				delete (*it).second;
			}
		}
	    int createIndex ( unsigned int hashNum, Element *ele ) ;
	    int findIndex ( unsigned int hashNum, Element *ele ) ;
	    int removeIndex ( unsigned int hashNum, Element *ele ) ;
		void print() ;
	};
private :
	int _size;
	int _index;
	int _elenum;
	vector<Bucket *>_bucket ;
	int rebuildBucket (int num);
public :
	BucketManager ()
	{   
		_size = 4;//2^(_index-1)  -  2^_index-1
		_index = 3;
		_elenum = 0;
	}   
	~BucketManager ()
	{   
		Bucket *pBucket = NULL ;
		for ( int i = 0; i < _size; ++i )
		{   
			pBucket = _bucket[i] ;
			if ( pBucket )
				delete pBucket ;
		}   
	}   
	int initialize () ;
	int addBucket () ;
	int rebuildAll () ;
    int createIndex ( Element *ele ) ;
    int findIndex ( Element *ele ) ;
    int removeIndex ( Element *ele ) ;
	void print() ;
};

int BucketManager::createIndex ( Element *ele )
{
	unsigned int random;
	unsigned int hashnum;
	unsigned int mask, maskhash;
	int rc;
	unsigned int size;

	hashnum = ossHash((const char*)&ele->id, sizeof(ele->id));
	mask = (1<<_index) - 1;
	size = 1 << (_index - 1);
	maskhash = hashnum & mask;
	if (maskhash < _size)
		random = maskhash;
	else
		random = maskhash - size;
	rc = _bucket[random]->createIndex ( hashnum, ele ) ;
	if (rc == 0)
		_elenum++;

	if (BUCKET_MAX_ELEMENT(_size) <= _elenum)
	{
		if ((1<<_index) - 1 == _size)
		{
			printf("before rebuildAll:\n");
			print();
			printf("++++++++++++++++++++++++++++\n");
			if (rebuildAll() != 0)
				return -1;
		}
		else
		{
			printf("before addBucket:\n");
			print();
			printf("++++++++++++++++++++++++++++\n");
			if (addBucket() != 0)
				return -1;
		}
	}

	return rc ;
}

int BucketManager::findIndex ( Element *ele )
{
	unsigned int random;
	unsigned int hashnum;
	unsigned int mask, maskhash;
	int rc;
	unsigned int size;

	hashnum = ossHash((const char*)&ele->id, sizeof(ele->id));
	mask = (1<<_index) - 1;
	size = 1 << (_index - 1);
	maskhash = hashnum & mask;
	if (maskhash < _size)
		random = maskhash;
	else
		random = maskhash - size;
	rc = _bucket[random]->findIndex ( hashnum, ele ) ;
	return rc ;
}

int BucketManager::removeIndex ( Element *ele )
{
	unsigned int random;
	unsigned int hashnum;
	unsigned int mask, maskhash;
	int rc;
	unsigned int size;

	hashnum = ossHash((const char*)&ele->id, sizeof(ele->id));
	mask = (1<<_index) - 1;
	size = 1 << (_index - 1);
	maskhash = hashnum & mask;
	if (maskhash < _size)
		random = maskhash;
	else
		random = maskhash - size;
	rc = _bucket[random]->removeIndex ( hashnum, ele ) ;
	return rc ;
}

int BucketManager::initialize ()
{
	int rc = 0 ;
	Bucket *temp = NULL ;
	for ( int i = 0; i < _size; ++i )
	{
		temp = new (std::nothrow) Bucket () ;
		if ( !temp )
		{
			rc = -1 ;
			goto error ;
		}
		_bucket.push_back ( temp ) ;
		temp = NULL ;
	}
done:
	return rc ;
error :
	goto done ;
}

int BucketManager::rebuildBucket (int num)
{
	unsigned int mask;
	unsigned int hashnum;
	Element *ele;
	unsigned int random;
	int rc;

	for (multimap<unsigned int, Element*>::iterator it = _bucket[num]->_bucketMap.begin();
		it != _bucket[num]->_bucketMap.end(); ++it)
	{
		hashnum = (*it).first;
		ele = (*it).second;
		mask = (1<<_index) - 1;
		random = hashnum & mask;
		if (random != _size - 1)
			continue;
		rc = _bucket[random]->createIndex ( hashnum, ele ) ;
		if (rc != 0)
			goto error;
		rc = _bucket[num]->removeIndex ( hashnum, ele ) ;
		if (rc != 0)
		{
			_bucket[random]->removeIndex ( hashnum, ele ) ;
			goto error;
		}
	}
done:
	return rc ;
error :
	goto done ;
}

int BucketManager::addBucket ()
{
	unsigned int num;
	int rc = 0 ;
	int i;
	Bucket *temp = NULL ;
	temp = new (std::nothrow) Bucket () ;
	if ( !temp )
	{
		rc = -1 ;
		goto error ;
	}
	_bucket.push_back ( temp ) ;

	num = 1 << (_index - 1);
	i = _size % num;
	_size++;
	rebuildBucket(i);

done:
	return rc ;
error :
	goto done ;
}

int BucketManager::rebuildAll ()
{
	unsigned int mask, maskhash;
	unsigned int hashnum;
	Element *ele;
	unsigned int random;
	int size;
	int rc = 0 ;
	vector<Bucket *>tmp_bucket ;
	Bucket *temp = NULL ;
	temp = new (std::nothrow) Bucket () ;
	if ( !temp )
	{
		rc = -1 ;
		goto error ;
	}
	_bucket.push_back ( temp ) ;
	_size++;
	_index++;
	cout<<"size:"<<_size<<" index:"<<_index<<"bucket size:"<<_bucket.size()<<endl;

	for(int num = 0; num < _size; num++)
	{
		Bucket *temp = NULL ;
		temp = new (std::nothrow) Bucket () ;
		if ( !temp )
		{
			rc = -1 ;
			goto error ;
		}
		tmp_bucket.push_back ( temp ) ;
	}
	
	for(int num = 0; num < _size; num++)
	{
		for (multimap<unsigned int, Element*>::iterator it = _bucket[num]->_bucketMap.begin();
			it != _bucket[num]->_bucketMap.end(); ++it )
		{
			hashnum = (*it).first;
			ele = (*it).second;
			mask = (1<<_index) - 1;
			size = 1 << (_index - 1);
			maskhash = hashnum & mask;
			if (maskhash < _size)
				random = maskhash;
			else
				random = maskhash - size;

			tmp_bucket[random]->_bucketMap.insert(pair<unsigned int, Element*>(hashnum, ele)) ;
		}
		_bucket[num]->_bucketMap.clear();
	}
	for(int num = 0; num < _size; num++)
	{
		for (multimap<unsigned int, Element*>::iterator 
			it = tmp_bucket[num]->_bucketMap.begin();
			it != tmp_bucket[num]->_bucketMap.end(); ++it )
		{
			hashnum = (*it).first;
			ele = (*it).second;
			_bucket[num]->_bucketMap.insert(pair<unsigned int, Element*>(hashnum, ele)) ;
		}
	}

	for ( int i = 0; i < _size; ++i )
	{   
		temp = tmp_bucket[i] ;
		if ( temp )
			delete temp ;
	}   
	
done:
	return rc ;
error :
	goto done ;
}

void BucketManager::print ()
{
	for (int i = 0; i < _size; i++) 
	{
		cout<<"bucket:"<<i<<endl;
		_bucket[i]->print();
		cout<<endl;
	}
}




int BucketManager::Bucket::createIndex ( unsigned int hashNum,
		                                 Element *ele )
{
	int rc = 0 ;
	Element *tmpele = new Element;
	*tmpele = *ele;
	_bucketMap.insert(pair<unsigned int, Element *>(hashNum, tmpele)) ;
	return rc ;
}

int BucketManager::Bucket::findIndex ( unsigned int hashNum,
		                                 Element *ele )
{
	int rc = 0 ;
	pair<multimap<unsigned int, Element*>::iterator,
			multimap<unsigned int, Element*>::iterator> ret ;
    ret = _bucketMap.equal_range (hashNum) ;
    for ( multimap<unsigned int, Element*>::iterator it = ret.first ;
		it != ret.second; ++it )
	{
		cout<<(*it).first<<"  "<<(*it).second<<endl;
	}
   return rc ;
}

int BucketManager::Bucket::removeIndex ( unsigned int hashNum,
		                                 Element *ele )
{
	int rc = 0 ;
	Element *tmpele;
	pair<multimap<unsigned int, Element*>::iterator,
			multimap<unsigned int, Element*>::iterator> ret ;
	ret = _bucketMap.equal_range (hashNum) ;
	for ( multimap<unsigned int, Element*>::iterator it = ret.first ;
		it != ret.second; ++it )
	{
		tmpele = (*it).second;
		if (!memcmp(tmpele->data, ele->data, sizeof(ele->data)))
		{
			_bucketMap.erase(it) ;
   			return rc ;
		}
	}
 
   return -1 ;
}

void BucketManager::Bucket::print ()
{
	pair<multimap<unsigned int, Element*>::iterator,
			multimap<unsigned int, Element*>::iterator> ret ;
	Element *ele;
	unsigned int hashnum;

    for ( multimap<unsigned int, Element*>::iterator it = _bucketMap.begin();
		it != _bucketMap.end(); ++it )
	{
		hashnum = (*it).first;
		ele = (*it).second;
		cout<<hex<<"hashnum:"<<hashnum;
		cout<<" id:"<<ele->id<<" data:"<<ele->data<<endl;
	}
}

void insert(BucketManager *bucketManager, Element &ele)
{
	cout<<"insert: id= "<<ele.id<<" data= "<<ele.data<<endl;
	cout<<"==============================="<<endl;
	bucketManager->createIndex(&ele); 
	bucketManager->print();
	cout<<"*******************************"<<endl<<endl<<endl;
}

int main(int argc, const char *argv[])
{
	unsigned int hashnum;
	Element ele;
	BucketManager *bucketManager = new BucketManager();

	bucketManager->initialize(); 

	ele.id = 1;
	ele.data = "hello1";
	insert(bucketManager, ele);

	ele.id = 2;
	ele.data = "hello2";
	insert(bucketManager, ele);

	ele.id = 3;
	ele.data = "hello3";
	insert(bucketManager, ele);

	ele.id = 13;
	ele.data = "hello13";
	insert(bucketManager, ele);

	ele.id = 1;
	ele.data = "aaa";
	insert(bucketManager, ele);

	ele.id = 2;
	ele.data = "bbb";
	insert(bucketManager, ele);

	ele.id = 3;
	ele.data = "ccc";
	insert(bucketManager, ele);

	ele.id = 4;
	ele.data = "ddd";
	insert(bucketManager, ele);

	ele.id = 5;
	ele.data = "eee";
	insert(bucketManager, ele);

	ele.id = 6;
	ele.data = "fff";
	insert(bucketManager, ele);

	ele.id = 7;
	ele.data = "ggg";
	insert(bucketManager, ele);

	ele.id = 18;
	ele.data = "hhh";
	insert(bucketManager, ele);

	ele.id = 19;
	ele.data = "iii";
	insert(bucketManager, ele);

	ele.id = 20;
	ele.data = "jjj";
	insert(bucketManager, ele);

	ele.id = 21;
	ele.data = "kkk";
	insert(bucketManager, ele);

	ele.id = 24;
	ele.data = "lll";
	insert(bucketManager, ele);

	ele.id = 2;
	ele.data = "haha";
	bucketManager->removeIndex(&ele);

	ele.id = 2;
	ele.data = "iii";
	bucketManager->removeIndex(&ele);

	//bucketManager->print();
#if 0
	multimap <unsigned int, char*> m;
	m.insert(pair<unsigned int,char*>(1,"Jack"));
	m.insert(pair<unsigned int,char*>(2,"Body"));
	m.insert(pair<unsigned int,char*>(2,"abc"));
	m.insert(pair<unsigned int,char*>(2,"ddd"));
	m.insert(pair<unsigned int,char*>(3,"Navy"));
	m.insert(pair<unsigned int,char*>(4,"Demo"));
	m.insert(pair<unsigned int,char*>(5,"Hello"));

	multimap<unsigned int, char*>::iterator it;
#if 0
	int num=m.count(2);
	it = m.find(2);
	cout<<"the search result is :"<<endl;
	for(int i=1;i<=num;i++)
	{
		cout<<(*it).first<<"  "<<(*it).second<<endl;
		it++;
	}
#else
	m.erase(2);

	pair<multimap<unsigned int, char*>::iterator,
			multimap<unsigned int, char*>::iterator> ret ;
    ret = m.equal_range (2) ;
    for ( multimap<unsigned int, char*>::iterator it = ret.first ;
		it != ret.second; ++it )
	{
		cout<<(*it).first<<"  "<<(*it).second<<endl;
	}
#endif
#endif
	return 0;
}

