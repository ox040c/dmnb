#ifndef indexManager
#define indexManager
#include "BtreeIndex"
class indexManager{
private:
	int indexcount;
	set tid->attrid;
	set<int> indexid;
public:
	bool hasIndex(int tid,int attrid);
	bool isIndex(int tid,int indexid);
	int drop_index(int indexid);
	int create_index(int tid,int attrid,int indexid);
	int search(int tid,int attrid,int key,int condition_info);
	int search(int tid,int attrid,float key,int condition_info);
	int search(int tid,int attrid,string key,int condition_info);
	int insert(int tid,int eid,std::vector<char> v);
	int delete(int tid,int eid,std::vector<char> v);

}
#endif