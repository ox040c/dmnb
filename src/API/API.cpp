#include "API.hpp"

/// TODO...
///
/// TxT
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

void API::dropIndex(const std::string &tableName, const std::string &indexName) {
    try {
        if (!catalogManager.isTableHasAttribute(tableName)) {
            throw runtime_error(tableName + " dose not have attribute ", indexName);
        } else {
            if (!indexManager.isTableHasIndex(table, index)) {
                throw runtime_error(tableName + " dost not have index on " + indexName);
            } else {
                indexManager.dropIndex(tableName, indexName);
            }
        }
    } catch (Exception e) { // Table dose not exist
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
