/*Record Manager
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
	std::map<std::string, unsigned int> datalenTable;
	//return to API
	Entry EntryResult;

	//save the present address 
	unsigned int nextAddr;

	//calculate the datalen
	unsigned int get_datalen(const Entry &entry);
	unsigned int get_datalen(std::string tableName);
	unsigned int get_datalen_from_file(std::string tableName);
	//use the info from API to bulid FilePtr
	FilePtr get_fileptr(const std::string &tablename,
						const unsigned int &dataaddr,
						const unsigned int &datalen);
	//calculate the wrapper,if define the type ande temp is value
	Wrapper get_wrapper(const std::list<Wrapper>::const_iterator it, const char * temp);
public:
	RecordManager(BufferManager &buffer) :buffer(buffer){}
	~RecordManager();
	//get the next address
	unsigned int getNext(const  std::string &tableName, bool reset);
	//creat schema,if exist, return false
	void creatSchema(const std::string &tableName, 
						const Entry &entry);
	//build a fileptr,then return the Entry
	Entry & getValue(const std::string &tableName, 
						const unsigned int &pos, 
						const Entry &entry);
	//build a fileptr,then return the attribuye
	Wrapper getAttValue(const std::string &tableName, 
						const unsigned int &pos, 
						const std::string &colName,
						const Entry &entry);
	//delete the entry pionted, if 
	void deleteEntry(const std::string &tableName,
						const unsigned int &pos);

	bool deleteSchema(const std::string &tableName);

	unsigned int insertEntry(const std::string &tableName,
						const Entry &entry);
};

#endif