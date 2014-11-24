#include "API.hpp"
#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
/// TODO...
///
///
///
using namespace std;

// #define DEBUG

void API::checkEntry(const std::string &tableName, Entry &entry) {
    //cout << "[API::checkEntry] calling..." << endl;
    //cout.flush();

    int j=0;
    if (entry.size() != catalogManager.getTableDef(tableName).size())
        throw runtime_error("Number of attributes not match");
    for (Entry::iterator i = entry.begin(); i != entry.end(); ++i, ++j) {
        Scheme att = catalogManager.getAtt(tableName, j);
        if (i->type == INT && att.type == FLOAT) {
            //cout << "API::checkEntry " << i->intv << " " << i->floatv << endl;
            i->type = FLOAT;
            i->floatv = i->intv;
        }
        if (i->type != att.type && !(i->type == INT && att.type == FLOAT))
            throw runtime_error("Type of attribute not match");
        if (catalogManager.isAttUnique(tableName, j)) {
            Condition con = *i;
            con.op = EQUAL;
            con.type = att.type;

            //cout << "[API::checkEntry] att.name = " << att.name << endl;

            string indexName = indexManager.getIndexName(tableName, att.name);
/*          cout << "[API::checkEntry] indexName = " << indexName << endl;
            cout << "[API::checkEntry] con.type = " << con.type <<
                    "\n\tcon.op = " << con.op <<
                    "\n\tcon.intv = " << con.intv <<
                    endl;
            cout.flush();
*/
            if (indexName.empty()) {
                // pass
                // dirty dirty
            } else {
            if (indexManager.select(indexName, con).size() > 0) // FIXME
                throw runtime_error("Key value conflict");
            }
        }
    }
}

void API::createTable(const std::string &tableName, TableDefinition &data) {
#ifdef DEBUG
    cout << "create" << data.size() << "attrs\n--\n";
    return;
#else
    if (catalogManager.isTableExist(tableName)) {
        throw runtime_error(tableName + " has existed");
    } else {
        catalogManager.createTable(tableName, data);
        // FIXME: recordManager.createTable(tableName, data);
        recordManager.creatSchema(tableName, data);

        for (TableDefinition::iterator i = data.begin(); i != data.end(); ++i)
            if (i->isIndex) {

                //cout << "[API] create index: " << tableName <<"_" << i->name << endl;
                indexMap[tableName+"_"+i->name] = 1;
                indexMap2[tableName+"_"+i->name] = tableName+"_"+i->name;

                indexManager.createIndex(tableName, i->name, tableName+"_"+i->name, i->type);
            }
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
        const TableDefinition &td = catalogManager.getTableDef(tableName);
        for (TableDefinition::const_iterator i = td.begin(); i != td.end(); ++i)
        {
            string idxName = indexManager.getIndexName(tableName, i->name);
            if (!idxName.empty()) indexManager.dropIndex(idxName);
        }
        catalogManager.dropTable(tableName);
        // FIXME: recordManager.dropTable(tableName);
        recordManager.dropSchema(tableName);
    }
#endif
}

DataType getDataType(const TableDefinition &td, const string &colName) {
    for (TableDefinition::const_iterator i = td.begin(); i != td.end(); ++i) {
        if (i->name == colName)
            return i->type;
    }
}

void API::createIndex(const string &tableName,
                      const string &colName,
                      const string &indexName) {
#ifdef DEBUG

    return;
#else
    const TableDefinition &td = catalogManager.getTableDef(tableName);
    int j=0;
    bool uflag = false, hflag = false;
    for (TableDefinition::const_iterator i = td.begin();
         i != td.end(); ++i, ++j) {
        if (i->name == colName) {
            if (catalogManager.isAttUnique(tableName, j)) {
                uflag = true;
            }
            hflag = true;
            break;
        }
    }
    if (!hflag) throw runtime_error(tableName + " dose not has " +colName);
    if (!uflag) throw runtime_error(colName + " is not unique");
    try {
        if (indexManager.hasIndex(tableName, colName)) {
            if (indexMap.count(tableName+"_"+colName) &&
                    indexMap[tableName+"_"+colName]>1)
                throw runtime_error("index on "+tableName+" ("+
                                    colName+") has existed");
            else {
                indexMap[tableName+"_"+colName] = 2;
                indexMap2[indexName] = tableName+"_"+colName;
            }
        } else {
            indexMap[tableName+"_"+colName] = 2;
            indexMap2[indexName] = indexName;
            const TableDefinition &td = catalogManager.getTableDef(tableName);
            indexManager.createIndex(tableName, colName, indexName, getDataType(td, colName));
            int pos = recordManager.getNext(tableName, true,0);
            while (pos > 0) {
                indexManager.insert(indexName, pos,
                                    recordManager.getAttValue(tableName, pos, colName, td));
                pos = recordManager.getNext(tableName, false,pos);
            }
        }
    } catch (runtime_error const &e) {
            throw e;
    }

#endif
}

