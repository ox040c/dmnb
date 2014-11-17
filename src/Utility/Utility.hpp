#ifndef DATA_TYPE_HPP
#define DATA_TYPE_HPP

#include <string>

enum Operator {EQUAL, UNEQUAL, LESS, MORE, NO_MORE, NO_LESS, TOTAL_OPERATOR};

enum DataType {INT, FLOAT, CHAR, TOTAL_TYPE};

typedef struct AttributeType {
    DataType type;
    void *dataPtr;
} Attribute;

struct DataAddr {
    std::string filename;
    unsigned int dataaddr;
    unsigned int datalen;
};

typedef struct SchemeType {
private:
    std::string name;
    DataType dataType;
    int charLength;
    bool indexed;
} Scheme;

typedef std::list < Scheme > TableDefinition;

DataType getDataType(const TableDefinition& td, const std::string attName) {
    for (const list <Scheme>::iterator &i = td.begin(); i != td.end(); ++i) {
        if (i->name == attName)
            return i->dataType;
    }
    return (DataType)TOTAL_TYPE;
}

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

#endif
