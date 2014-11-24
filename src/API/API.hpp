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
#include <cmath>
#include <set>
#include <map>

//using namespace utls;

class API {
typedef std::set < int> Idx;

private:
    RecordManager &recordManager;
    IndexManager &indexManager;
    CatalogManager &catalogManager;

    Entries result;
    Idx idxSet, tempIdxSet;

    std::map <string, int> indexMap;
    std::map <string, string> indexMap2;

    bool check(const int &x, const Operator &op, const int &y);
    bool check(const float &x, const Operator &op, const float &y);
    bool check(const std::string &x, const Operator &op, const std::string &y);
    void checkEntry(const std::string &tableName,
                    Entry &entry);

    void gainIdx(const std::string &tableName,
                 const Condition &condition,
                 Idx &idx);
    void selectIdx(const std::string &tableName,
                   const Conditions &conditions);

public:

    API(CatalogManager &cat,
        RecordManager &rec,
        IndexManager &ind
        ): catalogManager(cat), recordManager(rec), indexManager(ind)  {

    }

    ~API() {}

    void createTable(const std::string &tableName,
                     TableDefinition &data);

    void dropTable(const std::string &tableName);   // [del]indexManager.dropAddress()[/del] should be called
                                                    // sequencial index should be kept by recordManager?
    void createIndex(const std::string &tableName,
                     const std::string &colName,
                     const std::string &indexName);
    void dropIndex(const std::string &indexName);
    void insertEntry(const std::string &tableName,
                     Entry &entry);
    const Entries &select(const std::string &tableName);       // overload: select with no conditions
    const Entries &select(const std::string &tableName,
                const Conditions &conditions);      // no "or" in statement
    int remove(const std::string &tableName);
    int remove(const std::string &tableName,
               const Conditions &conditions);

    /* TODO:
    ConstItemIterator itemsBegin();
    ConstItemIterator itemsEnd();
    */
};

#endif
