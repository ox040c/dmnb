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

using namespace utls;

typedef std::list < Condition > Conditions;
typedef std::list < Attribute > Data;

class API {
private:
    RecordManager &recordManager;
    IndexManager &indexManager;
    CatalogManger &catalogManager;
    Data data;

public:
    API(const CatalogManager &cat,
        const RecordManager &rec,
        const IndexManager &ind
        ): catalogManager(cat), recordManager(rec), indexManager(ind) {}

    ~API() {}

    void createTable(const std::string &tableName,
                     const std::vector < Scheme > &data);

    void dropTable(const std::string &tableName);   // [del]indexManager.dropAddress()[/del] should be called
                                                    // sequencial index should be kept by recordManager?
    void createIndex(const std::string &tableName,
                     const std::string &colName,
                     const std::string &indexName);
    void dropIndex(const std::string &indexName);
    const Data &select(const std::tableName);       // overload: select with no conditions
    const Data &select(const std::string &tableName,
                const Conditions &conditions);      // no "or" in statement
    ConstItemIterator itemsBegin();
    ConstItemIterator itemsEnd();

};

#endif
