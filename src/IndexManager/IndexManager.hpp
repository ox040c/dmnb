#ifndef IndexManager_HPP
#define IndexManager_HPP
#include "BufferManager.hpp"
#include "fstream"
#include <list>
#include <map>
#include <set>
#include <string>
#include "Utility.hpp"
#include "BtreeIndex.hpp"
using namespace utls;
class IndexManager{
typedef std::map<std::string,unsigned int> NodeList;
private:
    std::map<std::string,std::string> indexList;//表+属性--索引名
    std::map<std::string,std::string> tableList;
    NodeList rootList;//表名根节点地址
    NodeList headList;//表名头节点地址
    std::map<std::string,DataType> dataTypeList;//每个表key种类
    BufferManager &buffer;
    //    std::map<std::string,int> dataLength;//每个表一个节点的长度
	
public:
    IndexManager(BufferManager &x): buffer(x){
        std::ifstream ifile("IndexManager.txt");
        std::string indexname,tablename;
        DataType dtype;
        unsigned int root,first;
        if (ifile){
            while(!ifile.eof()){
                ifile>>indexname>>tablename>>root>>first>>dtype;
                indexList[tablename] = indexname;
                tableList[indexname] = tablename;
                rootList[indexname] = root;
                headList[indexname] = first;
                dataTypeList[indexname] = dtype;
            }
        }else{
            indexList.clear ();
            tableList.clear ();
            rootList.clear ();
            headList.clear ();
            dataTypeList.clear ();
        }
    }
    ~IndexManager(){
        std::ofstream ofile("IndexManager.txt");
        std::string indexname;
        NodeList::const_iterator  it=rootList.begin();
        for(;it!=rootList.end();it++){
            indexname = it->first;
            ofile<<indexname<<" "<<tableList[indexname]<<" "<<
                   " "<<rootList[indexname]<<" "<<
                   " "<<headList[indexname]<<" "<<
                   " "<<dataTypeList[indexname]<<" "<<std::endl;
        }
    }

    bool hasIndex(const std::string tableName,const std::string colName){
        if (indexList.count(tableName+"_"+colName)>0 &&
                tableList.count (indexList[tableName+"_"+colName]))
            return true;
        else
            return false;
    }

    std::string getIndexName (const std::string tableName,const std::string colName){
        if (indexList.count(tableName+"_"+colName)>0 &&
                tableList.count (indexList[tableName+"_"+colName]))
            return indexList[tableName+"_"+colName];
        else
            return "";
    }

    bool dropIndex(const std::string indexName){
        if (tableList.count (indexName) == 0){
            std::cout<<"no index exit to drop!"<<std::endl;
            return false;
        }
        DataType dtype = dataTypeList[indexName];
        filepoint root = rootList[indexName];
        filepoint first = headList[indexName];
        switch (dtype){
            case INT:
                BtreeIndex<int> btree0(buffer,indexName,root,first);
                btree0.drop ();
                break;
            case FLOAT:
                BtreeIndex<float> btree1(buffer,indexName,root,first);
                btree1.drop ();
                break;
        }
        tableList.erase (tableList.find (indexName),tableList.find (indexName));
        rootList.erase (rootList.find (indexName),rootList.find (indexName));
        headList.erase (headList.find (indexName),headList.find (indexName));
        dataTypeList.erase (dataTypeList.find (indexName),dataTypeList.find (indexName));
    }

    void createIndex(const std::string tableName,
                      const std::string colName,
                      const std::string indexName,
                      DataType type){
        indexList[tableName+"_"+colName] = indexName;
        dataTypeList[indexName] = type;
        rootList[indexName] = 0;
        headList[indexName] = 0;
        tableList[indexName] = tableName+"_"+colName;
    }