void API::dropIndex(const std::string &indexName) {
#ifdef DEBUG

    return;
#else
    try {
        // if (!indexManager.hasIndex(indexName)) {
        //    throw runtime_error(indexName + " does not exist");
        // } else {
            if (!indexMap2.count(indexName))
                throw runtime_error(indexName + " does not exist");
            indexManager.dropIndex(indexMap2[indexName]);
        }
    catch (runtime_error const &e) { // FIXME
            throw e;
    }
#endif
}

void API::insertEntry(const string &tableName, Entry &entry) {
#ifdef DEBUG

    return;
#else
    try {
        if (!catalogManager.isTableExist(tableName)) {
            throw runtime_error(tableName + " does not exist");
        } else {
            //cout << "[API] insertEntry: tableName = " << tableName << endl;
            //cout.flush();

            checkEntry(tableName, entry);

            //cout << "[API] pass check!" << endl;


            int j = 0;
            for (Entry::iterator i = entry.begin(); i != entry.end(); ++i, ++j) {
                if (i->type == utls::CHAR) {
                    i->intv = catalogManager.getAtt(tableName, j).intv;

                    //cout << "[API]: " << i->strv << ": " << i->intv << endl;

                }
            }

            int pos = recordManager.insertEntry(tableName, entry);

            //cout << "[API] pos = " << pos << endl;

            const TableDefinition &df = catalogManager.getTableDef(tableName);
            for (TableDefinition::const_iterator i = df.begin(); i != df.end(); ++i) {
                string indexName = indexManager.getIndexName(tableName, i->name);

                //cout << "[API] after got indexName:" << indexName << endl;

                if (!indexName.empty()) {
                    //cout << "[API] add index: " << indexName << " value: " <<
                    //        recordManager.getAttValue(tableName, pos, i->name, df).strv << endl;
                    //cout.flush();

                    indexManager.insert(indexName, pos,
                                        recordManager.getAttValue(tableName, pos, i->name, df));
                    //cout << "[API] added index: " << indexName << endl;
                }

            }

            //cout << "[API] insertEntry finished!" << endl;
        }
    } catch (runtime_error const &e) {
        // cerr << "[API] err: " << e.what() << endl;
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
    case NO_MORE:   return y-x>=-1e-6;
    case EQUAL:     return abs(x-y)<=1e-6;
    case UNEQUAL:   return abs(x-y)>1e-6;
    case MORE:      return x-y>0;
    case NO_LESS:   return x-y>=-1e-6;
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
         int ptr = recordManager.getNext(tableName, true,0);
        while (ptr > 0) {
            Wrapper value = recordManager.getAttValue(tableName, ptr, condition.name, catalogManager.getTableDef(tableName));
            switch (value.type) {
            case INT:   if (check(value.intv, condition.op, condition.intv)) idx.insert(ptr); break;
            case FLOAT:
                //cout << "API: " << condition.floatv << endl;
                if (check(value.floatv, condition.op, condition.floatv)) idx.insert(ptr); break;
            case CHAR:  if (check(value.strv, condition.op, condition.strv)) idx.insert(ptr); break;
            }
            ptr = recordManager.getNext(tableName, false,ptr);
        }
    } else {
        const list < int> &idxList = indexManager.select(idxName, condition);
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
/*
    indexManager.print();
    cout << "----" << endl;
    indexManager.showdetail();
*/
}

const Entries &API::select(const std::string &tableName) {
#ifdef DEBUG
    cout << "select all\n--\n";
    //return Entries();
#else
    if (!catalogManager.isTableExist(tableName)) {
        throw runtime_error(tableName + " dose not exist");
    } else {
        //cout << "[API] select * called!" << endl;

        result.clear();
         int ptr = recordManager.getNext(tableName, true,0);

        //cout << "[API] gotta! ptr = " << ptr << endl;

        while (ptr > 0) {
            //cout << "[API] ptr = " << ptr << endl;
            /*
            for (TableDefinition::const_iterator i = catalogManager.getTableDef(tableName).begin();
                 i != catalogManager.getTableDef(tableName).end(); ++i)
                cout << "[API] " << i->name << " is " << i->type << endl;
            */
            result.push_back(recordManager.getValue(tableName, ptr, catalogManager.getTableDef(tableName)));
            ptr = recordManager.getNext(tableName, false,ptr);
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
        result.push_back(recordManager.getValue(tableName, *i, catalogManager.getTableDef(tableName)));
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

        //cout << "[API] delete finished!"<<endl;

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
        Entry entry = recordManager.getValue(tableName, *i, catalogManager.getTableDef(tableName));
        for (Entry::const_iterator j = entry.begin(); j != entry.end(); ++j) {
            string idxName = indexManager.getIndexName(tableName, j->name);
            if (!idxName.empty()) {
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

