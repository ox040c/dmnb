/** API
 *  Created by Ketian
 */

#ifndef API_HPP
#define API_HPP

typedef std::vector < Condition > Conditions;

class Condition {
private:
    std::string key;
    Operator op;
    void *valuePtr;

public:
    void setKey(const std::string &key);
    void setOp(const Operator &op);
    void setValue(const void *valuePtr);

    std::string getKey();
    Operator getOp();
    void *getValuePtr();
};


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
    void dropIndex(const std::string &tableName);
    void select(const std::string &tableName,
                const Conditions &conditions);      // no "or" in statement
    ConstItemIterator itemsBegin();
    ConstItemIterator itemsEnd();

};

#endif
