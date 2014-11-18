/** API
 *  Created by Ketian
 */

#ifndef API_HPP
#define API_HPP

#include <list>
#include "RecordManager.hpp"
#include "IndexManager.hpp"
#include "CatalogManager.hpp"
#include "Utility.hpp"

//using namespace utls;

class API {

private:
    RecordManager &recordManager;
    IndexManager &indexManager;
    CatalogManger &catalogManager;


public:

    API(const CatalogManager &cat,
        const RecordManager &rec,
    const IndexManager &ind
        ): catalogManager(cat), recordManager(rec), indexManager(ind) {}

    ~API() {}

    void createTable(const std::string &tableName,
                     const TableDefinition &data);

    void dropTable(const std::string &tableName);   // [del]indexManager.dropAddress()[/del] should be called
                                                    // sequencial index should be kept by recordManager?
    void createIndex(const std::string &tableName,
                     const std::string &colName,
                     const std::string &indexName);
    void dropIndex(const std::string &indexName);
    void insertEntry(const std::string &tableName,
                     const Entry &entry);
    const Entries &select(const std::string &tableName);       // overload: select with no conditions
    const Entries &select(const std::string &tableName,
                const Conditions &conditions);      // no "or" in statement
    void remove(const std::string &tableName);
    void remove(const std::string &tableName,
                const Conditions &conditions);

    /* TODO:
    ConstItemIterator itemsBegin();
    ConstItemIterator itemsEnd();
    */
};

#endif
