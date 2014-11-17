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
    if (indexManager.isIndexExist(indexName)) {

    }
}

void API::dropIndex(const std::string &indexName) {
    try {
        if (!indexManager.isIndexExist(indexName)) {
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
