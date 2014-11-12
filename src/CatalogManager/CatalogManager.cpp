#include "CatalogManager.hpp"
#include "TableProtobuf.pb.h"
#include <stdio.h>
#include <fstream>
#include <sstream>

using namespace std;

string parseNames(string &names) {
    string name;
    int idx = names.find("|");
    int len = names.copy(name, idx, 0);
    name[len] = '\0';
    len = names.copy(names, names.length()-idx-1, idx+1);
    names[len] = '\0';

    return name;
}

bool CatalogManager::isTableExisted(const std::string &tableName) {
    if (tables.count(tableName) != 0) return true;
    fstream input(tableName + ".def", ios::in | ios::binary);
    if (input.good()) return true;
    return false;
}

bool CatalogManager::isTableHasAttribute(const std::string &tableName, const std::string &attName) {
    try {
        readTable(tableName);
    } catch (Exception e) {
        throw e;
    }
    for (list < Scheme >::iterator i = tables[tableName].begin(); i != tables[tableName].end(); ++i) {
        if (i.getName() == attName)
            return true;
    }
    return false;
}

void CatalogManager::readTable(const std::string &tableName) {
    if (!isTableExisted(tableName)) {
        throw runtime_error(tablename + " dose not exist");
    } else {
        if (tables.count(tableName) != 0) return;

        fstream input(tableName + ".def", ios::in | ios::binary);
        TableProtobuf tablePb;

        if (!tablePb.ParseFromIstream(&input)) {
            throw runtime_error("failed to read " + tableName + ".def");
        } else {
            int total = 0;
            string names = tablePb.names();
            TableDefinition table;
            for (DataType type = INT; type < TOTAL_TYPE; ++type) {
                for (int i = 0; i < tablePb.totalint(); ++i) {
                    table.push_back(new TableDefinition());
                    table.back().setName(parseNames(names));
                    if (type == CHAR) {
                        int charLength;
                        stringstream(parseNames(names)) >> charLength;
                        table.back().setCharLength(charLength);
                    }
                    table.back().setDataType(type);
                }
            }

        }
    }

}

void CatalogManager::createTable(const std::string &tableName, const std::TableDefinition &table) {
    if (isTableExisted(tableName)) {
        throw runtime_error(tableName + " has existed");
    } else {
        fstream output(tableName+".def", ios::out | ios::binary);
        int total = 0;
        DataType type = INT;
        string names;
        TableProtobuf tablePb;
        for (vetor < Scheme >::iterator i = table.begin(); i != table.end(); ++i) {
            if (i.getDataType() != type) {
                switch (type) {
                    case INT: tablePb.set_totalint(total); break;
                    case FLOAT: tablePb.set_totalfloat(total); break;
                    case CHAR: tablePb.set_totalchar(total);
                }
                type = i.getDataType();
                total = 0;
            }
            ++total;
            names += i.getName() + "|";
            if (type == CHAR) names += "|" + to_string(i.getCharLength());
        }
        if (!table.SerializeToOstream(&output)) {
            throw runtime_error("failed to write " + tableName + ".def");
        }

        tables[tableName] = table;

    }

}

void CatalogManager::dropTable(const std::string &tableName) {
    if (!isTableExisted(tableName)) {
        throw runtime_error(tableName + ".def" + " does not exist");
    } else {
        if (remove(tableName + ".def") != 0) {
            throw runtime_error("failed to remove " + tableName + ".def");
        }
        if (tables.count(tableName) != 0) tables.erase(tableName);
    }
}

