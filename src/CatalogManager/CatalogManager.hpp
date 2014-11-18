/** Catalog Manager
 *  Created by Ketian
 **/

#ifndef CatalogManager_HPP
#define CatalogManager_HPP

#include <vector>
#include <map>
#include <string>
#include "CatalogManager.hpp"

#include "Utility.hpp"

using namespace utls;

typedef std::map < std::string, TableDefinition > TablesDefinition;

class CatalogManager {
private:
    TablesDefinition tables;

public:
    CatalogManager() {}
    ~CatalogManager() {}

    bool isTableExist(const std::string &tableName);
    bool isTableHasAttribute(const std::string &tableName, const std::string &attName);
    void readTable(const std::string &tableName);
    void createTable(const std::string &tableName, const TableDefinition &table);
    void dropTable(const std::string &tableName);
    const TableDefinition &getTableDef(const std::string &tableName) { return tables[tableName]; }
};

#endif
