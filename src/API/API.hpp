/** API
 *  Created by Ketian
 */

#ifndef API_HPP
#define API_HPP

typedef std::list < Condition > Conditions;
typedef std::list < Attribute > Data;
/// Attribute should be defined in recordManager.hpp
/// It looks like this:
///     class Attribute {
///

class API {
private:
    RecordManager &recordManager;
    IndexManager &indexManager;
    CatalogManger &catalogManager;

public:
    API(const CatalogManager &cat,
        const RecordManager &rec,
        const IndexManager &ind
        ):catalogManager(cat), recordManager(rec), indexManager(ind) {}

    ~API() {}

    void createTable(const std::string &tableName, const std::vector < Scheme > &data);

    void dropTable(const std::string &tableName);   // [del]indexManager.dropAddress()[/del] should be called
                                                    // sequencial index should be kept by recordManager?
    void dropIndex(const std::string &tableName,
                   const std::string &indexName);
    const Data &select(const std::tableName);       // overload: select with no conditions
    const Data &select(const std::string &tableName,
                const Conditions &conditions);      // no "or" in statement
    ConstItemIterator itemsBegin();
    ConstItemIterator itemsEnd();

};

#endif
