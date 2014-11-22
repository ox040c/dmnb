#include "RecordManager.hpp"
#include "Utility.hpp"
#include <list>
#include <fstream>
#include <iostream>
using namespace std;
//calculate the datalen
unsigned int RecordManager::get_datalen(const Entry &entry)
{
	unsigned int datalen = 0;
	list<Wrapper>::const_iterator it;
	it = entry.begin();
	while (it != entry.end())
	{
		switch (it->type)
		{
		case utls::INT:
			datalen += sizeof(int);
			break;
		case utls::FLOAT:
			datalen += sizeof(float);
			break;
		case utls::CHAR:
			datalen += it->intv * sizeof(char);
			break;
		default:
			break;
		}
		it++;
	}
	for (unsigned int i = 1; i < datalen; i = i * 2)
	if (i<=4096)
	{
		datalen = i;
	}
	else
	{
        throw runtime_error("too large entry");
//		cerr << "too large entry" << endl;
//		abort();
	}
	return datalen;
}

unsigned int RecordManager::get_datalen(std::string tableName)
{
	unsigned int datalen;
	map<string, unsigned int>::iterator it = datalenTable.find(tableName);
	if (it != datalenTable.end())
	{
		datalen = it->second;
	}
	else
	{
		datalen = get_datalen_from_file(tableName);
		datalenTable.insert(make_pair(tableName, datalen));
	}
	return datalen;
}

unsigned int RecordManager::get_datalen_from_file(std::string tableName)
{
	unsigned int result;
	string filename = tableName + ".db";
	ifstream infile(filename.c_str(), ios::binary | ios::in);
	infile.seekg(sizeof(unsigned int), ios::beg);
	infile.read((char *)&result, sizeof(unsigned int));
	infile.close();
	return result;
}

Wrapper RecordManager::get_wrapper(const std::list<Wrapper>::const_iterator it,const char * temp)
{
	switch (it->type)
	{
	case utls::INT:
		int k;
		k = *(int *)temp;
		return Wrapper(utls::INT, k);
		break;
	case utls::FLOAT:
		float kk;
		kk = *(float *)temp;
		return Wrapper(utls::FLOAT, kk);
		break;
	case utls::CHAR:
	{
					   char *ch = new char[it->intv];
					   for (int i = 0; i < it->intv; i++)
						   ch[i] = temp[i];
					   string s(ch);
					   delete[] ch;
					   return Wrapper(utls::CHAR, s);
					   break;
	}
	default:
        throw runtime_error("getAttValue error");
//		cerr << "getAttValue error" << endl;
//		abort();
		break;
	}
}

