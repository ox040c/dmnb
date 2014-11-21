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
    bool isTableHasAttribute(const std::string &tableName,
                             const std::string &attName);
    bool isAttUnique(const std::string &tableName,
                     const int &idx) {
        readTable(tableName);
        return  tables[tableName][idx].isIndex |
                tables[tableName][idx].isUnique;
    }

    void readTable(const std::string &tableName);
    void createTable(const std::string &tableName,
                     const TableDefinition &table);
    void dropTable(const std::string &tableName);

    const Scheme &getAtt(const std::string &tableName,
                         const int &idx) {
        readTable(tableName);
        return tables[tableName][idx];
    }

    const TableDefinition &getTableDef(const std::string &tableName) {
        readTable(tableName);
        return tables[tableName];
    }
};

#endif
