#include "RecordManager.hpp"
#include "Utility.hpp"
#include <list>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace std;
//calculate the datalen
 int RecordManager::get_datalen(const Entry &entry)
{
     int datalen = 0;
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
            datalen += (it->intv) * sizeof(char);
			break;
		default:
			break;
		}
		it++;
	}
     int i;
    for (i = 1; i < datalen; i = i * 2);
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

 int RecordManager::get_datalen(std::string tableName)
{
     int datalen;
    map<string,  int>::iterator it = datalenTable.find(tableName);
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

 int RecordManager::get_datalen_from_file(std::string tableName)
{
     int result;
	string filename = tableName + ".db";
	ifstream infile(filename.c_str(), ios::binary | ios::in);
    if(!infile) throw runtime_error("the is no table:"+tableName);
    infile.seekg(sizeof( int), ios::beg);
    infile.read((char *)&result, sizeof( int));
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
        //cout <<"int:"<< k <<endl;
		return Wrapper(utls::INT, k,it->name);
		break;
	case utls::FLOAT:
		float kk;
		kk = *(float *)temp;
        //cout <<"float"<< kk <<endl;
		return Wrapper(utls::FLOAT, kk,it->name);
		break;
	case utls::CHAR:
	{
        char *ch = new char[it->intv];
        for (int i = 0; i < it->intv; i++)
            ch[i] = temp[i];
        ch[it->intv] = 0;
        string s(ch);
        //cout <<"string"<< s <<endl;
        delete[] ch;
        return Wrapper(utls::CHAR, s,it->name,it->intv);
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
    const  int &dataaddr,
    const  int &datalen)
{
	FilePtr addr;
	addr.filename = tablename + ".db";
	addr.dataaddr = dataaddr;
	addr.datalen = datalen;
	return addr;
}
//get the next address
 int  RecordManager::getNext(const  std::string &tableName, bool reset , int pos)
{
    //cout << "[RM] getNext called!" << endl;
	FilePtr addr;
	addr.datalen = get_datalen(tableName);
    addr.dataaddr = pos;
	if (reset)
	{
        addr = get_fileptr(tableName, ( int)4096 - addr.datalen, addr.datalen);
    }else
    {
        addr = get_fileptr(tableName, addr.dataaddr, addr.datalen);
    }

    //cout << "[RM] before call buffer.nextAddr: " << addr.dataaddr << ":" << addr.datalen << endl;

	addr = buffer.nextAddr(addr);

    //cout << "[RM] gotta: " << addr.dataaddr << ":" << addr.datalen << endl;

	return addr.dataaddr;
}
//creat schema,if exist, return false
void  RecordManager::creatSchema(
	const std::string &tableName,
	const Entry &entry)
{
	FilePtr addr;
     int datalen;
	datalen = get_datalen(entry);
	addr = get_fileptr(tableName, 0, datalen);
	buffer.create(addr);
}
//build a fileptr,then return the Entry
Entry &  RecordManager::getValue(
	const std::string &tableName,
    const  int &pos,
	const Entry &entry)
{
    //cout << "[RM] call" << endl;

    FilePtr addr;
	int offset = 0;
	char * value, *temp;
     int datalen = get_datalen(tableName);
	value = new char[datalen];
	addr = get_fileptr(tableName, pos, datalen);

	buffer.search(addr, value);

	EntryResult.clear();
	list<Wrapper>::const_iterator it = entry.begin();
	while (it != entry.end())
	{
        temp = value + offset;
        EntryResult.push_back(get_wrapper(it, temp));
		switch (it->type)
		{
		case utls::INT:
			offset += sizeof(int);
			break;
		case utls::FLOAT:
			offset += sizeof(float);
			break;
		case utls::CHAR:
            offset += (it->intv) * sizeof(char);
			break;
		default:
            throw runtime_error("type erreor");
//			cerr << "Type error" << endl;
//			abort();
			break;
		}
		it++;
	}
	return EntryResult;

}
//build a fileptr,then return the attribuye
Wrapper  RecordManager::getAttValue(
	const std::string &tableName,
    const  int &pos,
	const std::string &colName,
	const Entry &entry)
{
	FilePtr addr;
	int offset = 0;
	char * value, *temp;
     int datalen = get_datalen(tableName);
	value = new char[datalen];
	addr = get_fileptr(tableName, pos, datalen);
	buffer.search(addr, value);

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
                offset += (it->intv) * sizeof(char);
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
        throw runtime_error("there is no such attribute:"+colName);
//		cerr << "there is no attribute " + colName << endl;
//		abort();
	}
	temp = value + offset;
	return get_wrapper(it, temp);
}
//delete the entry pionted
void  RecordManager::deleteEntry(
	const std::string &tableName,
    const  int &pos)
{
	FilePtr addr;
	addr = get_fileptr(tableName, pos, get_datalen(tableName));
    buffer.remove(addr);

    --totalEntries[tableName];
}

int RecordManager::deleteEntry(const std::string &tableName)
{
    int total = totalEntries[tableName];
    totalEntries[tableName] = 0;

	FilePtr addr;
	addr = get_fileptr(tableName, 0, get_datalen(tableName));
	buffer.drop(addr);
	buffer.create(addr);

    cout<<"delete"<<endl;

    return total;
}

bool RecordManager::dropSchema(const std::string &tableName)
{
    totalEntries.erase(tableName);

	FilePtr addr;
	addr = get_fileptr(tableName, 0, 0);
	buffer.drop(addr);
	return true;
}

 int  RecordManager::insertEntry(
	const std::string &tableName,
	const Entry &entry)
{
    //cout << "[RecordManager] called! " << endl;


    if (totalEntries.count(tableName) == 0) totalEntries[tableName] = 1;
    else ++totalEntries[tableName];


	FilePtr addr;
	char * insertvalue, * temp;
	int offset = 0;

    //cout << "[recordManager] 1" << endl;

	addr = get_fileptr(tableName, 0, get_datalen(tableName));

    //cout << "[rm] 2: " << addr.dataaddr << ", " << addr.datalen << endl;

    insertvalue = new char[addr.datalen];

    //cout << "[rm] 3" << endl;

	list<Wrapper>::const_iterator it = entry.begin();

    //cout << "[rm] 4: " << entry.size() << endl;

    while (it != entry.end())
	{
        //cout << "[rm] " << it->type << ": " << it->strv << endl;

		temp = insertvalue + offset;
		switch (it->type)
		{
		case utls::INT:
			for (int i = 0; i < sizeof (int); i++)
			{
                temp[i] = *((char *)&(it->intv)+i);
			}
            //cout << *(int *)(temp) <<endl;
            offset += sizeof(int);
            break;
		case utls::FLOAT:
			for (int i = 0; i < sizeof (float); i++)
			{
				temp[i] = *((char *)&(it->floatv) + i);
			}
            offset += sizeof(float);
            break;
		case utls::CHAR:
            for ( int i = 0; i < it->strv.length(); i++)
			{
				temp[i] = it->strv[i];
			}
			temp[it->strv.length()] = 0;

            offset += (it->intv) * sizeof(char);
            break;
		default:
            throw runtime_error("insert attribute type error");
//			cerr << "insert attribute type error" << endl;
//			abort();
			break;
		}

		it++;
	}

    //cout << "[recordManager] before buffer.insert" << endl;

    addr = buffer.insert(addr, insertvalue);
    cout << addr.dataaddr << endl;
    return addr.dataaddr;

    //cout << "[recordManager] after buffer.insert" << endl;
}
