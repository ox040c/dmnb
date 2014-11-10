/** Catalog Manager
 *  Created by Ketian
 **/

#ifndef CatalogManager_HPP
#define CatalogManager_HPP

#include <vector>
#include <map>
#include <string>

enum DataType {INT, FLOAT, CHAR, TOTAL_TYPE};

class Scheme {
private:
    std::string name;
    DataType dataType;
    int charLength;
    bool indexed;

public:
    void setName(const std::string &name);
    void setDataType(const DataType &dataType);
    void setCharLength(const int &charLength);

    std::string getName();
    DataType getDataType();
    int getCharLength();
    bool getIndexed();
};

typedef std::list < Scheme > TableDefinition;
typedef std::map < std::string, TableDefinition > TablesDefinition;

class CatalogManager {
private:
    TablesDefinition tables;

public:
    CatalogManager() {}
    ~CatalogManager() {}

    bool isTableExisted(const std::string &tableName);
    bool isTableHasEntry(const std::string &tableName, const std::string &entryName);
    void readTable(const std::string &tableName);
    void createTable(const std::string &tableName, const std::TableDefinition &table);
    void dropTable(const std::string &tableName);
};

#endif
