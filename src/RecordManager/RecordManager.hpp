﻿/*Record Manager
**
Created by gaotao*/
#ifndef RECORDMANAGER_HPP
#define RECORDMANAGER_HPP

#include "BufferManager.hpp"
#include "Utility.hpp"
#include <map>

class RecordManager {
private:
	BufferManager &buffer;
    std::map<std::string,  int> datalenTable;
	//return to API
	Entry EntryResult;
    std::map<std::string, int> totalEntries;

	//save the present address 
     int nextAddr;

	//calculate the datalen
     int get_datalen(const Entry &entry);
     int get_datalen(std::string tableName);
     int get_datalen_from_file(std::string tableName);
	//use the info from API to bulid FilePtr
	FilePtr get_fileptr(const std::string &tablename,
                        const  int &dataaddr,
                        const  int &datalen);
	//calculate the wrapper,if define the type ande temp is value
	Wrapper get_wrapper(const std::list<Wrapper>::const_iterator it, const char * temp);
public:
	RecordManager(BufferManager &buffer) :buffer(buffer){}
	~RecordManager() {};
	
     int getNext(const  std::string &tableName, bool reset,  int pos);//get the next address
	
	void creatSchema(const std::string &tableName, 
						const Entry &entry);//creat schema,if exist, return false
	
	Entry & getValue(const std::string &tableName, 
                        const  int &pos,
						const Entry &entry);//build a fileptr,then return the Entry
	
	Wrapper getAttValue(const std::string &tableName, 
                        const  int &pos,
						const std::string &colName,
						const Entry &entry);//build a fileptr,then return the attribuye
	
	void deleteEntry(const std::string &tableName,
                        const  int &pos);//delete the entry pionted
	
	int deleteEntry(const std::string &tableName);//delete the all entries in of the table
	bool dropSchema(const std::string &tableName);//drop the schema

     int insertEntry(const std::string &tableName,
						const Entry &entry);
};

#endif
