#ifndef DATA_TYPE_HPP
#define DATA_TYPE_HPP

#include <string>

// applying namespace to avoid naming conflicts, thanks
// NOTE: please add following line if you are using these types
/*
using namespace utls;
*/
namespace utls {
    
    enum Operator {EQUAL, UNEQUAL, LESS, MORE, NO_MORE, NO_LESS, TOTAL_OPERATOR};

    enum DataType {INT, FLOAT, CHAR, TOTAL_TYPE};
        
}

typedef struct AttributeType {
    utls::DataType type;
    void *dataPtr;
} Attribute;

struct DataAddr {
    std::string filename;
    unsigned int dataaddr;
    unsigned int datalen;
};

class Scheme {
private:
    std::string name;
    utls::DataType dataType;
    int charLength;
    bool indexed;

public:
    void setName(const std::string &name);
    void setDataType(const utls::DataType &dataType);
    void setCharLength(const int &charLength);

    std::string getName();
    utls::DataType getDataType();
    int getCharLength();
    bool getIndexed();
};

class Condition {
private:
    std::string key;
    utls::Operator op;
    void *valuePtr;

public:
    void setKey(const std::string &key);
    void setOp(const utls::Operator &op);
    void setValue(const void *valuePtr);

    std::string getKey();
    utls::Operator getOp();
    void *getValuePtr();
};

#endif