//use the info from API to bulid FilePtr
FilePtr RecordManager::get_fileptr(
	const std::string &tablename,
	const unsigned int &dataaddr,
	const unsigned int &datalen)
{
	FilePtr addr;
	addr.filename = tablename + ".db";
	addr.dataaddr = dataaddr;
	addr.datalen = datalen;
	return addr;
}
//get the next address
unsigned int  RecordManager::getNext(const  std::string &tableName, bool reset)
{
	FilePtr addr;
	addr.datalen = get_datalen(tableName);

	if (reset)
	{
		addr = get_fileptr(tableName, 4096 - addr.datalen, addr.datalen);
	}

	addr = buffer.nextAddr(addr);
	return addr.dataaddr;
}
//creat schema,if exist, return false
void  RecordManager::creatSchema(
	const std::string &tableName,
	const Entry &entry)
{
	FilePtr addr;
	unsigned int datalen;
	datalen = get_datalen(entry);
	addr = get_fileptr(tableName, 0, datalen);
	buffer.create(addr);
}
//build a fileptr,then return the Entry
Entry &  RecordManager::getValue(
	const std::string &tableName,
	const unsigned int &pos,
	const Entry &entry)
{
	FilePtr addr;
	int offset = 0;
	char * value, *temp;
	unsigned int datalen = get_datalen(tableName);
	value = new char[datalen];
	addr = get_fileptr(tableName, pos, datalen);
	buffer.Search(addr, value);

	EntryResult.clear();
	list<Wrapper>::const_iterator it = entry.begin();
	while (it != entry.end())
	{
		switch (it->type)
		{
		case utls::INT:
			offset += sizeof(int);
			break;
		case utls::FLOAT:
			offset += sizeof(float);
			break;
		case utls::CHAR:
			offset += it->intv * sizeof(char);
			break;
		default:
            throw runtime_error("type erreor");
//			cerr << "Type error" << endl;
//			abort();
			break;
		}
		temp = value + offset;
		EntryResult.push_back(get_wrapper(it, temp));
		it++;
	}
	return EntryResult;

}
//build a fileptr,then return the attribuye
Wrapper  RecordManager::getAttValue(
	const std::string &tableName,
	const unsigned int &pos,
	const std::string &colName,
	const Entry &entry)
{
	FilePtr addr;
	int offset = 0;
	char * value, *temp;
	unsigned int datalen = get_datalen(tableName);
	value = new char[datalen];
	addr = get_fileptr(tableName, pos, datalen);
	buffer.Search(addr, value);

	list<Wrapper>::const_iterator it = entry.begin();
	while (it != entry.end())
	{
		if (colName != it->name)
		{
			switch (it->type)
			{
			case utls::INT:
				offset += sizeof(int);
				break;
			case utls::FLOAT:
				offset += sizeof(float);
				break;
			case utls::CHAR:
				offset += it->intv * sizeof(char);
				break;
			default:
				break;
			}
		}
		else
			break;
		it++;
	}
	if (it == entry.end())
	{
        throw runtime_error("there is no attribute");
//		cerr << "there is no attribute " + colName << endl;
//		abort();
	}
	temp = value + offset;
	return get_wrapper(it, temp);
}
//delete the entry pionted, if 
void  RecordManager::deleteEntry(
	const std::string &tableName,
	const unsigned int &pos)
{
	FilePtr addr;
	addr = get_fileptr(tableName, pos, get_datalen(tableName));
    buffer.remove(addr);

    --totalEntries[tableName];
}

void RecordManager::deleteEntry(const std::string &tableName)
{
    int total = totalEntries[tableName];
    totalEntries[tableName] = 0;

	FilePtr addr;
	addr = get_fileptr(tableName, 0, get_datalen(tableName));
	buffer.Drop(addr);
	buffer.create(addr);

    return total;
}

bool RecordManager::dropSchema(const std::string &tableName)
{
    totalEntries.erase(tableName);

	FilePtr addr;
	addr = get_fileptr(tableName, 0, 0);
	buffer.Drop(addr);
	return true;
}

unsigned int  RecordManager::insertEntry(
	const std::string &tableName,
	const Entry &entry)
{
    if (totalEntries.count(tableName) == 0) totalEntries[tableName] = 1;
    else ++totalEntries[tableName];


	FilePtr addr;
	char * insertvalue, * temp;
	int offset = 0;
	addr = get_fileptr(tableName, 0, get_datalen(tableName));
	insertvalue = new char[addr.dataaddr];
	list<Wrapper>::const_iterator it = entry.begin();
	while (it != entry.end());
	{
		temp = insertvalue + offset;
		switch (it->type)
		{
		case utls::INT:
			for (int i = 0; i < sizeof (int); i++)
			{
				temp[i] = *((char *)&(it->intv)+i);
			}
			temp += sizeof(int);
		case utls::FLOAT:
			for (int i = 0; i < sizeof (float); i++)
			{
				temp[i] = *((char *)&(it->floatv) + i);
			}
			temp += sizeof(float);
		case utls::CHAR:
			for (unsigned int i = 0; i < it->strv.length(); i++)
			{
				temp[i] = it->strv[i];
			}
			temp[it->strv.length()] = 0;
			temp += it->intv * sizeof(char);
		default:
            throw runtime_error("insert attribute type error");
//			cerr << "insert attribute type error" << endl;
//			abort();
			break;
		}
		it++;
	}
	buffer.Insert(addr, insertvalue);
}
