#include "API.hpp"
#include <iostream>
#include <string>

/// TODO...
///
///
///
using namespace std;

#define DEBUG

void API::checkEntry(const std::string &tableName, const Entry &entry) {
    int j=0;
    if (entry.size() != catalogManager.getTableDef(tableName).size())
        throw runtime_error("Number of attributes not match");
    for (Entry::const_iterator i = entry.begin(); i != entry.end(); ++i, ++j) {
        Scheme att = catalogManager.getAtt(tableName, j);
        if (i->type != att.type)
            throw runtime_error("Type of attribute not match");
        if (catalogManager.isAttUnique(tableName, j)) {
            Condition con = att;
            con.op = EQUAL;
            if (indexManager.select(tableName, con).size() > 0) // FIXME
                throw runtime_error("Key value confilt");
        }
    }
}

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
            indexManager.dropIndex(indexName);
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
            checkEntry(tableName, entry);

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

bool API::check(const int &x, const Operator &op, const int &y) {
    switch (op) {
    case LESS:      return x<y;
    case NO_MORE:   return x<=y;
    case EQUAL:     return x==y;
    case UNEQUAL:   return x!=y;
    case MORE:      return x>y;
    case NO_LESS:   return x>=y;
    }
}

bool API::check(const float &x, const Operator &op, const float &y) {
    switch (op) {
    case LESS:      return y-x>1e-6;
    case NO_MORE:   return y-x>=1e-6;
    case EQUAL:     return abs(x-y)<=1e-6;
    case UNEQUAL:   return abs(x-y)>1e-6;
    case MORE:      return x-y>1e-6;
    case NO_LESS:   return x-y>=1e-6;
    }
}

bool API::check(const string &x, const Operator &op, const string &y) {
    switch (op) {
    case LESS:      return x<y;
    case NO_MORE:   return x<=y;
    case EQUAL:     return x==y;
    case UNEQUAL:   return x!=y;
    case MORE:      return x>y;
    case NO_LESS:   return x>=y;
    }
}

void API::gainIdx(const string &tableName, const Condition &condition, Idx &idx) {
    idx.clear();
    string idxName = indexManager.getIndexName(tableName, condition.name);
    if (idxName.empty()) {
        unsigned int ptr = recordManager.getNext(tableName, true);
        while (ptr != -1) {
            Wrapper value = recordManager.getAttValue(tableName, ptr, condition.name, catalogManager.getTableDef(tableName));
            switch (value.type) {
            case INT:   if (check(value.intv, condition.op, condition.intv)) idx.insert(ptr); break;
            case FLOAT: if (check(value.floatv, condition.op, condition.floatv)) idx.insert(ptr); break;
            case CHAR:  if (check(value.strv, condition.op, condition.strv)) idx.insert(ptr); break;
            }
            ptr = recordManager.getNext(tableName, false);
        }
    } else {
        const list <unsigned int> &idxList = indexManager.select(idxName, condition);
        idx.insert(idxList.begin(), idxList.end());
    }
}

void API::selectIdx(const string &tableName, const Conditions &conditions) {
    Idx resultSet;
    Conditions::const_iterator i = conditions.begin();
    gainIdx(tableName, *i, idxSet);
    ++i;
    for (; i != conditions.end(); ++i) {
        gainIdx(tableName, *i, tempIdxSet);
        resultSet.clear();
        set_intersection(idxSet.begin(), idxSet.end(),
                         tempIdxSet.begin(), tempIdxSet.end(),
                         insert_iterator<Idx>(resultSet, resultSet.begin()));
        idxSet = resultSet;
    }
}

const Entries &API::select(const std::string &tableName) {
#ifdef DEBUG
    cout << "select all\n--\n";
    //return Entries();
#else
    if (!catalogManager.isTableExist(tableName)) {
        throw runtime_error(tableName + " dose not exist");
    } else {
        result.clear();
        unsigned int ptr = getNext(tableName, true);
        while (ptr != -1) {
            result.push_back(recordManager.getValue(tableName, ptr, catalogManager.getTableDef(tableName)));
            ptr = getNext(tableName, false);
        }
        return result;
    }
#endif
}

const Entries &API::select(const std::string &tableName, const Conditions &conditions) {
#ifdef DEBUG
    cout << "select condi" << conditions.size() << "\n--\n";
    //return Entries();
#else
    selectIdx(tableName, conditions);
    result.clear();
    for (Idx::const_iterator i = idxSet.begin(); i != idxSet.end(); ++i) {
        result.push_back(recordManager.getValue(tableName, *i));
    }
    return result;

#endif

}

int API::remove(const string &tableName) {
#ifdef DEBUG
    cout << "remove all\n--\n";
    //return -1;
#else
    if (!catalogManager.isTableExist(tableName)) {
        throw runtime_error(tableName + " dose not exist");
    } else {
        TableDefinition td = catalogManager.getTableDef(tableName);
        // remove index
        for (TableDefinition::const_iterator i = td.begin(); i != td.end(); ++i) {
            string idxName = indexManager.getIndexName(tableName, i->name);
            if (!idxName.empty()) {
                indexManager.remove(idxName);
            }
        }
        // remove entries
        int total = recordManager.deleteEntry(tableName);
        return total;
    }
#endif
}

int API::remove(const std::string &tableName, const Conditions &conditions) {
#ifdef DEBUG
    cout << "remove condi" << conditions.size() << "\n--\n";
    //return -1;
#else
    if (conditions.size() > 1) throw runtime_error("No support for command `delete` with more than ONE conditions");
    selectIdx(tableName, conditions);
    int total = idxSet.size();
    for (Idx::const_iterator i = idxSet.begin(); i != idxSet.end(); ++i) {
        Entry entry = recordManager.getValue(tableName, *i);
        for (Entry::const_iterator j = entry.begin(); j != entry.end(); ++j) {
            string idxName = indexManager.getIndexName(tableName, j->name);
            if (!idxName) {
                Condition condition = *j;
                condition.op = EQUAL;
                indexManager.remove(idxName, condition);
            }
        }
        recordManager.deleteEntry(tableName, *i);
    }
    return total;

#endif

}

