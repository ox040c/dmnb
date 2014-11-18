#include "CatalogManager.hpp"
#include "TableProtobuf.pb.h"
#include <stdio.h>
#include <fstream>
#include <sstream>

using namespace std;

string parseNames(string &names) {
    char name[255];
    int idx = names.find("|");
    int len = names.copy(name, idx, 0);
    name[len] = '\0';
    names.erase(0, idx+1);

    return string(name);
}

bool CatalogManager::isTableExist(const std::string &tableName) {
    if (tables.count(tableName) != 0) return true;
    fstream input(tableName + ".def", ios::in | ios::binary);
    if (input.good()) return true;
    return false;
}

bool CatalogManager::isTableHasAttribute(const std::string &tableName, const std::string &attName) {
    try {
        readTable(tableName);
    } catch (exception e) {
        throw e;
    }
    for (TableDefinition::const_iterator i = tables[tableName].begin(); i != tables[tableName].end(); ++i) {
        if (i->name == attName)
            return true;
    }
    return false;
}

void CatalogManager::readTable(const std::string &tableName) {
    if (!isTableExist(tableName)) {
        throw runtime_error(tableName + " dose not exist");
    } else {
        if (tables.count(tableName) != 0) return;

        fstream input(tableName + ".def", ios::in | ios::binary);
        TableProtobuf tablePb;

        if (!tablePb.ParseFromIstream(&input)) {
            throw runtime_error("failed to read " + tableName + ".def");
        } else {
            int total = 0;
            string names = tablePb.names();
            string primaryKey = tablePb.primary();
            string uniqueKey = tablePb.unique();

            TableDefinition table;
            map <string, bool> primaryMap;
            map <string, bool> uniqueMap;

            while (primaryKey.length() > 0) {
                primaryMap[parseNames(primaryKey)] = true;
            }
            while (uniqueKey.length() > 0) {
                uniqueMap[parseNames(uniqueKey)] = true;
            }

            while (names.length() > 0) {
                string name = parseNames(names);
                string typeString = parseNames(names);
                enum DataType type;
                int charLength = 0;
                bool isPrimary = false, isUnique = false;

                if (typeString == "INT") type = INT;
                if (typeString == "FLOAT") type = FLOAT;
                if (typeString == "CHAR") {
                    type = CHAR;
                    stringstream(parseNames(names)) >> charLength;
                }

                if (primaryMap.count(name) > 0) isPrimary = true;
                if (uniqueMap.count(name) > 0) isUnique = true;

                table.push_back(Scheme(name,
                                       type,
                                       charLength,
                                       "",
                                       isPrimary));
                                       //TODO: isUnique));
            }
        }
    }
}

void CatalogManager::createTable(const std::string &tableName, const TableDefinition &table) {
    if (isTableExist(tableName)) {
        throw runtime_error(tableName + " has existed");
    } else {
        fstream output(tableName+".def", ios::out | ios::binary);
        // int total = 0;
        DataType type;
        string names;
        string primaryKey, uniqueKey;
        TableProtobuf tablePb;
        for (TableDefinition::const_iterator i = table.begin(); i != table.end(); ++i) {
            names += i->name + "|";
            type = i->type;
            switch (type) {
                case INT: names += "INT|"; break;
                case FLOAT: names += "FLOAT|"; break;
                case CHAR: names += "CHAR";
            }
            if (type == CHAR) names += "|" + to_string(i->intv) + "|";
            if (i->isIndex) primaryKey += i->name + "|";
            if (i->isUnique) uniqueKey += i->name + "|";
        }
        tablePb.set_names(names.c_str());
        tablePb.set_primary(primaryKey.c_str());
        tablePb.set_unique(uniqueKey.c_str());
        if (!tablePb.SerializeToOstream(&output)) {
            throw runtime_error("failed to write " + tableName + ".def");
        }

        tables[tableName] = table;

    }

}

void CatalogManager::dropTable(const std::string &tableName) {
    if (!isTableExist(tableName)) {
        throw runtime_error(tableName + ".def" + " does not exist");
    } else {
        if (remove((tableName + ".def").c_str()) != 0) {
            throw runtime_error("failed to remove " + tableName + ".def");
        }
        if (tables.count(tableName) != 0) tables.erase(tableName);
    }
}
