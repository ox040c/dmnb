#ifndef IndexManager_HPP
#define IndexManager_HPP
#include "BufferManager.hpp"
#include "fstream"
#include <list>
#include <map>
#include <string>
#include "Utility.hpp"
#include "BtreeIndex.hpp"
#include "BtreeIndex_s.hpp"
#include <stdio.h>
using namespace utls;
using namespace std;
class IndexManager{
typedef std::map<std::string,unsigned int> NodeList;
typedef std::map<std::string,std::string> StringList;
typedef std::map<std::string,DataType> TypeList;
public:
    StringList indexList;//表+属性--索引名
    StringList tableList;
    NodeList rootList;//表名根节点地址
    NodeList headList;//表名头节点地址
    TypeList dataTypeList;//每个表key种类
    BufferManager &buffer;
    std::list<filepoint> ans;
    //    std::map<std::string,int> dataLength;//每个表一个节点的长度

public:
    IndexManager(BufferManager &x): buffer(x){
        std::ifstream ifile("IndexManager.txt");
        std::string indexname,tablename;
        DataType dtype;
        int temptype;
        unsigned int root,first;
        if (ifile){
            while(!ifile.eof()){
                ifile>>indexname>>tablename>>root>>first>>temptype;
                indexList[tablename] = indexname;
                tableList[indexname] = tablename;
                rootList[indexname] = root;
                headList[indexname] = first;
                dataTypeList[indexname] = DataType(temptype);
            }
            ifile.close ();
        }else{
            indexList.clear ();
            tableList.clear ();
            rootList.clear ();
            headList.clear ();
            dataTypeList.clear ();
        }
    }
    ~IndexManager(){}

    void save(){
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
        ofile.close ();
    }

    void print(){
        cout<<"_______indexList_____"<<endl;
        StringList::iterator it0;
        for (it0 = indexList.begin ();it0!=indexList.end ();it0++){
            cout<<it0->first<<"->"<<it0->second<<endl;
        }
        cout<<"_______tableList_____"<<endl;
        StringList::iterator it1;
        for (it1 = tableList.begin ();it1!=tableList.end ();it1++){
            cout<<it1->first<<"->"<<it1->second<<endl;
        }
        cout<<"_______rootList_____"<<endl;
        NodeList::iterator it2;
        for (it2 = rootList.begin ();it2!=rootList.end ();it2++){
            cout<<it2->first<<"->"<<it2->second<<endl;
        }
        cout<<"_______headList_____"<<endl;
        NodeList::iterator it3;
        for (it3 = headList.begin ();it3!=headList.end ();it3++){
            cout<<it3->first<<"->"<<it3->second<<endl;
        }
        cout<<"_______dataTypeList_____"<<endl;
        TypeList::iterator it4;
        for (it4 = dataTypeList.begin ();it4!=dataTypeList.end ();it4++){
            cout<<it4->first<<"->"<<it4->second<<endl;
        }
    }
    void clear(){
        cout<<"clear()"<<endl;
        StringList::iterator it0;
        for (it0 = indexList.begin ();it0!=indexList.end ();it0++){
            struct DataAddr fp;
            fp.filename = it0->second;
            fp.datalen = 128;
            buffer.drop (fp);
        }
        remove("IndexManager.txt");
        indexList.clear ();
        tableList.clear ();
        rootList.clear ();
        headList.clear ();
        dataTypeList.clear ();
    }
    void showdetail(){
        cout<<"-----------------INDEX MANAGER------------"<<endl;
        StringList::iterator it0;
        for (it0 = indexList.begin ();it0!=indexList.end ();it0++){
            string indexname = it0->second;
            filepoint root = rootList[it0->second];
            filepoint first = headList[it0->second];
            DataType dtype = dataTypeList[it0->second];
            switch (dtype) {
            case INT:{
                BtreeIndex<int> btree0(buffer,indexname,root,first);
                btree0.print ();
            }
                break;
            case FLOAT:{
                BtreeIndex<float> btree1(buffer,indexname,root,first);
                btree1.print ();
            }
                break;
            case CHAR:{
                BtreeIndex_s btree2(buffer,indexname,root,first);
                btree2.print ();
            }
            default:
                break;
            }
        }
        cout<<"-----------------INDEX MANAGER------------"<<endl;
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
            case INT:{
                BtreeIndex<int> btree0(buffer,indexName,root,first);
                btree0.drop ();
            }
                break;
            case FLOAT:{
                BtreeIndex<float> btree1(buffer,indexName,root,first);
                btree1.drop ();
                break;
            }
        }
        indexList.erase (indexList.find(tableList[indexName]));
        tableList.erase (tableList.find (indexName));
        rootList.erase (rootList.find (indexName));
        headList.erase (headList.find (indexName));
        dataTypeList.erase (dataTypeList.find (indexName));
        struct DataAddr fp;
        fp.filename = indexName;
        fp.datalen = 128;
        buffer.drop (fp);
    }
    void createIndex(const std::string tableName,
                      const std::string colName,
                      const std::string indexName,
                      DataType type){
        struct DataAddr fp;
        fp.filename = indexName;
        fp.datalen = 4096;
        buffer.create (fp);
        indexList[tableName+"_"+colName] = indexName;
        dataTypeList[indexName] = type;
        rootList[indexName] = 0;
        headList[indexName] = 0;
        tableList[indexName] = tableName+"_"+colName;
    }

    void insert(const std::string indexname,
                        const unsigned int &pos,
                        const Condition &condition_info){
        DataType dtype = dataTypeList[indexname];
        filepoint root = rootList[indexname];
        filepoint first = headList[indexname];
        switch (dtype){
        case INT:{
            int key0 = condition_info.intv;
            BtreeIndex<int> btree0(buffer,indexname,root,first);
//                cout<<"BtreeIndex"<<key0<<pos<<" "<<root<<" "<<first<<endl;
            btree0.insert (key0,pos);
//                btree0.print ();
            rootList[indexname] = btree0.getRoot ();
            headList[indexname] = btree0.getFirst ();
        }
            break;
        case FLOAT:{
            float key1 = condition_info.floatv;
            BtreeIndex<float> btree1(buffer,indexname,root,first);
            btree1.insert (key1,pos);
            rootList[indexname] = btree1.getRoot ();
            headList[indexname] = btree1.getFirst ();
        }
            break;
        case CHAR:{
            string key2 = condition_info.strv;
            BtreeIndex_s btree2(buffer,indexname,root,first);
            btree2.insert (key2,pos);
            rootList[indexname] = btree2.getRoot ();
            headList[indexname] = btree2.getFirst ();
        }
        }
    }

