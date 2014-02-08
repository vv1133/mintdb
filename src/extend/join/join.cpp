#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;

typedef struct _TableA {
	int id;
	const char *name;
}TableA;

typedef struct _TableB {
	int id;
	int score;
}TableB;

typedef struct _TableC {
	int id;
	const char *name;
	int score;
}TableC;

/*********************  LOOP JOIN *******************/
vector<TableC> loop_join(vector<TableA> &x, vector<TableB> &y)
{
	vector<TableC> table_c;
	TableC ele;

	for (vector<TableA>::iterator it_a=x.begin(); it_a!=x.end(); it_a++) {
		for (vector<TableB>::iterator it_b=y.begin(); it_b!=y.end(); it_b++) {
			if (it_a->id == it_b->id) {
				ele.id = it_a->id;
				ele.name = it_a->name;
				ele.score = it_b->score;
				table_c.push_back(ele);
			}
		}
	}

	return table_c;
}

/*********************  MERGE JOIN *******************/
int cmp1(const void *x, const void *y)
{
	return (*(TableA*)x).id - (*(TableA*)y).id;
}

int cmp2(const void *x, const void *y)
{
	return (*(TableB*)x).id - (*(TableB*)y).id;
}

vector<TableC> merge_join(vector<TableA> &x, vector<TableB> &y)
{
	vector<TableC> table_c;
	TableC ele;

	vector<TableA>::iterator it_a = x.begin();
	vector<TableB>::iterator it_b = y.begin();
	vector<TableB>::iterator temp_begin, temp_end;
	for (; it_a!=x.end() && it_b!=y.end(); ) {
		if (it_a->id == it_b->id) {
			temp_begin = it_b;
			do {
				do {
					ele.id = it_a->id;
					ele.name = it_a->name;
					ele.score = it_b->score;
					table_c.push_back(ele);
					it_b++;
				} while (it_a->id == it_b->id);
				it_a++;
				temp_end = it_b;
				it_b = temp_begin;
			} while (it_a->id == it_b->id);
			it_b = temp_end;
		}
		else if (it_a->id > it_b->id)
			it_b++;
		else 
			it_a++;
	}

	return table_c;
}

/*********************  HASH JOIN *******************/
typedef struct _hash_ele {
	int id;
	TableB table_b;
	struct _hash_ele *next;
}hash_ele;

#define MAX_HASH_NUM 100000
hash_ele hash_map[MAX_HASH_NUM];
int hash(int id)
{
	return id % MAX_HASH_NUM;
}

void init_hash(void)
{
	for (int i = 0; i < MAX_HASH_NUM; i++) {
		hash_map[i].id = -1;
	}
}

void del_hash_ele(hash_ele *p)
{
	if (p->next == NULL) {
		delete p;
		p = NULL;
		return;
	}
	del_hash_ele(p->next);
}

void delete_hash(void)
{
	for (int i = 0; i < MAX_HASH_NUM; i++) {
		if (hash_map[i].id != -1 && hash_map[i].next != NULL) {
			del_hash_ele(hash_map[i].next);
		}
	}
}

int insert_hash(TableB &ele_b)
{
	int id = ele_b.id;
	int num = hash(id);
	hash_ele *ele;
	if (hash_map[num].id == -1) {
		hash_map[num].id = id;
		hash_map[num].table_b = ele_b;
		return 0;
	}
	if ((ele = new hash_ele) == NULL)
		return -1;
	ele->id = id;
	ele->table_b = ele_b;
	ele->next = hash_map[num].next;
	hash_map[num].next = ele;

	return 0;
}

hash_ele* search_hash(int id)
{
	int num = hash(id);
	if (hash_map[num].id == -1)
		return NULL;
	return &hash_map[num];
}

vector<TableC> hash_join(vector<TableA> &x, vector<TableB> &y)
{
	vector<TableC> table_c;
	TableC ele;
	hash_ele *hash_map;

	init_hash();

	for (vector<TableB>::iterator it_b=y.begin(); it_b!=y.end(); it_b++) {
		insert_hash(*it_b);
	}
	
	for (vector<TableA>::iterator it_a=x.begin(); it_a!=x.end(); it_a++) {
		hash_map = search_hash(it_a->id);
		do {
			if (hash_map == NULL) 
				break;

			if (hash_map->id == it_a->id) {
				ele.id = it_a->id;
				ele.name = it_a->name;
				ele.score = hash_map->table_b.score;
				table_c.push_back(ele);
			}
			hash_map = hash_map->next;
		} while (1);
	}

	delete_hash();

	return table_c;
}


int main(int argc, const char *argv[])
{
	vector<TableA> table_a;
	vector<TableB> table_b;
	vector<TableC> table_c;
	int i;

	TableA eleA[] = {
		{8, "jane" },
		{1, "jack" },
		{5, "den" },
		{5, "king" },
		{3, "john" },
		{7, "ring" },
		{9, "jones" },
		{2, "mike" },
	};
	TableB eleB[] = {
		{5, 47 },
		{6, 90 },
		{5, 66 },
		{1, 89 },
		{9, 38 },
	};

	qsort(eleA, sizeof(eleA)/sizeof(eleA[0]), sizeof(eleA[0]), cmp1);
	qsort(eleB, sizeof(eleB)/sizeof(eleB[0]), sizeof(eleB[0]), cmp2);

	for (i=0; i<sizeof(eleA)/sizeof(eleA[0]); i++)
		table_a.push_back(eleA[i]);

	for (i=0; i<sizeof(eleB)/sizeof(eleB[0]); i++)
		table_b.push_back(eleB[i]);

//	table_c = loop_join(table_a, table_b);/* LOOP JOIN */
	table_c = merge_join(table_a, table_b);/* MERGE JOIN */
//	table_c = hash_join(table_a, table_b);/* HASH JOIN */
	for (vector<TableC>::iterator it=table_c.begin(); it<table_c.end(); it++) {
		cout<<"id:"<<it->id<<" "<<it->name<<" "<<it->score<<endl;
	}

	return 0;
}
