#include "API.hpp"

/// TODO...
///
///
///

void API::createTable(const std::string &tableName, const TableDefinition &data) {
    if (catalogManager.isTableExist(tableName)) {
        throw runtime_error(tableName + " has existed");
    } else {
        catalogManager.createTable(tableName, data);
        recordManager.createTable(tableName, data);
    }
}

void API::dropTable(const std::string &tableName) {
    if (!catalogManager.isTableExist(tableName)) {
        throw runtime_error(tableName + " dose not exist");
    } else {
        catalogManager.dropTable(tableName);
        recordManager.dropTable(tableName);
        indexManager.dropTable(tableName);
    }
}

void API::createIndex(const string &tableName,
                      const string &colName,
                      const string &indexName) {
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
    } catch (Exception e) {
        throw e;
    }
}

void API::dropIndex(const std::string &indexName) {
    try {
        if (!indexManager.hasIndex(indexName)) {
            throw runtime_error(indexName + " does not exist");
        } else {
            indexManager.dropIndex(tableName, indexName);
        }
    } catch (Exception e) { // FIXME
            throw e;
    }
}

const Data &API::select(const std::tableName) {
    if (!catalogManager.isTableExist(tableName)) {
        throw runtime_error(tableName + " dose not exist");
    } else {

    }
}

const Data &API::select(const std::string &tableName, const Conditions &conditions) {

}
