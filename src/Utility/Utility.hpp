#ifdef DATA_TYPE_HPP
#define DATA_TYPE_HPP

enum Operator {EQUAL, UNEQUAL, LESS, MORE, NO_MORE, NO_LESS, TOTAL_OPERATOR};

enum DataType {INT, FLOAT, CHAR, TOTAL_TYPE};

class Scheme {
private:
    std::string name;
    DataType dataType;
    int charLength;
    bool indexed;

public:
    void setName(const std::string &name);
    void setDataType(const DataType &dataType);
    void setCharLength(const int &charLength);

    std::string getName();
    DataType getDataType();
    int getCharLength();
    bool getIndexed();
};

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
