#ifndef IndexManager_HPP
#define IndexManager_HPP

#include <list>
#include <map>
#include <set>
#include "BtreeIndex.hpp"

class IndexManager{
private:
	int indexcount;
//	std::set tid->attrid;
    std::set<int> indexid;
//	std::map<tid,attrid>->filePoint;
	BtreeIndex *Btree;
	
public:
	bool hasIndex(int tid,int attrid);
	bool isIndex(int tid,int indexid);
	int drop_index(int indexid);
	int create_index(int tid,int attrid,int indexid);
	int search(int tid,int attrid,int key,int condition_info);
	int search(int tid,int attrid,float key,int condition_info);
	int search(int tid,int attrid,string key,int condition_info);
	int insert(int tid,int eid,int attrid,std::vector<char> v);
//    int delete(int tid,int eid,int attrid,std::vector<char> v);
// FIXME: `delete` is a reversed word in C++, change a name plz...
};
#endif
