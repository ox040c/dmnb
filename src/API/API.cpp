#include "API.hpp"
#include <iostream>
#include <string>

/// TODO...
///
///
///
using namespace std;

#define DEBUG

void API::createTable(const std::string &tableName, const TableDefinition &data) {
#ifdef DEBUG
    cout << "create" << data.size() << "attrs\n--\n";
    return;
#else
    if (catalogManager.isTableExist(tableName)) {
        throw runtime_error(tableName + " has existed");
    } else {
        catalogManager.createTable(tableName, data);
        recordManager.createTable(tableName, data);
    }
#endif
}

void API::dropTable(const std::string &tableName) {
#ifdef DEBUG

    return;
#else
    if (!catalogManager.isTableExist(tableName)) {
        throw runtime_error(tableName + " dose not exist");
    } else {
        catalogManager.dropTable(tableName);
        recordManager.dropTable(tableName);
        indexManager.dropTable(tableName);
    }
#endif
}

void API::createIndex(const string &tableName,
                      const string &colName,
                      const string &indexName) {
#ifdef DEBUG

    return;
#else
    try {
        if (indexManager.hasIndex(indexName)) {
            throw runtime_error(indexName + " has existed");
        } else {
            indexManager.create_index(tableName, colName, indexName, getDataType(td, colName));
            const TableDefinition &td = catalogManager.getTableDef(tableName);
            while (int pos = recordManager.getNext(tableName)) {
                indexManager.insert(indexName, pos,
                                    recordManager.getAttValue(tableName, pos, colName));
            }
        }
    } catch (exception &e) {
        throw e;
    }
#endif
}

void API::dropIndex(const std::string &indexName) {
#ifdef DEBUG

    return;
#else
    try {
        if (!indexManager.hasIndex(indexName)) {
            throw runtime_error(indexName + " does not exist");
        } else {
            indexManager.dropIndex(tableName, indexName);
        }
    } catch (exception &e) { // FIXME
            throw e;
    }
#endif
}

void API::insertEntry(const string &tableName, const Entry &entry) {
#ifdef DEBUG

    return;
#else
    try {
        if (!catalogManager.isTableExist(tableName)) {
            throw runtime_error(tableName + " does not exist");
        } else {
            int pos = recordManager.insert(tableName, entry);
            TableDefinition &df = catalogManager.getTableDef(tableName);
            for (TableDefinition::const_iterator &i = df.begin(); i != df.end(); ++i) {
                string indexName = indexManager.getIndexName(tableName, i->name);
                if (!indexName.empty()) {
                    indexManager.insert(indexName, pos,
                                        recordManager.getAttValue(tableName, pos, i->name));
                }
            }
        }
    } catch (exception &e) {
        throw e;
    }
#endif
}

const Entries &API::select(const std::string &tableName) {
#ifdef DEBUG
    cout << "select all\n--\n";
    //return Entries();
#else
    if (!catalogManager.isTableExist(tableName)) {
        throw runtime_error(tableName + " dose not exist");
    } else {

    }
#endif
}

const Entries &API::select(const std::string &tableName, const Conditions &conditions) {
#ifdef DEBUG
    cout << "select condi" << conditions.size() << "\n--\n";
    //return Entries();
#endif

}