    void insert(const std::string indexname,
                        const unsigned int &pos,
                        void *dataPtr){
        DataType dtype = dataTypeList[indexname];
        filepoint root = rootList[indexname];
        filepoint first = headList[indexname];
        switch (dtype){
            case INT:
                int key0 = *((int*)dataPtr);
                BtreeIndex<int> btree0(buffer,indexname,root,first);
                btree0.insert (key0,pos);
                rootList[indexname] = btree0.getRoot ();
                headList[indexname] = btree0.getFirst ();
                break;
            case FLOAT:
                float key1 = *((float*)dataPtr);
                BtreeIndex<float> btree1(buffer,indexname,root,first);
                btree1.insert (key1,pos);
                rootList[indexname] = btree1.getRoot ();
                headList[indexname] = btree1.getFirst ();
                break;
        }
    }

// 在 tableName 的 colName 上建立索引 indexName, 这个 colName 的类型是 type
    std::list <unsigned int> select(const std::string &indexname,const Condition &condition_info){
        DataType dtype = dataTypeList[indexname];
        filepoint root = rootList[indexname];
        filepoint first = headList[indexname];
        switch (dtype){
            case INT:
                BtreeIndex<int> btree0(buffer,indexname,root,first);
                int key0 = condition_info.intv;

                switch (condition_info.op) {
                case EQUAL:
                        return btree0.findeq (key0);
                    break;
                case LESS:
                        return btree0.findless (key0);
                    break;
                case NO_MORE:
                        return btree0.findlesseq (key0);
                    break;
                case MORE:
                        return btree0.findbig (key0);
                    break;
                case NO_LESS:
                        return btree0.findbigeq (key0);
                    break;
                case UNEQUAL:
                        return btree0.findbig (key0).insert (btree0.findbig (key0).end (),btree0.findless (key0).begin (),btree0.findbig (key0).end ());
                    break;
                default:
                    break;
                }
                break;
            case FLOAT:
                BtreeIndex<float> btree1(buffer,root,first);
                int key1 = condition_info.floatv;
                switch (condition_info.op) {
                case EQUAL:
                        return btree1.findeq (key1);
                    break;
                case LESS:
                        return btree1.findless (key1);
                    break;
                case NO_MORE:
                        return btree1.findlesseq (key1);
                    break;
                case MORE:
                        return btree1.findbig (key1);
                    break;
                case NO_LESS:
                        return btree1.findbigeq (key1);
                    break;
                case UNEQUAL:
                        return btree1.findbig (key1).insert (btree1.findbig (key1).end (),btree1.findless (key1).begin (),btree1.findbig (key1).end ());
                    break;
                default:
                    break;
                }
                break;
        }
   }

    void remove(const std::string &indexname,void *dataPtr){
        DataType dtype = dataTypeList[indexname];
        filepoint root = rootList[indexname];
        filepoint first = headList[indexname];
        switch (dtype){
            case INT:
                int key0 = *((int*)dataPtr);
                BtreeIndex<int> btree0(buffer,root,first);
                btree0.remove (key0);
                break;
            case FLOAT:
                float key1 = *((float*)dataPtr);
                BtreeIndex<float> btree1(buffer,root,first);
                btree1.remove (key1);
                break;
        }
    }

    void remove(const std::string indexName){
        if (tableList.count (indexName) == 0){
            std::cout<<"no index exit to drop!"<<std::endl;
            return false;
        }
        DataType dtype = dataTypeList[indexname];
        filepoint root = rootList[indexname];
        filepoint first = headList[indexname];
        switch (dtype){
            case INT:
                BtreeIndex<int> btree0(buffer,indexname,root,first);
                btree0.clear ();
                break;
            case FLOAT:
                BtreeIndex<float> btree1(buffer,indexname,root,first);
                btree1.clear ();
                break;
        }
        rootList[indexname] = 0;
        headList[indexname] = 0;
    }



// FIXME: void insert(const std::string indexName,
//                    const int &pos,
//                    void *dataPtr);
// 在 indexName 中插入一个点，对应硬盘上的位置 pos，具体数据存在 dataPtr，
// 根据 indexName 可以判断出这个数据是什么类型的（建立索引的时候可以记录一下）

//    int delete(int tid,int eid,int attrid,std::vector<char> v);
// FIXME: `delete` is a reversed word in C++, change a name plz...

// TODO:
// 返回表 tableName 建立在 colName 上的索引名，没有建立索引返回空串
};
#endif