// 在 tableName 的 colName 上建立索引 indexName, 这个 colName 的类型是 type
    std::list <unsigned int> &select(const std::string &indexName,const Condition &condition_info){
        ans.clear ();
        DataType dtype = dataTypeList[indexName];
        filepoint root = rootList[indexName];
        filepoint first = headList[indexName];
        switch (dtype){
            case INT:{
                BtreeIndex<int> btree0(buffer,indexName,root,first);
                int key0 = condition_info.intv;
//                btree0.print ();
//                std::cout<<"key0 "<<key0<<std::endl;
                switch (condition_info.op) {
                case EQUAL:{
                    ans = btree0.findeq (key0);
                    return ans;
                }
                    break;
                case LESS:{
                    ans = btree0.findless (key0);
                    return ans;
                }
                    break;
                case NO_MORE:{
                    ans = btree0.findlesseq (key0);
                    return ans;
                }
                    break;
                case MORE:{
                    ans = btree0.findbig (key0);
                    return ans;
                }
                    break;
                case NO_LESS:{
                    ans = btree0.findbigeq (key0);
                    return ans;
                }
                    break;
                case UNEQUAL:{
                    std::list<filepoint> ans1;
                    ans = btree0.findbig (key0);
                    ans1 = btree0.findless (key0);
                    ans.merge (ans1);
                    return ans;
                }
                    break;
                default:{}
                    break;
                }
        }
                break;
            case FLOAT:{
                BtreeIndex<float> btree1(buffer,indexName,root,first);
                int key1 = condition_info.floatv;
                switch (condition_info.op) {
                case EQUAL:{
                    ans = btree1.findeq (key1);
                    return ans;
                }
                    break;
                case LESS:{
                    ans = btree1.findless (key1);
                    return ans;
                }
                    break;
                case NO_MORE:{
                    ans = btree1.findlesseq (key1);
                    return ans;
                }
                    break;
                case MORE:{
                    ans = btree1.findbig (key1);
                    return ans;
                }
                    break;
                case NO_LESS:{
                    ans = btree1.findbigeq (key1);
                    return ans;
                }
                    break;
                case UNEQUAL:{
                    std::list<filepoint> ans1;
                    ans = btree1.findbig (key1);
                    ans1 = btree1.findless (key1);
                    ans.merge (ans1);
                    return ans;
                }
                    break;
                default:{}
                    break;
                }
        }
                break;
            case CHAR:{
            BtreeIndex_s btree2(buffer,indexName,root,first);
            string key2 = condition_info.strv;
//                btree2.print ();
//                std::cout<<"key2 "<<key2<<std::endl;
            switch (condition_info.op) {
            case EQUAL:{
                ans = btree2.findeq (key2);
                return ans;
            }
                break;
            case LESS:{
                ans = btree2.findless (key2);
                return ans;
            }
                break;
            case NO_MORE:{
                ans = btree2.findlesseq (key2);
                return ans;
            }
                break;
            case MORE:{
                ans = btree2.findbig (key2);
                return ans;
            }
                break;
            case NO_LESS:{
                ans = btree2.findbigeq (key2);
                return ans;
            }
                break;
            case UNEQUAL:{
                std::list<filepoint> ans1;
                ans = btree2.findbig (key2);
                ans1 = btree2.findless (key2);
                ans.merge (ans1);
                return ans;
            }
                break;
            default:{}
                break;
            }
        }
                break;
        }
   }
    void remove(const std::string &indexName,const Condition &condition_info){
        DataType dtype = dataTypeList[indexName];
        filepoint root = rootList[indexName];
        filepoint first = headList[indexName];
        switch (dtype){
            case INT:{
                int key0 = condition_info.intv;
                BtreeIndex<int> btree0(buffer,indexName,root,first);
                btree0.remove (key0);
                rootList[indexName] = btree0.getRoot ();
                headList[indexName] = btree0.getFirst ();
            }
                break;
            case FLOAT:{
                float key1 = condition_info.floatv;
                BtreeIndex<float> btree1(buffer,indexName,root,first);
                btree1.remove (key1);
                rootList[indexName] = btree1.getRoot ();
                headList[indexName] = btree1.getFirst ();
            }
                break;
            case CHAR:{
                string key2 = condition_info.strv;
                BtreeIndex_s btree2(buffer,indexName,root,first);
                btree2.remove (key2);
                rootList[indexName] = btree2.getRoot ();
                headList[indexName] = btree2.getFirst ();
            }
                break;
        }
    }
    void remove(const std::string indexName){
        if (tableList.count (indexName) == 0){
            std::cout<<"no index exit to drop!"<<std::endl;
            return;
        }
        struct DataAddr fp;
        fp.filename = indexName;
        fp.datalen = 128;
        buffer.drop (fp);
        buffer.create (fp);
        rootList[indexName] = 0;
        headList[indexName] = 0;
    }
};
#endif
